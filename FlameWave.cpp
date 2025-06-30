#include "FlameWave.h"
#include <QPixmap>  
#include "Player.h"

QSoundEffect FlameWave::hitSound;

FlameWave::FlameWave(const QPointF& dir, int atk, QObject* parent)
    : Wave(dir, atk,600, 6.0f, QPixmap(":/Resource/flameWave.png").scaled(64, 64), parent) {
    setTargetType(PlayerTarget); 
}
void FlameWave::playCollisionEffect(QGraphicsItem* item) {
}

void FlameWave::playCollisionSound(QGraphicsItem* item) {
    if (dynamic_cast<Player*>(item)) {
        if (hitSound.source().isEmpty()) {
            hitSound.setSource(QUrl("qrc:/Resource/flameWaveHit.wav"));
            hitSound.setVolume(0.9f);
        }
        hitSound.play();
    }
}
void FlameWave::startTimers() {
    if (moveTimer) moveTimer->start();
}
void FlameWave::stopTimers() {
    if (moveTimer) moveTimer->stop();
}
