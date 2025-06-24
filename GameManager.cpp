#include "GameManager.h"
#include <QDebug>
#include <QtMath>
#include <QRandomGenerator>
#include <QEvent>
#include"DropItem.h"
#include "mainwindow.h"
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
    if (parent) {
        connect(parent, SIGNAL(viewResized()), this, SLOT(handleViewResize()));
        connect(parent, SIGNAL(forcePhantomsChasePlayer()), this, SLOT(onForcePhantomsChasePlayer()));
    }
    phantomSpawnTimer = new QTimer(this);
    connect(phantomSpawnTimer, &QTimer::timeout, this, &GameManager::spawnFlamePhantoms);
    connect(phantomSpawnTimer, &QTimer::timeout, this, &GameManager::spawnLurkPhantoms);
    
}
void GameManager::pauseGame() {
    if (gameLoopTimer && gameLoopTimer->isActive()) {
        gameLoopTimer->stop();
        gamePaused = true;
    }
}
void GameManager::onForcePhantomsChasePlayer() {
    PhantomBase::setForceChasePlayer(true);
}
void GameManager::resumeGame() {
    if (gameLoopTimer && !gameLoopTimer->isActive()) {
        gameLoopTimer->start(1000/60);
        gamePaused = false; 
    }
    if (gameView) {
        gameView->setFocusPolicy(Qt::StrongFocus);
        gameView->setFocus();
    }
    if (player) {
        player->clearPressedKeys(); // 清除按键集
        player->setFocus();
    }
}
GameManager::~GameManager() {//要补全,记得不要二次释放
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
    phantomSpawnTimer->start(3000); //phantom生成时间间隔
    player->setPos(gameMap->getMapBounds().center());

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

    //游戏主循环
    connect(gameLoopTimer, &QTimer::timeout, this, [=]() {
        //各种结局
        if (player->getHp() <= 0) {
            emit playerDead();
            gameLoopTimer->stop();
            phantomSpawnTimer->stop();
            return;
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
        int gridSize = gameMap->getGridSize();
        int playerRow = static_cast<int>((player->y() + player->pixmap().height() / 2) / gridSize);
        int playerCol = static_cast<int>((player->x() + player->pixmap().width() / 2) / gridSize);
        if (gameMap->getTileType(playerRow, playerCol) == -1) {//传送
            MainWindow* mw = qobject_cast<MainWindow*>(parent());
            bool portalEnabled = true;
            if (mw) portalEnabled = mw->isPortalEnabled();
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
                }
            }
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
void GameManager::spawnFlamePhantoms() {
    if (!gameMap || !gameScene || !player) return;
    spawnPhantoms<FlamePhantom>(
        flamePhantoms,
        gameMap->getFlamePhantomBases(),
        10,
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
        10,
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
            phantom->setSpeed(phantom->getSpeed() * 3.0f);
        }
    }
    for (auto* phantom : lurkPhantoms) {
        if (phantom) {
            phantom->setSpeed(phantom->getSpeed() * 4.0f);
        }
    }
}