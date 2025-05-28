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
#include <QResizeEvent>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , map(nullptr)
    , gameManager(nullptr)  
    , gameStarted(false) 
{
    ui->setupUi(this);
    setWindowTitle("PhantomGenesis");
    ui->stackedWidget->setCurrentIndex(0);//初始界面
    //各种按钮与界面的连接
    connect(ui->startBtn, &QPushButton::clicked, this, &MainWindow::startGame);
    connect(ui->exitBtn, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->backBtn,&QPushButton::clicked,this,&MainWindow::goBackToStartPage);
    connect(ui->backBtn_2, &QPushButton::clicked, this, &MainWindow::goBackToStartPage);
    connect(ui->backBtn_3, &QPushButton::clicked, this, &MainWindow::goBackToStartPage);
    connect(ui->backBtn_4, &QPushButton::clicked, this, &MainWindow::goBackToStartPage);
    connect(ui->saveBtn, &QPushButton::clicked, this, &MainWindow::saveGame);
    connect(ui->saveBtn_2, &QPushButton::clicked, this, &MainWindow::saveGame);
    connect(ui->loadBtn, &QPushButton::clicked, this, &MainWindow::loadGame);
    connect(ui->loadBtn_2, &QPushButton::clicked, this, &MainWindow::loadGame);
    connect(ui->edBtn, &QPushButton::clicked, this, &MainWindow::showEdPage);
    connect(ui->pauseBtn, &QPushButton::clicked, this, &MainWindow::togglePause);
    connect(ui->continueBtn, &QPushButton::clicked, this, &MainWindow::returnToGame);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gameManager;
}
void MainWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::ActivationChange) {
        if (gameManager && gameStarted) { 
            if (isActiveWindow()) {
                if (gameManager->isGamePaused()) { 
                    gameManager->resumeGame();
                    ui->pauseBtn->setText("暂停"); 
                    ui->continueBtn->hide(); 
                }
            } else {
                if (!gameManager->isGamePaused()) {
                    gameManager->pauseGame();
                    ui->pauseBtn->setText("继续"); 
                    ui->continueBtn->show(); 
                }
            }
        }
    }
    QMainWindow::changeEvent(event);
}
bool MainWindow::eventFilter(QObject* obj, QEvent* event)//防滑动
{
    if (obj == ui->graphicsView) {
        switch(event->type()) {
        case QEvent::Wheel:
        case QEvent::GraphicsSceneWheel:
            return true;
        case QEvent::FocusIn: 
            ui->graphicsView->setFocus();
            return true;
        default:
            break;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::startGame(){//game开始
    ui->stackedWidget->setCurrentIndex(1);
    if (!gameManager) {
        gameManager = new GameManager(ui->graphicsView, this);
        gameManager->startGame();
        gameStarted = true;
    } else if (gameStarted) {
        returnToGame();
    } else {
        gameManager->startGame();
        gameStarted = true;
    }
    ui->pauseBtn->setText("暂停"); //显示暂停
    ui->continueBtn->hide(); // 隐藏继续按钮
}
void MainWindow::returnToGame(){
    ui->stackedWidget->setCurrentIndex(1);
    if (gameManager) {
        ui->graphicsView->setGeometry(0, 0, ui->stackedWidget->width(), ui->stackedWidget->height());
        ui->graphicsView->setFocusPolicy(Qt::StrongFocus);
        ui->graphicsView->setFocus();
        emit viewResized();
        if (gameStarted) {
            gameManager->resumeGame();
            ui->pauseBtn->setText("暂停"); 
        }
    }
    ui->continueBtn->hide(); 
}
void MainWindow::togglePause() {
    if (gameManager) {
        if (gameManager->isGamePaused()) {
            gameManager->resumeGame();
            ui->pauseBtn->setText("暂停");
            ui->continueBtn->hide(); 
        } else {
            gameManager->pauseGame();
            ui->pauseBtn->setText("继续");
            ui->continueBtn->show(); 
        }
    }
}
void MainWindow::loadGame(){//读取，先简单实现
    int prevIndex = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::showEdPage(){//ED鉴赏
    int prevIndex = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::saveGame(){//保存
    int prevIndex = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::goBackToStartPage(){//返回标题
    if (gameManager && gameStarted) {
    }
    ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::resizeEvent(QResizeEvent *event){
    QMainWindow::resizeEvent(event);
    ui->stackedWidget->setGeometry(0, 0, width(), height());
    if (ui->stackedWidget->currentIndex() == 1) {
        ui->graphicsView->setGeometry(0, 0, ui->stackedWidget->width(), ui->stackedWidget->height());
        emit viewResized();
    }
}
