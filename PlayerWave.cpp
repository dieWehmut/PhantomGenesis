#include "PlayerWave.h"
#include <QTransform>
#include <QtMath>
#include "GameManager.h"
#include "Player.h"
#include "PhantomBase.h"
#include <QGraphicsScene>
#include <QList>
PlayerWave::PlayerWave(const QPointF &direction, int damage, QObject *parent)
    : Wave(parent), hitObjects() {
    setDirection(direction);
    setSpeed(10.0f);
    setAtk(damage);
    setMaxDistance(600.0f);
    setStaticPixmap(QPixmap("Resource/playerWave.png").scaled(64, 64)); 
    maxPierceCnt = 3; 
    curPierceCnt = 0;  
    aoeRadius = 0;
    startAoeTimer(3000);
    startRotate(16);
}
void PlayerWave::onAoeTimerTimeout() {
    if (!scene()) return;
    applyAoeDamage(nullptr);
}


PlayerWave::~PlayerWave() {
    stopAoeTimer();
    stopRotate();
    if (moveTimer) moveTimer->stop();
    hitObjects.clear();
}
void PlayerWave::handleCollision(QGraphicsItem *item) {
    if (!scene()) return;
    if (dynamic_cast<Player*>(item)) {
        return;
    }
    ActiveObject *obj = dynamic_cast<ActiveObject*>(item);
    if (obj && !hitObjects.contains(obj) && dynamic_cast<PhantomBase*>(obj)) {
        obj->setHp(obj->getHp() - getAtk());
        hitObjects.insert(obj);
        curPierceCnt++;
        QPointF bulletCenter = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
        QPointF objCenter = obj->pos() + QPointF(obj->boundingRect().width()/2, obj->boundingRect().height()/2);
        QPointF knockbackDir = objCenter - bulletCenter;
        qreal len = std::hypot(knockbackDir.x(), knockbackDir.y());
        if (len > 1e-3) {
            knockbackDir /= len;
            qreal knockbackDist = 50.0;
            obj->setPos(obj->pos() + knockbackDir * knockbackDist);
        }
        applyAoeDamage(obj);
        if (curPierceCnt >= maxPierceCnt) {
            deleteLater();
            return;
        }
    }
}

void PlayerWave::applyAoeDamage(ActiveObject* directHitObj) {
    if (!scene() || aoeRadius <= 0) return;
    QPointF center = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
    QList<QGraphicsItem*> items = scene()->items(QRectF(center.x() - aoeRadius, center.y() - aoeRadius, aoeRadius * 2, aoeRadius * 2));
    for (QGraphicsItem *item : items) {
        ActiveObject *obj = dynamic_cast<ActiveObject*>(item);
        if (obj && obj->scene() == this->scene() && obj != directHitObj && !hitObjects.contains(obj) && dynamic_cast<PhantomBase*>(obj)) {
            if (!obj->scene()) continue;
            QPointF objCenter = obj->pos() + QPointF(obj->boundingRect().width()/2, obj->boundingRect().height()/2);
            if (QLineF(center, objCenter).length() <= aoeRadius) {
                obj->setHp(obj->getHp() - int(getAtk() * 0.8f));
            }
        }
    }
}
void PlayerWave::updatePosition() {
    Wave::updatePosition(); 
}
void PlayerWave::onMoveTimerTimeout() {
    if (!scene()) return;
    updatePosition();
}
