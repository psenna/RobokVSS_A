#ifndef BALL_H
#define BALL_H

#include "unit.h"

class Ball : public Unit
{
private:
    Position prediction;

public:
    Ball();
    Position get_prediction() const;
    void set_prediction(const Position &position);
    void set_prediction(const int &x, const int &y);
};

#endif // BALL_H
