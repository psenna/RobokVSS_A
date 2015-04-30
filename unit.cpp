#include "unit.h"

using namespace robok;

Unit::Unit()
{
}

/*
 * GETTERS AND SETTERS
 */
Position Unit::getPosition() const {
    return m_Position;
}

void Unit::setPosition(const Position& position){
    m_Position = position;
}

void Unit::setPosition(const int& x, const int& y){
    m_Position.x = x;
    m_Position.y = y;
}
