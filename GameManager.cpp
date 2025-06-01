#include "GameManager.h"
#include <QDebug>
#include <QtMath>
#include <QRandomGenerator>
#include <QEvent>
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
    int idx = QRandomGenerator::global()->bounded(spawnPoints.size());
    QPoint pt = spawnPoints[idx];
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
        phantom->setPos(pt.x() * gameMap->getGridSize(), pt.y() * gameMap->getGridSize());
        gameScene->addItem(phantom);
        container.append(phantom);
    }
}
GameManager::GameManager(QGraphicsView* graphicsView, QObject *parent) : QObject(parent), gameView(graphicsView), gameScene(nullptr), gameMap(nullptr), player(nullptr), gameLoopTimer(nullptr), gamePaused(false) { 
    if (parent) {
        connect(parent, SIGNAL(viewResized()), this, SLOT(handleViewResize()));
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
    curMapId = 1; //初始地图1
    gameMap->initMap(curMapId); //初始化地图
    player = new Player();//创建玩家
    gameScene->addItem(player);//放玩家
    phantomSpawnTimer->start(5000); //幻影生成时间间隔
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
        player->updatePosition();
        player->setPos(qBound(0.0, player->x(), gameMap->getMapBounds().width() - player->pixmap().width()),qBound(0.0, player->y(), gameMap->getMapBounds().height() - player->pixmap().height()));
        int gridSize = gameMap->getGridSize();
        int playerRow = static_cast<int>((player->y() + player->pixmap().height() / 2) / gridSize);
        int playerCol = static_cast<int>((player->x() + player->pixmap().width() / 2) / gridSize);
        if (gameMap->getTileType(playerRow, playerCol) == -1) {//传送
            int centerRow = 10;
            int centerCol = 12;
            player->setPos(centerCol * gridSize, centerRow * gridSize);
        }
        gameView->centerOn(player); // 视野居中
        gameView->viewport()->update();
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
}
void GameManager::separatePhantoms(QVector<PhantomBase*>& phantoms) {
    const qreal minDist = 32.0;
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
void GameManager::spawnFlamePhantoms() {
    if (!gameMap || !gameScene || !player) return;
    spawnPhantoms<FlamePhantom>(
        flamePhantoms,
        gameMap->getFlamePhantomBases(),
        20,
        [this]() { return new FlamePhantom(player); }
    );
}
void GameManager::updateFlamePhantoms() {
    for (int i = flamePhantoms.size() - 1; i >= 0; i--) {
        if (!flamePhantoms[i] || !flamePhantoms[i]->scene() || flamePhantoms[i]->getHp() <= 0) {
            if (flamePhantoms[i]) {
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
}
void GameManager::spawnLurkPhantoms() {
    if (!gameMap || !gameScene || !player) return;
    spawnPhantoms<LurkPhantom>(
        lurkPhantoms,
        gameMap->getLurkPhantomBases(),
        12,
        [this]() { return new LurkPhantom(player); }
    );
}
void GameManager::updateLurkPhantoms() {
    for (int i = lurkPhantoms.size() - 1; i >= 0; i--) {
        if (!lurkPhantoms[i] || !lurkPhantoms[i]->scene() || lurkPhantoms[i]->getHp() <= 0) {
            if (lurkPhantoms[i]) {
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
}