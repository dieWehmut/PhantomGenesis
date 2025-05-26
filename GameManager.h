#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QGraphicsScene>
#include <QTimer>
#include "Map.h"
#include "Player.h"
#include <QGraphicsView>
class GameManager : public QObject 
{
    Q_OBJECT
public:
    explicit GameManager(QGraphicsView* graphicsView, QObject *parent = nullptr);
    ~GameManager();
    void startGame();
private:
    QGraphicsView* gameView;//游戏视图
    QGraphicsScene* gameScene;//游戏场景
    Map* gameMap;//地图
    Player* player;//玩家
    QTimer* gameLoopTimer;//计时器
};

#endif // GAMEMANAGER_H
