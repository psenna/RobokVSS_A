#ifndef UNIT_H
#define UNIT_H

#include "utils.cpp"

namespace robok {

class Unit
{
public:

    Unit();

    Position getPosition() const;
    void setPosition(const Position &position);
    void setPosition(const int &x, const int &y);

private:
    Position m_Position;

};

} // end namespace robok

#endif // UNIT_H
