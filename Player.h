#ifndef PLAYER_H
#define PLAYER_H
#include"ActiveObject.h"

class Player : public ActiveObject {
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;//显示血条
    void keyPressEvent(QKeyEvent *event) override;//玩家按键骚操作
    void keyReleaseEvent(QKeyEvent *event) override;//释放按键操作
    void updatePosition() override;//重写位置更新
    void clearPressedKeys();//清除按键集
    QPointF getLastMoveDirection() const { return lastMoveDirection; } // 最后移动方向
    void playerStartAtk();//攻击
    //多的属性设置
    int getAtkCD() const { return atkCD; }
    void setAtkCD(int value) { atkCD = value; }
    void setCurCD(int value) { curCD = value; }
    int getCurCD() const { return curCD; }
    void setLastMoveDirection(const QPointF &value) { lastMoveDirection = value; }
    QPointF getLastMoveDirection() { return lastMoveDirection; }
protected:
    void focusInEvent(QFocusEvent *event) override; //焦点获得
    void focusOutEvent(QFocusEvent *event) override; //焦点失去
private:
    QSet<int> pressedKeys;//各种按键集
    QMovie* playerMovie;
    QPointF lastMoveDirection; // 记录最后移动方向
    int atkCD; // 攻击冷却时间
    int curCD; // 当前冷却计时
private slots:
    void updatePixmap(int frameNumber);
};


#endif // PLAYER_H
