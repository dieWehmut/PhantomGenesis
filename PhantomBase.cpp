#include "PhantomBase.h"
#include <QPointer>
PhantomBase::PhantomBase() {}
bool PhantomBase::forceChasePlayer = false; 
PhantomBase::~PhantomBase() {

}
void PhantomBase::setHp(int v) {
    showForAWhile(3000);
    ActiveObject::setHp(v);
}
void PhantomBase::showForAWhile(int ms) {
    showTimer.start();
    showDuration = ms;
    exposed = true;
    QPointer<PhantomBase> self(this);
    QTimer::singleShot(ms, [self]() {
        if (!self) return;
        self->exposed = false;
    });
}

void PhantomBase::shootWave(const QPointF&) {
}