#include "Wave.h"
#include <QGraphicsScene>
#include <QtMath>

Wave::Wave(QObject *parent) : ActiveObject(parent) {
    // 设置属性
    setSpeed(1.0f);
    setAtk(10);
    maxDistance = 400.0f;
    curDistance = 0.0f;
    direction = QPointF(1, 0);
    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Wave::onMoveTimerTimeout);
    moveTimer->start(1000 / 60); 

    rotateTimer = new QTimer(this);
    connect(rotateTimer, &QTimer::timeout, this, &Wave::onRotateTimerTimeout);
}

Wave::~Wave(){//消失
    if (moveTimer) {
        moveTimer->stop();
        moveTimer = nullptr;
    }
    if (aoeTimer) {
        aoeTimer->stop();
        aoeTimer = nullptr;
    }
    if (rotateTimer) {
        rotateTimer->stop();
        rotateTimer = nullptr;
    }
    if (scene()) {
        scene()->removeItem(this);
    }
}
void Wave::startRotate(int intervalMs) {
    if (rotateTimer) {
        rotateTimer->start(intervalMs);
    }
}

void Wave::stopRotate() {
    if (rotateTimer) {
        rotateTimer->stop();
    }
}

void Wave::onRotateTimerTimeout() {
    if (!scene()) return;
    angle += rotateStep;
    if (angle >= 360) angle -= 360;
    setAngle(angle);
    update();
}
void Wave::startAoeTimer(int intervalMs) {
    if (!aoeTimer) {
        aoeTimer = new QTimer(this);
        connect(aoeTimer, &QTimer::timeout, this, &Wave::onAoeTimerTimeout);
    }
    aoeTimer->start(intervalMs);
}

void Wave::stopAoeTimer() {
    if (aoeTimer) aoeTimer->stop();
}

void Wave::onAoeTimerTimeout() {
}
void Wave::setDirection(const QPointF &dir) {
    qreal length = qSqrt(dir.x() * dir.x() + dir.y() * dir.y());
    if (length > 0) {
        direction.setX(dir.x() / length);
        direction.setY(dir.y() / length);
    } else {
        this->direction = QPointF(1, 0);//默认向右
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
    QPointF delta = direction * getSpeed();
    QPointF newPos = pos() + delta;
    curDistance += std::hypot(delta.x(), delta.y());
    if (curDistance >= maxDistance) {
        deleteLater();
        return;
    }
    setPos(newPos);
    checkCollision(newPos);
}

bool Wave::checkCollision(const QPointF& newPos){
    QList<QGraphicsItem *> collidingItems = scene()->items(QRectF(newPos, QSizeF(boundingRect().width(), boundingRect().height())));
    bool collided = false;
    for (QGraphicsItem *item : collidingItems){
        if (item != this && item != ignoreItem && item->isVisible()){
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
    if (!scene()) return;
    updatePosition();
}
void Wave::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->save();
    QSize sz = pixmap().size();
    QPointF center(sz.width() / 2.0, sz.height() / 2.0);
    painter->translate(center);
    qreal drawAngle = angle;
    if (qFuzzyIsNull(angle)) {
        drawAngle = qAtan2(direction.y(), direction.x()) * 180 / M_PI + 180;
    }
    painter->rotate(drawAngle);
    painter->translate(-center);
    painter->drawPixmap(0, 0, pixmap());
    painter->restore();
}
