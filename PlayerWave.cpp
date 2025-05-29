#include "PlayerWave.h"
#include <QTransform>
#include <QtMath>
#include "GameManager.h"
#include "Player.h"
#include "PhantomBase.h"
#include <QGraphicsScene>
#include <QList>
PlayerWave::PlayerWave(const QPointF &direction, int damage, QObject *parent)
    : Wave(parent){
    setDirection(direction);
    setSpeed(3.0f);
    setAtk(damage);
    setMaxDistance(300.0f);
    setPixmap(QPixmap("Resource/playerWave.png").scaled(32, 32));
    setTransformOriginPoint(16, 16);
    maxPierceCnt = 3; 
    curPierceCnt = 0;  
    aoeRadius = 0;  
}
void PlayerWave::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if (!scene()) return; 
    Player* player = nullptr;
    for (QGraphicsItem* item : scene()->items()) {
        player = dynamic_cast<Player*>(item);
        if (player) break;
    }
    if (player) {
        QPointF playerCenter = player->pos() + QPointF(player->pixmap().width() / 2, player->pixmap().height() / 2);
        QPointF waveCenter = pos() + QPointF(boundingRect().width() / 2, boundingRect().height() / 2);
        qreal dist = QLineF(playerCenter, waveCenter).length();
        if (dist >3*player->getSightRange()) {
            return;
        }
    }
    painter->save();
    qreal angle = qAtan2(getDirection().y(), getDirection().x()) * 180 / M_PI+180;
    painter->translate(24, 24);
    painter->rotate(angle);
    painter->translate(-24, -24);
    QPainterPath bigCircle;
    bigCircle.addEllipse(QRectF(0, 0, 48, 48));
    QPainterPath smallCircle;
    smallCircle.addEllipse(QRectF(8, 0, 48, 48));
    QPainterPath crescent = bigCircle.subtracted(smallCircle);
    painter->setClipPath(crescent);
    painter->drawPixmap(0, 0, pixmap().scaled(48, 48));
    painter->restore();
}



PlayerWave::~PlayerWave() {

}
void PlayerWave::handleCollision(QGraphicsItem *item) {
    if (dynamic_cast<Player*>(item)) {
        return;
    }
    ActiveObject *obj = dynamic_cast<ActiveObject*>(item);
    if (obj && !hitObjects.contains(obj) && dynamic_cast<PhantomBase*>(obj)) {
        obj->setHp(obj->getHp() - getAtk());
        hitObjects.insert(obj);
        curPierceCnt++;
        applyAoeDamage();
        if (curPierceCnt >= maxPierceCnt) {
            deleteLater();
        }
    }
}
void PlayerWave::applyAoeDamage() {
    if (!scene() || aoeRadius <= 0) return;
    QList<QGraphicsItem*> items = scene()->items(QRectF(pos().x() - aoeRadius, pos().y() - aoeRadius, aoeRadius * 2, aoeRadius * 2));
    for (QGraphicsItem *item : items) {
        ActiveObject *obj = dynamic_cast<ActiveObject*>(item);
        if (obj && !hitObjects.contains(obj)) {
            QPointF center = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
            QPointF objCenter = obj->pos() + QPointF(obj->boundingRect().width()/2, obj->boundingRect().height()/2);
            if (QLineF(center, objCenter).length() <= aoeRadius) {
                obj->setHp(obj->getHp() - getAtk());
                hitObjects.insert(obj);
            }
        }
    }
}
