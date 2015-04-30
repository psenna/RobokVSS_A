#ifndef ROBOT_H
#define ROBOT_H

#include "unit.h"

namespace robok {

class Robot : public Unit
{
public:

    Robot();

    Position getObjective() const;
    void setObjective(const Position& position);
    void setObjective(const int& x, const int& y);

private:
    Position m_Objective;

};

}

#endif // ROBOT_H
