#include "FlamePhantom.h"
#include <QGraphicsScene>

FlamePhantom::FlamePhantom(Player* targetPlayer) 
    : player(targetPlayer) {
    rangeIndicator = QPixmap("Resource/flamePhantomRangeIndicator.png");
    setPixmap(QPixmap("Resource/flamePhantom.png"));
}
void FlamePhantom::updateStatus() {//各种状态更新
    qreal distance = QLineF(pos(), player->pos()).length();//计算距离
    if(distance < atkRange) {
        rangeDebuff();
        if(collidesWithItem(player)) {
            meleeAtk();
        }
    }
}

void FlamePhantom::rangeDebuff() {//范围伤害
    if(!scene()->items().contains(rangeIndicatorItem)) { 
        rangeIndicatorItem = new QGraphicsPixmapItem(rangeIndicator);
        rangeIndicatorItem->setPos(x() - getAtkRange(), y() - getAtkRange());
        scene()->addItem(rangeIndicatorItem);
    }
    player->setSpeed(player->getSpeed() * 0.8);
}
void FlamePhantom::meleeAtk() {//近战攻击
    player->setHp(player->getHp() - atk);
}
