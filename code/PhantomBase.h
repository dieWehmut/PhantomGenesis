#ifndef PHANTOMBASE_H
#define PHANTOMBASE_H
#include "ActiveObject.h"
#include <QGraphicsEllipseItem>
#include <QElapsedTimer>
#include<QGraphicsScene>

class PhantomBase : public ActiveObject {
public:
    PhantomBase();
    virtual ~PhantomBase();
    virtual void updateStatus() = 0;//状态更新
    virtual void shootWave(const QPointF& direction);//发射波
    void setHp(int v) override;//扣血时暴露
    void showForAWhile(int ms);//显示一段时间
    bool isExposed() const { return exposed; }
    void setWanderTarget(const QPointF& target) { wanderTarget = target; }
    static void setForceChasePlayer(bool v) { forceChasePlayer = v; }
    static bool isForceChasePlayer() { return forceChasePlayer; } 
    virtual void stopTimers() override {}
    virtual void startTimers() override {}
protected:
    QPointF wanderTarget;//随机移动
    QElapsedTimer showTimer;//显示计时器
    int showDuration = 0;//显示时长
    bool exposed = false;//是否暴露
    static bool forceChasePlayer;//强制锁定
};

#endif // PHANTOMBASE_H