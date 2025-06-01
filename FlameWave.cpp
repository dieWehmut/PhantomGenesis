#include "FlameWave.h"
#include <QPixmap>

FlameWave::FlameWave(const QPointF& dir, int atk, QObject* parent)
    : Wave(dir, atk, 800, 5.0f, QPixmap("Resource/flameWave.png").scaled(64,64), parent)
{
    setTargetType(PlayerTarget);
}