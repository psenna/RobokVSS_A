#ifndef UNIT_H
#define UNIT_H

#include "structs.cpp"

class Unit
{
private:
    Position position;

public:
    Unit();
    Position get_position() const;
    void set_position(const Position &position);
    void set_position(const int &x, const int &y);
};

#endif // UNIT_H
