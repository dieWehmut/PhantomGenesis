#ifndef FLAMEPHANTOM_H
#define FLAMEPHANTOM_H
#include "PhantomBase.h"
#include "Player.h"
class FlamePhantom : public PhantomBase {//继承幽灵基类
    Q_OBJECT
public:
    FlamePhantom(Player* targetPlayer);
    ~FlamePhantom();
    void updateStatus()override;//状态更新
    void shootWave(const QPointF& direction) override;//发射波
    bool getIsLocking() const { return isLocking; }//获取锁定信息
    void setRangeAtkCD(int cd) { rangeAtkCD = cd; }
    int getRangeAtkCD() const { return rangeAtkCD; }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;//画射线
    void stopTimers() override;
    void startTimers() override;
private:
    void rangeDebuff(); // 范围干扰
    void meleeAtk(); // 近战攻击
    Player* player;//目标玩家
    bool isLocking = false; //是否锁定玩家
    bool isMeleeActive = false;//是否开始近战攻击
    QElapsedTimer rangeAtkTimer;//攻击cd计时
    int rangeAtkCD;//atkcd
    bool lastInRange = false;//上次是否在攻击范围内
};
#endif // FLAMEPHANTOM_H
