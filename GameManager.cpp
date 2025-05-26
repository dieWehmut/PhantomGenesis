#include "GameManager.h"
#include <QDebug>
#include <QtMath>

GameManager::GameManager(QGraphicsView* graphicsView, QObject *parent) : QObject(parent), gameView(graphicsView), gameScene(nullptr), gameMap(nullptr), player(nullptr), gameLoopTimer(nullptr) {}

GameManager::~GameManager() {//要补全
    delete gameScene;
    delete gameMap;
    delete player;
    delete gameLoopTimer;
}

void GameManager::startGame() {
    gameScene = new QGraphicsScene(this);//创建场景
    gameView->setScene(gameScene);

    gameMap = new Map(gameScene);
    gameMap->initMap(1); // 地图1

    player = new Player(this);
    gameScene->addItem(player);
    player->setPos(gameMap->getMapBounds().center());

    // 各种阻止鼠标滚轮的滑动
    gameView->setFocusPolicy(Qt::StrongFocus);
    gameView->setFocus();
    gameView->setDragMode(QGraphicsView::NoDrag);
    gameView->setAlignment(Qt::AlignCenter);
    gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    gameView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    gameView->setRenderHint(QPainter::Antialiasing, false);
    gameView->setRenderHint(QPainter::SmoothPixmapTransform, false);
    gameView->setCacheMode(QGraphicsView::CacheBackground);

    gameLoopTimer = new QTimer(this);
    connect(gameLoopTimer, &QTimer::timeout, this, [=]() {
        player->updatePosition();
        player->setPos(qBound(0.0, player->x(), gameMap->getMapBounds().width() - gameMap->getGridSize()),qBound(0.0, player->y(), gameMap->getMapBounds().height() - gameMap->getGridSize()));
        gameView->centerOn(player); // 视野居中
        gameView->viewport()->update();
    });
    gameLoopTimer->start(1000/60);//60帧
}
