#ifndef FLAMEPHANTOM_H
#define FLAMEPHANTOM_H
#include "PhantomBase.h"
#include "Player.h"

class FlamePhantom : public PhantomBase {//继承幽灵基类
    Q_OBJECT
public:
    FlamePhantom(Player* targetPlayer);
    void updateStatus();//状态更新
    void setRangeAtkCD(int cd) { rangeAtkCD = cd; }
    int getRangeAtkCD() const { return rangeAtkCD; }
private:
    void rangeDebuff(); // 范围干扰
    void meleeAtk(); // 近战攻击
    Player* player;//目标玩家
    bool isMeleeActive = false;//是否开始近战攻击
    QElapsedTimer rangeAtkTimer;//攻击cd计时
    int rangeAtkCD;//atkcd
};
#endif // FLAMEPHANTOM_H
