#ifndef LURKPHANTOM_H
#define LURKPHANTOM_H
#include "PhantomBase.h"
#include "Player.h"
#include <QElapsedTimer>
class LurkPhantom:public PhantomBase
{
    Q_OBJECT
public:
    LurkPhantom(Player* targetPlayer);
    ~LurkPhantom();
    void updateStatus() override;//状态更新
    void shootWave(const QPointF& direction) override;//发射射线 
    bool isShieldActive() const { return shieldActive; } //获取护盾状态
    void setHp(int v) override;//增加回血效果
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;//锁定玩家，画护盾
    void stopTimers() override;
    void startTimers() override;
private:
    Player* player;
    QElapsedTimer lastBlinkTimer;//闪现计时器
    int blinkCooldown;//闪现冷却时间
    bool shieldActive = false;//护盾状态
    bool hasDealtDamage = false;//在范围指示器内只伤害一次
    int shieldDuration = 3000;//护盾持续时间
    QElapsedTimer shieldTimer;//护盾计时器
    void activateShield();//激活护盾
};
#endif // LURKPHANTOM_H