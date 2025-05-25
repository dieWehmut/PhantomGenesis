#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Player.h"
#include "Map.h"
#include <QTimer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , map(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("PhantomGenesis");
    setFixedSize(1080,720);
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->startBtn, &QPushButton::clicked, this, &MainWindow::startGame);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete map;
}
void MainWindow::startGame(){//开始游戏
    ui->stackedWidget->setCurrentIndex(1);
    QGraphicsScene* scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    map = new Map(scene);
    map->initMap(1); // 第一关
    Player* player = new Player(this);
    scene->addItem(player);
    player->setPos(map->getMapBounds().center());
    ui->graphicsView->setFocusPolicy(Qt::StrongFocus);
    ui->graphicsView->setFocus();
    QTimer* gameLoop = new QTimer(this);
    connect(gameLoop, &QTimer::timeout, [=]() {
    player->updatePosition();
    player->setPos(qBound(0.0, player->x(), map->getMapBounds().width() - map->getGridSize()), qBound(0.0, player->y(), map->getMapBounds().height() - map->getGridSize()));});
    ui->graphicsView->setAlignment(Qt::AlignCenter); 
    gameLoop->start(16);
}
