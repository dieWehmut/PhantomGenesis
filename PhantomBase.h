#ifndef PHANTOMBASE_H
#define PHANTOMBASE_H
#include"ActiveObject.h"
class PhantomBase: public ActiveObject{
public:
    PhantomBase();
    int getAtkRange() const { return atkRange; }
protected:

};

#endif // PHANTOMBASE_H
