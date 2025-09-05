#include "VisionMaskItem.h"
#include "GameManager.h"
#include <QtMath>

void VisionMaskItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    if (!player0) return;
    painter->save();
    painter->setBrush(QColor(0, 0, 0, 100));
    painter->setPen(Qt::NoPen);
    painter->drawRect(sceneRect0);
    QPainterPath maskPath;
    maskPath.addRect(sceneRect0 );
    // 玩家视野
    QPointF playerCenter = player0->pos() + player0->boundingRect().center();
    qreal sightRange = player0->getSightRange();
    maskPath.addEllipse(playerCenter, sightRange, sightRange);
    if (gm0) {
        // FlamePhantom
        for (auto* phantom : gm0->getFlamePhantoms()) {
            if (phantom && phantom->scene() && phantom->isExposed()) {
                QPointF center = phantom->pos() + phantom->boundingRect().center();
                qreal dist = QLineF(playerCenter, center).length();
                if (dist > sightRange) {//在视野外挖空
                    maskPath.addEllipse(center, 40, 40);
                }
            }
        }
        // LurkPhantom
        for (auto* phantom : gm0->getLurkPhantoms()) {
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
