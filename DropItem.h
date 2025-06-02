#ifndef DROPITEM_H
#define DROPITEM_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include "Player.h"

enum DropType {
    flamePhantomDrop,
    lurkPhantomDrop
};

class DropItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    DropItem(DropType type, QObject* parent = nullptr);
    void strengthenPlayer(Player* player);//拾取时调用
protected:
    void advance(int phase) override;//掉落物拾取
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;//加旋转
private:
    DropType dropType;
    qreal angle = 0;
};

#endif // DROPITEM_H
