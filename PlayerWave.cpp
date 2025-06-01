#include "PlayerWave.h"
#include <QTransform>
#include <QtMath>
#include "Player.h"
#include "PhantomBase.h"
#include <QGraphicsScene>
#include <QList>
PlayerWave::PlayerWave(const QPointF &direction, int damage, QObject *parent)
    : Wave(direction, damage, 800, 5.0f, QPixmap("Resource/playerWave.png").scaled(64,64), parent),
      hitObjects(){
    setDirection(direction);
    setSpeed(10.0f);
    setAtk(damage);
    originalAtk = damage;
    setMaxDistance(1000.0f);
    setStaticPixmap(QPixmap("Resource/playerWave.png").scaled(96, 96)); 
    maxPierceCnt = 3; 
    curPierceCnt = 0;
    aoeRadius = 100;
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
    QPointF waveCenter = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
    qreal searchRadius = aoeRadius * 1.5;
    QRectF searchArea(
        waveCenter.x() - searchRadius,
        waveCenter.y() - searchRadius,
        searchRadius * 2,
        searchRadius * 2
    );
    QList<QGraphicsItem*> items = scene()->items(searchArea);
    for (QGraphicsItem *item : items) {
        ActiveObject *obj = dynamic_cast<ActiveObject*>(item);
        PhantomBase *phantom = dynamic_cast<PhantomBase*>(item);
        if (obj && phantom && obj->scene() == this->scene() && 
            obj != directHitObj && !hitObjects.contains(obj)) {
            QPointF objCenter = obj->pos() + QPointF(obj->boundingRect().width()/2, obj->boundingRect().height()/2);
            qreal distance = QLineF(waveCenter, objCenter).length();
            if (distance <= aoeRadius) {
                qreal damageRatio = 1.0 - (distance / aoeRadius) * 0.3;
                int damage = int(getAtk() * damageRatio);
                damage = qMax(damage, int(getAtk() * 0.7));
                obj->setHp(obj->getHp() - damage);
                hitObjects.insert(obj);
                QPointF knockbackDir = objCenter - waveCenter;//击退效果
                qreal len = std::hypot(knockbackDir.x(), knockbackDir.y());
                if (len > 1e-3) {
                    knockbackDir /= len;
                    qreal knockbackDist = 30.0 * (1.0 - distance / aoeRadius);
                    obj->setPos(obj->pos() + knockbackDir * knockbackDist);
                }
            }
        }
    }
}
void PlayerWave::updatePosition() {
    updateBoostState(); 
    Wave::updatePosition(); 
}
void PlayerWave::updateBoostState() {
    if (!scene()) return;
    Player* player = nullptr;
    for (QGraphicsItem* item : scene()->items()) {
        player = dynamic_cast<Player*>(item);
        if (player) break;
    }
    if (!player) return;
    QPointF waveCenter = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
    QPointF playerCenter = player->pos() + QPointF(player->boundingRect().width()/2, player->boundingRect().height()/2);
    qreal dist = QLineF(waveCenter, playerCenter).length();

    float atkRange = player->getAtkRange();
    if (dist <= atkRange) {
        if (!boosted) {
            setSpeed(originalSpeed * 5.0f);//速度加成
            setAtk(int(originalAtk * 2.5f));//伤害加成
            boosted = true;
        }
    } else {
        if (boosted) {
            setSpeed(originalSpeed);
            setAtk(originalAtk);
            boosted = false;
        }
    }
}
void PlayerWave::onMoveTimerTimeout() {
    if (!scene()) return;
    updatePosition();
}
void PlayerWave::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->save();
    QColor aoeColor(128, 0, 128, 50);
    painter->setBrush(QBrush(aoeColor));
    painter->setPen(Qt::NoPen);
    QRectF bounds = boundingRect();
    QPointF center(bounds.width() / 2, bounds.height() / 2);
    painter->drawEllipse(center, aoeRadius, aoeRadius);
    painter->restore();
    Wave::paint(painter, option, widget);
}
