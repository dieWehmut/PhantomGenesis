#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QGraphicsScene>
#include <QTimer>
#include "Map.h"
#include "Player.h"
#include "FlamePhantom.h"
#include "LurkPhantom.h"
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
public slots:
    void handleViewResize(); // 处理视图的改变
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
    QTimer* phantomSpawnTimer;//phantom定时器
    void separatePhantoms(QVector<PhantomBase*>& phantoms);//分离重叠的PhantomBase
    VisionMaskItem* visionMask = nullptr;//视野遮罩
};

#endif // GAMEMANAGER_H
