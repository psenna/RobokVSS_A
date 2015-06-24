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
    void goalkeeper(Fieldstate *fs, int id);
    void attacker(Fieldstate *fs, int id);
    void defender(Fieldstate *fs, int id);
    Position goleiroObj, atacanteObj, zagueiroObj;

private:


};

#endif // DEFAULT_H
