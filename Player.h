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
private:
    QSet<int> pressedKeys;//各种按键集
};


#endif // PLAYER_H
