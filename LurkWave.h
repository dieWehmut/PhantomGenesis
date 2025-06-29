#ifndef LURKWAVE_H
#define LURKWAVE_H

#include "Wave.h"
#include <QSoundEffect>

class LurkWave : public Wave {
    Q_OBJECT
public:
    LurkWave(const QPointF& dir, int atk, QObject* parent = nullptr);
    void startTimers() override;
    void stopTimers() override;
protected:
    void playCollisionEffect(QGraphicsItem* item) override;
    void playCollisionSound(QGraphicsItem* item) override;
    static QSoundEffect hitSound;
};

#endif // LURKWAVE_H
