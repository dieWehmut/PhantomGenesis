#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"Map.h"
#include"GameManager.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;//游戏内专用
    void resizeEvent(QResizeEvent *event) override; // 窗口自由改变
private:
    Ui::MainWindow *ui;
    Map* map;//地图
    GameManager* gameManager;//游戏管理器
    bool gameStarted; // 标记游戏是否开始
private slots://页面跳转
    void startGame();//开始游戏
    void loadGame();//读取
    void showEdPage();//ED鉴赏
    void saveGame();//保存
    void goBackToStartPage();//返回标题
    void returnToGame(); //返回game
signals:
    void viewResized();//窗口改变
};
#endif // MAINWINDOW_H
