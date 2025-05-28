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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
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
    QPushButton *saveBtn;
    QWidget *gamePage;
    QGraphicsView *graphicsView;
    QPushButton *backBtn_4;
    QPushButton *saveBtn_2;
    QPushButton *loadBtn_2;
    QPushButton *pauseBtn;
    QPushButton *helpBtn;
    QPushButton *settingBtn;
    QPushButton *continueBtn;
    QWidget *savePage;
    QPushButton *backBtn_2;
    QWidget *loadPage;
    QPushButton *backBtn_3;
    QWidget *edPage;
    QPushButton *backBtn;
    QWidget *edScene;

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
        startPage->setStyleSheet(QString::fromUtf8("background-color: rgb(190, 255, 253);"));
        startBtn = new QPushButton(startPage);
        startBtn->setObjectName("startBtn");
        startBtn->setGeometry(QRect(40, 230, 181, 51));
        sizePolicy.setHeightForWidth(startBtn->sizePolicy().hasHeightForWidth());
        startBtn->setSizePolicy(sizePolicy);
        startBtn->setStyleSheet(QString::fromUtf8("background-color: rgb(188, 255, 196);"));
        loadBtn = new QPushButton(startPage);
        loadBtn->setObjectName("loadBtn");
        loadBtn->setGeometry(QRect(40, 320, 181, 61));
        edBtn = new QPushButton(startPage);
        edBtn->setObjectName("edBtn");
        edBtn->setGeometry(QRect(40, 430, 181, 51));
        exitBtn = new QPushButton(startPage);
        exitBtn->setObjectName("exitBtn");
        exitBtn->setGeometry(QRect(40, 520, 181, 51));
        saveBtn = new QPushButton(startPage);
        saveBtn->setObjectName("saveBtn");
        saveBtn->setGeometry(QRect(319, 462, 121, 71));
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
        saveBtn_2 = new QPushButton(gamePage);
        saveBtn_2->setObjectName("saveBtn_2");
        saveBtn_2->setGeometry(QRect(149, -8, 151, 51));
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
        stackedWidget->addWidget(edScene);

        verticalLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        startBtn->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\346\270\270\346\210\217", nullptr));
        loadBtn->setText(QCoreApplication::translate("MainWindow", "\350\257\273\345\217\226\345\255\230\346\241\243", nullptr));
        edBtn->setText(QCoreApplication::translate("MainWindow", "ED\351\211\264\350\265\217", nullptr));
        exitBtn->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272\346\270\270\346\210\217", nullptr));
        saveBtn->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
        backBtn_4->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\346\240\207\351\242\230", nullptr));
        saveBtn_2->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
        loadBtn_2->setText(QCoreApplication::translate("MainWindow", "\350\257\273\345\217\226", nullptr));
        pauseBtn->setText(QCoreApplication::translate("MainWindow", "\346\232\202\345\201\234", nullptr));
        helpBtn->setText(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
        settingBtn->setText(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        continueBtn->setText(QCoreApplication::translate("MainWindow", "\347\273\247\347\273\255\346\270\270\346\210\217", nullptr));
        backBtn_2->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\346\240\207\351\242\230", nullptr));
        backBtn_3->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\346\240\207\351\242\230", nullptr));
        backBtn->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\346\240\207\351\242\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
