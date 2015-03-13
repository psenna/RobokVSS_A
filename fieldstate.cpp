#include "fieldstate.h"

using namespace robok;

/*
 * GETTERS AND SETTERS
 */
void Fieldstate::set_goalLeft(const Goal &goal){
    this->goal_left_ = goal;
}

Goal Fieldstate::get_goalLeft() const{
    return this->goal_left_;
}

void Fieldstate::set_goalRight(const Goal &goal){
    this->goal_right_ = goal;
}

Goal Fieldstate::get_goalRight() const{
    return this->goal_right_;
}

void Fieldstate::set_ball(const Ball &ball){
    this->ball_ = ball;
}

Ball Fieldstate::get_ball() const{
    return this->ball_;
}

void Fieldstate::set_RobotsTeam(const std::vector<Robot> &robots){
    this->robots_team_ = robots;
}

std::vector<Robot> Fieldstate::get_robots_team() const {
   return this->robots_team_;
}

Robot Fieldstate::get_robotTeamById(int id){
    return robots_team_.at(id);
}

void Fieldstate::set_RobotsEnemy(const std::vector<Robot> &robots){
    this->robots_enemy_ = robots;
}

std::vector<Robot> Fieldstate::get_robots_enemy() const {
    return this->robots_enemy_;
}

void Fieldstate::set_field_centre(const Position &position){
    this->field_centre_ = position;
}

Position Fieldstate::get_field_centre() const {
    return this->field_centre_;
}
