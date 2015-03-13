#include "robot.h"

Robot::Robot()
{
}

Position Robot::get_objective(){
    return this->objective;
}

void Robot::set_objective(Position position){
    this->objective = position;
}

void Robot::set_objective(int x, int y){
    this->objective.x = x;
    this->objective.y = y;
}
