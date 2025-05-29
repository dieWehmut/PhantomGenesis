#ifndef PHANTOMBASE_H
#define PHANTOMBASE_H
#include"ActiveObject.h"
#include <QGraphicsEllipseItem>
#include<QGraphicsScene>
class PhantomBase: public ActiveObject{
public:
    PhantomBase();
    virtual ~PhantomBase();
    int getAtkRange() const { return atkRange; }
protected:
    QPointF wanderTarget;//随机移动目标
    QGraphicsEllipseItem* rangeIndicatorItem = nullptr;//范围指示器
};

#endif // PHANTOMBASE_H
