#include "ball.h"

Ball::Ball()
{
}

/*
 * GETTERS AND SETTERS
 */
Position Ball::getPrediction() const {
    return m_Prediction;
}

void Ball::setPrediction(const Position& position){
    m_Prediction = position;
}

void Ball::setPrediction(const int& x, const int& y){
    m_Prediction.x = x;
    m_Prediction.y = y;
}
