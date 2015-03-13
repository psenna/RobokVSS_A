#ifndef FIELDSTATE_H
#define FIELDSTATE_H

#include "ball.h"
#include <iostream>
#include <vector>
#include "robot.h"

namespace robok {

class Fieldstate
{
private:
    Goal goal_left_;
    Goal goal_right_;
    Ball ball_;
    std::vector<Robot> robots_team_;
    std::vector<Robot> robots_enemy_;
    Position field_centre_;

public:
    Fieldstate();
    Goal get_goalLeft() const;
    void set_goalLeft(const Goal &goal);
    Goal get_goalRight() const;
    void set_goalRight(const Goal &goal);
    Ball get_ball() const;
    void set_ball(const Ball &ball);
    std::vector<Robot> get_robots_team() const;
    void set_RobotsTeam(const std::vector<Robot> &robots);
    Robot get_robotTeamById(int id);
    std::vector<Robot> get_robots_enemy() const;
    void set_RobotsEnemy(const std::vector<Robot> &robots);
    Position get_field_centre() const;
    void set_field_centre(const Position &position);

};

}

#endif // FIELDSTATE_H
