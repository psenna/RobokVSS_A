#ifndef BALL_H
#define BALL_H

#include "unit.h"

class Ball : public Unit
{
private:
    posicao prediction;

public:
    Ball();
    posicao get_prediction();
    void set_prediction(posicao position);
    void set_prediction(int x, int y);
};

#endif // BALL_H
