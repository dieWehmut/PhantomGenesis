#ifndef ACTIVEOBJECT_H
#define ACTIVEOBJECT_H
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QGraphicsEllipseItem>
#include <QFile>
class ActiveObject:public QObject, public QGraphicsPixmapItem {//所有能动的对象的基类
    Q_OBJECT
public:
    explicit ActiveObject(QObject *parent = nullptr);
    virtual ~ActiveObject();
    //各种属性获取和设置
    int getHp() const{return hp;} 
    int getMaxHp() const{return maxHp;}
    int getAtk() const{return atk;}
    int getAtkRange() const{return atkRange;}
    int getSightRange() const{return sightRange;}
    float getSpeed() const{return speed;}
    virtual void setHp(int v);//血条跟着变，子类要重写
    void setMaxHp(int v){maxHp = v;update();}
    void setAtk(int v){atk = v;}
    void setAtkRange(int v){atkRange = v; updateRangeIndicator();}
    void setSightRange(int v){sightRange = v;}
    void setSpeed(float v){speed = v;}

    virtual void updatePosition();//位置更新
    virtual bool checkCollision(const QPointF& newPos);//碰撞检测
    virtual void handleCollision(QGraphicsItem *item);//碰撞处理
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;//血条等绘制
    virtual QRectF boundingRect() const override;//图像边界
    void setStaticPixmap(const QPixmap& pix);//图像
    void createRangeIndicator(const QColor& color = QColor(0,128,255,60), int penWidth = 2);//创建范围指示器
    void setRangeIndicatorVisible(bool visible);//设置可见性
    void updateRangeIndicator();//更新范围指示器
    virtual void stopTimers() {}//停止所有定时
    virtual void startTimers() {}//启动所有定时
protected:
    int hp;//血
    int maxHp;//最大血
    int atk;//攻击
    int sightRange;//视野
    int atkRange;//攻击范围
    float speed;//速度
    QPointF velocity;//移动速度参数
    QGraphicsEllipseItem* rangeIndicatorItem = nullptr;//范围指示器
};

#endif // ACTIVEOBJECT_H
