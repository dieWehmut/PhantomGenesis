#ifndef WAVE_H
#define WAVE_H

#include "ActiveObject.h"
#include <QTimer>
#include <QPainter>
#include <QPainterPath>
class Wave : public ActiveObject {
    Q_OBJECT
public:
    explicit Wave(QObject *parent = nullptr);
    virtual ~Wave();//消失
    //各种获取和设置函数
    void setDirection(const QPointF &dir);
    QPointF getDirection() const;
    void setMaxDistance(float distance);
    float getMaxDistance() const;
    void setCurDistance(float distance);
    float getCurDistance() const;
    void updatePosition() override;
    bool checkCollision(const QPointF& newPos) override;
protected:
    QPointF direction;//移动方向
    float maxDistance;//最大飞行距离
    float curDistance;//已飞距离
    QTimer *moveTimer;//计时
    virtual void handleCollision(QGraphicsItem *item);//碰撞
private slots:
    void onMoveTimerTimeout(); // 计时器超时
};
#endif // WAVE_H