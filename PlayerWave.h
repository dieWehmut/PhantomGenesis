#ifndef PLAYERWAVE_H
#define PLAYERWAVE_H

#include "Wave.h"

class PlayerWave : public Wave {
    Q_OBJECT

public:
    explicit PlayerWave(const QPointF &direction, int damage, QObject *parent = nullptr);
    ~PlayerWave() override;
    void updatePosition() override;//位置更新
    void onMoveTimerTimeout() override;
    //属性相关
    void setMaxPierceCnt(int count) { maxPierceCnt = count; }
    void setAoeRadius(float r) { aoeRadius = r; }
    void setCurPierceCnt(int cnt) { curPierceCnt = cnt; }
    int getMaxPierceCnt() const { return maxPierceCnt; }
    int getCurPierceCnt() const { return curPierceCnt; }
    float getAoeRadius() const { return aoeRadius; }
protected:
    void handleCollision(QGraphicsItem *item) override;
    void applyAoeDamage(ActiveObject* directHitObj);
    void onAoeTimerTimeout() override;
    int maxPierceCnt;//最大穿透数
    int curPierceCnt;//已穿透数
    float aoeRadius;//波及半径
    QSet<ActiveObject*> hitObjects;//防止重复
};

#endif // PLAYERWAVE_H