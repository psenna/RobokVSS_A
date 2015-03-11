#include "robot.h"

Robot::Robot()
{
}

posicao Robot::get_objective(){
    return this->objective;
}

void Robot::set_objective(posicao position){
    this->objective = position;
}

void Robot::set_objective(int x, int y){
    this->objective.x = x;
    this->objective.y = y;
}
