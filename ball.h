#ifndef BALL_H
#define BALL_H

#include "unit.h"

namespace robok {

class Ball : public Unit
{
private:
    Position m_Prediction;   

public:
    Ball();
    Position getPrediction() const;
    void setPrediction(const Position& position);
    void setPrediction(const int& x, const int& y);
};

}

#endif // BALL_H
