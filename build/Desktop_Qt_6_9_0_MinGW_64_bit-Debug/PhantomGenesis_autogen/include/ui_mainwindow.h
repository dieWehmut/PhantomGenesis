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
    QWidget *savePage;
    QPushButton *backBtn_2;
    QWidget *loadPage;
    QPushButton *backBtn_3;
    QWidget *edPage;
    QPushButton *backBtn;
    QWidget *edScene;
    QStackedWidget *stackedWidget_2;
    QWidget *dePage;
    QTextBrowser *textBrowser;
    QWidget *tePage;

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
        startBtn->setGeometry(QRect(130, 230, 181, 51));
        sizePolicy.setHeightForWidth(startBtn->sizePolicy().hasHeightForWidth());
        startBtn->setSizePolicy(sizePolicy);
        startBtn->setStyleSheet(QString::fromUtf8("background-color: rgb(188, 255, 196);"));
        loadBtn = new QPushButton(startPage);
        loadBtn->setObjectName("loadBtn");
        loadBtn->setGeometry(QRect(130, 320, 181, 61));
        edBtn = new QPushButton(startPage);
        edBtn->setObjectName("edBtn");
        edBtn->setGeometry(QRect(130, 430, 181, 51));
        exitBtn = new QPushButton(startPage);
        exitBtn->setObjectName("exitBtn");
        exitBtn->setGeometry(QRect(130, 520, 181, 51));
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
        backBtn_4->setGeometry(QRect(-10, -10, 151, 51));
        saveBtn = new QPushButton(gamePage);
        saveBtn->setObjectName("saveBtn");
        saveBtn->setGeometry(QRect(149, -8, 151, 51));
        loadBtn_2 = new QPushButton(gamePage);
        loadBtn_2->setObjectName("loadBtn_2");
        loadBtn_2->setGeometry(QRect(309, -8, 141, 51));
        pauseBtn = new QPushButton(gamePage);
        pauseBtn->setObjectName("pauseBtn");
        pauseBtn->setGeometry(QRect(460, -10, 131, 51));
        helpBtn = new QPushButton(gamePage);
        helpBtn->setObjectName("helpBtn");
        helpBtn->setGeometry(QRect(610, -10, 131, 51));
        settingBtn = new QPushButton(gamePage);
        settingBtn->setObjectName("settingBtn");
        settingBtn->setGeometry(QRect(759, -8, 121, 51));
        continueBtn = new QPushButton(gamePage);
        continueBtn->setObjectName("continueBtn");
        continueBtn->setGeometry(QRect(330, 230, 421, 261));
        continueBtn->setStyleSheet(QString::fromUtf8("background-color: rgb(149, 255, 183);"));
        countdownLabel = new QLabel(gamePage);
        countdownLabel->setObjectName("countdownLabel");
        countdownLabel->setGeometry(QRect(970, 0, 101, 41));
        countdownLabel->setStyleSheet(QString::fromUtf8("color: red; background: transparent; font-size: 28px;"));
        stackedWidget->addWidget(gamePage);
        savePage = new QWidget();
        savePage->setObjectName("savePage");
        savePage->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 127);"));
        backBtn_2 = new QPushButton(savePage);
        backBtn_2->setObjectName("backBtn_2");
        backBtn_2->setGeometry(QRect(0, 550, 151, 61));
        stackedWidget->addWidget(savePage);
        loadPage = new QWidget();
        loadPage->setObjectName("loadPage");
        loadPage->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 211, 37);"));
        backBtn_3 = new QPushButton(loadPage);
        backBtn_3->setObjectName("backBtn_3");
        backBtn_3->setGeometry(QRect(20, 530, 151, 61));
        stackedWidget->addWidget(loadPage);
        edPage = new QWidget();
        edPage->setObjectName("edPage");
        edPage->setStyleSheet(QString::fromUtf8("background-color: rgb(69, 165, 255);"));
        backBtn = new QPushButton(edPage);
        backBtn->setObjectName("backBtn");
        backBtn->setGeometry(QRect(0, 10, 151, 61));
        stackedWidget->addWidget(edPage);
        edScene = new QWidget();
        edScene->setObjectName("edScene");
        stackedWidget_2 = new QStackedWidget(edScene);
        stackedWidget_2->setObjectName("stackedWidget_2");
        stackedWidget_2->setGeometry(QRect(-1, -1, 1081, 601));
        dePage = new QWidget();
        dePage->setObjectName("dePage");
        textBrowser = new QTextBrowser(dePage);
        textBrowser->setObjectName("textBrowser");
        textBrowser->setGeometry(QRect(0, 0, 1071, 611));
        stackedWidget_2->addWidget(dePage);
        tePage = new QWidget();
        tePage->setObjectName("tePage");
        stackedWidget_2->addWidget(tePage);
        stackedWidget->addWidget(edScene);

        verticalLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        startBtn->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\346\270\270\346\210\217", nullptr));
        loadBtn->setText(QCoreApplication::translate("MainWindow", "\350\257\273\345\217\226\345\255\230\346\241\243", nullptr));
        edBtn->setText(QCoreApplication::translate("MainWindow", "ED\351\211\264\350\265\217", nullptr));
        exitBtn->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272\346\270\270\346\210\217", nullptr));
        backBtn_4->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\346\240\207\351\242\230", nullptr));
        saveBtn->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
        loadBtn_2->setText(QCoreApplication::translate("MainWindow", "\350\257\273\345\217\226", nullptr));
        pauseBtn->setText(QCoreApplication::translate("MainWindow", "\346\232\202\345\201\234", nullptr));
        helpBtn->setText(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
        settingBtn->setText(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        continueBtn->setText(QCoreApplication::translate("MainWindow", "\347\273\247\347\273\255\346\270\270\346\210\217", nullptr));
        countdownLabel->setText(QCoreApplication::translate("MainWindow", "02\357\274\23200", nullptr));
        backBtn_2->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\346\240\207\351\242\230", nullptr));
        backBtn_3->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\346\240\207\351\242\230", nullptr));
        backBtn->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\346\240\207\351\242\230", nullptr));
        textBrowser->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:22pt; font-style:italic; color:#ff0000;\">\344\275\240\357\274\214\350\277\231\350\266\237\345\220\215\344\270\272\342\200\234\345\255\230\345\234\250\342\200\235\347\232\204\346\202\240\351\225\277\346\227\205\350\241\214\347\232\204\346\227\205\350\200\205\357\274\214\350\204\232\346\255\245\343\200\202\344\270\207\347\261\201\344\277\261\345\257\202"
                        "\357\274\214\344\273\277\344\275\233\350\277\236\345\221\274\345\220\270\351\203\275\345\267\262\345\275\222\350\277\230\347\273\231\346\265\251\346\270\272\347\232\204\350\231\232\347\251\272\343\200\202</span></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:22pt; font-style:italic; color:#ff0000;\"><br /></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
