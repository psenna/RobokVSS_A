#ifndef BALL_H
#define BALL_H

#include "unit.h"

using namespace robok;

class Ball : public Unit
{
private:
    Position prediction_;

public:
    Ball();
    Position get_prediction() const;
    void set_prediction(const Position &position_);
    void set_prediction(const int &x, const int &y);
};

#endif // BALL_H
