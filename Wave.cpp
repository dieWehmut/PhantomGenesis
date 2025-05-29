#include "Wave.h"
#include <QGraphicsScene>
#include <QtMath>

Wave::Wave(QObject *parent) : ActiveObject(parent) {
    // 设置属性
    setSpeed(1.0f);
    setAtk(10);
    maxDistance = 200.0f;
    curDistance = 0.0f;
    direction = QPointF(1, 0);
    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Wave::onMoveTimerTimeout);
    moveTimer->start(1000 / 60); 
}

Wave::~Wave(){//消失
    if (moveTimer) {
        moveTimer->stop();
        delete moveTimer;
    }
    if (scene()) {
        scene()->removeItem(this);
    }
}

void Wave::setDirection(const QPointF &dir) {
    qreal length = qSqrt(dir.x() * dir.x() + dir.y() * dir.y());
    if (length > 0) {
        direction.setX(dir.x() / length);
        direction.setY(dir.y() / length);
    } else {
        this->direction = QPointF(1, 0); // 默认向右
    }
}
QPointF Wave::getDirection() const {
    return direction;
}
void Wave::setMaxDistance(float distance) {
    maxDistance = distance;
}
float Wave::getMaxDistance() const {
    return maxDistance;
}
void Wave::setCurDistance(float distance) {
    curDistance = distance;
}
float Wave::getCurDistance() const {
    return curDistance;
}

void Wave::updatePosition() {
    QPointF newPos = pos() + direction * getSpeed();
    setPos(newPos);
    checkCollision(newPos);
}

bool Wave::checkCollision(const QPointF& newPos){
    QPointF oldPos = pos();
    setPos(newPos);
    QList<QGraphicsItem *> collidingItems = scene()->collidingItems(this);
    setPos(oldPos);
    bool collided = false;
    for (QGraphicsItem *item : collidingItems){
        if (item != this){
            handleCollision(item);
            collided = true;
        }
    }
    return collided;
}
void Wave::handleCollision(QGraphicsItem *item) {
    deleteLater();
}
void Wave::onMoveTimerTimeout() {
    updatePosition();
}
