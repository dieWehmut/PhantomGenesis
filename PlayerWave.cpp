#include "PlayerWave.h"
#include <QTransform>
#include <QtMath>
#include "GameManager.h"
#include "Player.h"
#include "PhantomBase.h"
PlayerWave::PlayerWave(const QPointF &direction, int damage, QObject *parent)
    : Wave(parent){
    // 设置属性
    QPointF normDir = direction;
    qreal len = qSqrt(direction.x()*direction.x() + direction.y()*direction.y());
    if (len > 0) {
        normDir.setX(direction.x() / len);
        normDir.setY(direction.y() / len);
    }
    setDirection(normDir);
    setSpeed(2.0f);
    setAtk(damage);
    setMaxDistance(300.0f);
    setPixmap(QPixmap("Resource/playerWave.png").scaled(32, 32));
setTransformOriginPoint(16, 16);
}
QRectF PlayerWave::boundingRect() const {
    return QRectF(0, 0, 48, 48);
}

void PlayerWave::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    Player* player = nullptr;
    for (QGraphicsItem* item : scene()->items()) {
        player = dynamic_cast<Player*>(item);
        if (player) break;
    }
    if (player) {
        QPointF playerCenter = player->pos() + QPointF(player->pixmap().width() / 2, player->pixmap().height() / 2);
        QPointF waveCenter = pos() + QPointF(boundingRect().width() / 2, boundingRect().height() / 2);
        qreal dist = QLineF(playerCenter, waveCenter).length();
        if (dist >3*player->getSightRange()) {
            return;
        }
    }
    painter->save();
    qreal angle = qAtan2(getDirection().y(), getDirection().x()) * 180 / M_PI + 180;
    painter->translate(24, 24);
    painter->rotate(angle);
    painter->translate(-24, -24);
    QPainterPath bigCircle;
    bigCircle.addEllipse(QRectF(0, 0, 48, 48));
    QPainterPath smallCircle;
    smallCircle.addEllipse(QRectF(8, 0, 48, 48));
    QPainterPath crescent = bigCircle.subtracted(smallCircle);
    painter->setClipPath(crescent);
    painter->drawPixmap(0, 0, pixmap().scaled(48, 48));
    painter->restore();
}



PlayerWave::~PlayerWave() {

}
void PlayerWave::handleCollision(QGraphicsItem *item) {
    PhantomBase *enemy = dynamic_cast<PhantomBase*>(item);
    if (enemy) {
        enemy->setHp(enemy->getHp() - getAtk());
        deleteLater();
    }
}
