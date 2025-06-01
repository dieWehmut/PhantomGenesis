#include "LurkWave.h"
#include <QPixmap>

LurkWave::LurkWave(const QPointF& dir, int atk, QObject* parent)
    : Wave(dir, atk, 1000, 2.0f, QPixmap("Resource/lurkWave.png").scaled(64,64), parent)
{
    setTargetType(PlayerTarget);
}