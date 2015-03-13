#include "ball.h"

Ball::Ball()
{
}

/*
 * GETTERS AND SETTERS
 */
Position Ball::get_prediction() const {
    return this->prediction;
}

void Ball::set_prediction(const Position &position){
    this->prediction = position;
}

void Ball::set_prediction(const int &x, const int &y){
    this->prediction.x = x;
    this->prediction.y = y;
}
