#include "FlamePhantom.h"
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QtMath>
#include <QPainter>
#include "FlameWave.h"
FlamePhantom::FlamePhantom(Player* targetPlayer) 
    : player(targetPlayer) {
    setStaticPixmap(QPixmap(":/Resource/flamePhantom.png").scaled(64, 64));
    wanderTarget = pos();
    setMaxHp(10000);
    hp = maxHp;
    setSpeed(2.0f);
    setSightRange(8000);
    setAtkRange(500);
    setAtk(500);
    setRangeAtkCD(3000);
    rangeAtkTimer.start();
    rangeIndicatorItem = new QGraphicsEllipseItem();
    rangeIndicatorItem->setZValue(0);
    QPen pen(Qt::red);
    pen.setWidth(4);
    rangeIndicatorItem->setPen(pen);
    rangeIndicatorItem->setBrush(Qt::NoBrush);
    rangeIndicatorItem->hide();

}
FlamePhantom::~FlamePhantom() {

}
void FlamePhantom::shootWave(const QPointF& direction) {
    if (!scene()) return;
    FlameWave* wave = new FlameWave(direction, atk);
    wave->setIgnoreItem(this);
    QPointF spawnPos = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
    wave->setPos(spawnPos);
    scene()->addItem(wave);
}
void FlamePhantom::updateStatus() {
    if (!player) return;
    QPointF myCenter = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
    QPointF playerCenter = player->pos() + QPointF(player->boundingRect().width()/2, player->boundingRect().height()/2);
    qreal distance = QLineF(myCenter, playerCenter).length();
    bool inRange = (distance <= getAtkRange());
    isLocking = inRange;
    if (rangeIndicatorItem && scene() && !rangeIndicatorItem->scene()) {
        scene()->addItem(rangeIndicatorItem);
        rangeIndicatorItem->hide();
    }
    if (inRange) {
        rangeDebuff();
        if (rangeAtkTimer.elapsed() >= getRangeAtkCD()) {
            player->setHp(player->getHp() - 0.1*atk);//定时扣血
            // 发射火焰波
            QPointF dir = playerCenter - myCenter;
            qreal len = std::hypot(dir.x(), dir.y());
            if (len > 1e-2) dir /= len;
            shootWave(dir);
            rangeAtkTimer.restart();
        }
    }

    if (inRange != lastInRange) {
        if (inRange) {
            qreal atkRange = getAtkRange();
            rangeIndicatorItem->setRect(
                myCenter.x() - atkRange,
                myCenter.y() - atkRange,
                atkRange * 2,
                atkRange * 2
            );
            rangeIndicatorItem->show();
        } else {
            rangeIndicatorItem->hide();
        }
        lastInRange = inRange;
    }
    bool moved = false;
    if (distance < getSightRange() || PhantomBase::isForceChasePlayer()) {
        QPointF dir = playerCenter - myCenter;
        qreal len = std::hypot(dir.x(), dir.y());
        if (len > 1e-2) {
            dir /= len;
            setPos(pos() + dir * getSpeed());
            moved = true;
        }
        if (inRange) {
            if (collidesWithItem(player)) {
                meleeAtk();
            }
        }
    } else {
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
            moved = true;
        }
    }
    if (inRange && moved) {
        myCenter = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
        rangeIndicatorItem->setRect(
            myCenter.x() - getAtkRange(),
            myCenter.y() - getAtkRange(),
            getAtkRange() * 2,
            getAtkRange() * 2
        );
    }
}

void FlamePhantom::rangeDebuff() {
    if (!scene()) return;
    QList<QGraphicsItem*> items = scene()->items(QRectF(
        x() - getAtkRange(), y() - getAtkRange(),
        getAtkRange() * 2, getAtkRange() * 2
    ));
    for (QGraphicsItem* item : items) {
        Player* p = dynamic_cast<Player*>(item);
        if (p) {
            QPointF center = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
            QPointF pCenter = p->pos() + QPointF(p->boundingRect().width()/2, p->boundingRect().height()/2);
            if (QLineF(center, pCenter).length() <= getAtkRange()) {
                p->setHp(p->getHp() - 0.1*atk);
                p->setBurning(true); 
            }
        }
    }
}
void FlamePhantom::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    PhantomBase::paint(painter, option, widget);
    if (isLocking && player) {
        QPointF myCenter = QPointF(boundingRect().width()/2, boundingRect().height()/2);
        QPointF playerCenter = mapFromScene(player->pos() + QPointF(player->boundingRect().width()/2, player->boundingRect().height()/2));
        QPen pen(Qt::red, 3);
        painter->setPen(pen);
        painter->drawLine(myCenter, playerCenter);
        QColor lightRed(255, 100, 100, 30); 
    rangeIndicatorItem->setBrush(lightRed);
    }
}

void FlamePhantom::meleeAtk() {
    if (!player) return;
    player->setHp(player->getHp() - atk);
    player->setBurning(true);
    QPointF myCenter = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
    QPointF playerCenter = player->pos() + QPointF(player->boundingRect().width()/2, player->boundingRect().height()/2);
    QPointF knockbackDir = playerCenter - myCenter;
    qreal len = std::hypot(knockbackDir.x(), knockbackDir.y());
    if (len > 1e-3) {
        knockbackDir /= len;
        qreal knockbackDist = 300.0;
        player->setPos(player->pos() + knockbackDir * knockbackDist);
    }
}
void FlamePhantom::stopTimers() {

}
void FlamePhantom::startTimers() {

}
