#ifndef WAVE_H
#define WAVE_H

#include "ActiveObject.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QPainter>
#include <QPainterPath>
#include <QSoundEffect>
#include <QGraphicsEllipseItem>
class Wave : public ActiveObject {
    Q_OBJECT
public:
    explicit Wave(const QPointF& dir, int atk, float maxDist, float speed, const QPixmap& pix, QObject *parent = nullptr);
    virtual ~Wave();//消失
    enum TargetType { None, PlayerTarget, PhantomTarget };
    void setTargetType(TargetType t) { targetType = t; }
    //各种获取和设置函数
    void setDirection(const QPointF &dir);
    void setIgnoreItem(QGraphicsItem* item) { ignoreItem = item; }
    QPointF getDirection() const;
    void setMaxDistance(float distance);
    float getMaxDistance() const;
    void setCurDistance(float distance);
    float getCurDistance() const;
    void updatePosition() override;
    bool checkCollision(const QPointF& newPos) override;//碰撞检测
    void setAngle(qreal a) { angle = a; }
    qreal getAngle() const { return angle; }
    QRectF boundingRect() const override;
    virtual void startTimers() {}
    virtual void stopTimers() {}
protected:
    TargetType targetType = None;
    void handleCollision(QGraphicsItem *item) override;
    QPointF direction;//移动方向
    float maxDistance;//最大飞行距离
    float curDistance;//已飞距离
    QTimer *moveTimer;//移动计时器
    QTimer *aoeTimer = nullptr;//范围伤害计时器
    virtual void onAoeTimerTimeout();//波及定时器槽函数
    void startAoeTimer(int intervalMs);//启动波及定时器
    void stopAoeTimer();//停止波及定时器
    QGraphicsItem* ignoreItem = nullptr;//忽略碰撞的对象
    qreal angle = 0;
    QTimer* rotateTimer = nullptr;//旋转定时器
    int rotateStep = 30;//每次旋转的角度
    void startRotate(int intervalMs = 16);//启动旋转
    void stopRotate();//停止旋转
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    virtual void playCollisionEffect(QGraphicsItem* item);//碰撞特效
    virtual void playCollisionSound(QGraphicsItem* item);//碰撞音效
private slots:
    virtual void onMoveTimerTimeout();
    void onRotateTimerTimeout();//旋转
};
#endif // WAVE_H
