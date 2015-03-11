#include "ball.h"

Ball::Ball()
{
}

posicao Ball::get_prediction(){
    return this->prediction;
}

void Ball::set_prediction(posicao position){
    this->prediction = position;
}

void Ball::set_prediction(int x, int y){
    this->prediction.x = x;
    this->prediction.y = y;
}
