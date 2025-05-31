#ifndef PHANTOMBASE_H
#define PHANTOMBASE_H
#include"ActiveObject.h"
#include <QGraphicsEllipseItem>
#include<QGraphicsScene>
#include <QElapsedTimer> 
class PhantomBase: public ActiveObject{
public:
    PhantomBase();
    virtual ~PhantomBase();
    int getAtkRange() const { return atkRange; }
    virtual void shootWave(const QPointF& direction);//发射波
    void setHp(int v) override;//扣血了会暴露
void setRangeIndicatorVisible(bool visible) {
    if (!rangeIndicatorItem) return;//防止悬空指针访问
    if (!this->scene()) return;//没有场景也不处理
    if (!rangeIndicatorItem->scene()) {
        this->scene()->addItem(rangeIndicatorItem);
    }
    rangeIndicatorItem->setVisible(visible);
}
    void showForAWhile(int ms = 5000);//显示一段时间
    bool isExposed() const { return exposed; }
protected:
    QPointF wanderTarget;//随机移动目标
    QGraphicsEllipseItem* rangeIndicatorItem = nullptr;//范围指示器
    QElapsedTimer showTimer;
    int showDuration = 0; 
    bool exposed = false;
    bool destroyed = false;
};

#endif // PHANTOMBASE_H
