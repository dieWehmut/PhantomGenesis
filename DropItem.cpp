#include "DropItem.h"
#include <QGraphicsScene>
#include <QPainter>
DropItem::DropItem(DropType type, QObject* parent)
    : QObject(parent), QGraphicsPixmapItem(), dropType(type) {
    switch (dropType) {
        case flamePhantomDrop:
            setPixmap(QPixmap(":/Resource/flamePhantomDrop.png").scaled(64, 64));
            break;
        case lurkPhantomDrop:
            setPixmap(QPixmap(":/Resource/lurkPhantomDrop.png").scaled(64, 64));
            break;
    }
    setZValue(10);
}

void DropItem::strengthenPlayer(Player* player) {
    player->setHp(player->getHp()+20000);
    switch (dropType){
        case DropType::flamePhantomDrop:
            if(player->getSightRange()<400){//2次
                player->setSightRange(player->getSightRange()+100);
            } else if(player->getPlayerWaveAoeRadius()<200.0f) {//5次
                player->setPlayerWaveAoeRadius(player->getPlayerWaveAoeRadius() + 20.0f);
            }else if(player->getAtkRange()<150){//5次
                player->setAtkRange(player->getAtkRange()+10);
            } else if(player->getPlayerWaveSpeed()<20.0f) {//5次
                player->setPlayerWaveSpeed(player->getPlayerWaveSpeed() + 2.0f);
            } else if(player->getMaxHp()<1000000) {
                player->setMaxHp(player->getMaxHp() + 15000);
            }
        break;
        case DropType::lurkPhantomDrop:
            if(player->getSpeed()<20.0f) {//4次
                player->setSpeed(player->getSpeed() + 2.0f);
            } else if(player->getAtkCD()>=200) {//3次
                player->setAtkCD(player->getAtkCD() - 200);
            }else if(player->getMaxHp()<1000000) {
                player->setMaxHp(player->getMaxHp() + 15000);
            }
        break;
    }
}


void DropItem::advance(int phase) {
    if (phase == 0) return;
    angle = std::fmod(angle + 30, 360.0);
    update();
    for (QGraphicsItem* item : collidingItems()) {
        Player* player = dynamic_cast<Player*>(item);
        if (player) {
            strengthenPlayer(player);
            if (scene()) scene()->removeItem(this);
            deleteLater();
            break;
        }
    }
}

void DropItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->save();
    QSize sz = pixmap().size();
    QPointF center(sz.width() / 2.0, sz.height() / 2.0);
    painter->translate(center);
    painter->rotate(angle);
    painter->translate(-center);
    painter->drawPixmap(0, 0, pixmap());
    painter->restore();
}
