#include "robot.h"

using namespace robok;

Robot::Robot()
{
}

Position Robot::getObjective() const{
    return m_Objective;
}

void Robot::setObjective(const Position &position){
    m_Objective = position;
}

void Robot::setObjective(const int &x, const int &y){
    m_Objective.x = x;
    m_Objective.y = y;
}

Command Robot::getCommand() const
{
    return m_Command;
}

void Robot::setCommand(const Command &command)
{
    m_Command = command;
}

void Robot::setOrientation(const float &orientation){
    this->m_Orientation = orientation;
}

float Robot::getOrientation() const{
    return this->m_Orientation;
}
