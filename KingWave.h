#ifndef KINGWAVE_H
#define KINGWAVE_H

#include "Wave.h"

class KingWave : public Wave {
    Q_OBJECT
public:
    KingWave(const QPointF& dir, int atk, QObject* parent = nullptr);
    void startTimers() override;
    void stopTimers() override;
protected:
    void handleCollision(QGraphicsItem* item) override;
    void playCollisionEffect(QGraphicsItem* item) override;
    void playCollisionSound(QGraphicsItem* item) override;
};

#endif // KINGWAVE_H