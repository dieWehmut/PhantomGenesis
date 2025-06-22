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
#include<Qicon>
#include <QMovie>
#include <QLabel>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , map(nullptr)
    , gameManager(nullptr)
    , gameStarted(false) 
{
    ui->setupUi(this);
    setWindowTitle("PhantomGenesis");
    setWindowIcon(QIcon("Resource/PhantomGenesis.ico"));
    ui->stackedWidget->setCurrentIndex(0);//初始界面
    //放音乐
    bgmPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    bgmPlayer->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);
    bgmPlayer->setLoops(QMediaPlayer::Infinite);

    playBgm(BgmType::StartPage);//开始界面音乐
    ui->graphicsView->installEventFilter(this);
    ui->graphicsView->viewport()->installEventFilter(this);
    setupGifBackground();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //各种按钮与界面的连接
    connect(ui->startBtn, &QPushButton::clicked, this, &MainWindow::startGame);
    connect(ui->exitBtn, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->backBtn,&QPushButton::clicked,this,&MainWindow::goBackToStartPage);
    connect(ui->backBtn_2, &QPushButton::clicked, this, &MainWindow::goBackToStartPage);
    connect(ui->backBtn_3, &QPushButton::clicked, this, &MainWindow::goBackToStartPage);
    connect(ui->backBtn_4, &QPushButton::clicked, this, &MainWindow::goBackToStartPage);
    connect(ui->saveBtn, &QPushButton::clicked, this, &MainWindow::saveGame);
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
    delete backgroundMovie;
    delete bgmPlayer;
    delete audioOutput;
}
void MainWindow::playBgm(BgmType type) {
    if (currentBgm == type) return;
    currentBgm = type;
    if (bgmPlayer) {
        bgmPlayer->stop();
    }
    switch (type) {
    case BgmType::StartPage:
        bgmPlayer->setSource(QUrl::fromLocalFile("Resource/startPage.mp3"));
        break;
    case BgmType::GamePage:
        bgmPlayer->setSource(QUrl::fromLocalFile("Resource/trapped0.flac"));
        break;
    default:
        bgmPlayer->setSource(QUrl());
        break;
    }
    if (type != BgmType::None) {
        bgmPlayer->play();
    }
}
void MainWindow::cleanupGame() {//进入ed前的准备
    if (gameManager) {
        delete gameManager;
        gameManager = nullptr;
    }
    map = nullptr;
    gameStarted = false;
    if (ui->graphicsView->scene()) {
        ui->graphicsView->setScene(nullptr);
    }
}
void MainWindow::onPlayerDead() {
    if (deadEndPlayed) return;//只放一次
    deadEndPlayed = true;
    cleanupGame();
    ui->stackedWidget->setCurrentWidget(ui->edScene);
    ui->stackedWidget_2->setCurrentWidget(ui->dePage);
    bgmPlayer->stop();
    bgmPlayer->setSource(QUrl::fromLocalFile("Resource/deadEnd.flac"));
    bgmPlayer->setLoops(1);
    bgmPlayer->play();
    connect(bgmPlayer, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status){
        if (status == QMediaPlayer::EndOfMedia) {
            onDeadEndMusicFinished();
        }
    });
}

void MainWindow::onDeadEndMusicFinished() {
    bgmPlayer->setLoops(QMediaPlayer::Infinite);
    ui->stackedWidget->setCurrentWidget(ui->startPage);
    playBgm(BgmType::StartPage);
    deadEndPlayed = false;
    disconnect(bgmPlayer, nullptr, this, nullptr);
}

void MainWindow::adjustBackgroundLabel() {
    if (backgroundLabel && ui->stackedWidget->currentIndex() == 0) {
        int pageWidth = ui->startPage->width();
        int pageHeight = ui->startPage->height();
        int gifWidth, gifHeight;
        if (pageWidth * 2 > pageHeight * 3) {
            gifHeight = pageHeight;
            gifWidth = pageHeight * 3 / 2;
        } else {
            gifWidth = pageWidth;
            gifHeight = pageWidth * 2 / 3;
        }
        int x = (pageWidth - gifWidth) / 2;
        int y = (pageHeight - gifHeight) / 2;
        backgroundLabel->setGeometry(x, y, gifWidth, gifHeight);
    }
}
void MainWindow::maintainAspectRatio(QResizeEvent *event) {
    static bool resizing = false;
    if (resizing) return;
    
    resizing = true;
    
    QSize newSize = event->size();
    int newWidth = newSize.width();
    int newHeight = newSize.height();
    int idealHeightForWidth = qRound(newWidth / aspectRatio);
    int idealWidthForHeight = qRound(newHeight * aspectRatio);
    if (idealHeightForWidth <= newHeight) {
        newHeight = idealHeightForWidth;
    } else {
        newWidth = idealWidthForHeight;
    }
    if (newWidth != width() || newHeight != height()) {
        resize(newWidth, newHeight);
    }
    
    resizing = false;
}
void MainWindow::setupGifBackground() {
    backgroundLabel = new QLabel(ui->startPage);
    backgroundMovie = new QMovie("Resource/startPage.gif");
    backgroundMovie->setCacheMode(QMovie::CacheAll);
    backgroundLabel->setMovie(backgroundMovie);
    backgroundLabel->setAlignment(Qt::AlignCenter);
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setGeometry(0, 0, ui->startPage->width(), ui->startPage->height());
    backgroundLabel->lower();
    backgroundMovie->start();
    adjustBackgroundLabel();
    QTimer::singleShot(0, this, [this](){ adjustBackgroundLabel(); });
}
bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if ((obj == ui->graphicsView || obj == ui->graphicsView->viewport()) && 
    (event->type() == QEvent::Wheel || event->type() == QEvent::GraphicsSceneWheel)) {
        event->accept();
        return true;
    }
    if (obj == ui->graphicsView) {
        switch(event->type()) {
        case QEvent::Wheel:
        case QEvent::GraphicsSceneWheel:
            return true;
        case QEvent::FocusIn: 
            ui->graphicsView->setFocus();
            return true;
        case QEvent::MouseButtonPress: 
            if (gameManager && gameStarted) {
                Player* player = gameManager->getPlayer();
                if (player) {
                    player->setFocus();
                }
            }
            break;
        default:
            break;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::startGame() {
    ui->stackedWidget->setCurrentIndex(1);
    playBgm(BgmType::GamePage);
    if (!gameManager) {
        gameManager = new GameManager(ui->graphicsView, this);
        connect(gameManager, &GameManager::playerDead, this, &MainWindow::onPlayerDead);
        gameManager->startGame();
        gameStarted = true;
    } else if (gameStarted) {
        returnToGame();
    } else {
        gameManager->startGame();
        gameStarted = true;
    }
    ui->pauseBtn->setText("暂停");//显示暂停
    ui->continueBtn->hide();//隐藏继续按钮
    
}
void MainWindow::togglePause() {
    if (gameManager) {
        if (gameManager->isGamePaused()) {
            gameManager->resumeGame();
            ui->pauseBtn->setText("暂停");
            ui->continueBtn->hide();
            if (bgmPlayer && bgmPlayer->source().isValid()) {
                bgmPlayer->play();
            }
        } else {
            gameManager->pauseGame();
            ui->pauseBtn->setText("继续");
            ui->continueBtn->show();
            if (bgmPlayer) {
                bgmPlayer->pause();
            }
        }
    }
}

void MainWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::ActivationChange) {
        if (gameManager && gameStarted) { 
            if (isActiveWindow()) {
                if (gameManager->isGamePaused()) { 
                    gameManager->resumeGame();
                    ui->pauseBtn->setText("暂停"); 
                    ui->continueBtn->hide(); 
                    if (bgmPlayer && bgmPlayer->source().isValid()) {
                        bgmPlayer->play();
                    }
                }
            } else {
                if (!gameManager->isGamePaused()) {
                    gameManager->pauseGame();
                    ui->pauseBtn->setText("继续"); 
                    ui->continueBtn->show(); 
                    if (bgmPlayer) {
                        bgmPlayer->pause();
                    }
                }
            }
        }
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::returnToGame() {
    ui->stackedWidget->setCurrentIndex(1);
    playBgm(BgmType::GamePage);
    if (gameManager) {
        ui->graphicsView->setGeometry(0, 0, ui->stackedWidget->width(), ui->stackedWidget->height());
        ui->graphicsView->setFocusPolicy(Qt::NoFocus);
        emit viewResized();
        if (gameStarted) {
            gameManager->resumeGame();
            ui->pauseBtn->setText("暂停");
            if (bgmPlayer && bgmPlayer->playbackState() != QMediaPlayer::PlayingState) {
                bgmPlayer->play();
            }
        }
        Player* player = gameManager->getPlayer();
        if (player) {
            player->setFocus();
        }
    }
    ui->continueBtn->hide();
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
    playBgm(BgmType::StartPage); 
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    QRect availableGeometry = screen()->availableGeometry();
    bool maximized = isMaximized();
    if (!maximized) {
        int w = qMin(width(), availableGeometry.width());
        int h = qMin(height(), availableGeometry.height());
        int idealH = qRound(w / aspectRatio);
        int idealW = qRound(h * aspectRatio);
        if (idealH > availableGeometry.height()) {
            h = availableGeometry.height();
            w = qRound(h * aspectRatio);
        } else if (idealW > availableGeometry.width()) {
            w = availableGeometry.width();
            h = qRound(w / aspectRatio);
        } else if (idealH != h) {
            h = idealH;
        } else if (idealW != w) {
            w = idealW;
        }
        if (w != width() || h != height()) {
            resize(w, h);
            return;
        }
    }
    ui->stackedWidget->setGeometry(0, 0, width(), height());
    adjustBackgroundLabel();
    int currentIndex = ui->stackedWidget->currentIndex();
    if (currentIndex == 0) { 
        int startBtnWidth = 181;
        int startBtnHeight = 51;
        int loadBtnWidth = 181;
        int loadBtnHeight = 61;
        int edBtnWidth = 181;
        int edBtnHeight = 51;
        int exitBtnWidth = 181;
        int exitBtnHeight = 51;
        double heightRatio = height() / 720.0; 
        ui->startBtn->setGeometry(40, int(230 * heightRatio), startBtnWidth, startBtnHeight);
        ui->loadBtn->setGeometry(40, int(320 * heightRatio), loadBtnWidth, loadBtnHeight);
        ui->edBtn->setGeometry(40, int(430 * heightRatio), edBtnWidth, edBtnHeight);
        ui->exitBtn->setGeometry(40, int(520 * heightRatio), exitBtnWidth, exitBtnHeight);
    }
    else if (currentIndex == 1) { 
        double widthRatio = width() / 1080.0;
        int backBtnWidth = 151;
        int backBtnHeight = 51;
        int saveBtnWidth = 151;
        int saveBtnHeight = 51;
        int loadBtnWidth = 141;
        int loadBtnHeight = 51;
        int pauseBtnWidth = 131;
        int pauseBtnHeight = 51;
        int helpBtnWidth = 131;
        int helpBtnHeight = 51;
        int settingBtnWidth = 121;
        int settingBtnHeight = 51;
        ui->backBtn_4->setGeometry(int(-10 * widthRatio), -10, backBtnWidth, backBtnHeight);
        ui->saveBtn->setGeometry(int(149 * widthRatio), -8, saveBtnWidth, saveBtnHeight);
        ui->loadBtn_2->setGeometry(int(309 * widthRatio), -8, loadBtnWidth, loadBtnHeight);
        ui->pauseBtn->setGeometry(int(460 * widthRatio), -10, pauseBtnWidth, pauseBtnHeight);
        ui->helpBtn->setGeometry(int(610 * widthRatio), -10, helpBtnWidth, helpBtnHeight);
        ui->settingBtn->setGeometry(int(759 * widthRatio), -8, settingBtnWidth, settingBtnHeight);
        int contentW = width();
        int contentH = height() - 49;
        int viewW = contentW;
        int viewH = contentH;
        int x = 0, y = 49;
        if (maximized) {
            if (contentW * 2 > contentH * 3) {
                viewH = contentH;
                viewW = contentH * 3 / 2;
            } else {
                viewW = contentW;
                viewH = contentW * 2 / 3;
            }
            x = (contentW - viewW) / 2;
            y = 49 + (contentH - viewH) / 2;
        }
        ui->graphicsView->setGeometry(x, y, viewW, viewH);

        int btnWidth = 421;
        int btnHeight = 261;
        int btnX = (width() - btnWidth) / 2;
        int btnY = (height() - btnHeight) / 2;
        ui->continueBtn->setGeometry(btnX, btnY, btnWidth, btnHeight);
    }
    else if (currentIndex == 2) {
        double heightRatio = height() / 720.0;
        ui->backBtn_2->setGeometry(0, int(550 * heightRatio), 151, 61);
    }
    else if (currentIndex == 3) { 
        double heightRatio = height() / 720.0;
        ui->backBtn_3->setGeometry(20, int(530 * heightRatio), 151, 61);
    }
    else if (currentIndex == 4) { 
        ui->backBtn->setGeometry(0, 10, 151, 61);
    }

    emit viewResized();
    if (gameManager && gameStarted) {//玩家视野居中
        Player* player = gameManager->getPlayer();
        if (player && ui->graphicsView->scene()) {
            ui->graphicsView->centerOn(player);
        }
    }
}

