#ifndef UNIT_H
#define UNIT_H

#include "structs.cpp"

namespace robok {

class Unit
{
private:
    Position position_;

public:
    Unit();
    Position get_position() const;
    void set_position(const Position &position);
    void set_position(const int &x, const int &y);
};

} // end namespace robok

#endif // UNIT_H
