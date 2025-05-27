#ifndef ACTIVEOBJECT_H
#define ACTIVEOBJECT_H
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QMovie>
class ActiveObject:public QObject, public QGraphicsPixmapItem {//所有能动的对象的基类
    Q_OBJECT
public:
    explicit ActiveObject(QObject *parent = nullptr);
    //各种属性获取和设置
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

    virtual void updatePosition();//位置更新
    virtual bool checkCollision(const QPointF& newPos);//碰撞检测
protected:
    int hp;//血
    int atk;//攻击
    int sightRange;//视野
    int atkRange;//攻击范围
    float speed;//速度
    QPointF velocity;//移动速度参数

};
#endif // ACTIVEOBJECT_H
