#ifndef ROBOT_H
#define ROBOT_H

#include "unit.h"

namespace robok {

class Robot : public Unit
{
public:

    Robot();

    int getId() const;
    void setId(const int& id);
    Position getObjective() const;
    void setObjective(const Position& position);
    void setObjective(const int& x, const int& y);
    Command getCommand() const;
    void setCommand(const Command& command);

private:
    int m_Id;
    Position m_Objective;
    Command m_Command;
};

}

#endif // ROBOT_H
