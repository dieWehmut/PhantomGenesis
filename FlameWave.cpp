#include "FlameWave.h"
#include <QPixmap>
#include <QTimer>
#include "Player.h"

FlameWave::FlameWave(const QPointF& dir, int atk, QObject* parent)
    : Wave(parent)
{
    setDirection(dir);
    setAtk(atk);
    setMaxDistance(500);
    setSpeed(12.0f);
    setStaticPixmap(QPixmap("Resource/flameWave.png").scaled(64,64));
startRotate(16); 
}
QRectF FlameWave::boundingRect() const {
    return QRectF(0, 0, 64, 64);
}
void FlameWave::handleCollision(QGraphicsItem* item) {
    Player* player = dynamic_cast<Player*>(item);
    if (player && item != ignoreItem) {
        player->setHp(player->getHp() - getAtk());
        deleteLater();
        return;
    }
}

