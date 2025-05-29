#include "PhantomBase.h"

PhantomBase::PhantomBase(){
    if (rangeIndicatorItem) {
        if (rangeIndicatorItem->scene())
            rangeIndicatorItem->scene()->removeItem(rangeIndicatorItem);
        delete rangeIndicatorItem;
        rangeIndicatorItem = nullptr;
    }
}

PhantomBase::~PhantomBase(){
    if (rangeIndicatorItem) {
        if (rangeIndicatorItem->scene())
            rangeIndicatorItem->scene()->removeItem(rangeIndicatorItem);
        delete rangeIndicatorItem;
        rangeIndicatorItem = nullptr;
    }
}