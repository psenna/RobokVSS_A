#include "fieldstate.h"

using namespace robok;

Fieldstate::Fieldstate(){
    m_RobotsTeam.resize(5);
    m_RobotsEnemy.resize(5);
}

/*
 * GETTERS AND SETTERS
 */
void Fieldstate::setGoalLeft(const Goal &goal){
    this->m_GoalLeft = goal;
}

Goal Fieldstate::getGoalLeft() const{
    return this->m_GoalLeft;
}

void Fieldstate::setGoalRight(const Goal &goal){
    this->m_GoalRight = goal;
}

Goal Fieldstate::getGoalRight() const{
    return this->m_GoalRight;
}

void Fieldstate::setBall(const Ball &ball){
    this->m_Ball = ball;
}

Ball Fieldstate::getBall() const{
    return this->m_Ball;
}

void Fieldstate::setRobotsTeam(const std::vector<Robot> &robots){
    this->m_RobotsTeam = robots;
}

std::vector<Robot> Fieldstate::getRobotsTeam() const {
   return this->m_RobotsTeam;
}

Robot Fieldstate::getRobotTeamById(const int &id){
    return m_RobotsTeam.at(id);
}

void Fieldstate::setRobotTeamById(const Robot &robot, const int &id){
    m_RobotsTeam[id] = robot;
}

void Fieldstate::setRobotsEnemy(const std::vector<Robot> &robots){
    this->m_RobotsEnemy = robots;
}

std::vector<Robot> Fieldstate::getRobotsEnemy() const {
    return this->m_RobotsEnemy;
}

void Fieldstate::setFieldCenter(const Position &position){
    this->m_FieldCenter = position;
}

Position Fieldstate::getFieldCenter() const {
    return this->m_FieldCenter;
}

void Fieldstate::setArea(int area){
    area_x = area;
}

int Fieldstate::getArea(){
    return this->area_x;
}

void Fieldstate::setArea2(int area){
    area2_x = area;
}

int Fieldstate::getArea2(){
    return this->area2_x;
}
