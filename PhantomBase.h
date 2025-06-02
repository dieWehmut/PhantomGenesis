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
protected:
    QPointF wanderTarget;//随机移动
    QElapsedTimer showTimer;//显示计时器
    int showDuration = 0;//显示时长
    bool exposed = false;//是否暴露
};

#endif // PHANTOMBASE_H