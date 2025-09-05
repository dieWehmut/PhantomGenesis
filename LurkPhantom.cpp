#include "LurkPhantom.h"
#include <QRandomGenerator>
#include <QtMath>
#include "LurkWave.h"
LurkPhantom::LurkPhantom(Player* targetPlayer)
    : player(targetPlayer)
{
    setStaticPixmap(QPixmap(":/Resource/lurkPhantom.png").scaled(96, 96));
    wanderTarget = pos();
    setMaxHp(5000);
    hp = maxHp;
    setSpeed(4.0f);
    setSightRange(10000);
    setAtkRange(300);
    setAtk(500);
    blinkCooldown = 3000;
    lastBlinkTimer.start();
    createRangeIndicator(QColor(0, 128, 255, 80), 1);
}

LurkPhantom::~LurkPhantom() {

}

void LurkPhantom::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if (shieldActive) {
        painter->save();
        QPointF center = boundingRect().center();
        float radius = getAtkRange();
        QLinearGradient gradient(center, QPointF(center.x() + radius, center.y()));
        gradient.setColorAt(0, QColor(0, 128, 255, 180));
        gradient.setColorAt(1, QColor(0, 64, 255, 40));
        QPen decorativePen;
        decorativePen.setWidth(2);
        decorativePen.setCapStyle(Qt::RoundCap);
        for (int i = 0; i < 16; ++i) {
            double angle = i * M_PI / 8;
            QPointF start = center + QPointF(20 * cos(angle), 20 * sin(angle));
            QPointF end(center.x() + radius * cos(angle), center.y() + radius * sin(angle));
            gradient.setStart(start);
            gradient.setFinalStop(end);
            decorativePen.setBrush(gradient);
            painter->setPen(decorativePen);
            painter->drawLine(start, end);
        }
        if (player) {//锁玩家
            QPointF playerCenter = mapFromScene(player->pos() + 
                QPointF(player->boundingRect().width()/2, player->boundingRect().height()/2));
            QPointF myCenter = boundingRect().center();
            qreal distance = QLineF(myCenter, playerCenter).length();
            if (distance <= getAtkRange()) {
                if (!hasDealtDamage) {
                    player->setHp(player->getHp() - 4*getAtk());
                    static QSoundEffect laserEffect;
                    if (laserEffect.source().isEmpty())
                        laserEffect.setSource(QUrl("qrc:/Resource/lurkPhantomLaser.wav"));
                    laserEffect.setVolume(1.0f);
                    laserEffect.play();
                    hasDealtDamage = true;
                }
                QPen lockPen(QColor(0, 64, 255), 4);
                painter->setPen(lockPen);
                QPointF dir = playerCenter - myCenter;
                if (!qFuzzyIsNull(dir.x()) || !qFuzzyIsNull(dir.y())) {
                    qreal len = std::hypot(dir.x(), dir.y());
                    QPointF unitDir = dir / len;
                    QPointF extendedEnd = playerCenter + unitDir * len;
                    painter->drawLine(myCenter, extendedEnd);
                } else {
                    painter->drawLine(myCenter, playerCenter);
                }
            }
        }
        painter->restore();
    }
    PhantomBase::paint(painter, option, widget);
}
void LurkPhantom::activateShield() {
    shieldActive = true;
    shieldTimer.start();
    setRangeIndicatorVisible(true);
    hasDealtDamage = false;
}

void LurkPhantom::updateStatus() {
    if (!player) return;
    if (shieldActive && shieldTimer.elapsed() >= shieldDuration) {
        shieldActive = false;
        setRangeIndicatorVisible(false);
    }
    QPointF myCenter = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
    QPointF playerCenter = player->pos() + QPointF(player->boundingRect().width()/2, player->boundingRect().height()/2);
    qreal dx = myCenter.x() - playerCenter.x();
    qreal dy = myCenter.y() - playerCenter.y();
    qreal distanceSquared = dx*dx + dy*dy;
    qreal atkRangeSquared = getAtkRange() * getAtkRange();
    setRangeIndicatorVisible(shieldActive);
    if (distanceSquared < getSightRange() * getSightRange() || PhantomBase::isForceChasePlayer()) {
        if (lastBlinkTimer.elapsed() >= blinkCooldown) {
            QPointF V = playerCenter - myCenter;
            QPointF blinkPos = playerCenter + V - QPointF(boundingRect().width()/2, boundingRect().height()/2);
            setPos(blinkPos);
            lastBlinkTimer.restart();
            QPointF dir = playerCenter - blinkPos;
            qreal len = std::hypot(dir.x(), dir.y());
            if (len > 1e-2) dir /= len;
            shootWave(dir);
        }
        if (distanceSquared < (player->getAtkRange() + 50)*(player->getAtkRange() + 50)) {
            QPointF dir = myCenter - playerCenter;
            qreal len = std::hypot(dir.x(), dir.y());
            if (len > 1e-2) {
                dir /= len;
                setPos(pos() + dir * getSpeed() * 1.5);
            }
        }
    }
    if (QLineF(myCenter, wanderTarget).length() < 5.0) {
        qreal angle = QRandomGenerator::global()->generateDouble() * 2 * M_PI;
        qreal radius = 30.0 + QRandomGenerator::global()->generateDouble() * (100.0 - 30.0);
        wanderTarget = myCenter + QPointF(qCos(angle), qSin(angle)) * radius;
    }
    QPointF dir = wanderTarget - myCenter;
    qreal len = std::hypot(dir.x(), dir.y());
    if (len > 1e-2) {
        dir /= len;
        setPos(pos() + dir * getSpeed() * 0.5);
    }
}

void LurkPhantom::shootWave(const QPointF& direction) {
    if (!scene()) return;
    QPointF dir1 = direction;
    double angle = std::atan2(direction.y(), direction.x());
    double angle7_5 = M_PI / 24;
    double angle15 = M_PI / 12;
    QPointF dirLeft7_5(std::cos(angle + angle7_5), std::sin(angle + angle7_5));
    QPointF dirRight7_5(std::cos(angle - angle7_5), std::sin(angle - angle7_5));
    QPointF dirLeft15(std::cos(angle + angle15), std::sin(angle + angle15));
    QPointF dirRight15(std::cos(angle - angle15), std::sin(angle - angle15));
    LurkWave* wave1 = new LurkWave(dir1, getAtk(), this);
    LurkWave* wave2 = new LurkWave(dirLeft7_5, getAtk(), this);
    LurkWave* wave3 = new LurkWave(dirRight7_5, getAtk(), this);
    LurkWave* wave4 = new LurkWave(dirLeft15, getAtk(), this);
    LurkWave* wave5 = new LurkWave(dirRight15, getAtk(), this);
    QPointF centerPos = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
    QPointF offset = QPointF(wave1->boundingRect().width()/2, wave1->boundingRect().height()/2);
    wave1->setPos(centerPos - offset);
    wave2->setPos(centerPos - offset);
    wave3->setPos(centerPos - offset);
    wave4->setPos(centerPos - offset);
    wave5->setPos(centerPos - offset);
    wave1->setIgnoreItem(this);
    wave2->setIgnoreItem(this);
    wave3->setIgnoreItem(this);
    wave4->setIgnoreItem(this);
    wave5->setIgnoreItem(this);
    scene()->addItem(wave1);
    scene()->addItem(wave2);
    scene()->addItem(wave3);
    scene()->addItem(wave4);
    scene()->addItem(wave5);
    activateShield();
}

void LurkPhantom::setHp(int v) {
    int oldHp = getHp();
    if (shieldActive && v < oldHp) {
        int damage = oldHp - v;
        int recovery = damage*0.5;
        PhantomBase::setHp(v + recovery);
    } else {
        PhantomBase::setHp(v);
    }
}
void LurkPhantom::stopTimers() {

}
void LurkPhantom::startTimers() {

}
