#include "FlameWave.h"
#include <QPixmap>
#include "Player.h"
FlameWave::FlameWave(const QPointF& dir, int atk, QObject* parent)
    : Wave(dir, atk, 800, 5.0f, QPixmap("Resource/flameWave.png").scaled(64,64), parent)
{
    setTargetType(PlayerTarget);
}
void FlameWave::playCollisionEffect(QGraphicsItem* item) {

}

void FlameWave::playCollisionSound(QGraphicsItem* item) {
    if (dynamic_cast<Player*>(item)) {
        static QSoundEffect effect;
        effect.setSource(QUrl::fromLocalFile("Resource/flameWaveHit.wav"));
        effect.setVolume(0.7f);
        effect.play();
    }
}