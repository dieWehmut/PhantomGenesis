#include "DropItem.h"
#include <QGraphicsScene>
#include <QPainter>
DropItem::DropItem(DropType type, QObject* parent)
    : QObject(parent), QGraphicsPixmapItem(), dropType(type) {
    switch (dropType) {
        case flamePhantomDrop:
            setPixmap(QPixmap("Resource/flamePhantomDrop.png").scaled(64, 64));
            break;
        case lurkPhantomDrop:
            setPixmap(QPixmap("Resource/lurkPhantomDrop.png").scaled(64, 64));
            break;
    }
    setZValue(10);
}

void DropItem::strengthenPlayer(Player* player) {
    player->setHp(player->getHp()+100);
    switch (dropType){
        case DropType::flamePhantomDrop:
            if(player->getAtk()<=500) { 
                player->setAtk(player->getAtk()+100);
            } else if(player->getSightRange()<300){
                player->setSightRange(player->getSightRange()+10);
            } else if(player->getPlayerWaveAoeRadius()<300.0f) {
                player->setPlayerWaveAoeRadius(player->getPlayerWaveAoeRadius() + 30.0f);
            }else if(player->getAtkRange()<250){
                player->setAtkRange(player->getAtkRange()+100);
            } else if(player->getPlayerWaveSpeed()<20.0f) {
                player->setPlayerWaveSpeed(player->getPlayerWaveSpeed() + 2.0f);
            } else if(player->getMaxHp()<20000) {
                player->setMaxHp(player->getMaxHp() + 1000);
            }
        break;
        case DropType::lurkPhantomDrop: 
            if(player->getSpeed()<20.0f) {
                player->setSpeed(player->getSpeed() + 0.5f);
            } else if(player->getAtkCD()>=200) {
                player->setAtkCD(player->getAtkCD() - 100);
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
