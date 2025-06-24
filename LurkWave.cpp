#include "LurkWave.h"
#include <QPixmap>
#include "Player.h"
LurkWave::LurkWave(const QPointF& dir, int atk, QObject* parent)
    : Wave(dir, atk, 400, 2.0f, QPixmap("Resource/lurkWave.png").scaled(64,64), parent)
{
    setTargetType(PlayerTarget);
}

void LurkWave::playCollisionEffect(QGraphicsItem* item)
{

}

void LurkWave::playCollisionSound(QGraphicsItem* item)
{
    if (dynamic_cast<Player*>(item)) {
        static QSoundEffect effect;
        effect.setSource(QUrl::fromLocalFile("Resource/lurkWaveHit.wav"));
        effect.setVolume(1.7f);
        effect.play();
    }
}
