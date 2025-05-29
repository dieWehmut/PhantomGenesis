#include "FlamePhantom.h"
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QtMath>

FlamePhantom::FlamePhantom(Player* targetPlayer) 
    : player(targetPlayer) {
    setStaticPixmap(QPixmap("Resource/flamePhantom.png").scaled(64, 64));
    wanderTarget = pos();
    setMaxHp(200);
    setHp(getMaxHp());
    setSpeed(1.5f);
    setSightRange(400);
    setAtkRange(200);
    setAtk(1);
    setRangeAtkCD(5000);
    rangeAtkTimer.start();
}

void FlamePhantom::updateStatus() {
    if (!player) return;
    static bool lastInRange = false;
    QPointF myCenter = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
    QPointF playerCenter = player->pos() + QPointF(player->boundingRect().width()/2, player->boundingRect().height()/2);
    qreal distance = QLineF(myCenter, playerCenter).length();
    bool inRange = (distance < getAtkRange());
    if (!rangeIndicatorItem) {
        rangeIndicatorItem = new QGraphicsEllipseItem();
        rangeIndicatorItem->setZValue(0);
        QPen pen(Qt::red);
        pen.setWidth(2);
        rangeIndicatorItem->setPen(pen);
        rangeIndicatorItem->setBrush(Qt::NoBrush);
        if (scene())
            scene()->addItem(rangeIndicatorItem);
        rangeIndicatorItem->hide();
    }
    if (inRange != lastInRange) {
        if (inRange) {
            rangeIndicatorItem->setRect(
                myCenter.x() - getAtkRange(),
                myCenter.y() - getAtkRange(),
                getAtkRange() * 2,
                getAtkRange() * 2
            );
            rangeIndicatorItem->show();
        } else {
            rangeIndicatorItem->hide();
        }
        lastInRange = inRange;
    }
    bool moved = false;
    if (distance < getSightRange()) {
        QPointF dir = playerCenter - myCenter;
        qreal len = std::hypot(dir.x(), dir.y());
        if (len > 1e-2) {
            dir /= len;
            setPos(pos() + dir * getSpeed());
            moved = true;
        }
        if (inRange) {
            if(rangeAtkTimer.elapsed() >= getRangeAtkCD()) {
                rangeDebuff();
                rangeAtkTimer.restart();
            }
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
                p->setHp(p->getHp() - atk);
            }
        }
    }
}

void FlamePhantom::meleeAtk() {
    player->setHp(player->getHp() - atk);
}
