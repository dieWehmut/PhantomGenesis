#ifndef PLAYER_H
#define PLAYER_H
#include <QGraphicsPixmapItem>
#include <QObject>

class Player : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    //各种属性函数
    int getHp() const{return hp;};
    int getAtk() const{return atk;};
    int getAtkRange() const{return atkRange;};
    int getSightRange() const{return sightRange;};
    float getSpeed() const{return speed;};
    void setHp(int v){hp = v;};
    void setAtk(int v){atk = v;};
    void setAtkRange(int v){atkRange = v;};
    void setSightRange(int v){sightRange = v;};
    void setSpeed(float v){speed = v;};

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;//显示血条
    void updatePosition();//位置更新
    void keyPressEvent(QKeyEvent *event) override;//玩家骚操作
    void keyReleaseEvent(QKeyEvent *event) override;//释放操作
private:
    QSet<int> pressedKeys;//各种按键
    QPointF velocity;//速度
    int hp = 100;//血量
    int sightRange = 200;//视野
    int atkRange = 50;//攻击范围
    int atk = 10;//攻击
    float speed = 1.0f;//速度
};


#endif // PLAYER_H
