#ifndef VISIONMASKITEM_H
#define VISIONMASKITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include "Player.h"
class GameManager;

class VisionMaskItem : public QGraphicsItem {
public:
    VisionMaskItem(Player* player, const QRectF& sceneRect, GameManager* gm = nullptr)
        : player0(player), sceneRect0(sceneRect), gm0(gm) {}
    QRectF boundingRect() const override {//返回场景大小
        return sceneRect0;
    }
    void setSceneRect(const QRectF& rect) {//设置场景大小
        prepareGeometryChange();
        sceneRect0=rect;
    }
    void setGameManager(GameManager* gm) { gm0 = gm; }//设置游戏管理器
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;//绘制遮罩
private:
    Player* player0;
    QRectF sceneRect0;
    GameManager* gm0 = nullptr;
};

#endif // VISIONMASKITEM_H
