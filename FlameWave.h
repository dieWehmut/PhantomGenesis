#ifndef FLAMEWAVE_H
#define FLAMEWAVE_H

#include "Wave.h"
#include <QSoundEffect>

class FlameWave : public Wave {
    Q_OBJECT
public:
    explicit FlameWave(const QPointF& dir, int atk, QObject* parent = nullptr);
    void startTimers() override;
    void stopTimers() override;
protected:
    void playCollisionEffect(QGraphicsItem* item) override;
    void playCollisionSound(QGraphicsItem* item) override;
    static QSoundEffect hitSound;
};

#endif // FLAMEWAVE_H
