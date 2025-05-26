#ifndef FLAMEPHANTOM_H
#define FLAMEPHANTOM_H
#include "PhantomBase.h"
#include "Player.h"

class FlamePhantom : public PhantomBase {//继承幽灵基类
    Q_OBJECT
public:
    FlamePhantom(Player* targetPlayer);
    void updateStatus();//状态更新
private:
    void rangeDebuff(); // 范围干扰
    void meleeAtk(); // 近战攻击
    Player* player;
    QPixmap rangeIndicator; // 攻击范围指示器
    bool isMeleeActive = false;//是否开始近战攻击
    QGraphicsPixmapItem* rangeIndicatorItem = nullptr;
};
#endif // FLAMEPHANTOM_H
