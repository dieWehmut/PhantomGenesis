#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QGraphicsScene>
#include <QTimer>
#include "Map.h"
#include "Player.h"
#include "FlamePhantom.h"
#include<QVector>
#include <QGraphicsView>
class GameManager : public QObject 
{
    Q_OBJECT
public:
    explicit GameManager(QGraphicsView* graphicsView, QObject *parent = nullptr);
    ~GameManager();
    void startGame();//开始游戏
    void pauseGame();//暂停
    void resumeGame();//恢复游戏
    bool eventFilter(QObject *watched, QEvent *event) override;
    Player* getPlayer() const { return player; } 
    bool isGamePaused() const { return gamePaused; } 
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
    QVector<FlamePhantom*> flamePhantoms;//幻影容器
    void spawnFlamePhantoms();//幻影生成
    void updatePhantoms();//更新所有幻影的状态
    QTimer* phantomSpawnTimer;//幻影定时器
    void updateVisibleTiles();//更新视野地图
};

#endif // GAMEMANAGER_H
