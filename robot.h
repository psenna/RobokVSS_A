#ifndef ROBOT_H
#define ROBOT_H

#include "unit.h"

class Robot : public Unit
{
private:
    posicao objective;

public:
    Robot();
    posicao get_objective();
    void set_objective(posicao position);
    void set_objective(int x, int y);
};

#endif // ROBOT_H
