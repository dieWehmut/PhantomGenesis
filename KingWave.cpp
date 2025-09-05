#include "KingWave.h"
#include "Player.h"
#include "FlamePhantom.h"
#include "LurkPhantom.h"
#include "King.h"
#include <QPixmap>

KingWave::KingWave(const QPointF& dir, int atk, QObject* parent)
    : Wave(dir, atk, 1000, 15.0f, QPixmap(":/Resource/kingWave.png").scaled(128,128), parent)

{
    setTargetType(None); // 可伤害所有非King对象
}
void KingWave::playCollisionEffect(QGraphicsItem* item)
{


}
void KingWave::playCollisionSound(QGraphicsItem* item)
{
    if (dynamic_cast<Player*>(item)) {
        static QSoundEffect effect;
        effect.setSource(QUrl("qrc:/Resource/kingWaveHit.wav"));
        effect.setVolume(0.7f);
        effect.play();
    }
}
void KingWave::handleCollision(QGraphicsItem* item)
{
    if (item == ignoreItem) return;
    if (dynamic_cast<King*>(item)) return;
    if (auto* player = dynamic_cast<Player*>(item)) {
        playCollisionSound(item);
        player->setHp(player->getHp() - getAtk());
        QPointF waveCenter = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
        QPointF playerCenter = player->pos() + QPointF(player->boundingRect().width()/2, player->boundingRect().height()/2);
        QPointF knockbackDir = playerCenter - waveCenter;
        qreal len = std::hypot(knockbackDir.x(), knockbackDir.y());
        if (len > 1e-3) {
            knockbackDir /= len;
            qreal knockbackDist = 200.0;//击退距离
            player->setPos(player->pos() + knockbackDir * knockbackDist);
        }
        deleteLater();
    } else if (auto* flame = dynamic_cast<FlamePhantom*>(item)) {
        flame->setHp(flame->getHp() - getAtk());
        deleteLater();
    } else if (auto* lurk = dynamic_cast<LurkPhantom*>(item)) {
        lurk->setHp(lurk->getHp() - getAtk());
        deleteLater();
    }
}
void KingWave::startTimers() {
    if (moveTimer) moveTimer->start();
}
void KingWave::stopTimers() {
    if (moveTimer) moveTimer->stop();
}
