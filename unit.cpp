#include "unit.h"

Unit::Unit()
{
}

posicao Unit::get_position(){
    return this->position;
}

void Unit::set_position(posicao position){
    this->position = position;
}

void Unit::set_position(int x, int y){
    this->position.x = x;
    this->position.y = y;
}
