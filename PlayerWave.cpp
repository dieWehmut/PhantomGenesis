#include "PlayerWave.h"
#include <QTransform>
#include <QtMath>
#include "Player.h"
#include "PhantomBase.h"
#include <QGraphicsScene>
#include <QList>
#include <QSoundEffect>
#include "LurkWave.h"
#include "FlameWave.h"
PlayerWave::PlayerWave(const QPointF &direction, int damage, QObject *parent)
    : Wave(direction, damage, 800, 10.0f, QPixmap(":/Resource/playerWave.png").scaled(64,64), parent),

    hitObjects(), aoeRadius(0), playerWaveEffectInitialized(false) {
    setDirection(direction);
    setAtk(damage);
    originalAtk = damage;
    setMaxDistance(800.0f);

    maxPierceCnt = 10;
    curPierceCnt = 0;
    startAoeTimer(100);
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
        if (auto* lurkWave = dynamic_cast<LurkWave*>(item)) {
        curPierceCnt++;
        if (!playerWaveEffectInitialized) {
            playerWaveHitEffect.setSource(QUrl("qrc:/Resource/playerWaveHit.wav"));
            playerWaveHitEffect.setVolume(1.0f);
            playerWaveEffectInitialized = true;
        }
        playerWaveHitEffect.play();
        lurkWave->deleteLater();
        if (curPierceCnt >= maxPierceCnt) {
            deleteLater();
            return;
        }
        return;
    }
    if (auto* flameWave = dynamic_cast<FlameWave*>(item)) {
        curPierceCnt++;
        if (!playerWaveEffectInitialized) {
            playerWaveHitEffect.setSource(QUrl("qrc:/Resource/playerWaveHit.wav"));
            playerWaveHitEffect.setVolume(1.0f);
            playerWaveEffectInitialized = true;
        }
        playerWaveHitEffect.play();
        flameWave->deleteLater();
        if (curPierceCnt >= maxPierceCnt) {
            deleteLater();
            return;
        }
        return;
    }
    ActiveObject *obj = dynamic_cast<ActiveObject*>(item);
    if (obj && !hitObjects.contains(obj) && dynamic_cast<PhantomBase*>(obj)) {
        obj->setHp(obj->getHp() - getAtk());
        hitObjects.insert(obj);
        curPierceCnt++;
        if (!playerWaveEffectInitialized) {
            playerWaveHitEffect.setSource(QUrl("qrc:/Resource/playerWaveHit.wav"));
            playerWaveHitEffect.setVolume(1.0f);//大一点
            playerWaveEffectInitialized = true;
        }
        playerWaveHitEffect.play();
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
    if (!scene() || getAoeRadius() <= 0) return;
    QPointF waveCenter = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
    qreal searchRadius = getAoeRadius() * 1.5;
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
                damage = qMax(damage, int(getAtk() * 0.5));
                obj->setHp(obj->getHp() - damage);
                hitObjects.insert(obj);
            if (!playerWaveEffectInitialized) {
                playerWaveHitEffect.setSource(QUrl("qrc:/Resource/playerWaveHit.wav"));

                playerWaveHitEffect.setVolume(0.7f);
                playerWaveEffectInitialized = true;
            }
            playerWaveHitEffect.play();
                QPointF knockbackDir = objCenter - waveCenter;//击退
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
            setAtk(int(originalAtk * 2.5f));//伤害加成
            boosted = true;
        }
    } else {
        if (boosted) {
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
void PlayerWave::playCollisionEffect(QGraphicsItem* item) {

}

void PlayerWave::playCollisionSound(QGraphicsItem* item) {

}
void PlayerWave::startTimers() {
    if (moveTimer) moveTimer->start();
    if (aoeTimer) aoeTimer->start();
}
void PlayerWave::stopTimers() {
    if (moveTimer) moveTimer->stop();
    if (aoeTimer) aoeTimer->stop();
}
