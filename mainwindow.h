#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"Map.h"
#include"GameManager.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QScreen>
class QMovie;
class QLabel;
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
    void resizeEvent(QResizeEvent *event) override;//窗口大小调整
    void changeEvent(QEvent *event) override;//窗口状态改变
private:
    Ui::MainWindow *ui;
    Map* map;//地图
    GameManager* gameManager;//游戏管理器
    bool gameStarted; // 标记游戏是否开始
    QMovie* backgroundMovie;//背景gif
    QLabel* backgroundLabel;
    void setupGifBackground();//设置背景
    const double aspectRatio = 3.0 / 2.0;//宽高比
    void maintainAspectRatio(QResizeEvent *event);//保持宽高比
    void adjustBackgroundLabel();//调整窗口大小
    QMediaPlayer* bgmPlayer = nullptr;//bgm播放器
    QAudioOutput* audioOutput = nullptr;//音频输出
    enum class BgmType { None, StartPage, GamePage };//bgm类型
    BgmType currentBgm = BgmType::None;//当前bgm
    void playBgm(BgmType type);//播放bgm
    bool deadEndPlayed = false;
    void cleanupGame();//清理游戏资源
private slots://页面跳转
    void startGame();//开始游戏
    void loadGame();//读取
    void showEdPage();//ED鉴赏
    void saveGame();//保存
    void goBackToStartPage();//返回标题
    void returnToGame(); //返回game
    void togglePause();//暂停
signals:
    void viewResized();//窗口改变
public slots:
    void onPlayerDead();//玩家死亡时调用
    void onDeadEndMusicFinished();//死亡音乐播放结束
};
#endif // MAINWINDOW_H
