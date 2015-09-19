#ifndef DOUBLEATTACK_H
#define DOUBLEATTACK_H

#include "strategy.h"
#include "robot.h"
#include <iostream>
#include <vector>

using namespace robok;

class DoubleAttack : public Strategy
{
public:
    DoubleAttack();
    void play(Fieldstate *fs);
    void goalkeeper(Fieldstate *fs, int id);
    void roboEsquerda(Fieldstate *fs, int id);
    void roboDireita(Fieldstate *fs, int id);
    Position goleiroObj, esquerdaObj, direitaObj;
};

#endif // DOUBLEATTACK_H
