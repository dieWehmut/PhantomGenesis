#include "ActiveObject.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

ActiveObject::ActiveObject(QObject *parent) : QObject(parent), QGraphicsPixmapItem() {
}
void ActiveObject::setStaticPixmap(const QPixmap& pix) {
    setPixmap(pix);
}

void ActiveObject::updatePosition() {}
bool ActiveObject::checkCollision(const QPointF&) { return false; }

void ActiveObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->drawPixmap(0, 0, pixmap().scaled(64, 64));
    int barWidth = 60;
    int barHeight = 6;
    float hpPercent = maxHp > 0 ? qBound(0.0f, (float)hp / maxHp, 1.0f) : 1.0f;
    QRectF itemRect = boundingRect();
    qreal x = (itemRect.width() - barWidth) / 2;
    qreal y = -barHeight - 5;
    painter->setBrush(Qt::gray);
    painter->setPen(Qt::NoPen);
    painter->drawRect(x, y, barWidth, barHeight);
    painter->setBrush(Qt::red);
    painter->drawRect(x, y, static_cast<int>(barWidth * hpPercent), barHeight);
}

QRectF ActiveObject::boundingRect() const {
    int barWidth = 60;
    int barHeight = 6;
    qreal x = (64 - barWidth) / 2.0;
    qreal y = -barHeight - 5;
    QRectF imgRect(0, 0, 64, 64);
    QRectF hpBarRect(x, y, barWidth, barHeight + 2);
    return imgRect.united(hpBarRect);
}
