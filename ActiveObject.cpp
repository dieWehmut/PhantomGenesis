#include "ActiveObject.h"

ActiveObject::ActiveObject(QObject *parent) : QObject(parent), QGraphicsPixmapItem() {
    
}

void ActiveObject::updatePosition() {
}
bool ActiveObject::checkCollision(const QPointF& newPos) {
    return false;
}