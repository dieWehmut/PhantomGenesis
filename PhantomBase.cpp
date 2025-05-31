#include "PhantomBase.h"
#include <QPointer>
PhantomBase::PhantomBase(){
rangeIndicatorItem = nullptr;
}
void PhantomBase::shootWave(const QPointF& direction) {
}
PhantomBase::~PhantomBase(){
    destroyed = true;
    if (rangeIndicatorItem) {
        if (rangeIndicatorItem->scene())
            rangeIndicatorItem->scene()->removeItem(rangeIndicatorItem);
        delete rangeIndicatorItem;
        rangeIndicatorItem = nullptr;
    }
}

void PhantomBase::showForAWhile(int ms) {
    showTimer.start();
    showDuration = ms;
    exposed = true;
    setVisible(true);
    setRangeIndicatorVisible(true);

    QPointer<PhantomBase> self(this);
    QTimer::singleShot(ms, [self]() {
        if (!self) return;
        if (self->destroyed) return;
        if (!self->rangeIndicatorItem) return;
        self->exposed = false;
        self->setRangeIndicatorVisible(false);
    });
}
void PhantomBase::setHp(int v) {
    if (v < getHp()) {
        showForAWhile();
    }
    ActiveObject::setHp(v);
}
