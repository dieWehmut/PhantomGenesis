#include "Player.h"
#include <QKeyEvent>
#include <QDebug>
#include <QPainter>

Player::Player(QObject *parent) : ActiveObject(parent) {
    //属性初始值
    setHp(100);
    setSpeed(1.0f);
    setSightRange(200);
    setAtkRange(50);
    setAtk(10);
    //玩家人物图
    setPixmap(QPixmap("Resource/player.png").scaled(64, 64));
    setTransformOriginPoint(pixmap().width()/2, pixmap().height()/2);
    setFlag(QGraphicsItem::ItemIsFocusable); 
        setBoundingRegionGranularity(1.0);
    setCacheMode(DeviceCoordinateCache);
    setFocus(); 
}

void Player::updatePosition() {//位置跟新
    velocity = QPointF(0, 0);
    if(pressedKeys.contains(Qt::Key_W)) velocity.setY(-getSpeed());
    if(pressedKeys.contains(Qt::Key_S)) velocity.setY(getSpeed());
    if(pressedKeys.contains(Qt::Key_A)) velocity.setX(-getSpeed());
    if(pressedKeys.contains(Qt::Key_D)) velocity.setX(getSpeed());
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
    int barWidth = 60;
    int barHeight = 5;
    qreal x = (baseRect.width() - barWidth) / 2;
    qreal y = -barHeight - 5;
    QRectF healthBarRect(x, y, barWidth, barHeight);
    return baseRect.united(healthBarRect);
}
