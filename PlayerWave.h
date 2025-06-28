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
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;//多画一个蓝条
protected:
    void handleCollision(QGraphicsItem *item) override;//增加波及处理
    void applyAoeDamage(ActiveObject* directHitObj);//波及伤害加成
    void onAoeTimerTimeout() override;//波及定时器槽函数
    int maxPierceCnt;//最大穿透数
    int curPierceCnt;//已穿透数
    float aoeRadius;//波及半径
    QSet<ActiveObject*> hitObjects;//防止重复
    int originalAtk = 0;//原始伤害
    bool boosted = false;//是否加成
    void updateBoostState();//更新加成状态
    void playCollisionEffect(QGraphicsItem* item) override;
    void playCollisionSound(QGraphicsItem* item) override;
    void startTimers() override;
    void stopTimers() override;
private:
    QSoundEffect playerWaveHitEffect;
    bool playerWaveEffectInitialized = false;
};

#endif // PLAYERWAVE_H
