#ifndef PLAYERWAVE_H
#define PLAYERWAVE_H

#include "Wave.h"

class PlayerWave : public Wave {
    Q_OBJECT

public:
    explicit PlayerWave(const QPointF &direction, int damage, QObject *parent = nullptr);
    ~PlayerWave() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;//没有血条和蓝条，但是要画形状
    //属性相关
    void setMaxPierceCnt(int count) { maxPierceCnt = count; }
    void setAoeRadius(float r) { aoeRadius = r; }
    void setCurPierceCnt(int cnt) { curPierceCnt = cnt; }
    int getMaxPierceCnt() const { return maxPierceCnt; }
    int getcurPierceCnt() const { return maxPierceCnt; }
    float getAoeRadius() const { return aoeRadius; }
protected:
    void handleCollision(QGraphicsItem *item) override;//重写碰撞处理
    void applyAoeDamage();
    int maxPierceCnt;//最大穿透数
    int curPierceCnt;//已穿透数
    float aoeRadius;//波及半径
    QSet<ActiveObject*> hitObjects;//防止重复
};

#endif // PLAYERWAVE_H