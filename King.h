#ifndef KING_H
#define KING_H

#include "PhantomBase.h"
#include <QTimer>
class Player;
class King : public PhantomBase
{
    Q_OBJECT
public:
    explicit King(QObject* parent = nullptr);
    ~King();
    void updateStatus() override;
    void shootWave(const QPointF& direction) override;
    void stopTimers() override;
    void startTimers() override;
private:
    void shootAllDirections();
    QTimer* rangeTimer = nullptr;
    QTimer* waveTimer = nullptr;
    Player* player = nullptr;
};

#endif // KING_H