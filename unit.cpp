#include "unit.h"

Unit::Unit()
{
}

/*
 * GETTERS AND SETTERS
 */
Position Unit::get_position() const {
    return this->position;
}

void Unit::set_position(const Position &position){
    this->position = position;
}

void Unit::set_position(const int &x, const int &y){
    this->position.x = x;
    this->position.y = y;
}
