#include "robot.h"

using namespace robok;

Robot::Robot()
{
}

Position Robot::get_objective() const{
    return this->objective_;
}

void Robot::set_objective(const Position &position){
    this->objective_ = position;
}

void Robot::set_objective(const int &x, const int &y){
    this->objective_.x = x;
    this->objective_.y = y;
}
