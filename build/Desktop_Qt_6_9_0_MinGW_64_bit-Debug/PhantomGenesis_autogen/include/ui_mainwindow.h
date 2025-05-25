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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *startPage;
    QPushButton *startBtn;
    QWidget *gamePage;
    QGraphicsView *graphicsView;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1080, 720);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(0, 0, 1080, 720));
        startPage = new QWidget();
        startPage->setObjectName("startPage");
        startPage->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 242, 219);"));
        startBtn = new QPushButton(startPage);
        startBtn->setObjectName("startBtn");
        startBtn->setGeometry(QRect(440, 500, 181, 101));
        startBtn->setStyleSheet(QString::fromUtf8("background-color: rgb(188, 255, 196);"));
        stackedWidget->addWidget(startPage);
        gamePage = new QWidget();
        gamePage->setObjectName("gamePage");
        gamePage->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        graphicsView = new QGraphicsView(gamePage);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(0, 0, 1080, 720));
        graphicsView->setStyleSheet(QString::fromUtf8("background-color: rgb(211, 255, 255);"));
        stackedWidget->addWidget(gamePage);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        startBtn->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\346\270\270\346\210\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
