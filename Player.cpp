#include "Player.h"
#include <QKeyEvent>
#include <QPainter>
#include <QGraphicsScene>
#include"PlayerWave.h"
Player::Player(QObject *parent) : ActiveObject(parent) {
    //属性初始值设置
    setMaxHp(100);
    setHp(getMaxHp());
    setSpeed(8.0f);
    setOriginSpeed(getSpeed());
    setSightRange(350);
    setAtkRange(150);
    setAtk(50);
    setAtkCD(500);
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
    createRangeIndicator(QColor(128, 0, 128, 60), 2);
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
    if (shieldActive && shieldTimer.elapsed() >= shieldDuration) {
        shieldActive = false;
        setRangeIndicatorVisible(false);
    }
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
        activateShield();
        QPointF atkDir(0, 0);
        bool pressedW = pressedKeys.contains(Qt::Key_W);
        bool pressedS = pressedKeys.contains(Qt::Key_S);
        bool pressedA = pressedKeys.contains(Qt::Key_A);
        bool pressedD = pressedKeys.contains(Qt::Key_D);
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
            QPointF spawnPos = pos() + QPointF(pixmap().width()/2, pixmap().height()/2);
            PlayerWave* centerWave = new PlayerWave(atkDir, getAtk());
            centerWave->setPos(spawnPos);
            scene()->addItem(centerWave);
            qreal leftAngle = -30.0 * M_PI / 180.0;
            QPointF leftDir(
                atkDir.x() * qCos(leftAngle) - atkDir.y() * qSin(leftAngle),
                atkDir.x() * qSin(leftAngle) + atkDir.y() * qCos(leftAngle)
            );
            PlayerWave* leftWave = new PlayerWave(leftDir, getAtk());
            leftWave->setPos(spawnPos);
            scene()->addItem(leftWave);
            qreal rightAngle = 30.0 * M_PI / 180.0;
            QPointF rightDir(
                atkDir.x() * qCos(rightAngle) - atkDir.y() * qSin(rightAngle),
                atkDir.x() * qSin(rightAngle) + atkDir.y() * qCos(rightAngle)
            );
            PlayerWave* rightWave = new PlayerWave(rightDir, getAtk());
            rightWave->setPos(spawnPos);
            scene()->addItem(rightWave);
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
void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {//蓝条
    ActiveObject::paint(painter, option, widget);
    int barWidth = 60;
    int barHeight = 6;
    int cdBarHeight = 5;
    QRectF imgRect(0, 0, 64, 64);
    qreal x = (imgRect.width() - barWidth) / 2;
    qreal y = -barHeight - 5;
    qreal cdBarY = y + barHeight + 2;
    float cdPercent = 1.0f;
    if (atkCD > 0) {
        int elapsed = atkTimer.elapsed();
        if (elapsed <= 0 || elapsed >= atkCD) {
            cdPercent = 1.0f;
        } else {
            cdPercent = qBound(0.0f, (float)elapsed / atkCD, 1.0f);
        }
    }
    painter->setBrush(Qt::gray);
    painter->drawRect(x, cdBarY, barWidth, cdBarHeight);
    painter->setBrush(QColor(0, 128, 255));
    painter->drawRect(x, cdBarY, static_cast<int>(barWidth * cdPercent), cdBarHeight);
}

void Player::activateShield() {
    shieldActive = true;
    shieldTimer.start();
    setRangeIndicatorVisible(true);
}
void Player::setHp(int v) {
    int oldHp = getHp();
    if (shieldActive && v < oldHp) {
        int damage = oldHp - v;
        int recovery = damage * 0.8;
        ActiveObject::setHp(v + recovery);
    } else {
        ActiveObject::setHp(v);
    }
}
