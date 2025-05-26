#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GameManager.h"
#include <QTimer>
#include <QEvent>
#include <QWheelEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QtMath>

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
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)//防滑动
{
    if (obj == ui->graphicsView) {
        switch(event->type()) {
        case QEvent::Wheel:
        case QEvent::GraphicsSceneWheel:
            return true;
        default:
            break;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::startGame(){//game开始
    ui->stackedWidget->setCurrentIndex(1);
    GameManager* gameManager = new GameManager(ui->graphicsView, this);
    gameManager->startGame();
}
