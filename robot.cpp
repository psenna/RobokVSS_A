#include "robot.h"

using namespace robok;

Robot::Robot()
{
}

Robot::Robot(const Robot &robot){
    this->setObjective(robot.getObjective());
    this->setPosition(robot.getPosition());
    this->setOrientation(robot.getOrientation());
    this->setCommand(robot.getCommand());
    this->setId(robot.getId());
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

int Robot::getId() const{
    return this->m_Id;
}

void Robot::setId(const int &id){
    m_Id = id;
}
