#ifndef ROBOT_H
#define ROBOT_H

#include "unit.h"

namespace robok {

class Robot : public Unit
{
private:
    Position objective_;

public:
    Robot();
    Position get_objective() const;
    void set_objective(const Position &position_);
    void set_objective(const int &x, const int &y);
};

}

#endif // ROBOT_H
