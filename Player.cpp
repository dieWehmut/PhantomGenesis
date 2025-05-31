#include "Player.h"
#include <QKeyEvent>
#include <QPainter>
#include <QGraphicsScene>
#include"PlayerWave.h"
Player::Player(QObject *parent) : ActiveObject(parent) {
    //属性初始值设置
    setMaxHp(1000);
    setHp(getMaxHp());
    setSpeed(15.0f);
    setOriginSpeed(getSpeed());
    setSightRange(200);
    setAtkRange(50);
    setAtk(30);
    setAtkCD(100);
    atkTimer.restart();
    cdUpdateTimer = new QTimer(this);
    cdUpdateTimer->setInterval(1000/60); 
        connect(cdUpdateTimer, &QTimer::timeout, this, [this]() {
        this->update();
    });
    cdUpdateTimer->start(); 
    setLastMoveDirection(QPointF(1, 0));
    setStaticPixmap(QPixmap("Resource/player.png").scaled(64, 64));
    setTransformOriginPoint(pixmap().width()/2, pixmap().height()/2);
    setFlag(QGraphicsItem::ItemIsFocusable); 
    setBoundingRegionGranularity(1.0);
    setCacheMode(DeviceCoordinateCache);
    setFocus(); 
}
void Player::setSlowed(bool s) {
    if (slowed == s) return;
    slowed = s;
    if (slowed) {
        setSpeed(originalSpeed * 0.4f);
    } else {
        setSpeed(originalSpeed);
    }
}
void Player::updatePosition() {//位置跟新
    velocity = QPointF(0, 0);
    if(pressedKeys.contains(Qt::Key_W)) velocity.setY(-getSpeed());
    if(pressedKeys.contains(Qt::Key_S)) velocity.setY(getSpeed());
    if(pressedKeys.contains(Qt::Key_A)) velocity.setX(-getSpeed());
    if(pressedKeys.contains(Qt::Key_D)) velocity.setX(getSpeed());
    if (velocity.x() != 0 && velocity.y() != 0) {
        velocity *= 0.7071f;
    }
    if (velocity.x() != 0 || velocity.y() != 0) {
        QPointF norm = velocity;
        qreal length = qSqrt(norm.x()*norm.x() + norm.y()*norm.y());
        if (length > 0) {
            norm.setX(norm.x() / length);
            norm.setY(norm.y() / length);
        }
        setLastMoveDirection(norm);
    }
    QPointF newPos = pos() + velocity;
    setPos(newPos);
}

void Player::clearPressedKeys() {
    pressedKeys.clear();
}
void Player::keyPressEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()) {
        pressedKeys.insert(event->key());
        if (event->key() == Qt::Key_Space) {
            playerStartAtk();
        }
    }
    QGraphicsPixmapItem::keyPressEvent(event);
}

void Player::keyReleaseEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()) {
        pressedKeys.remove(event->key());
    }
    QGraphicsPixmapItem::keyReleaseEvent(event);
}
void Player::playerStartAtk() {
    if (atkTimer.elapsed() >= atkCD) {
        QPointF atkDir(0, 0);
        bool pressedW = pressedKeys.contains(Qt::Key_W);
        bool pressedS = pressedKeys.contains(Qt::Key_S);
        bool pressedA = pressedKeys.contains(Qt::Key_A);
        bool pressedD = pressedKeys.contains(Qt::Key_D);
        //设置攻击方向
        if (pressedW) atkDir.setY(atkDir.y() - 1);
        if (pressedS) atkDir.setY(atkDir.y() + 1);
        if (pressedA) atkDir.setX(atkDir.x() - 1);
        if (pressedD) atkDir.setX(atkDir.x() + 1);
        if (atkDir.x() != 0 || atkDir.y() != 0) {
            qreal len = qSqrt(atkDir.x() * atkDir.x() + atkDir.y() * atkDir.y());
            atkDir.setX(atkDir.x() / len);
            atkDir.setY(atkDir.y() / len);
        } else {
            atkDir = lastMoveDirection;
        }
        if (scene()) {
            PlayerWave* wave = new PlayerWave(atkDir, getAtk());
            QPointF spawnPos = pos() + QPointF(pixmap().width()/2, pixmap().height()/2);
            wave->setPos(spawnPos);
            wave->setCurPierceCnt(0);
            wave->setMaxPierceCnt(5);
            wave->setAoeRadius(2000.0f);
            scene()->addItem(wave);
        }
        atkTimer.restart();
        cdUpdateTimer->start();
    }
}
void Player::focusInEvent(QFocusEvent *event) {
    QGraphicsPixmapItem::focusInEvent(event);
}
void Player::focusOutEvent(QFocusEvent *event) {
    QGraphicsPixmapItem::focusOutEvent(event);
    clearPressedKeys();
}
void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    ActiveObject::paint(painter, option, widget);
    // 蓝条，玩家多出来的
    int barWidth = 60;
    int barHeight = 6;
    int cdBarHeight = 5;
    QRectF itemRect = boundingRect();
    qreal x = (itemRect.width() - barWidth) / 2;
    qreal y = -barHeight - 5;
    qreal cdBarY = y + barHeight + 2;
    float cdPercent = 1.0f;

    if (atkCD > 0) {
        int elapsed = atkTimer.elapsed();
        if (elapsed <= 0 || elapsed >= atkCD) {
            cdPercent = 1.0f; // 冷却结束或还未攻击过，画满
        } else {
            cdPercent = qBound(0.0f, (float)elapsed / atkCD, 1.0f);//按比例
        }
    }
    painter->setBrush(Qt::gray);
    painter->drawRect(x, cdBarY, barWidth, cdBarHeight);
    painter->setBrush(QColor(0, 128, 255));
    painter->drawRect(x, cdBarY, static_cast<int>(barWidth * cdPercent), cdBarHeight);
}
