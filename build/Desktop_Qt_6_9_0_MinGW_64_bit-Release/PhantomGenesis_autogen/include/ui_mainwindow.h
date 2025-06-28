/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *startPage;
    QPushButton *startBtn;
    QPushButton *loadBtn;
    QPushButton *edBtn;
    QPushButton *exitBtn;
    QPushButton *continueBtn_2;
    QPushButton *helpBtn_2;
    QPushButton *settingBtn_2;
    QLabel *label;
    QWidget *gamePage;
    QGraphicsView *graphicsView;
    QPushButton *backBtn_4;
    QPushButton *saveBtn;
    QPushButton *loadBtn_2;
    QPushButton *pauseBtn;
    QPushButton *helpBtn;
    QPushButton *settingBtn;
    QPushButton *continueBtn;
    QLabel *countdownLabel;
    QWidget *saveAndLoadPage;
    QPushButton *backBtn_3;
    QPushButton *continueBtn_5;
    QPushButton *archive;
    QPushButton *archive_2;
    QPushButton *archive_3;
    QPushButton *archive_4;
    QPushButton *archive_5;
    QPushButton *archive_6;
    QPushButton *archive_7;
    QPushButton *archive_8;
    QWidget *confirmWidget;
    QLabel *confirmText;
    QPushButton *confirmBtn;
    QPushButton *cancelBtn;
    QWidget *edPage;
    QPushButton *backBtn;
    QPushButton *leBtn;
    QPushButton *teBtn;
    QPushButton *feBtn;
    QPushButton *deBtn;
    QLabel *endCounter;
    QWidget *settingsPage;
    QPushButton *backBtn_2;
    QPushButton *continueBtn_3;
    QWidget *helpPage;
    QPushButton *backBtn_5;
    QPushButton *continueBtn_4;
    QWidget *edScene;
    QStackedWidget *stackedWidget_2;
    QWidget *dePage;
    QTextBrowser *textBrowser;
    QPushButton *skipBtn;
    QWidget *tePage;
    QPushButton *skipBtn_4;
    QTextBrowser *textBrowser_5;
    QWidget *fePage;
    QPushButton *skipBtn_2;
    QTextBrowser *textBrowser_3;
    QWidget *lePage;
    QPushButton *skipBtn_3;
    QTextBrowser *textBrowser_4;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1080, 720);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1080, 720));
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        sizePolicy.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy);
        startPage = new QWidget();
        startPage->setObjectName("startPage");
        sizePolicy.setHeightForWidth(startPage->sizePolicy().hasHeightForWidth());
        startPage->setSizePolicy(sizePolicy);
        startPage->setStyleSheet(QString::fromUtf8(""));
        startBtn = new QPushButton(startPage);
        startBtn->setObjectName("startBtn");
        startBtn->setGeometry(QRect(0, 110, 200, 70));
        sizePolicy.setHeightForWidth(startBtn->sizePolicy().hasHeightForWidth());
        startBtn->setSizePolicy(sizePolicy);
        startBtn->setStyleSheet(QString::fromUtf8("background-color: rgb(188, 255, 196);"));
        loadBtn = new QPushButton(startPage);
        loadBtn->setObjectName("loadBtn");
        loadBtn->setGeometry(QRect(0, 270, 200, 70));
        edBtn = new QPushButton(startPage);
        edBtn->setObjectName("edBtn");
        edBtn->setGeometry(QRect(0, 430, 200, 70));
        exitBtn = new QPushButton(startPage);
        exitBtn->setObjectName("exitBtn");
        exitBtn->setGeometry(QRect(0, 590, 200, 70));
        continueBtn_2 = new QPushButton(startPage);
        continueBtn_2->setObjectName("continueBtn_2");
        continueBtn_2->setGeometry(QRect(0, 190, 200, 70));
        helpBtn_2 = new QPushButton(startPage);
        helpBtn_2->setObjectName("helpBtn_2");
        helpBtn_2->setGeometry(QRect(0, 350, 200, 70));
        settingBtn_2 = new QPushButton(startPage);
        settingBtn_2->setObjectName("settingBtn_2");
        settingBtn_2->setGeometry(QRect(0, 510, 200, 70));
        label = new QLabel(startPage);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 10, 271, 81));
        label->setStyleSheet(QString::fromUtf8("color: white;\n"
"background: transparent;\n"
"font-size: 32px;\n"
"font-weight: bold;"));
        stackedWidget->addWidget(startPage);
        gamePage = new QWidget();
        gamePage->setObjectName("gamePage");
        sizePolicy.setHeightForWidth(gamePage->sizePolicy().hasHeightForWidth());
        gamePage->setSizePolicy(sizePolicy);
        gamePage->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        graphicsView = new QGraphicsView(gamePage);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(0, 49, 1080, 671));
        graphicsView->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        backBtn_4 = new QPushButton(gamePage);
        backBtn_4->setObjectName("backBtn_4");
        backBtn_4->setGeometry(QRect(0, 0, 150, 50));
        saveBtn = new QPushButton(gamePage);
        saveBtn->setObjectName("saveBtn");
        saveBtn->setGeometry(QRect(160, 0, 150, 50));
        loadBtn_2 = new QPushButton(gamePage);
        loadBtn_2->setObjectName("loadBtn_2");
        loadBtn_2->setGeometry(QRect(320, 0, 150, 50));
        pauseBtn = new QPushButton(gamePage);
        pauseBtn->setObjectName("pauseBtn");
        pauseBtn->setGeometry(QRect(480, 0, 150, 50));
        helpBtn = new QPushButton(gamePage);
        helpBtn->setObjectName("helpBtn");
        helpBtn->setGeometry(QRect(640, 0, 150, 50));
        settingBtn = new QPushButton(gamePage);
        settingBtn->setObjectName("settingBtn");
        settingBtn->setGeometry(QRect(800, 0, 150, 50));
        continueBtn = new QPushButton(gamePage);
        continueBtn->setObjectName("continueBtn");
        continueBtn->setGeometry(QRect(330, 230, 421, 261));
        continueBtn->setStyleSheet(QString::fromUtf8("background-color: rgb(149, 255, 183);"));
        countdownLabel = new QLabel(gamePage);
        countdownLabel->setObjectName("countdownLabel");
        countdownLabel->setGeometry(QRect(960, 0, 100, 50));
        countdownLabel->setStyleSheet(QString::fromUtf8("color: red; background: transparent; font-size: 28px;"));
        stackedWidget->addWidget(gamePage);
        saveAndLoadPage = new QWidget();
        saveAndLoadPage->setObjectName("saveAndLoadPage");
        saveAndLoadPage->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 211, 37);"));
        backBtn_3 = new QPushButton(saveAndLoadPage);
        backBtn_3->setObjectName("backBtn_3");
        backBtn_3->setGeometry(QRect(0, 0, 150, 80));
        continueBtn_5 = new QPushButton(saveAndLoadPage);
        continueBtn_5->setObjectName("continueBtn_5");
        continueBtn_5->setGeometry(QRect(160, 0, 160, 80));
        archive = new QPushButton(saveAndLoadPage);
        archive->setObjectName("archive");
        archive->setGeometry(QRect(10, 150, 240, 200));
        archive_2 = new QPushButton(saveAndLoadPage);
        archive_2->setObjectName("archive_2");
        archive_2->setGeometry(QRect(270, 150, 240, 200));
        archive_3 = new QPushButton(saveAndLoadPage);
        archive_3->setObjectName("archive_3");
        archive_3->setGeometry(QRect(530, 150, 240, 200));
        archive_4 = new QPushButton(saveAndLoadPage);
        archive_4->setObjectName("archive_4");
        archive_4->setGeometry(QRect(790, 150, 240, 200));
        archive_5 = new QPushButton(saveAndLoadPage);
        archive_5->setObjectName("archive_5");
        archive_5->setGeometry(QRect(10, 390, 240, 200));
        archive_6 = new QPushButton(saveAndLoadPage);
        archive_6->setObjectName("archive_6");
        archive_6->setGeometry(QRect(270, 390, 240, 200));
        archive_7 = new QPushButton(saveAndLoadPage);
        archive_7->setObjectName("archive_7");
        archive_7->setGeometry(QRect(530, 390, 240, 200));
        archive_8 = new QPushButton(saveAndLoadPage);
        archive_8->setObjectName("archive_8");
        archive_8->setGeometry(QRect(790, 390, 240, 200));
        confirmWidget = new QWidget(saveAndLoadPage);
        confirmWidget->setObjectName("confirmWidget");
        confirmWidget->setGeometry(QRect(300, 200, 400, 350));
        confirmText = new QLabel(confirmWidget);
        confirmText->setObjectName("confirmText");
        confirmText->setGeometry(QRect(138, 68, 201, 141));
        confirmBtn = new QPushButton(confirmWidget);
        confirmBtn->setObjectName("confirmBtn");
        confirmBtn->setGeometry(QRect(100, 260, 111, 51));
        cancelBtn = new QPushButton(confirmWidget);
        cancelBtn->setObjectName("cancelBtn");
        cancelBtn->setGeometry(QRect(231, 267, 101, 41));
        stackedWidget->addWidget(saveAndLoadPage);
        edPage = new QWidget();
        edPage->setObjectName("edPage");
        edPage->setStyleSheet(QString::fromUtf8("background-color: rgb(69, 165, 255);"));
        backBtn = new QPushButton(edPage);
        backBtn->setObjectName("backBtn");
        backBtn->setGeometry(QRect(0, 0, 150, 80));
        leBtn = new QPushButton(edPage);
        leBtn->setObjectName("leBtn");
        leBtn->setGeometry(QRect(30, 310, 250, 150));
        teBtn = new QPushButton(edPage);
        teBtn->setObjectName("teBtn");
        teBtn->setGeometry(QRect(310, 310, 250, 150));
        feBtn = new QPushButton(edPage);
        feBtn->setObjectName("feBtn");
        feBtn->setGeometry(QRect(310, 130, 250, 150));
        deBtn = new QPushButton(edPage);
        deBtn->setObjectName("deBtn");
        deBtn->setGeometry(QRect(30, 130, 250, 150));
        endCounter = new QLabel(edPage);
        endCounter->setObjectName("endCounter");
        endCounter->setGeometry(QRect(163, 0, 200, 80));
        stackedWidget->addWidget(edPage);
        settingsPage = new QWidget();
        settingsPage->setObjectName("settingsPage");
        settingsPage->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 127);"));
        backBtn_2 = new QPushButton(settingsPage);
        backBtn_2->setObjectName("backBtn_2");
        backBtn_2->setGeometry(QRect(0, 0, 150, 80));
        continueBtn_3 = new QPushButton(settingsPage);
        continueBtn_3->setObjectName("continueBtn_3");
        continueBtn_3->setGeometry(QRect(160, 0, 150, 80));
        stackedWidget->addWidget(settingsPage);
        helpPage = new QWidget();
        helpPage->setObjectName("helpPage");
        backBtn_5 = new QPushButton(helpPage);
        backBtn_5->setObjectName("backBtn_5");
        backBtn_5->setGeometry(QRect(0, 0, 150, 80));
        continueBtn_4 = new QPushButton(helpPage);
        continueBtn_4->setObjectName("continueBtn_4");
        continueBtn_4->setGeometry(QRect(160, 0, 150, 80));
        stackedWidget->addWidget(helpPage);
        edScene = new QWidget();
        edScene->setObjectName("edScene");
        stackedWidget_2 = new QStackedWidget(edScene);
        stackedWidget_2->setObjectName("stackedWidget_2");
        stackedWidget_2->setGeometry(QRect(-1, -1, 1081, 601));
        dePage = new QWidget();
        dePage->setObjectName("dePage");
        textBrowser = new QTextBrowser(dePage);
        textBrowser->setObjectName("textBrowser");
        textBrowser->setGeometry(QRect(0, 80, 1080, 640));
        skipBtn = new QPushButton(dePage);
        skipBtn->setObjectName("skipBtn");
        skipBtn->setGeometry(QRect(0, 0, 150, 80));
        stackedWidget_2->addWidget(dePage);
        tePage = new QWidget();
        tePage->setObjectName("tePage");
        skipBtn_4 = new QPushButton(tePage);
        skipBtn_4->setObjectName("skipBtn_4");
        skipBtn_4->setGeometry(QRect(0, 0, 150, 80));
        textBrowser_5 = new QTextBrowser(tePage);
        textBrowser_5->setObjectName("textBrowser_5");
        textBrowser_5->setGeometry(QRect(0, 80, 1080, 640));
        stackedWidget_2->addWidget(tePage);
        fePage = new QWidget();
        fePage->setObjectName("fePage");
        skipBtn_2 = new QPushButton(fePage);
        skipBtn_2->setObjectName("skipBtn_2");
        skipBtn_2->setGeometry(QRect(0, 0, 150, 80));
        textBrowser_3 = new QTextBrowser(fePage);
        textBrowser_3->setObjectName("textBrowser_3");
        textBrowser_3->setGeometry(QRect(0, 80, 1080, 640));
        stackedWidget_2->addWidget(fePage);
        lePage = new QWidget();
        lePage->setObjectName("lePage");
        skipBtn_3 = new QPushButton(lePage);
        skipBtn_3->setObjectName("skipBtn_3");
        skipBtn_3->setGeometry(QRect(0, 0, 150, 80));
        textBrowser_4 = new QTextBrowser(lePage);
        textBrowser_4->setObjectName("textBrowser_4");
        textBrowser_4->setGeometry(QRect(0, 70, 1080, 640));
        stackedWidget_2->addWidget(lePage);
        stackedWidget->addWidget(edScene);

        verticalLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(6);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        startBtn->setText(QCoreApplication::translate("MainWindow", "\346\226\260\347\232\204\345\274\200\345\247\213", nullptr));
        loadBtn->setText(QCoreApplication::translate("MainWindow", "\350\257\273\345\217\226\345\255\230\346\241\243", nullptr));
        edBtn->setText(QCoreApplication::translate("MainWindow", "ED\351\211\264\350\265\217", nullptr));
        exitBtn->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272\346\270\270\346\210\217", nullptr));
        continueBtn_2->setText(QCoreApplication::translate("MainWindow", "\347\273\247\347\273\255\346\216\242\347\264\242", nullptr));
        helpBtn_2->setText(QCoreApplication::translate("MainWindow", "\346\203\205\346\212\245\346\237\245\347\234\213", nullptr));
        settingBtn_2->setText(QCoreApplication::translate("MainWindow", "\346\270\270\346\210\217\350\256\276\347\275\256", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "PhantomGenesis", nullptr));
        backBtn_4->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\346\240\207\351\242\230", nullptr));
        saveBtn->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
        loadBtn_2->setText(QCoreApplication::translate("MainWindow", "\350\257\273\345\217\226", nullptr));
        pauseBtn->setText(QCoreApplication::translate("MainWindow", "\346\232\202\345\201\234", nullptr));
        helpBtn->setText(QCoreApplication::translate("MainWindow", "\346\203\205\346\212\245", nullptr));
        settingBtn->setText(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        continueBtn->setText(QCoreApplication::translate("MainWindow", "\347\273\247\347\273\255\346\216\242\347\264\242", nullptr));
        countdownLabel->setText(QCoreApplication::translate("MainWindow", "02\357\274\23200", nullptr));
        backBtn_3->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\346\240\207\351\242\230", nullptr));
        continueBtn_5->setText(QCoreApplication::translate("MainWindow", "\347\273\247\347\273\255\346\216\242\347\264\242", nullptr));
        archive->setText(QCoreApplication::translate("MainWindow", "\347\251\272", nullptr));
        archive_2->setText(QCoreApplication::translate("MainWindow", "\347\251\272", nullptr));
        archive_3->setText(QCoreApplication::translate("MainWindow", "\347\251\272", nullptr));
        archive_4->setText(QCoreApplication::translate("MainWindow", "\347\251\272", nullptr));
        archive_5->setText(QCoreApplication::translate("MainWindow", "\347\251\272", nullptr));
        archive_6->setText(QCoreApplication::translate("MainWindow", "\347\251\272", nullptr));
        archive_7->setText(QCoreApplication::translate("MainWindow", "\347\251\272", nullptr));
        archive_8->setText(QCoreApplication::translate("MainWindow", "\347\251\272", nullptr));
        confirmText->setText(QCoreApplication::translate("MainWindow", "\347\241\256\350\256\244\350\246\201\350\246\206\347\233\226\345\220\227", nullptr));
        confirmBtn->setText(QCoreApplication::translate("MainWindow", "\347\241\256\350\256\244", nullptr));
        cancelBtn->setText(QCoreApplication::translate("MainWindow", "\345\217\226\346\266\210", nullptr));
        backBtn->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\346\240\207\351\242\230", nullptr));
        leBtn->setText(QCoreApplication::translate("MainWindow", "\350\277\267\345\244\261", nullptr));
        teBtn->setText(QCoreApplication::translate("MainWindow", "\347\234\237\347\233\270", nullptr));
        feBtn->setText(QCoreApplication::translate("MainWindow", "\350\231\232\346\227\240", nullptr));
        deBtn->setText(QCoreApplication::translate("MainWindow", "\345\244\261\350\264\245", nullptr));
        endCounter->setText(QCoreApplication::translate("MainWindow", "\345\267\262\350\276\276\347\273\223\345\261\200\346\225\260\357\274\2320", nullptr));
        backBtn_2->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\346\240\207\351\242\230", nullptr));
        continueBtn_3->setText(QCoreApplication::translate("MainWindow", "\347\273\247\347\273\255\346\216\242\347\264\242", nullptr));
        backBtn_5->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\346\240\207\351\242\230", nullptr));
        continueBtn_4->setText(QCoreApplication::translate("MainWindow", "\347\273\247\347\273\255\346\216\242\347\264\242", nullptr));
        textBrowser->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        skipBtn->setText(QCoreApplication::translate("MainWindow", "\350\267\263\350\277\207", nullptr));
        skipBtn_4->setText(QCoreApplication::translate("MainWindow", "\350\267\263\350\277\207", nullptr));
        textBrowser_5->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        skipBtn_2->setText(QCoreApplication::translate("MainWindow", "\350\267\263\350\277\207", nullptr));
        textBrowser_3->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        skipBtn_3->setText(QCoreApplication::translate("MainWindow", "\350\267\263\350\277\207", nullptr));
        textBrowser_4->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
