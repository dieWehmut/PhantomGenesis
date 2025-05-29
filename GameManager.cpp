#include "GameManager.h"
#include <QDebug>
#include <QtMath>
#include <QRandomGenerator>
#include <QEvent>
GameManager::GameManager(QGraphicsView* graphicsView, QObject *parent) : QObject(parent), gameView(graphicsView), gameScene(nullptr), gameMap(nullptr), player(nullptr), gameLoopTimer(nullptr), gamePaused(false) { 
    if (parent) {
        connect(parent, SIGNAL(viewResized()), this, SLOT(handleViewResize()));
    }
    phantomSpawnTimer = new QTimer(this);
    connect(phantomSpawnTimer, &QTimer::timeout, this, &GameManager::spawnFlamePhantoms);
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
    curMapId = 1; // 初始地图1
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
        updateVisibleTiles();
        gameView->centerOn(player); // 视野居中
        gameView->viewport()->update();
        updatePhantoms();
    });

    gameLoopTimer->start(1000/60);
}
void GameManager::updateVisibleTiles() {
    if (!player || !gameMap) return;
    // 算玩家视野区
    qreal sightRange = player->getSightRange();
    qreal playerCenterX = player->x() + player->pixmap().width() / 2;
    qreal playerCenterY = player->y() + player->pixmap().height() / 2;
    QRectF visibleArea(playerCenterX - sightRange,playerCenterY - sightRange,sightRange * 2,sightRange * 2);
    int gridSize = gameMap->getGridSize();
    // 算视野格子索引
    int startRow = qMax(0, static_cast<int>((visibleArea.top()) / gridSize));
    int endRow = qMin(gameMap->getGridRow() - 1, static_cast<int>((visibleArea.bottom()) / gridSize));
    int startCol = qMax(0, static_cast<int>((visibleArea.left()) / gridSize));
    int endCol = qMin(gameMap->getGridCol() - 1, static_cast<int>((visibleArea.right()) / gridSize));
    for (int row = startRow; row <= endRow; row++) {
        for (int col = startCol; col <= endCol; col++) {
            QRectF tileRect(col * gridSize, row * gridSize, gridSize, gridSize);
            bool isVisible = visibleArea.intersects(tileRect);
            int targetType = isVisible ? gameMap->getOriginalTileType(row, col, curMapId) : 0;
            if (gameMap->getTileType(row, col) != targetType) {
                gameMap->updateSingleTile(row, col, targetType);
            }
        }
    }
    for (int row = 0; row < gameMap->getGridRow(); row++) {
        for (int col = 0; col < gameMap->getGridCol(); col++) {
            if (row >= startRow && row <= endRow && col >= startCol && col <= endCol) {
                continue;
            }
            if (gameMap->getTileType(row, col) != 0) {
                gameMap->updateSingleTile(row, col, 0);
            }
        }
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
}

void GameManager::spawnFlamePhantoms() {
    if (!gameMap || !gameScene || !player) return;
    for (int i = flamePhantoms.size() - 1; i >= 0; --i) {
        if (!flamePhantoms[i] || !flamePhantoms[i]->scene() || flamePhantoms[i]->getHp() <= 0) {
            if (flamePhantoms[i]) {
                gameScene->removeItem(flamePhantoms[i]);
                delete flamePhantoms[i];
            }
            flamePhantoms.remove(i);
        }
    }
    if (flamePhantoms.size() >= 24) return;//限制
    const QVector<QPoint>& spawnPoints = gameMap->getFlamePhantomBases();
    if (!spawnPoints.isEmpty()) {
        int idx = QRandomGenerator::global()->bounded(spawnPoints.size());
        QPoint pt = spawnPoints[idx];
        bool occupied = false;
        for (auto* phantom : flamePhantoms) {
            if (phantom && phantom->scene() &&
                (phantom->pos().x() / gameMap->getGridSize() == pt.x()) &&
                (phantom->pos().y() / gameMap->getGridSize() == pt.y())) {
                occupied = true;
                break;
            }
        }
        if (!occupied) {
            FlamePhantom* phantom = new FlamePhantom(player);
            phantom->setPos(pt.x() * gameMap->getGridSize(), pt.y() * gameMap->getGridSize());
            gameScene->addItem(phantom);
            flamePhantoms.append(phantom);
        }
    }
}

void GameManager::updatePhantoms() {
    for (int i = flamePhantoms.size() - 1; i >= 0; i--) {
        if (!flamePhantoms[i] || !flamePhantoms[i]->scene() || flamePhantoms[i]->getHp() <= 0) {
            if (flamePhantoms[i]) {
                gameScene->removeItem(flamePhantoms[i]);
                delete flamePhantoms[i];
            }
            flamePhantoms.remove(i);
        }
    }
    for (auto* phantom : flamePhantoms) {
        if (phantom && phantom->scene()) {
            phantom->updateStatus();
        }
    }
        bool playerShouldSlow = false;
    for (auto* phantom : flamePhantoms) {
        if (phantom && phantom->scene()) {
            QPointF center = phantom->pos() + QPointF(phantom->boundingRect().width()/2, phantom->boundingRect().height()/2);
            QPointF pCenter = player->pos() + QPointF(player->boundingRect().width()/2, player->boundingRect().height()/2);
            if (QLineF(center, pCenter).length() <= phantom->getAtkRange()) {
                playerShouldSlow = true;
                break;
            }
        }
    }
    player->setSlowed(playerShouldSlow);
}
