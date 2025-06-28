#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QGraphicsScene>
#include <QTimer>
#include "Map.h"
#include "Player.h"
#include "FlamePhantom.h"
#include "LurkPhantom.h"
#include "King.h"
#include<QVector>
#include <QGraphicsView>
#include "VisionMaskItem.h"
#include <functional>

class GameManager : public QObject 
{
    Q_OBJECT
public:
    explicit GameManager(QGraphicsView* graphicsView, QObject *parent = nullptr);
    ~GameManager();
    void startGame();//开始游戏
    void pauseGame();//暂停
    void resumeGame();//恢复游戏
    bool eventFilter(QObject *watched, QEvent *event) override;//事件过滤器
    Player* getPlayer() const { return player; } 
    bool isGamePaused() const { return gamePaused; } 
    const QVector<FlamePhantom*>& getFlamePhantoms() const { return flamePhantoms; }
    const QVector<LurkPhantom*>& getLurkPhantoms() const { return lurkPhantoms; }
    void setCountdown(int seconds);
    int getCountdown() const;
    bool isPortalEnabled() const;
    void startCountdown();
    void stopCountdown();
    void saveToJson(const QString& filename) const;//保存为json
    void loadFromJson(const QString& filename); //加载json
    int getSavedBgmType() const { return savedBgmType; }
    void setSavedBgmType(int type) { savedBgmType = type; }
    bool isSecondCountdown() const { return inSecondCountdown; }
public slots:
    void handleViewResize(); // 处理视图的改变
    void onForcePhantomsChasePlayer();// 强制锁定
    void accelerateAllPhantoms();//加快所有幽灵速度
private:
    QGraphicsView* gameView;//游戏视图
    QGraphicsScene* gameScene;//游戏场景
    bool gamePaused; // 标记是否暂停
    Map* gameMap;//地图
    Player* player;//玩家
    int curMapId; // 当前地图ID
    QTimer* gameLoopTimer;//游戏计时器
    QVector<FlamePhantom*> flamePhantoms;//flamePhantom容器
    void spawnFlamePhantoms();//flamePhantom生成
    void updateFlamePhantoms();//更新所有flamePhantom的状态
    QVector<LurkPhantom*> lurkPhantoms;//lurkPhantom容器
    void spawnLurkPhantoms();//lurkPhantom生成
    void updateLurkPhantoms();//更新所有lurkPhantom的状态
    template<typename PhantomType>
    void spawnPhantoms(QVector<PhantomType*>& container, const QVector<QPoint>& spawnPoints, int maxCount, std::function<PhantomType*()> createPhantom);
    QTimer* phantomSpawnTimer;//phantom生成定时器
    void separatePhantoms(QVector<PhantomBase*>& phantoms);//分离重叠的PhantomBase
    King* king = nullptr;
    void checkKingSpawn();//检查是否生成King
    VisionMaskItem* visionMask = nullptr;//视野遮罩
    QElapsedTimer trueEndTimer;//te计时
    bool inTrueEndZone = false; 
    void pauseAllWaves();//wave的暂停与恢复
    void resumeAllWaves();
    int countdownSeconds = 0;//倒数秒数
    QTimer* countdownTimer = nullptr;//倒计时
    bool portalEnabled = true; // 传送门是否启用
    bool inSecondCountdown = false;// 是否在第二次倒计时中
    void tryTeleportPhantom(PhantomBase* phantom);//传送PhantomBase
    int savedBgmType = 0; // 保存的游戏bgm
signals:
    void playerDead();
    void playerLost();
    void playerTrueEnd(); 
    void countdownChanged(int seconds);// 倒计时改变      
    void countdownFinished();// 倒计时结束  
    void portalStateChanged(bool enabled);// 传送门状态改变 
};

#endif // GAMEMANAGER_H
