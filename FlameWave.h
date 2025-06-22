#ifndef FLAMEWAVE_H
#define FLAMEWAVE_H

#include "Wave.h"

class FlameWave : public Wave {
    Q_OBJECT
public:
    explicit FlameWave(const QPointF& dir, int atk, QObject* parent = nullptr);
protected:
    void playCollisionEffect(QGraphicsItem* item) override;
    void playCollisionSound(QGraphicsItem* item) override;
};

#endif // FLAMEWAVE_H