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
    switch (dropType) {
        case DropType::flamePhantomDrop: player->setAtk(player->getAtk() +100); break;
        case DropType::lurkPhantomDrop: player->setAtkRange(player->getAtkRange() + 10); break;
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
