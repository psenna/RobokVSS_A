#ifndef DEFAULT_H
#define DEFAULT_H

#include "strategy.h"
#include "robot.h"
#include <iostream>
#include <vector>

using namespace robok;

class Default : public Strategy
{
public:
    Default();
    void play(Fieldstate *fs);
    void goalkeeper(Fieldstate *fs);
    void attacker(Fieldstate *fs);
    void defender(Fieldstate *fs);

};

#endif // DEFAULT_H
