#ifndef FIELDSTATE_H
#define FIELDSTATE_H

#include "ball.h"
#include <iostream>
#include <vector>
#include "robot.h"

class Fieldstate
{
private:
    Goal goal_left;
    Goal goal_right;
    Ball ball;
    std::vector<Robot> robots_team;
    std::vector<Robot> robots_enemy;
    posicao field_centre;

public:
    Fieldstate();
    Goal get_goalLeft();
    void set_goalLeft(Goal goal);
    Goal get_goalRight();
    void set_goalRight(Goal goal);
    Ball get_ball();
    void set_ball(Ball ball);
    std::vector<Robot> get_RobotsTeam();
    void set_RobotsTeam(std::vector<Robot> robots);
    Robot get_robotTeamById(int id);
    std::vector<Robot> get_RobotsEnemy();
    void set_RobotsEnemy(std::vector<Robot> robots);
    posicao get_fieldCentre();
    void set_fieldCentre(posicao position);

};

#endif // FIELDSTATE_H
