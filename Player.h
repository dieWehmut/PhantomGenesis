#ifndef PLAYER_H
#define PLAYER_H
#include"ActiveObject.h"
#include <QTimer>
#include <QElapsedTimer>
#include <QGraphicsEllipseItem>
class Player : public ActiveObject {
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;//多画一个蓝条
    void keyPressEvent(QKeyEvent *event) override;//按按键
    void keyReleaseEvent(QKeyEvent *event) override;//释放按键
    void updatePosition() override;//重写位置更新
    void clearPressedKeys();//清除按键集
    void playerStartAtk();//攻击
    //多的属性设置
    void setPlayerWaveAoeRadius(float r) { playerWaveAoeRadius = r; }
    float getPlayerWaveAoeRadius() const { return playerWaveAoeRadius; }
    void setPlayerWaveSpeed(float s) { playerWaveSpeed = s; }
    float getPlayerWaveSpeed() const { return playerWaveSpeed; }
    int getAtkCD() const { return atkCD; }
    void setAtkCD(int value) { atkCD = value; }
    void setLastMoveDirection(const QPointF &value) { lastMoveDirection = value; }
    QPointF getLastMoveDirection() const { return lastMoveDirection; } // 最后移动方向
    void setSlowed(bool slowed);
    bool isSlowed() const { return slowed; }
    float getOriginSpeed() const { return originalSpeed; }
    void setOriginSpeed(float value) { originalSpeed = value; }
    
    void activateShield(); // 激活防护盾
    void setHp(int v) override;//护盾相关，回血逻辑
    void setBurning(bool on);
    void burnTick();
    int getTeleportCount() const { return teleportCount; }
    void setTeleportCount(int count) { teleportCount = count; }
    void addTeleportCount() { teleportCount++; }
    void stopTimers() override;
    void startTimers() override;
protected:
    void focusInEvent(QFocusEvent *event) override; //焦点获得
    void focusOutEvent(QFocusEvent *event) override; //焦点失去
private:
    QSet<int> pressedKeys;//各种按键集
    QPointF lastMoveDirection;//记录最后移动方向
    int atkCD;//攻击冷却
    QElapsedTimer atkTimer; //cd计时
    QTimer* cdUpdateTimer = nullptr;//攻击cd更新定时器
    bool slowed=false;//是否减速
    float originalSpeed;//减速前的速度
    bool shieldActive = false;//护盾是否激活
    QElapsedTimer shieldTimer;//护盾计时器
    static const int shieldDuration = 3000;//护盾持续时间
    float playerWaveAoeRadius = 100.0f;//波的波及半径
    float playerWaveSpeed = 12.0f;//波的速度
    bool burning = false;//是否燃烧
    QTimer* burnTickTimer = nullptr;//燃烧定时器
    QElapsedTimer burnTimer;
    int burnDurationMs = 4000;
    int burnTickInterval = 400;//扣血时间
    int burnDamagePerTick = 10;//每次燃烧扣血
    //玩家不同状态
    QPixmap normalPixmap;
    QPixmap burningPixmap;
    QPixmap lowHpPixmap;
    int teleportCount = 0;//玩家传送次数
};

#endif // PLAYER_H
