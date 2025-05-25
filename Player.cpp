#include "Player.h"
#include <QKeyEvent>
#include <QDebug>
#include <QPainter>
Player::Player(QObject *parent) : QObject(parent) {
    setPixmap(QPixmap("Resource/player.png").scaled(64, 64));
    setTransformOriginPoint(pixmap().width()/2, pixmap().height()/2);
    setFlag(QGraphicsItem::ItemIsFocusable); 
        setBoundingRegionGranularity(1.0);
    setCacheMode(DeviceCoordinateCache);
    setFocus(); 
}

void Player::updatePosition() {//位置跟新
    velocity = QPointF(0, 0);
    if(pressedKeys.contains(Qt::Key_W)) velocity.setY(-speed);
    if(pressedKeys.contains(Qt::Key_S)) velocity.setY(speed);
    if(pressedKeys.contains(Qt::Key_A)) velocity.setX(-speed);
    if(pressedKeys.contains(Qt::Key_D)) velocity.setX(speed);
    if(velocity.x() != 0 && velocity.y() != 0) {
        velocity *= 0.7071f; 
    }
    QPointF newPos = pos() + velocity;
    setPos(newPos);
}

void Player::keyPressEvent(QKeyEvent *event) {
    pressedKeys.insert(event->key());
}

void Player::keyReleaseEvent(QKeyEvent *event) {
    pressedKeys.remove(event->key());
}
void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QGraphicsPixmapItem::paint(painter, option, widget);
    int barWidth = 40;
    int barHeight = 6;
    float hpPercent = qMax(0, getHp()) / 100.0f;
    QRectF itemRect = boundingRect();
    qreal x = (itemRect.width() - barWidth) / 2;
    qreal y = -barHeight - 5;
    painter->setBrush(Qt::gray);
    painter->drawRect(x, y, barWidth, barHeight);
    painter->setBrush(Qt::red);
    painter->drawRect(x, y, static_cast<int>(barWidth * hpPercent), barHeight);
}
QRectF Player::boundingRect() const {
    QRectF baseRect = QGraphicsPixmapItem::boundingRect();
    int barWidth = 40;
    int barHeight = 6;
    qreal x = (baseRect.width() - barWidth) / 2;
    qreal y = -barHeight - 5;
    QRectF healthBarRect(x, y, barWidth, barHeight);
    return baseRect.united(healthBarRect);
}
