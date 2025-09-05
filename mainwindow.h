#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"Map.h"
#include"GameManager.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QScreen>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileSystemWatcher>
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
    enum class BgmType { None, StartPage, Trapped0,Trapped1,DeadEnd, LostEnd,FakeEnd,TrueEnd};//bgm类型
    bool deadEndPlayed = false;
    bool lostEndPlayed = false;
    bool trueEndPlayed = false;
    bool fakeEndPlayed = false;
    void playBgm(BgmType type);//播放bgm
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;//游戏内专用
    void resizeEvent(QResizeEvent *event) override;//窗口大小调整
    void changeEvent(QEvent *event) override;//窗口状态改变

private:
    Ui::MainWindow *ui;
    Map* map;//地图
    GameManager* gameManager;//游戏管理器
    bool gameStarted; // 标记游戏是否开始
    //页面背景
    QLabel* startPageBgLabel = nullptr;
    QMovie* startPageBgMovie = nullptr; 
    QLabel* saveAndLoadPageBgLabel = nullptr;
    QLabel* edPageBgLabel = nullptr;
    QLabel* settingsPageBgLabel = nullptr;
    QLabel* helpPageBgLabel = nullptr;
    
    void setupBackground();//设置背景
    const double aspectRatio = 3.0 / 2.0;//宽高比
    void maintainAspectRatio(QResizeEvent *event);//保持宽高比
    void adjustBackgroundLabel();//调整窗口大小
    QMediaPlayer* bgmPlayer = nullptr;//bgm播放器
    QAudioOutput* audioOutput = nullptr;//音频输出
    BgmType currentBgm = BgmType::None;//当前bgm
    BgmType lastGameBgm = BgmType::None;//上一次的游戏bgm
    void cleanupGame();//清理游戏资源
    struct SaveSlot {//存档框
        GameManager* gameManager = nullptr;
        bool isLoading = false;
    };
    QVector<SaveSlot> saveSlots = QVector<SaveSlot>(8);//存档框数组
    enum class SaveLoadMode { None, Save, Load };
    SaveLoadMode saveLoadMode = SaveLoadMode::None;
    int pendingSaveSlotIndex = -1; // 记录待确认的存档框
    void onSaveSlotClicked(int index);//点击存档框
    void onConfirmSave();//确认保存
    void onCancelSave();//取消保存
    QJsonObject endingsRecord; // 结局达成记录
    void loadEndingsRecord();//加载结局记录
    void saveEndingsRecord();//保存结局记录
    void updateEndingButtonsAndCounter(); //更新结局按钮和计数器
    void setEndingAchieved(const QString& endingKey);//设置结局达成状态
    void refreshSaveSlotButtons();//刷新存档
    QFileSystemWatcher* endingsWatcher = nullptr;//监测结局文件
    QFileSystemWatcher* lastGameWatcher = nullptr; //监测上次游戏文件
    QFileSystemWatcher* savesWatcher = nullptr;//监测所有存档
    void saveLastGame();//保存上次游戏
    void updateContinueButtons(); //更新继续按钮
private slots://页面跳转
    void startGame();//开始游戏
    void showEdPage();//ED鉴赏
    void saveAndLoadGame();//保存和读取
    void goBackToStartPage();//返回标题
    void skipEnd();//跳过
    void returnToGame();//返回game
    void togglePause();//暂停
    void showHelpPage();//显示帮助
    void showSettingsPage();//显示设置
signals:
    void viewResized();//窗口改变
    void forcePhantomsChasePlayer();//强制锁定
public slots:
    void onEnd(BgmType type, QWidget* page, bool* playedFlag);//放结局
    void onEndFinished(bool fromGame = false);//结局结束
};
#endif // MAINWINDOW_H
