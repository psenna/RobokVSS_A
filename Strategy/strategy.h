#ifndef STRATEGY_H
#define STRATEGY_H

#include "fieldstate.h"

class Strategy
{
public:
    Strategy() {}
    ~Strategy() {}
    virtual void play(robok::Fieldstate *fs) = 0;
};

#endif // STRATEGY_H
