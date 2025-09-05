#include "ActiveObject.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

ActiveObject::ActiveObject(QObject *parent) : QObject(parent), QGraphicsPixmapItem() {
    rangeIndicatorItem = nullptr;
}
 
void ActiveObject::setStaticPixmap(const QPixmap& pix) {
    setPixmap(pix);
}
void ActiveObject::setHp(int v) {
    hp = qBound(0, v, maxHp);
    update();
}
void ActiveObject::updatePosition() {}
ActiveObject::~ActiveObject() {
    if (rangeIndicatorItem) {
        rangeIndicatorItem->setVisible(false);
    }
}
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

void ActiveObject::handleCollision(QGraphicsItem *item) {
    Q_UNUSED(item);
}
void ActiveObject::createRangeIndicator(const QColor& color, int penWidth) {
    if (!rangeIndicatorItem) {
        rangeIndicatorItem = new QGraphicsEllipseItem(this);
        rangeIndicatorItem->setBrush(color);
        rangeIndicatorItem->setPen(QPen(color.darker(), penWidth));
        rangeIndicatorItem->setVisible(false);
        updateRangeIndicator();
    }
}
void ActiveObject::setRangeIndicatorVisible(bool visible) {
    if (rangeIndicatorItem) {
        rangeIndicatorItem->setVisible(visible);
    }
}
void ActiveObject::updateRangeIndicator() {
    if (rangeIndicatorItem) {
        QRectF rect(-atkRange, -atkRange, atkRange * 2, atkRange * 2);
        rangeIndicatorItem->setRect(rect);
        rangeIndicatorItem->setPos(boundingRect().center());
    }
}