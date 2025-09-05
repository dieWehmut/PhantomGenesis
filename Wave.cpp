#include "Wave.h"
#include <QGraphicsScene>
#include <QtMath>
#include "Player.h"
Wave::Wave(const QPointF& dir, int atk, float maxDist, float speed, const QPixmap& pix, QObject *parent)
    : ActiveObject(parent),
      maxDistance(maxDist), curDistance(0.0f), direction(1, 0),
      moveTimer(new QTimer(this)), rotateTimer(new QTimer(this))
{
    setDirection(dir);
    setAtk(atk);
    setMaxDistance(maxDist);
    setSpeed(speed);
    setStaticPixmap(pix);
    connect(moveTimer, &QTimer::timeout, this, &Wave::onMoveTimerTimeout);
    moveTimer->start(1000 / 60);
    connect(rotateTimer, &QTimer::timeout, this, &Wave::onRotateTimerTimeout);
    startRotate(16);
}

Wave::~Wave() {
    if (moveTimer) moveTimer->stop();
    if (aoeTimer) aoeTimer->stop();
    if (rotateTimer) rotateTimer->stop();
    if (scene()) scene()->removeItem(this);
}

void Wave::startRotate(int intervalMs) {
    if (rotateTimer) rotateTimer->start(intervalMs);
}

void Wave::stopRotate() {
    if (rotateTimer) rotateTimer->stop();
}

void Wave::onRotateTimerTimeout() {
    angle = std::fmod(angle + rotateStep, 360.0);
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
QRectF Wave::boundingRect() const {
    return QRectF(0, 0, 64, 64);
}
void Wave::stopAoeTimer() {
    if (aoeTimer) aoeTimer->stop();
}
void Wave::playCollisionEffect(QGraphicsItem* item) {

}

void Wave::playCollisionSound(QGraphicsItem* item) {

}
void Wave::handleCollision(QGraphicsItem *item) {
    playCollisionEffect(item);
    playCollisionSound(item);
    if (targetType == PlayerTarget) {
        Player* player = dynamic_cast<Player*>(item);
        if (player && item != ignoreItem) {
            player->setHp(player->getHp() - getAtk());
            deleteLater();
        }
    } else if (targetType == PhantomTarget) {//给playerWave
    }
}
void Wave::onAoeTimerTimeout() {}

void Wave::setDirection(const QPointF &dir) {
    qreal length = std::hypot(dir.x(), dir.y());
    direction = (length > 0) ? dir / length : QPointF(1, 0);
}

QPointF Wave::getDirection() const { return direction; }
void Wave::setMaxDistance(float d) { maxDistance = d; }
float Wave::getMaxDistance() const { return maxDistance; }
void Wave::setCurDistance(float d) { curDistance = d; }
float Wave::getCurDistance() const { return curDistance; }

void Wave::updatePosition() {
    Player* player = nullptr;
    if (scene()) {
        for (QGraphicsItem* item : scene()->items()) {
            player = dynamic_cast<Player*>(item);
            if (player) break;
        }
    }
    if (player) {//仅在视野内可见
        QPointF waveCenter = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
        QPointF playerCenter = player->pos() + QPointF(player->boundingRect().width()/2, player->boundingRect().height()/2);
        setVisible(QLineF(waveCenter, playerCenter).length() <= player->getSightRange());
    } else {
        setVisible(false);
    }

    if (!scene()) return;
    QPointF delta = direction * getSpeed();
    QPointF newPos = pos() + delta;
    curDistance += std::hypot(delta.x(), delta.y());
    if (curDistance >= maxDistance) { deleteLater(); return; }
    setPos(newPos);
    if (scene()) checkCollision(newPos);
}

bool Wave::checkCollision(const QPointF& newPos) {
    if (!scene()) return false;
    QRectF collisionRect(newPos, QSizeF(boundingRect().width(), boundingRect().height()));
    QList<QGraphicsItem *> collidingItems = scene()->items(collisionRect);
    bool collided = false;
    for (QGraphicsItem *item : collidingItems) {
        if (item && item != this && item != ignoreItem && item->isVisible()) {
            handleCollision(item);
            collided = true;
        }
    }
    return collided;
}


void Wave::onMoveTimerTimeout() {
    if (scene()) updatePosition();
}

void Wave::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->save();
    QSize sz = pixmap().size();
    QPointF center(sz.width() / 2.0, sz.height() / 2.0);
    painter->translate(center);
    painter->rotate(qFuzzyIsNull(angle) ? qAtan2(direction.y(), direction.x()) * 180 / M_PI + 180 : angle);
    painter->translate(-center);
    painter->drawPixmap(0, 0, pixmap());
    painter->restore();
}
