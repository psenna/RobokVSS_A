#ifndef UNIT_H
#define UNIT_H

#include "structs.cpp"

class Unit
{
private:
    posicao position;

public:
    Unit();
    posicao get_position();
    void set_position(posicao position);
    void set_position(int x, int y);
};

#endif // UNIT_H
