#ifndef FLAMEWAVE_H
#define FLAMEWAVE_H

#include "Wave.h"

class FlameWave : public Wave {
    Q_OBJECT
public:
    explicit FlameWave(const QPointF& dir, int atk, QObject* parent = nullptr);
};

#endif // FLAMEWAVE_H