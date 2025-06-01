#include "VisionMaskItem.h"
#include "GameManager.h"
#include <QtMath>

void VisionMaskItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    if (!m_player) return;
    painter->save();
    painter->setBrush(QColor(0, 0, 0, 100));
    painter->setPen(Qt::NoPen);
    painter->drawRect(m_sceneRect);
    QPainterPath maskPath;
    maskPath.addRect(m_sceneRect);
    // 玩家视野
    QPointF playerCenter = m_player->pos() + QPointF(m_player->pixmap().width()/2, m_player->pixmap().height()/2);
    qreal sightRange = m_player->getSightRange();
    maskPath.addEllipse(playerCenter, sightRange, sightRange);
    if (m_gm) {
        // FlamePhantom
        for (auto* phantom : m_gm->getFlamePhantoms()) {
            if (phantom && phantom->scene() && phantom->isExposed()) {
                QPointF center = phantom->pos() + QPointF(phantom->boundingRect().width()/2, phantom->boundingRect().height()/2);
                qreal dist = QLineF(playerCenter, center).length();
                if (dist > sightRange) { // 只在视野外挖空
                    maskPath.addEllipse(center, 40, 40);
                }
            }
        }
        // LurkPhantom
        for (auto* phantom : m_gm->getLurkPhantoms()) {
            if (phantom && phantom->scene() && phantom->isExposed()) {
                QPointF center = phantom->pos() + QPointF(phantom->boundingRect().width()/2, phantom->boundingRect().height()/2);
                qreal dist = QLineF(playerCenter, center).length();
                if (dist > sightRange) {
                    maskPath.addEllipse(center, 40, 40);
                }
            }
        }
    }

    painter->setCompositionMode(QPainter::CompositionMode_Clear);
    painter->drawPath(maskPath.simplified());
    painter->restore();
}