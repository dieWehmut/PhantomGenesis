#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsDropShadowEffect>
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
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , map(nullptr)
    , gameManager(nullptr)
    , gameStarted(false) 
{
    ui->setupUi(this);
    setWindowTitle("PhantomGenesis");
    setWindowIcon(QIcon(":/Resource/PhantomGenesis.ico"));

    ui->stackedWidget->setCurrentIndex(0);//初始界面
    ui->label->setText("PhantomGenesis");
    ui->label->setStyleSheet(
        "background: transparent;"
        "font-size: 32px;"
        "font-weight: 400;"
        "color: #66d9cc;"          
        "letter-spacing: 1px;"
        "border: none;"

    );

    auto *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(32);
    effect->setColor(QColor("#ff00cc")); 
    effect->setOffset(0, 0);
    ui->label->setGraphicsEffect(effect);

    ui->graphicsView->installEventFilter(this);
    ui->graphicsView->viewport()->installEventFilter(this);
    setupBackground();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //各种按钮与界面的连接
    connect(ui->startBtn, &QPushButton::clicked, this, &MainWindow::startGame);
    connect(ui->exitBtn, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->backBtn,&QPushButton::clicked,this,&MainWindow::goBackToStartPage);
    connect(ui->backBtn_2, &QPushButton::clicked, this, &MainWindow::goBackToStartPage);
    connect(ui->backBtn_3, &QPushButton::clicked, this, &MainWindow::goBackToStartPage);
    connect(ui->backBtn_4, &QPushButton::clicked, this, &MainWindow::goBackToStartPage);
    connect(ui->backBtn_5, &QPushButton::clicked, this, &MainWindow::goBackToStartPage);
    connect(ui->skipBtn, &QPushButton::clicked, this, &MainWindow::skipEnd);
    connect(ui->skipBtn_2, &QPushButton::clicked, this, &MainWindow::skipEnd);
    connect(ui->skipBtn_3, &QPushButton::clicked, this, &MainWindow::skipEnd);
    connect(ui->skipBtn_4, &QPushButton::clicked, this, &MainWindow::skipEnd);
    connect(ui->saveBtn, &QPushButton::clicked, this, &MainWindow::saveAndLoadGame);
    connect(ui->loadBtn, &QPushButton::clicked, this, &MainWindow::saveAndLoadGame);
    connect(ui->loadBtn_2, &QPushButton::clicked, this, &MainWindow::saveAndLoadGame);
    connect(ui->edBtn, &QPushButton::clicked, this, &MainWindow::showEdPage);
    connect(ui->pauseBtn, &QPushButton::clicked, this, &MainWindow::togglePause);
    connect(ui->continueBtn, &QPushButton::clicked, this, &MainWindow::returnToGame);
    connect(ui->continueBtn_2, &QPushButton::clicked, this, &MainWindow::returnToGame);
    connect(ui->continueBtn_3, &QPushButton::clicked, this, &MainWindow::returnToGame);
    connect(ui->continueBtn_4, &QPushButton::clicked, this, &MainWindow::returnToGame);
    connect(ui->continueBtn_5, &QPushButton::clicked, this, &MainWindow::returnToGame);
    connect(ui->helpBtn, &QPushButton::clicked, this, &MainWindow::showHelpPage);
    connect(ui->helpBtn_2, &QPushButton::clicked, this, &MainWindow::showHelpPage);
    connect(ui->settingBtn, &QPushButton::clicked, this, &MainWindow::showSettingsPage);
    connect(ui->settingBtn_2, &QPushButton::clicked, this, &MainWindow::showSettingsPage);
    connect(ui->deBtn, &QPushButton::clicked, this, [this]() {
        onEnd(BgmType::DeadEnd, ui->dePage, &deadEndPlayed);
    });
    connect(ui->leBtn, &QPushButton::clicked, this, [this]() {
        onEnd(BgmType::LostEnd, ui->lePage, &lostEndPlayed);
    });
    connect(ui->teBtn, &QPushButton::clicked, this, [this]() {
        onEnd(BgmType::TrueEnd, ui->tePage, &trueEndPlayed);
    });
    connect(ui->feBtn, &QPushButton::clicked, this, [this]() {
        onEnd(BgmType::FakeEnd, ui->fePage, &fakeEndPlayed);
    });
    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [this](int idx) {
        if (ui->stackedWidget->widget(idx) == ui->gamePage) {
            if (gameStarted && gameManager) {
                if (gameManager->isSecondCountdown()) {
                    playBgm(BgmType::Trapped1);
                } else {
                    playBgm(BgmType::Trapped0);
                }
            }
        }
    });
    for (int i = 0; i < 8; ++i) {
        QString btnName = QString("archive_%1").arg(i + 1);
        QPushButton* btn = findChild<QPushButton*>(btnName);
        if (btn) {
            connect(btn, &QPushButton::clicked, this, [this, i]() { onSaveSlotClicked(i); });
        }
    }
    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [this](int idx) {//自动保存
        QWidget* cur = ui->stackedWidget->widget(idx);
        static int lastIndex = ui->stackedWidget->currentIndex();
        QWidget* lastWidget = ui->stackedWidget->widget(lastIndex);
        if (lastWidget == ui->gamePage && cur != ui->gamePage) {
            QFile::remove("last_game.json");
            saveLastGame();
        }
        lastIndex = idx;
    });
    QPushButton* confirmBtn = findChild<QPushButton*>("confirmBtn");
    QPushButton* cancelBtn = findChild<QPushButton*>("cancelBtn");
    if (confirmBtn) connect(confirmBtn, &QPushButton::clicked, this, &MainWindow::onConfirmSave);
    if (cancelBtn) connect(cancelBtn, &QPushButton::clicked, this, &MainWindow::onCancelSave);



    ui->continueBtn->setEnabled(false);
    ui->continueBtn_2->setEnabled(false);
    ui->continueBtn_3->setEnabled(false);
    ui->continueBtn_4->setEnabled(false);
    ui->continueBtn_5->setEnabled(false); 

    //按钮样式
    QString BtnStyle = R"(
        QPushButton {
            background-color: #e9dce5;
            color: #444444;
            border: 2px solid #d6c7ef;
            border-radius: 18px;
            font-size: 24px;

            font-weight: 500;
            letter-spacing: 1.5px;
            padding: 8px 0;
        }
        QPushButton:hover {
            background-color: #f3eaf2;
            color: #333333;
            border: 2px solid #c7def0;
        }
        QPushButton:pressed {
            background-color: #d8cde2;
            color: #222222;
            border: 2px solid #bfdbe8;
        }
    )";

    QList<QPushButton*> allBtns = {
        // startPage
        ui->startBtn,
        ui->continueBtn_2,
        ui->loadBtn,
        ui->helpBtn_2,
        ui->edBtn,
        ui->settingBtn_2,
        ui->exitBtn,
        // gamePage
        ui->backBtn_4,
        ui->saveBtn,
        ui->loadBtn_2,
        ui->pauseBtn,
        ui->helpBtn,
        ui->settingBtn,
        ui->continueBtn,
        // saveAndLoadPage
        ui->backBtn_3,
        ui->continueBtn_5,
        ui->archive_1,
        ui->archive_2,
        ui->archive_3,
        ui->archive_4,
        ui->archive_5,
        ui->archive_6,
        ui->archive_7,
        ui->archive_8,
        // 存档确认弹窗
        findChild<QPushButton*>("confirmBtn"),
        findChild<QPushButton*>("cancelBtn"),
        // settingsPage
        ui->backBtn_2,
        ui->continueBtn_3,
        // helpPage
        ui->backBtn_5,
        ui->continueBtn_4,
        // edPage
        ui->backBtn,
        ui->leBtn,
        ui->teBtn,
        ui->feBtn,
        ui->deBtn,
        // edScene/结局页面
        ui->skipBtn,
        ui->skipBtn_2,
        ui->skipBtn_3,
        ui->skipBtn_4
    };
    for (auto btn : allBtns) {
        if (btn) {
            btn->setStyleSheet(BtnStyle);
            auto *effect = new QGraphicsDropShadowEffect(btn);
            effect->setBlurRadius(18);
            effect->setColor(QColor("#00ffe7"));
            effect->setOffset(0, 0);
            btn->setGraphicsEffect(effect);
        }
    }
    QString LabelStyle = R"(
        color: #66d9cc;         
        background-color: transparent;
        font-size: 28px;

        font-weight: 400;          
        letter-spacing: 1px;
        border: none;
    )";
    QList<QLabel*> allLabels = {
        ui->countdownLabel,
        ui->endingCounterLabel
    };
    for (auto label : allLabels) {
        if (label) {
            label->setStyleSheet(LabelStyle);
            auto *effect = new QGraphicsDropShadowEffect(label);
            effect->setBlurRadius(24);
            effect->setColor(QColor("#00ffe7"));
            effect->setOffset(0, 0);
            label->setGraphicsEffect(effect);
        }
    }
    QWidget* confirmWidget = findChild<QWidget*>("confirmWidget");
    if (confirmWidget) {
        confirmWidget->setStyleSheet(R"(
            background-color: rgba(255,255,255,0.92);
            border-radius: 24px;
            border: 3px solid #bfa6ff;
        )");
        QPushButton* confirmBtn = confirmWidget->findChild<QPushButton*>("confirmBtn");
        QPushButton* cancelBtn = confirmWidget->findChild<QPushButton*>("cancelBtn");
        QString BtnStyle = R"(
            QPushButton {
                background: #cba0aa;
                color: #222;
                border: 2px solid #bfa6ff;
                border-radius: 18px;
                font-size: 26px;
                font-weight: bold;
                letter-spacing: 2px;
                padding: 8px 0;
            }
            QPushButton:hover {
                background: #e0c3fc;
                color: #000;
                border: 2px solid #aeefff;
            }
            QPushButton:pressed {
                background: #bfa6ff;
                color: #000;
                border: 2px solid #aeefff;
            }
        )";
        if (confirmBtn) confirmBtn->setStyleSheet(BtnStyle);
        if (cancelBtn) cancelBtn->setStyleSheet(BtnStyle);

        // 设置确认文本样式
        QLabel* confirmText = confirmWidget->findChild<QLabel*>("confirmText");
        if (confirmText) {
            auto *effect = new QGraphicsDropShadowEffect(confirmText);
            effect->setBlurRadius(16);
            effect->setColor(QColor("#bfa6ff"));
            effect->setOffset(0, 0);
            confirmText->setGraphicsEffect(effect);
        }
    }
    loadEndingsRecord();
    updateEndingButtonsAndCounter();
    ui->deBtn->setVisible(false);
    ui->leBtn->setVisible(false);
    ui->teBtn->setVisible(false);
    ui->feBtn->setVisible(false);
    endingsWatcher = new QFileSystemWatcher(this);
    if (QFile::exists("endings.json")) {
        endingsWatcher->addPath("endings.json");
    }
    connect(endingsWatcher, &QFileSystemWatcher::fileChanged, this, [this](const QString& path){
        if (path == "endings.json") {
            loadEndingsRecord();
            updateEndingButtonsAndCounter();
            if (!endingsWatcher->files().contains("endings.json") && QFile::exists("endings.json")) {
                endingsWatcher->addPath("endings.json");
            }
        }
    });
    lastGameWatcher = new QFileSystemWatcher(this);
    lastGameWatcher->addPath("last_game.json");
    connect(lastGameWatcher, &QFileSystemWatcher::fileChanged, this, [this](const QString&) {
        updateContinueButtons();
        if (!QFile::exists("last_game.json")) {
            lastGameWatcher->removePath("last_game.json");
        } else if (!lastGameWatcher->files().contains("last_game.json")) {
            lastGameWatcher->addPath("last_game.json");
        }
    });
    savesWatcher = new QFileSystemWatcher(this);
    QStringList saveFiles;
    for (int i = 1; i <= 8; ++i) {
        QString file = QString("save%1.json").arg(i);
        saveFiles << file;
    }
    savesWatcher->addPaths(saveFiles);
    connect(savesWatcher, &QFileSystemWatcher::fileChanged, this, [this](const QString& path) {
        refreshSaveSlotButtons();
        if (QFile::exists(path) && !savesWatcher->files().contains(path)) {
            savesWatcher->addPath(path);
        }
    });
    updateContinueButtons();
    QTimer::singleShot(0, this, [this]() {
        bgmPlayer = new QMediaPlayer(this);
        audioOutput = new QAudioOutput(this);
        bgmPlayer->setAudioOutput(audioOutput);
        bgmPlayer->setLoops(QMediaPlayer::Infinite);
        playBgm(BgmType::StartPage);
    });
}

MainWindow::~MainWindow()
{
    QFile::remove("last_game.json");
            QFile::remove("last_game.json");
            saveLastGame();
    delete ui;
}

void MainWindow::showHelpPage() {
                QFile::remove("last_game.json");
            saveLastGame();
    ui->stackedWidget->setCurrentWidget(ui->helpPage);
    if (gameManager && gameStarted && !gameManager->isGamePaused()) {
        gameManager->pauseGame();
        ui->pauseBtn->setText("继续");
        if (bgmPlayer) bgmPlayer->pause();
        gameManager->stopCountdown(); 
    }
}
void MainWindow::showSettingsPage() {
                QFile::remove("last_game.json");
            saveLastGame();
    ui->stackedWidget->setCurrentWidget(ui->settingsPage);
    if (gameManager && gameStarted && !gameManager->isGamePaused()) {
        gameManager->pauseGame();
        ui->pauseBtn->setText("继续");
        if (bgmPlayer) bgmPlayer->pause();
        gameManager->stopCountdown(); 
    }
}

void MainWindow::playBgm(BgmType type) {
    
    if (currentBgm == type) return;
    currentBgm = type;
    if (type == BgmType::Trapped0 || type == BgmType::Trapped1) {
        lastGameBgm = type;
    }
    if (bgmPlayer) {
        bgmPlayer->stop();
    }
    switch (type) {
    case BgmType::StartPage:
        bgmPlayer->setSource(QUrl("qrc:/Resource/startPage.mp3"));
        break;
    case BgmType::Trapped0:
        bgmPlayer->setSource(QUrl("qrc:/Resource/trapped0.mp3"));
        break;
    case BgmType::Trapped1:
        bgmPlayer->setSource(QUrl("qrc:/Resource/trapped1.mp3"));
        break;
    case BgmType::DeadEnd:
        bgmPlayer->setSource(QUrl("qrc:/Resource/deadEnd.mp3"));
        break;
    case BgmType::LostEnd:
        bgmPlayer->setSource(QUrl("qrc:/Resource/lostEnd.mp3"));
        break;
    case BgmType::TrueEnd:
        bgmPlayer->setSource(QUrl("qrc:/Resource/trueEnd.mp3"));
        break;
    case BgmType::FakeEnd:
        bgmPlayer->setSource(QUrl("qrc:/Resource/fakeEnd.mp3"));
        break;
    default:
        bgmPlayer->setSource(QUrl());
        break;
    }
    audioOutput->setVolume(0.5);
    if (type != BgmType::None) {
        bgmPlayer->play();
    }
}


void MainWindow::cleanupGame() {//进入ed前的准备
    if (gameManager) {
        disconnect(gameManager, nullptr, this, nullptr);
        gameManager->pauseGame();
    }
    if (ui->graphicsView->scene()) {
        ui->graphicsView->setScene(nullptr);
    }
    if (gameManager) {
        delete gameManager;
        gameManager = nullptr;
    }
    map = nullptr;
    gameStarted = false;
}



void MainWindow::adjustBackgroundLabel() {
    if (startPageBgLabel && ui->stackedWidget->currentIndex() == 0) {
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
        startPageBgLabel->setGeometry(x, y, gifWidth, gifHeight);
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
void MainWindow::setupBackground() {
    // 开始界面GIF背景
    startPageBgLabel = new QLabel(ui->startPage);
    startPageBgMovie = new QMovie(":/Resource/startPage.gif");
    startPageBgMovie->setCacheMode(QMovie::CacheAll);
    startPageBgLabel->setMovie(startPageBgMovie);
    startPageBgLabel->setAlignment(Qt::AlignCenter);
    startPageBgLabel->setScaledContents(true);
    startPageBgLabel->setGeometry(0, 0, ui->startPage->width(), ui->startPage->height());
    startPageBgLabel->lower();
    startPageBgMovie->start();
    adjustBackgroundLabel();
    QTimer::singleShot(0, this, [this](){ adjustBackgroundLabel(); });

    // 其他页面图片背景
    saveAndLoadPageBgLabel = new QLabel(ui->saveAndLoadPage);
    saveAndLoadPageBgLabel->setPixmap(QPixmap(":/Resource/saveAndLoadPage.png"));
    saveAndLoadPageBgLabel->setScaledContents(true);
    saveAndLoadPageBgLabel->setGeometry(ui->saveAndLoadPage->rect());
    saveAndLoadPageBgLabel->lower();

    edPageBgLabel = new QLabel(ui->edPage);
    edPageBgLabel->setPixmap(QPixmap(":/Resource/edPage.png"));
    edPageBgLabel->setScaledContents(true);
    edPageBgLabel->setGeometry(ui->edPage->rect());
    edPageBgLabel->lower();

    settingsPageBgLabel = new QLabel(ui->settingsPage);
    settingsPageBgLabel->setPixmap(QPixmap(":/Resource/settingsPage.png"));
    settingsPageBgLabel->setScaledContents(true);
    settingsPageBgLabel->setGeometry(ui->settingsPage->rect());
    settingsPageBgLabel->lower();

    helpPageBgLabel = new QLabel(ui->helpPage);
    helpPageBgLabel->setPixmap(QPixmap(":/Resource/helpPage.png"));
    helpPageBgLabel->setScaledContents(true);
    helpPageBgLabel->setGeometry(ui->helpPage->rect());
    helpPageBgLabel->lower();
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
    QMediaPlayer* clickPlayer = new QMediaPlayer(this);
    QAudioOutput* clickAudio = new QAudioOutput(this);
    clickPlayer->setAudioOutput(clickAudio);
    clickAudio->setVolume(1.0);
    clickPlayer->setSource(QUrl("qrc:/Resource/click.wav"));
    connect(clickPlayer, &QMediaPlayer::mediaStatusChanged, clickPlayer, [clickPlayer, clickAudio](QMediaPlayer::MediaStatus status){
        if (status == QMediaPlayer::EndOfMedia || status == QMediaPlayer::InvalidMedia) {
            clickPlayer->deleteLater();
            clickAudio->deleteLater();
        }
    });
    clickPlayer->play();
    deadEndPlayed = false;
    lostEndPlayed = false;
    trueEndPlayed = false;
    fakeEndPlayed = false;
    ui->stackedWidget->setCurrentWidget(ui->gamePage);
    playBgm(BgmType::Trapped0);
    lastGameBgm = BgmType::Trapped0;
    if (gameManager) {
        delete gameManager;
        gameManager = nullptr;
    }
    ui->continueBtn->setEnabled(true);
    ui->continueBtn_2->setEnabled(true);
    ui->continueBtn_3->setEnabled(true);
    ui->continueBtn_4->setEnabled(true);
    ui->continueBtn_5->setEnabled(true); 
    gameManager = new GameManager(ui->graphicsView, this);
    gameManager->startGame();
    gameStarted = true;

    if (!QFile::exists("last_game.json")) {
        gameManager->setSavedBgmType(static_cast<int>(currentBgm));
        gameManager->saveToJson("last_game.json");
    }

    ui->pauseBtn->setText("暂停");
    ui->continueBtn->hide();
    connect(gameManager, &GameManager::countdownChanged, this, [this](int seconds) {
        int min = seconds / 60;
        int sec = seconds % 60;
        ui->countdownLabel->setText(QString("%1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0')));
    });
    connect(gameManager, &GameManager::countdownFinished, this, [this]() {
        onEnd(BgmType::FakeEnd, ui->fePage, &fakeEndPlayed);
    });
    int seconds = gameManager->getCountdown();
    int min = seconds / 60;
    int sec = seconds % 60;
    ui->countdownLabel->setText(QString("%1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0')));
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
            gameManager->startCountdown();
        } else {
            gameManager->pauseGame();
            ui->pauseBtn->setText("继续");
            ui->continueBtn->show();
            if (bgmPlayer) {
                bgmPlayer->pause();
            }
            gameManager->stopCountdown();
        }
    }
}


void MainWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::ActivationChange) {
        if (gameManager && gameStarted) { 
            if (isActiveWindow()) {
                // 只在gamePage时自动继续
                if (ui->stackedWidget->currentWidget() == ui->gamePage) {
                    if (gameManager->isGamePaused()) { 
                        gameManager->resumeGame();
                        gameManager->startCountdown();
                        ui->pauseBtn->setText("暂停"); 
                        ui->continueBtn->hide(); 
                        if (bgmPlayer && bgmPlayer->source().isValid()) {
                            bgmPlayer->play();
                        }
                    }
                } else {
                    // 只恢复焦点，不自动继续
                    if (ui->graphicsView) {
                        ui->graphicsView->setFocus();
                    }
                }
            } else {
                if (!gameManager->isGamePaused()) {
                    gameManager->pauseGame();
                    gameManager->stopCountdown();
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
    // 如果有last_game.json，直接读取
    if (QFile::exists("last_game.json")) {
        if (gameManager) {
            delete gameManager;
            gameManager = nullptr;
        }
        ui->stackedWidget->setCurrentWidget(ui->gamePage);
        gameManager = new GameManager(ui->graphicsView, this);
        gameManager->startGame();
        gameManager->loadFromJson("last_game.json");
        gameStarted = true;

        // 重新连接信号槽
        connect(gameManager, &GameManager::countdownChanged, this, [this](int seconds) {
            int min = seconds / 60;
            int sec = seconds % 60;
            ui->countdownLabel->setText(QString("%1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0')));
        });
        connect(gameManager, &GameManager::countdownFinished, this, [this]() {
            onEnd(BgmType::FakeEnd, ui->fePage, &fakeEndPlayed);
        });

        // 读取BGM类型并播放
        int bgmType = gameManager->getSavedBgmType();
        if (gameManager->isSecondCountdown()) {
            playBgm(BgmType::Trapped1);
            lastGameBgm = BgmType::Trapped1;
        } else {
            playBgm(BgmType::Trapped0);
            lastGameBgm = BgmType::Trapped0;
        }
        lastGameBgm = static_cast<BgmType>(bgmType);

        ui->pauseBtn->setText("暂停");
        ui->continueBtn->hide();
        if (bgmPlayer && bgmPlayer->source().isValid()) {
            bgmPlayer->play();
        }
        gameManager->resumeGame();
        gameManager->startCountdown();
        Player* player = gameManager->getPlayer();
        if (player) {
            player->setFocus();
        }
    } else {
        // 没有last_game.json，等同于新的开始：新建并保存
        if (gameManager) {
            delete gameManager;
            gameManager = nullptr;
        }
        ui->stackedWidget->setCurrentWidget(ui->gamePage);
        gameManager = new GameManager(ui->graphicsView, this);
        gameManager->startGame();
        gameStarted = true;
        gameManager->setSavedBgmType(static_cast<int>(currentBgm));
        gameManager->saveToJson("last_game.json");

        ui->pauseBtn->setText("暂停");
        ui->continueBtn->hide();
        if (bgmPlayer && bgmPlayer->source().isValid()) {
            bgmPlayer->play();
        }
        gameManager->resumeGame();
        gameManager->startCountdown();
        Player* player = gameManager->getPlayer();
        if (player) {
            player->setFocus();
        }
    }
}


void MainWindow::showEdPage(){//ED鉴赏
    int prevIndex = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentWidget(ui->edPage);
    updateEndingButtonsAndCounter();
}



void MainWindow::goBackToStartPage(){//返回标题
                QFile::remove("last_game.json");
            saveLastGame();
    if (gameManager && gameStarted) {
        lastGameBgm = currentBgm;
        gameManager->pauseGame(); 
    }
    ui->stackedWidget->setCurrentWidget(ui->startPage);
    playBgm(BgmType::StartPage);
}
void MainWindow::skipEnd() {//跳过结局
    ui->stackedWidget->setCurrentIndex(0);
    playBgm(BgmType::StartPage);
    deadEndPlayed = false;
    lostEndPlayed = false;
    trueEndPlayed = false;
    fakeEndPlayed = false;
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    QRect fullRect = this->rect(); // 获取整个主窗口区域
    if (startPageBgLabel) startPageBgLabel->setGeometry(ui->startPage->rect()); 
    if (saveAndLoadPageBgLabel) saveAndLoadPageBgLabel->setGeometry(fullRect); 
    if (edPageBgLabel) edPageBgLabel->setGeometry(fullRect);
    if (settingsPageBgLabel) settingsPageBgLabel->setGeometry(fullRect);
    if (helpPageBgLabel) helpPageBgLabel->setGeometry(fullRect);
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
        if (ui->label) { 
            ui->label->setGeometry(10, 10, 400, 60); 
        }
        int btnWidth = 200;
        int btnHeight = 70;
        int btnCount = 7;
        int spacing = 10;
        int totalHeight = btnCount * btnHeight + (btnCount - 1) * spacing;
        int startY = height() - totalHeight - 30; 
        QList<QPushButton*> btns = {
            ui->startBtn,
            ui->continueBtn_2,
            ui->loadBtn,
            ui->helpBtn_2,
            ui->edBtn,
            ui->settingBtn_2,
            ui->exitBtn
        };
        for (int i = 0; i < btns.size(); ++i) {
            if (btns[i]) {
                btns[i]->setGeometry(0, startY + i * (btnHeight + spacing), btnWidth, btnHeight);
            }
        }
    }
    else if (currentIndex == 1) { 
        // 统一按钮尺寸和间距
        int btnWidth = 150;
        int btnHeight = 50;
        int spacing = 10;
        int topMargin = 0;
        int leftMargin = 0;
        QList<QPushButton*> topBtns = {
            ui->backBtn_4,
            ui->saveBtn,
            ui->loadBtn_2,
            ui->pauseBtn,
            ui->helpBtn,
            ui->settingBtn
        };
        for (int i = 0; i < topBtns.size(); ++i) {
            if (topBtns[i]) {
                topBtns[i]->setGeometry(
                    leftMargin + i * (btnWidth + spacing),
                    topMargin,
                    btnWidth,
                    btnHeight
                );
            }
        }
        int contentW = width();
        int contentH = height() - btnHeight;
        int viewW = contentW;
        int viewH = contentH;
        int x = 0, y = btnHeight;
        if (maximized) {
            if (contentW * 2 > contentH * 3) {
                viewH = contentH;
                viewW = contentH * 3 / 2;
            } else {
                viewW = contentW;
                viewH = contentW * 2 / 3;
            }
            x = (contentW - viewW) / 2;
            y = btnHeight + (contentH - viewH) / 2;
        }
        ui->graphicsView->setGeometry(x, y, viewW, viewH);

        int continueBtnWidth = 421;
        int continueBtnHeight = 261;
        int btnX = (width() - continueBtnWidth) / 2;
        int btnY = (height() - continueBtnHeight) / 2;
        ui->continueBtn->setGeometry(btnX, btnY, continueBtnWidth, continueBtnHeight);
        double widthRatio = width() / 1080.0;
        double heightRatio = height() / 720.0;
        int countdownWidth = int(101 * widthRatio);
        int countdownHeight = int(41 * heightRatio);
        ui->countdownLabel->setGeometry(width() - countdownWidth, 0, countdownWidth, countdownHeight);
    }
    if (ui->stackedWidget->currentWidget() == ui->saveAndLoadPage) {
        // 存档框参数
        int slotW = 240, slotH = 200, spacing = 10;
        int cols = 4, rows = 2;
        int totalW = cols * slotW + (cols - 1) * spacing;
        int totalH = rows * slotH + (rows - 1) * spacing;
        int startX = (width() - totalW) / 2;
        int startY = (height() - totalH) / 2;

        // 布局8个存档按钮
        for (int i = 0; i < 8; ++i) {
            int row = i / cols;
            int col = i % cols;
            int x = startX + col * (slotW + spacing);
            int y = startY + row * (slotH + spacing);
            QString btnName = QString("archive_%1").arg(i + 1);
            QPushButton* btn = findChild<QPushButton*>(btnName);
            if (btn) {
                btn->setGeometry(x, y, slotW, slotH);
            }
        }

        // 确认弹窗居中
        QWidget* confirmWidget = findChild<QWidget*>("confirmWidget");
        if (confirmWidget) {
            int w = confirmWidget->width();
            int h = confirmWidget->height();
            int x = (width() - w) / 2;
            int y = (height() - h) / 2;
            confirmWidget->move(x, y);
        }
    }

    emit viewResized();
    if (!gameStarted) return;

    if (gameManager) {
        Player* player = gameManager->getPlayer();
        if (player && ui->graphicsView->scene()) {
            ui->graphicsView->centerOn(player);
        }
    }
}



void MainWindow::onEnd(BgmType endType, QWidget* endPage, bool* playedFlag) {
    if (playedFlag && *playedFlag) return;
    if (playedFlag) *playedFlag = true;
    switch (endType) {
        case BgmType::DeadEnd: setEndingAchieved("deadEnd"); break;
        case BgmType::LostEnd: setEndingAchieved("lostEnd"); break;
        case BgmType::TrueEnd: setEndingAchieved("trueEnd"); break;
        case BgmType::FakeEnd: setEndingAchieved("fakeEnd"); break;
        default: break;
    }
    bool fromGame = (ui->stackedWidget->currentWidget() == ui->gamePage);
    if (fromGame) {
        cleanupGame();
    }
    ui->stackedWidget->setCurrentWidget(ui->edScene);
    ui->stackedWidget_2->setCurrentWidget(endPage);
    bgmPlayer->stop();
    playBgm(endType);
    bgmPlayer->setLoops(1);
    bgmPlayer->play();
    disconnect(bgmPlayer, &QMediaPlayer::mediaStatusChanged, this, nullptr);
    connect(bgmPlayer, &QMediaPlayer::mediaStatusChanged, this, [this, fromGame](QMediaPlayer::MediaStatus status){
        if (status == QMediaPlayer::EndOfMedia) {
            onEndFinished(fromGame);
        }
    });
    if (fromGame) {
        ui->continueBtn->setEnabled(false);
        ui->continueBtn_2->setEnabled(false);
        ui->continueBtn_3->setEnabled(false);
        ui->continueBtn_4->setEnabled(false);
        ui->continueBtn_5->setEnabled(false);
    } else {
        if(gameManager){
            ui->continueBtn->setEnabled(true);
            ui->continueBtn_2->setEnabled(true);
            ui->continueBtn_3->setEnabled(true);
            ui->continueBtn_4->setEnabled(true);
            ui->continueBtn_5->setEnabled(true);
        }
    }
}

void MainWindow::onEndFinished(bool fromGame) {
    bgmPlayer->setLoops(QMediaPlayer::Infinite);
    ui->stackedWidget->setCurrentWidget(ui->startPage);
    playBgm(BgmType::StartPage);
    disconnect(bgmPlayer, nullptr, this, nullptr);
    if (fromGame) {
        ui->continueBtn->setEnabled(false);
        ui->continueBtn_2->setEnabled(false);
        ui->continueBtn_3->setEnabled(false);
        ui->continueBtn_4->setEnabled(false);
        ui->continueBtn_5->setEnabled(false);
    }
    deadEndPlayed = false;
    lostEndPlayed = false;
    trueEndPlayed = false;
    fakeEndPlayed = false;
}


void MainWindow::saveAndLoadGame() {
    if (sender() == ui->saveBtn) {
        saveLoadMode = SaveLoadMode::Save;
    } else {
        saveLoadMode = SaveLoadMode::Load;
    }
    for (int i = 0; i < 8; ++i) {
        QString btnName = QString("archive_%1").arg(i + 1);
        QPushButton* btn = findChild<QPushButton*>(btnName);
        if (btn) {
            disconnect(btn, nullptr, nullptr, nullptr); // 防止重复连接
            connect(btn, &QPushButton::clicked, this, [this, i]() {
                onSaveSlotClicked(i);
            });
        }
    }
    if (gameManager && gameStarted && !gameManager->isGamePaused()) {
        gameManager->pauseGame();
        ui->pauseBtn->setText("继续");
        if (bgmPlayer) bgmPlayer->pause();
        gameManager->stopCountdown(); 
    }
    int prevIndex = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentWidget(ui->saveAndLoadPage);
    ui->stackedWidget->setCurrentWidget(ui->saveAndLoadPage);
    QWidget* confirmWidget = findChild<QWidget*>("confirmWidget");
    if (confirmWidget) confirmWidget->hide();
    for (int i = 0; i < 8; ++i) {
        QString btnName = QString("archive_%1").arg(i + 1);
        QPushButton* btn = findChild<QPushButton*>(btnName);
        QString saveFile = QString("save%1.json").arg(i + 1);
        QFile file(saveFile);
        if (btn) {
            if (file.exists()) {
                QString timeStr = "已存档";
                if (file.open(QIODevice::ReadOnly)) {
                    QByteArray data = file.readAll();
                    file.close();
                    QJsonDocument doc = QJsonDocument::fromJson(data);
                    QJsonObject root = doc.object();
                    if (root.contains("save_time")) {
                        QString saveTime = root["save_time"].toString();
                        QStringList parts = saveTime.split(' ');
                        if (parts.size() == 2) {
                            timeStr = parts[0] + "\n" + parts[1];
                        } else {
                            timeStr = saveTime;
                        }
                    }
                }
                btn->setText(timeStr);
            } else {
                btn->setText("空");
            }
        }
    }
    refreshSaveSlotButtons(); 
}

void MainWindow::onSaveSlotClicked(int index) {
    QString saveFile = QString("save%1.json").arg(index + 1);
    QFile file(saveFile);
    if (saveLoadMode == SaveLoadMode::Save) {
        if (file.exists()) {
            QWidget* confirmWidget = findChild<QWidget*>("confirmWidget");
            if (confirmWidget) confirmWidget->show();
            pendingSaveSlotIndex = index;
            return;
        }
        if (gameManager) {
            gameManager->setSavedBgmType(static_cast<int>(currentBgm));
            gameManager->saveToJson("last_game.json");
        }
        QFile::remove(saveFile);
        QFile::copy("last_game.json", saveFile);
        refreshSaveSlotButtons();
    } else if (saveLoadMode == SaveLoadMode::Load) {
        if (!file.exists()) {
            return;
        }
        QFile::remove("last_game.json");
        QFile::copy(saveFile, "last_game.json");
        returnToGame();
    }
}

void MainWindow::refreshSaveSlotButtons() {
    for (int i = 0; i < 8; ++i) {
        QString btnName = QString("archive_%1").arg(i + 1);
        QPushButton* btn = findChild<QPushButton*>(btnName);
        QString saveFile = QString("save%1.json").arg(i + 1);
        QFile file(saveFile);
        if (btn) {
            if (file.exists()) {
                QString timeStr = "已存档";
                if (file.open(QIODevice::ReadOnly)) {
                    QByteArray data = file.readAll();
                    file.close();
                    QJsonDocument doc = QJsonDocument::fromJson(data);
                    QJsonObject root = doc.object();
                    if (root.contains("save_time")) {
                        QString saveTime = root["save_time"].toString();
                        QStringList parts = saveTime.split(' ');
                        if (parts.size() == 2) {
                            timeStr = parts[0] + "\n" + parts[1];
                        } else {
                            timeStr = saveTime;
                        }
                    }
                }
                btn->setText(timeStr);
            } else {
                btn->setText("空");
            }
        }
        // 保证监控
        if (file.exists() && !savesWatcher->files().contains(saveFile)) {
            savesWatcher->addPath(saveFile);
        }
    }
}
void MainWindow::onConfirmSave() {
    if (pendingSaveSlotIndex >= 0 && pendingSaveSlotIndex < saveSlots.size()) {
        QString saveFile = QString("save%1.json").arg(pendingSaveSlotIndex + 1);
        if (gameManager) {
            gameManager->setSavedBgmType(static_cast<int>(currentBgm));
            gameManager->saveToJson("last_game.json");
        }
        QFile::remove(saveFile);
        QFile::copy("last_game.json", saveFile);
        refreshSaveSlotButtons();
    }
    QWidget* confirmWidget = findChild<QWidget*>("confirmWidget");
    if (confirmWidget) confirmWidget->hide();
    pendingSaveSlotIndex = -1;
}


void MainWindow::onCancelSave() {
    QWidget* confirmWidget = findChild<QWidget*>("confirmWidget");
    if (confirmWidget) confirmWidget->hide();
    pendingSaveSlotIndex = -1;
}

void MainWindow::loadEndingsRecord() {
    QFile file("endings.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        endingsRecord = doc.object();
        file.close();
    } else {
        endingsRecord = QJsonObject{
            {"deadEnd", false},
            {"lostEnd", false},
            {"trueEnd", false},
            {"fakeEnd", false}
        };
    }
}

void MainWindow::saveEndingsRecord() {
    QFile file("endings.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(endingsRecord);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
    if (endingsWatcher && !endingsWatcher->files().contains("endings.json") && QFile::exists("endings.json")) {
        endingsWatcher->addPath("endings.json");
    }
    updateEndingButtonsAndCounter(); 
}

void MainWindow::setEndingAchieved(const QString& endingKey) {
    if (!endingsRecord.value(endingKey).toBool()) {
        endingsRecord[endingKey] = true;
        saveEndingsRecord();
        updateEndingButtonsAndCounter();
    }
}

void MainWindow::updateEndingButtonsAndCounter() {
    ui->deBtn->setVisible(endingsRecord.value("deadEnd").toBool());
    ui->leBtn->setVisible(endingsRecord.value("lostEnd").toBool());
    ui->teBtn->setVisible(endingsRecord.value("trueEnd").toBool());
    ui->feBtn->setVisible(endingsRecord.value("fakeEnd").toBool());
    int count = 0;
    for (const QString& key : {"deadEnd", "lostEnd", "trueEnd", "fakeEnd"}) {
        if (endingsRecord.value(key).toBool()) ++count;
    }
    ui->endingCounterLabel->setText(QString("已达结局数：%1").arg(count));
    QTextBrowser* helpTextBrowser = findChild<QTextBrowser*>("textBrowser");
    if (helpTextBrowser) {
        QString html = R"(
<p align="center"><span style="font-weight:700;">通过WASD移动，空格发射波</span></p>
<p align="center"><span style="font-weight:700;">等倒计时彻底结束</span></p>
)";
        if (count >= 2) {
            html += R"(<p align="center"><span style="font-weight:700;">传送达到一定次数</span></p>)";
        }
        if (count >= 3) {
            html += R"(<p align="center"><span style="font-weight:700;">在迷宫外与众不同的格子停留</span></p>)";
        }
        helpTextBrowser->setHtml(html);
    }
}

void MainWindow::saveLastGame() {
    if (gameManager) {
        gameManager->saveToJson("last_game.json");
    }
}
void MainWindow::updateContinueButtons() {
    bool hasLastGame = QFile::exists("last_game.json");
    ui->continueBtn->setEnabled(hasLastGame);
    ui->continueBtn_2->setEnabled(hasLastGame);
    ui->continueBtn_3->setEnabled(hasLastGame);
    ui->continueBtn_4->setEnabled(hasLastGame);
    ui->continueBtn_5->setEnabled(hasLastGame);
}
