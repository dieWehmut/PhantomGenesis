#include "Player.h"
#include <QKeyEvent>
#include <QPainter>
#include <QGraphicsScene>
#include"PlayerWave.h"
Player::Player(QObject *parent) : ActiveObject(parent) {
    //属性初始值设置
    setHp(100);
    setSpeed(1.5f);
    setSightRange(100);
    setAtkRange(50);
    setAtk(10);
    setAtkCD(500);
    setCurCD(0);
    setLastMoveDirection(QPointF(1, 0));
    playerMovie = new QMovie("Resource/player.gif", QByteArray(), this);//玩家人物图
    if (playerMovie->isValid()) {
        connect(playerMovie, &QMovie::frameChanged, this, &Player::updatePixmap);
        playerMovie->start(); 
    } else {
        setPixmap(QPixmap("Resource/player.png").scaled(256, 256)); // 失败处理
    }
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
    if (velocity.x() != 0 || velocity.y() != 0) {
        setLastMoveDirection(velocity);
        qreal length = qSqrt(lastMoveDirection.x()*lastMoveDirection.x()+lastMoveDirection.y() * lastMoveDirection.y());
        if (length > 0){
            lastMoveDirection.setX(lastMoveDirection.x() / length);
            lastMoveDirection.setY(lastMoveDirection.y() / length);
        }
    }
    if(velocity.x() != 0 && velocity.y() != 0) {
        velocity *= 0.7071f; 
    }
    QPointF newPos = pos() + velocity;
    setPos(newPos);
    if (curCD > 0) {
        curCD -= 1000/60;
    }
}

void Player::keyPressEvent(QKeyEvent *event) {
    pressedKeys.insert(event->key());
    if (event->key() == Qt::Key_Space) {//检测攻击
        playerStartAtk();
    }
}

void Player::keyReleaseEvent(QKeyEvent *event) {
    pressedKeys.remove(event->key());
}
void Player::clearPressedKeys() {
    pressedKeys.clear();
}
void Player::playerStartAtk() {//发动攻击
    if (curCD <= 0) {
        PlayerWave *wave = new PlayerWave(lastMoveDirection, getAtk(), nullptr);
        QPointF playerCenter = pos() + QPointF(pixmap().width() / 2, pixmap().height() / 2);
        wave->setPos(playerCenter);
        if (scene()) {
            scene()->addItem(wave);
        }
        setCurCD(atkCD);
    }
}
void Player::focusInEvent(QFocusEvent *event) {
    QGraphicsPixmapItem::focusInEvent(event);
}
void Player::focusOutEvent(QFocusEvent *event) {
    QGraphicsPixmapItem::focusOutEvent(event);
    clearPressedKeys();
}
void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {//血条
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (playerMovie && playerMovie->isValid() && playerMovie->currentPixmap().isNull()) {
        return;
    }
    QGraphicsPixmapItem::paint(painter, option, widget);
    int barWidth = 50;
    int barHeight = 6;
    float hpPercent = qMax(0, getHp()) / 100.0f;
    QRectF itemRect = boundingRect();
    qreal x = (itemRect.width() - barWidth) / 2;
    qreal y = -barHeight - 5;
    painter->setBrush(Qt::gray);
    painter->drawRect(x, y, barWidth, barHeight);
    painter->setBrush(Qt::red);//血条
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
void Player::updatePixmap(int frameNumber) {//更新图片
    Q_UNUSED(frameNumber);
    if (playerMovie && playerMovie->isValid()) {
        setPixmap(playerMovie->currentPixmap().scaled(64, 64));
    }
}
