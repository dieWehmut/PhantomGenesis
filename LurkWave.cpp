#include "LurkWave.h"
#include <QPixmap>
#include "Player.h"

QSoundEffect LurkWave::hitSound;

LurkWave::LurkWave(const QPointF& dir, int atk, QObject* parent)
    : Wave(dir, atk, 500, 8.0f, QPixmap(":/Resource/lurkWave.png").scaled(64,64), parent)
{
    setTargetType(PlayerTarget);
}
void LurkWave::playCollisionEffect(QGraphicsItem* item) {
}

void LurkWave::playCollisionSound(QGraphicsItem* item) {
    if (dynamic_cast<Player*>(item)) {
        if (hitSound.source().isEmpty()) {
            hitSound.setSource(QUrl("qrc:/Resource/lurkWaveHit.wav"));
            hitSound.setVolume(1.0f);
        }
        hitSound.play();
    }
}
void LurkWave::startTimers() {
    if (moveTimer) moveTimer->start();
}
void LurkWave::stopTimers() {
    if (moveTimer) moveTimer->stop();
}
