#include "fieldstate.h"


void Fieldstate::set_goalLeft(Goal goal){
    this->goal_left = goal;
}

Goal Fieldstate::get_goalLeft(){
    return this->goal_left;
}

void Fieldstate::set_goalRight(Goal goal){
    this->goal_right = goal;
}

Goal Fieldstate::get_goalRight(){
    return this->goal_right;
}

void Fieldstate::set_ball(Ball ball){
    this->ball = ball;
}

Ball Fieldstate::get_ball(){
    return this->ball;
}

void Fieldstate::set_RobotsTeam(std::vector<Robot> robots){
    this->robotsTeam = robots;
}

std::vector<Robot> Fieldstate::get_RobotsTeam(){
   return this->robotsTeam;
}

Robot Fieldstate::get_robotTeamById(int id){
    return robotsTeam.at(id);
}

void Fieldstate::set_RobotsEnemy(std::vector<Robot> robots){
    this->robots_enemy = robots;
}

std::vector<Robot> Fieldstate::get_RobotsEnemy(){
    return this->robots_enemy;
}

void Fieldstate::set_fieldCentre(Position position){
    this->field_centre = position;
}

Position Fieldstate::get_fieldCentre(){
    return this->field_centre;
}
