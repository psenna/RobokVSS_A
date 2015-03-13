#include "ball.h"

Ball::Ball()
{
}

/*
 * GETTERS AND SETTERS
 */
Position Ball::get_prediction() const {
    return this->prediction_;
}

void Ball::set_prediction(const Position &position){
    this->prediction_ = position;
}

void Ball::set_prediction(const int &x, const int &y){
    this->prediction_.x = x;
    this->prediction_.y = y;
}
