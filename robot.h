#ifndef ROBOT_H
#define ROBOT_H

#include "unit.h"

class Robot : public Unit
{
private:
    Position objective;

public:
    Robot();
    Position get_objective();
    void set_objective(Position position);
    void set_objective(int x, int y);
};

#endif // ROBOT_H
