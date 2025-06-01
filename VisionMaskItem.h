#ifndef VISIONMASKITEM_H
#define VISIONMASKITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include "Player.h"
class GameManager;

class VisionMaskItem : public QGraphicsItem {
public:
    VisionMaskItem(Player* player, const QRectF& sceneRect, GameManager* gm = nullptr)
        : m_player(player), m_sceneRect(sceneRect), m_gm(gm) {}

    QRectF boundingRect() const override {
        return m_sceneRect;
    }

    void setSceneRect(const QRectF& rect) {
        prepareGeometryChange();
        m_sceneRect = rect;
    }

    void setGameManager(GameManager* gm) { m_gm = gm; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

private:
    Player* m_player;
    QRectF m_sceneRect;
    GameManager* m_gm = nullptr;
};

#endif // VISIONMASKITEM_H
