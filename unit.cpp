#include "unit.h"

using namespace robok;

Unit::Unit()
{
}

/*
 * GETTERS AND SETTERS
 */
Position Unit::get_position() const {
    return this->position_;
}

void Unit::set_position(const Position &position){
    this->position_ = position;
}

void Unit::set_position(const int &x, const int &y){
    this->position_.x = x;
    this->position_.y = y;
}
