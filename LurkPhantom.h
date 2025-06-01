#ifndef LURKPHANTOM_H
#define LURKPHANTOM_H
#include "PhantomBase.h"
#include "Player.h"
#include <QElapsedTimer>
class LurkPhantom:public PhantomBase
{
public:
    LurkPhantom(Player* targetPlayer);
    ~LurkPhantom();
    void updateStatus() override;
    void shootWave(const QPointF& direction) override; 
    bool isShieldActive() const { return shieldActive; } 
    void setHp(int v) override; 
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
private:
    Player* player;
    QElapsedTimer lastBlinkTimer;
    int blinkCooldown;
    bool shieldActive = false;
    bool hasDealtDamage = false; 
    int shieldDuration = 3000;
    QElapsedTimer shieldTimer;
    void activateShield();
};
#endif // LURKPHANTOM_H