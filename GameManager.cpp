#include "GameManager.h"
#include <QDebug>
#include <QtMath>
#include <QRandomGenerator>
#include <QEvent>
#include"DropItem.h"
#include "mainwindow.h"
#include "Wave.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QJsonArray>
#include "PlayerWave.h" 
#include "FlameWave.h"
#include "LurkWave.h"
#include "KingWave.h"
template<typename PhantomType>
void GameManager::spawnPhantoms(QVector<PhantomType*>& container, const QVector<QPoint>& spawnPoints, int maxCount, std::function<PhantomType*()> createPhantom)
{
    for (int i = container.size() - 1; i >= 0; i--) {//删除死亡的
        if (!container[i] || !container[i]->scene() || container[i]->getHp() <= 0) {
            if (container[i]) {
                gameScene->removeItem(container[i]);
                delete container[i];
            }
            container.remove(i);
        }
    }
    if (container.size() >= maxCount) return;
    if (spawnPoints.isEmpty()) return;
    for (const QPoint& pt : spawnPoints) {
        bool occupied = false;
        for (auto* phantom : container) {
            if (phantom && phantom->scene() &&
                (phantom->pos().x() / gameMap->getGridSize() == pt.x()) &&
                (phantom->pos().y() / gameMap->getGridSize() == pt.y())) {
                occupied = true;
                break;
            }
        }
        if (!occupied) {
    PhantomType* phantom = createPhantom();
    phantom->setPos(
        pt.x() * gameMap->getGridSize() + (gameMap->getGridSize() - phantom->boundingRect().width()) / 2.0,
        pt.y() * gameMap->getGridSize() + (gameMap->getGridSize() - phantom->boundingRect().height()) / 2.0
    );
    phantom->setWanderTarget(phantom->pos() + QPointF(phantom->boundingRect().width()/2, phantom->boundingRect().height()/2));
    gameScene->addItem(phantom);
    container.append(phantom);
    break;
        }
    }
}
GameManager::GameManager(QGraphicsView* graphicsView, QObject *parent) : QObject(parent), gameView(graphicsView), gameScene(nullptr), gameMap(nullptr), player(nullptr), gameLoopTimer(nullptr), gamePaused(false) { 
    MainWindow* mw = qobject_cast<MainWindow*>(parent);
    if (mw) {
        connect(mw, SIGNAL(viewResized()), this, SLOT(handleViewResize()));
        connect(mw, SIGNAL(forcePhantomsChasePlayer()), this, SLOT(onForcePhantomsChasePlayer()));
    }
    phantomSpawnTimer = new QTimer(this);
    connect(phantomSpawnTimer, &QTimer::timeout, this, &GameManager::spawnFlamePhantoms);
    connect(phantomSpawnTimer, &QTimer::timeout, this, &GameManager::spawnLurkPhantoms);
    countdownTimer = new QTimer(this);
    connect(countdownTimer, &QTimer::timeout, this, [this]() {
        if (countdownSeconds > 0) {
            countdownSeconds--;
            emit countdownChanged(countdownSeconds);
        }
        if (countdownSeconds == 0) {
            if (!inSecondCountdown) {
                portalEnabled = false;
                emit portalStateChanged(false);
                QMetaObject::invokeMethod(this->parent(), "forcePhantomsChasePlayer");
                MainWindow* mw = qobject_cast<MainWindow*>(this->parent());
                if (mw) mw->playBgm(MainWindow::BgmType::Trapped1);
                accelerateAllPhantoms();
                setCountdown(60);
                inSecondCountdown = true;
            } else {
                countdownTimer->stop();

                MainWindow* mw = qobject_cast<MainWindow*>(this->parent());
                if (mw) mw->onEnd(MainWindow::BgmType::FakeEnd, mw->findChild<QWidget*>("fePage"), &(mw->fakeEndPlayed));

            }
        }
    });
}
void GameManager::setCountdown(int seconds) {
    countdownSeconds = seconds;
    emit countdownChanged(countdownSeconds);
}
int GameManager::getCountdown() const {
    return countdownSeconds;
}
bool GameManager::isPortalEnabled() const {
    return portalEnabled;
}
void GameManager::startCountdown() {
    if (countdownTimer) {
        countdownTimer->stop();
        countdownTimer->start(1000);
    }
}
void GameManager::stopCountdown() {
    if (countdownTimer) countdownTimer->stop();
}
void GameManager::pauseGame() {
    if (gameLoopTimer && gameLoopTimer->isActive()) {
        gameLoopTimer->stop();
        phantomSpawnTimer->stop();
        pauseAllWaves(); 
        for (auto* phantom : flamePhantoms) {
            if (phantom) phantom->stopTimers();
        }
        for (auto* phantom : lurkPhantoms) {
            if (phantom) phantom->stopTimers();
        }
        if (king) king->stopTimers();
        if (player) player->stopTimers();
        gamePaused = true;
    }
}

void GameManager::onForcePhantomsChasePlayer() {
    PhantomBase::setForceChasePlayer(true);
}
void GameManager::resumeGame() {
    if (gameLoopTimer && !gameLoopTimer->isActive()) {
        gameLoopTimer->start(1000/60);
        resumeAllWaves(); 
        for (auto* phantom : flamePhantoms) {
            if (phantom) phantom->startTimers();
        }
        for (auto* phantom : lurkPhantoms) {
            if (phantom) phantom->startTimers();
        }
        if (king) king->startTimers();
        if (player) player->startTimers();
        gamePaused = false; 
    }
    if (gameView) {
        gameView->setFocusPolicy(Qt::StrongFocus);
        gameView->setFocus();
    }
    if (player) {
        player->clearPressedKeys();//清除按键集
        player->setFocus();
    }
}
GameManager::~GameManager() {//要补全,记得不要二次释放
    if (gameLoopTimer) gameLoopTimer->stop();
    if (phantomSpawnTimer) phantomSpawnTimer->stop();
    if (countdownTimer) countdownTimer->stop();
    delete gameScene;
    delete gameMap;
    delete gameLoopTimer;
}
bool GameManager::eventFilter(QObject *watched, QEvent *event) {
    if (watched == gameView->viewport() && event->type() == QEvent::Resize) {
        handleViewResize();
    }
    return QObject::eventFilter(watched, event);
}
void GameManager::startGame() {
    gameScene = new QGraphicsScene(this);//创建场景
    gameView->setScene(gameScene);
    gameMap = new Map(gameScene);
    curMapId = 0;//初始地图0
    gameMap->initMap(curMapId); //初始化地图
    player = new Player();//创建玩家
    gameScene->addItem(player);//放玩家
    phantomSpawnTimer->start(3000);//phantom生成时间间隔
    player->setPos(gameMap->getMapBounds().center());
    inTrueEndZone = false;
    trueEndTimer.invalidate();
    // 各种阻止鼠标滚轮的滑动
    gameView->setFocusPolicy(Qt::NoFocus);
    gameView->setFocus();
    gameView->setDragMode(QGraphicsView::NoDrag);
    gameView->setAlignment(Qt::AlignCenter);
    gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate); 
    gameView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    gameView->setRenderHint(QPainter::Antialiasing, false);
    gameView->setRenderHint(QPainter::SmoothPixmapTransform, false);
    gameView->setCacheMode(QGraphicsView::CacheBackground);
    
    handleViewResize();
    gameLoopTimer = new QTimer(this);
    player->setFocus();
    setCountdown(120);
    portalEnabled = true;
    inSecondCountdown = false;
    startCountdown();
    //游戏主循环
    connect(gameLoopTimer, &QTimer::timeout, this, [=]() {
        //各种结局
        if(player->getTeleportCount()>=10){
            MainWindow* mw = qobject_cast<MainWindow*>(parent());
            if (mw) mw->onEnd(MainWindow::BgmType::LostEnd, mw->findChild<QWidget*>("lePage"), &(mw->lostEndPlayed));
            gameLoopTimer->stop();
            phantomSpawnTimer->stop();
            return;
        }
        if (player->getHp() <= 0) {
            MainWindow* mw = qobject_cast<MainWindow*>(parent());
            if (mw) mw->onEnd(MainWindow::BgmType::DeadEnd, mw->findChild<QWidget*>("dePage"), &(mw->deadEndPlayed));
            gameLoopTimer->stop();
            phantomSpawnTimer->stop();
            return;
        }
        int gridSize = gameMap->getGridSize();
        int playerRow = static_cast<int>((player->y() + player->pixmap().height() / 2) / gridSize);
        int playerCol = static_cast<int>((player->x() + player->pixmap().width() / 2) / gridSize);
        if (gameMap->getTileType(playerRow, playerCol) == -3) {
            if (!inTrueEndZone) {
                inTrueEndZone = true;
                trueEndTimer.restart();
            } else if (trueEndTimer.isValid() && trueEndTimer.elapsed() >= 3000) {
                MainWindow* mw = qobject_cast<MainWindow*>(parent());
                if (mw) mw->onEnd(MainWindow::BgmType::TrueEnd, mw->findChild<QWidget*>("tePage"), &(mw->trueEndPlayed));
                gameLoopTimer->stop();
                phantomSpawnTimer->stop();
                return;
            }
        } else {
            inTrueEndZone = false;
            trueEndTimer.invalidate();
        }
        //王的消失
        if (king && king->getHp() <= 0) {
            if (king->scene()) {
                gameScene->removeItem(king);
            }
            delete king;
            king = nullptr;
        }
        player->updatePosition();
        player->setPos(qBound(0.0, player->x(), gameMap->getMapBounds().width() - player->pixmap().width()),qBound(0.0, player->y(), gameMap->getMapBounds().height() - player->pixmap().height()));

        if (gameMap->getTileType(playerRow, playerCol) == -1) {//传送
            if (portalEnabled) {
                QVector<QPoint> portalEnds;
                for (int r = 0; r < gameMap->getGridRow(); ++r) {
                    for (int c = 0; c < gameMap->getGridCol(); ++c) {
                        if (gameMap->getTileType(r, c) == -2) {
                            portalEnds.append(QPoint(c, r));
                        }
                    }
                }
                if (!portalEnds.isEmpty()) {
                    int idx = QRandomGenerator::global()->bounded(portalEnds.size());
                    QPoint target = portalEnds[idx];
                    qreal x = target.x() * gridSize + (gridSize - player->pixmap().width()) / 2.0;
                    qreal y = target.y() * gridSize + (gridSize - player->pixmap().height()) / 2.0;
                    player->setPos(x, y);
                    player->clearPressedKeys();
                    player->setFocus(); 
                    player->addTeleportCount(); // 增加传送次数
                    QMediaPlayer* teleportPlayer = new QMediaPlayer(this);
                    QAudioOutput* teleportAudio = new QAudioOutput(this);
                    teleportPlayer->setAudioOutput(teleportAudio);
                    teleportAudio->setVolume(1.0);
                    teleportPlayer->setSource(QUrl("qrc:/Resource/teleport.wav"));
                    connect(teleportPlayer, &QMediaPlayer::mediaStatusChanged, teleportPlayer, [teleportPlayer, teleportAudio](QMediaPlayer::MediaStatus status){
                        if (status == QMediaPlayer::EndOfMedia || status == QMediaPlayer::InvalidMedia) {
                            teleportPlayer->deleteLater();
                            teleportAudio->deleteLater();
                        }
                    });
                    teleportPlayer->play();
                }
            }
        }
        for (auto* phantom : flamePhantoms) {
            tryTeleportPhantom(phantom);
        }
        for (auto* phantom : lurkPhantoms) {
            tryTeleportPhantom(phantom);
        }
        if (king) {
            tryTeleportPhantom(king);
        }
        gameView->centerOn(player); // 视野居中
        gameView->viewport()->update();
        gameScene->advance();
        updateFlamePhantoms();
        updateLurkPhantoms();
        if (visionMask) visionMask->update();
    });

    gameLoopTimer->start(1000/60);
    if (!visionMask) {//视野遮罩
        visionMask = new VisionMaskItem(player, gameMap->getMapBounds(), this);
        visionMask->setZValue(10000);
        gameScene->addItem(visionMask);
    } else {
        visionMask->setGameManager(this);
    }
}
void GameManager::handleViewResize() {//处理视图的改变
    if (!gameScene || !gameMap) return;
    QRectF mapBounds = gameMap->getMapBounds();
    gameScene->setSceneRect(mapBounds);
    gameView->setSceneRect(mapBounds);
    if (player) {// 视野居中
        gameView->centerOn(player);
    }
        if (visionMask) {
        visionMask->setSceneRect(mapBounds);
    }
    gameView->viewport()->update();
}
void GameManager::separatePhantoms(QVector<PhantomBase*>& phantoms) {
    const qreal minDist = 64.0;//最小距离
    for (int i = 0; i < phantoms.size(); ++i) {
        PhantomBase* a = phantoms[i];
        if (!a || !a->scene()) continue;
        QPointF aCenter = a->pos() + QPointF(a->boundingRect().width()/2, a->boundingRect().height()/2);
        for (int j = i + 1; j < phantoms.size(); ++j) {
            PhantomBase* b = phantoms[j];
            if (!b || !b->scene()) continue;
            QPointF bCenter = b->pos() + QPointF(b->boundingRect().width()/2, b->boundingRect().height()/2);
            qreal dist = QLineF(aCenter, bCenter).length();
            if (dist < minDist && dist > 1e-2) {
                QPointF dir = (bCenter - aCenter) / dist;
                QPointF moveVec = dir * (minDist - dist) / 2.0;
                a->setPos(a->pos() - moveVec);
                b->setPos(b->pos() + moveVec);
            }
        }
    }
}
void GameManager::checkKingSpawn()
{
    if (king && king->scene()) return;
    QVector<QGraphicsItem*> all;
    for (auto* p : flamePhantoms) if (p && p->scene()) all.append(p);
    for (auto* p : lurkPhantoms) if (p && p->scene()) all.append(p);
    if (all.size() < 5) return;
    for (int i = 0; i <= all.size() - 5; ++i) {//5个聚集生成1个
        int closeCount = 1;
        for (int j = i+1; j < all.size(); ++j) {
            if (QLineF(all[i]->pos(), all[j]->pos()).length() < 200)
                ++closeCount;
            if (closeCount >= 5) {
                king = new King();
                king->setPos(all[i]->pos());
                gameScene->addItem(king);
                return;
            }
        }
    }
}
void GameManager::tryTeleportPhantom(PhantomBase* phantom) {
    if (!phantom || !gameMap) return;
    int gridSize = gameMap->getGridSize();
    int row = static_cast<int>((phantom->y() + phantom->boundingRect().height() / 2) / gridSize);
    int col = static_cast<int>((phantom->x() + phantom->boundingRect().width() / 2) / gridSize);
    if (gameMap->getTileType(row, col) == -1 && portalEnabled) {
        QVector<QPoint> portalEnds;
        for (int r = 0; r < gameMap->getGridRow(); ++r) {
            for (int c = 0; c < gameMap->getGridCol(); ++c) {
                if (gameMap->getTileType(r, c) == -2) {
                    portalEnds.append(QPoint(c, r));
                }
            }
        }
        if (!portalEnds.isEmpty()) {
            int idx = QRandomGenerator::global()->bounded(portalEnds.size());
            QPoint target = portalEnds[idx];
            qreal x = target.x() * gridSize + (gridSize - phantom->boundingRect().width()) / 2.0;
            qreal y = target.y() * gridSize + (gridSize - phantom->boundingRect().height()) / 2.0;
            phantom->setPos(x, y);
        }
    }
}
void GameManager::spawnFlamePhantoms() {
    if (!gameMap || !gameScene || !player) return;
    spawnPhantoms<FlamePhantom>(
        flamePhantoms,
        gameMap->getFlamePhantomBases(),
        15,
        [this]() { return new FlamePhantom(player); }
    );
}
void GameManager::updateFlamePhantoms() {
    for (int i = flamePhantoms.size() - 1; i >= 0; i--) {
        if (!flamePhantoms[i] || !flamePhantoms[i]->scene() || flamePhantoms[i]->getHp() <= 0) {
            if (flamePhantoms[i]) {
                // 先生成掉落物
                if (flamePhantoms[i]->getHp() <= 0 && flamePhantoms[i]->scene()) {
                    DropItem* drop = new DropItem(flamePhantomDrop);
                    drop->setPos(flamePhantoms[i]->pos());
                    gameScene->addItem(drop);
                }
                gameScene->removeItem(flamePhantoms[i]);
                delete flamePhantoms[i];
            }
            flamePhantoms.remove(i);
            continue;
        }
    }
    for (auto* phantom : flamePhantoms) {
        if (phantom && phantom->scene()) {
            phantom->updateStatus();
        }
    }
    bool playerShouldSlow = false;
    for (auto* phantom : flamePhantoms) {
        if (phantom && phantom->scene() && phantom->isVisible()) {
            QPointF center = phantom->pos() + QPointF(phantom->boundingRect().width()/2, phantom->boundingRect().height()/2);
            QPointF pCenter = player->pos() + QPointF(player->boundingRect().width()/2, player->boundingRect().height()/2);
            if (QLineF(center, pCenter).length() <= phantom->getAtkRange()) {
                playerShouldSlow = true;
                break;
            }
        }
    }
    player->setSlowed(playerShouldSlow);
        QVector<PhantomBase*> allPhantoms;
    for (auto* p : flamePhantoms) if (p && p->scene()) allPhantoms.append(p);
    separatePhantoms(allPhantoms);
    checkKingSpawn();
}
void GameManager::spawnLurkPhantoms() {
    if (!gameMap || !gameScene || !player) return;
    spawnPhantoms<LurkPhantom>(
        lurkPhantoms,
        gameMap->getLurkPhantomBases(),
        20,
        [this]() { return new LurkPhantom(player); }
    );
}
void GameManager::updateLurkPhantoms() {
    for (int i = lurkPhantoms.size() - 1; i >= 0; i--) {
        if (!lurkPhantoms[i] || !lurkPhantoms[i]->scene() || lurkPhantoms[i]->getHp() <= 0) {
            if (lurkPhantoms[i]) {
                // 先生成掉落物
                if (lurkPhantoms[i]->getHp() <= 0 && lurkPhantoms[i]->scene()) {
                    DropItem* drop = new DropItem(lurkPhantomDrop);
                    drop->setPos(lurkPhantoms[i]->pos());
                    gameScene->addItem(drop);
                }
                gameScene->removeItem(lurkPhantoms[i]);
                delete lurkPhantoms[i];
            }
            lurkPhantoms.remove(i);
            continue;
        }
    }
    for (auto* phantom : lurkPhantoms) {
        if (phantom && phantom->scene()) {
            phantom->updateStatus();
        }
    }
    QVector<PhantomBase*> allPhantoms;
    for (auto* p : lurkPhantoms) if (p && p->scene()) allPhantoms.append(p);
    separatePhantoms(allPhantoms);
    checkKingSpawn();
}
void GameManager::accelerateAllPhantoms() {
    for (auto* phantom : flamePhantoms) {
        if (phantom) {
            phantom->setSpeed(phantom->getSpeed() * 4.0f);
        }
    }
    for (auto* phantom : lurkPhantoms) {
        if (phantom) {
            phantom->setSpeed(phantom->getSpeed() * 5.0f);
        }
    }
}
void GameManager::pauseAllWaves() {
    if (!gameScene) return;
    for (QGraphicsItem* item : gameScene->items()) {
        Wave* wave = dynamic_cast<Wave*>(item);
        if (wave) wave->stopTimers();
    }
}
void GameManager::resumeAllWaves() {
    if (!gameScene) return;
    for (QGraphicsItem* item : gameScene->items()) {
        Wave* wave = dynamic_cast<Wave*>(item);
        if (wave) wave->startTimers();
    }
}


void GameManager::saveToJson(const QString& filename) const {
    QJsonObject root;
    // 玩家基本信息
    root["player_x"] = player->pos().x();
    root["player_y"] = player->pos().y();
    root["player_hp"] = player->getHp();
    root["player_maxHp"] = player->getMaxHp();
    root["player_atk"] = player->getAtk();
    root["player_atkRange"] = player->getAtkRange();
    root["player_sightRange"] = player->getSightRange();
    root["player_speed"] = player->getSpeed();
    root["player_atkCD"] = player->getAtkCD();
    root["player_waveAoeRadius"] = player->getPlayerWaveAoeRadius();
    root["player_waveSpeed"] = player->getPlayerWaveSpeed();
    root["player_teleportCount"] = player->getTeleportCount();
    root["inSecondCountdown"] = inSecondCountdown;
    root["countdownSeconds"] = countdownSeconds;
    root["bgmType"] = savedBgmType;
    root["save_time"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    root["curMapId"] = curMapId;

    // flamePhantoms
    QJsonArray flameArray;
    for (auto* phantom : flamePhantoms) {
        if (phantom && phantom->scene()) {
            QJsonObject obj;
            obj["x"] = phantom->pos().x();
            obj["y"] = phantom->pos().y();
            obj["hp"] = phantom->getHp();
            obj["speed"] = phantom->getSpeed();
            obj["atk"] = phantom->getAtk();
            obj["atkRange"] = phantom->getAtkRange();
            flameArray.append(obj);
        }
    }
    root["flamePhantoms"] = flameArray;

    // lurkPhantoms
    QJsonArray lurkArray;
    for (auto* phantom : lurkPhantoms) {
        if (phantom && phantom->scene()) {
            QJsonObject obj;
            obj["x"] = phantom->pos().x();
            obj["y"] = phantom->pos().y();
            obj["hp"] = phantom->getHp();
            obj["speed"] = phantom->getSpeed();
            obj["atk"] = phantom->getAtk();
            obj["atkRange"] = phantom->getAtkRange();
            lurkArray.append(obj);
        }
    }
    root["lurkPhantoms"] = lurkArray;

    // King
    if (king && king->scene()) {
        QJsonObject kingObj;
        kingObj["x"] = king->pos().x();
        kingObj["y"] = king->pos().y();
        kingObj["hp"] = king->getHp();
        kingObj["speed"] = king->getSpeed();
        kingObj["atk"] = king->getAtk();
        kingObj["atkRange"] = king->getAtkRange();
        root["king"] = kingObj;
    }

    // DropItems
    QJsonArray dropItemsArray;
    for (QGraphicsItem* item : gameScene->items()) {
        DropItem* drop = dynamic_cast<DropItem*>(item);
        if (drop) {
            QJsonObject dropObj;
            dropObj["type"] = static_cast<int>(drop->data(0).toInt());
            dropObj["x"] = drop->pos().x();
            dropObj["y"] = drop->pos().y();
            dropItemsArray.append(dropObj);
        }
    }
    root["dropItems"] = dropItemsArray;

    // Waves
    QJsonArray wavesArray;
    for (QGraphicsItem* item : gameScene->items()) {
        Wave* wave = dynamic_cast<Wave*>(item);
        if (wave) {
            QJsonObject waveObj;
            waveObj["type"] = wave->metaObject()->className();
            waveObj["x"] = wave->pos().x();
            waveObj["y"] = wave->pos().y();
            waveObj["atk"] = wave->getAtk();
            waveObj["direction_x"] = wave->getDirection().x();
            waveObj["direction_y"] = wave->getDirection().y();
            waveObj["curDistance"] = wave->getCurDistance();
            waveObj["maxDistance"] = wave->getMaxDistance();
            waveObj["angle"] = wave->getAngle();
            // PlayerWave专属
            if (waveObj["type"] == "PlayerWave") {
                PlayerWave* pw = static_cast<PlayerWave*>(wave);
                waveObj["maxPierceCnt"] = pw->getMaxPierceCnt();
                waveObj["curPierceCnt"] = pw->getCurPierceCnt();
                waveObj["aoeRadius"] = pw->getAoeRadius();
            }
            wavesArray.append(waveObj);
        }
    }
    root["waves"] = wavesArray;

    QJsonDocument doc(root);
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
}

void GameManager::loadFromJson(const QString& filename) {
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject root = doc.object();

        // 玩家
        QPointF pos(root["player_x"].toDouble(), root["player_y"].toDouble());
        player->setPos(pos);
        player->setHp(root["player_hp"].toInt());
        player->setMaxHp(root["player_maxHp"].toInt());
        player->setAtk(root["player_atk"].toInt());
        player->setAtkRange(root["player_atkRange"].toInt());
        player->setSightRange(root["player_sightRange"].toInt());
        float playerSpeed = root["player_speed"].toDouble();
        player->setSpeed(playerSpeed);
        player->setOriginSpeed(playerSpeed);
        player->setAtkCD(root["player_atkCD"].toInt());
        player->setPlayerWaveAoeRadius(root["player_waveAoeRadius"].toDouble());
        player->setPlayerWaveSpeed(root["player_waveSpeed"].toDouble());
        player->setTeleportCount(root["player_teleportCount"].toInt());

        // 地图
        curMapId = root["curMapId"].toInt();
        gameMap->initMap(curMapId);
        if (root.contains("bgmType")) {
            savedBgmType = root["bgmType"].toInt(0);
        } else {
            savedBgmType = 0;
        }
        countdownSeconds = root["countdownSeconds"].toInt();

        // flamePhantoms
        for (auto* phantom : flamePhantoms) {
            if (phantom) { phantom->scene()->removeItem(phantom); delete phantom; }
        }
        flamePhantoms.clear();
        QJsonArray flameArray = root["flamePhantoms"].toArray();
        for (const QJsonValue& v : flameArray) {
            QJsonObject obj = v.toObject();
            auto* phantom = new FlamePhantom(player);
            phantom->setPos(QPointF(obj["x"].toDouble(), obj["y"].toDouble()));
            phantom->setHp(obj["hp"].toInt());
            if (obj.contains("speed")) phantom->setSpeed(obj["speed"].toDouble());
            if (obj.contains("atk")) phantom->setAtk(obj["atk"].toInt());
            if (obj.contains("atkRange")) phantom->setAtkRange(obj["atkRange"].toInt());
            gameScene->addItem(phantom);
            flamePhantoms.append(phantom);
        }

        // lurkPhantoms
        for (auto* phantom : lurkPhantoms) {
            if (phantom) { phantom->scene()->removeItem(phantom); delete phantom; }
        }
        lurkPhantoms.clear();
        QJsonArray lurkArray = root["lurkPhantoms"].toArray();
        for (const QJsonValue& v : lurkArray) {
            QJsonObject obj = v.toObject();
            auto* phantom = new LurkPhantom(player);
            phantom->setPos(QPointF(obj["x"].toDouble(), obj["y"].toDouble()));
            phantom->setHp(obj["hp"].toInt());
            if (obj.contains("speed")) phantom->setSpeed(obj["speed"].toDouble());
            if (obj.contains("atk")) phantom->setAtk(obj["atk"].toInt());
            if (obj.contains("atkRange")) phantom->setAtkRange(obj["atkRange"].toInt());
            gameScene->addItem(phantom);
            lurkPhantoms.append(phantom);
        }

        // King
        if (root.contains("king")) {
            if (king) { king->scene()->removeItem(king); delete king; king = nullptr; }
            QJsonObject kingObj = root["king"].toObject();
            king = new King();
            king->setPos(QPointF(kingObj["x"].toDouble(), kingObj["y"].toDouble()));
            king->setHp(kingObj["hp"].toInt());
            if (kingObj.contains("speed")) king->setSpeed(kingObj["speed"].toDouble());
            if (kingObj.contains("atk")) king->setAtk(kingObj["atk"].toInt());
            if (kingObj.contains("atkRange")) king->setAtkRange(kingObj["atkRange"].toInt());
            gameScene->addItem(king);
        }

        inSecondCountdown = root["inSecondCountdown"].toBool(false);
        countdownSeconds = root["countdownSeconds"].toInt(0);

        // 清理现有掉落物
        for (QGraphicsItem* item : gameScene->items()) {
            if (dynamic_cast<DropItem*>(item)) {
                gameScene->removeItem(item);
                delete item;
            }
        }
        // 恢复掉落物
        QJsonArray dropItemsArray = root["dropItems"].toArray();
        for (const QJsonValue& v : dropItemsArray) {
            QJsonObject obj = v.toObject();
            DropType type = static_cast<DropType>(obj["type"].toInt());
            DropItem* drop = new DropItem(type);
            drop->setPos(QPointF(obj["x"].toDouble(), obj["y"].toDouble()));
            gameScene->addItem(drop);
        }

        // 清理现有Wave
        for (QGraphicsItem* item : gameScene->items()) {
            if (dynamic_cast<Wave*>(item)) {
                gameScene->removeItem(item);
                delete item;
            }
        }
        // 恢复Wave
        QJsonArray wavesArray = root["waves"].toArray();
        for (const QJsonValue& v : wavesArray) {
            QJsonObject obj = v.toObject();
            QString type = obj["type"].toString();
            QPointF pos(obj["x"].toDouble(), obj["y"].toDouble());
            QPointF dir(obj["direction_x"].toDouble(), obj["direction_y"].toDouble());
            int atk = obj["atk"].toInt();
            float curDist = float(obj["curDistance"].toDouble());
            float maxDist = float(obj["maxDistance"].toDouble());
            qreal angle = obj.contains("angle") ? obj["angle"].toDouble() : 0;
            Wave* wave = nullptr;
            if (type == "PlayerWave") {
                auto* pw = new PlayerWave(dir, atk);
                if (obj.contains("maxPierceCnt")) pw->setMaxPierceCnt(obj["maxPierceCnt"].toInt());
                if (obj.contains("curPierceCnt")) pw->setCurPierceCnt(obj["curPierceCnt"].toInt());
                if (obj.contains("aoeRadius")) pw->setAoeRadius(obj["aoeRadius"].toDouble());
                wave = pw;
            } else if (type == "FlameWave") {
                wave = new FlameWave(dir, atk);
            } else if (type == "LurkWave") {
                wave = new LurkWave(dir, atk);
            } else if (type == "KingWave") {
                wave = new KingWave(dir, atk);
            }
            if (wave) {
                wave->setPos(pos);
                wave->setCurDistance(curDist);
                wave->setMaxDistance(maxDist);
                wave->setAngle(angle);
                gameScene->addItem(wave);
            }
        }
        emit countdownChanged(countdownSeconds);
    }
}

