#ifndef LURKWAVE_H
#define LURKWAVE_H

#include "Wave.h"

class LurkWave : public Wave {
public:
    LurkWave(const QPointF& dir, int atk, QObject* parent = nullptr);
};

#endif // LURKWAVE_H