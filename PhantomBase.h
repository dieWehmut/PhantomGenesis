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
    int getAtkRange() const { return atkRange; }
    virtual void updateStatus() = 0;
    virtual void shootWave(const QPointF& direction);
    void setHp(int v) override;//扣血时暴露
    void showForAWhile(int ms);//显示一段时间
    bool isExposed() const { return exposed; }
protected:
    QPointF wanderTarget;
    QElapsedTimer showTimer;
    int showDuration = 0;
    bool exposed = false;
};

#endif // PHANTOMBASE_H