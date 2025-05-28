#ifndef PLAYERWAVE_H
#define PLAYERWAVE_H

#include "Wave.h"

class PlayerWave : public Wave {
    Q_OBJECT

public:
    explicit PlayerWave(const QPointF &direction, int damage, QObject *parent = nullptr);
    ~PlayerWave() override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    void handleCollision(QGraphicsItem *item) override;//重写碰撞处理
};

#endif // PLAYERWAVE_H