#ifndef FIELDSTATE_H
#define FIELDSTATE_H

#include "ball.h"
#include <iostream>
#include <vector>
#include "robot.h"

namespace robok {

class Fieldstate
{
public:

    Fieldstate();

    Goal getGoalLeft() const;
    void setGoalLeft(const Goal& goal);

    Goal getGoalRight() const;
    void setGoalRight(const Goal &goal);

    Ball getBall() const;
    void setBall(const Ball& ball);

    std::vector<Robot> getRobotsTeam() const;
    void setRobotsTeam(const std::vector<Robot> &robots);
    Robot getRobotTeamById(const int& id);
    void setRobotTeamById(const Robot &robot, const int &id);


    std::vector<Robot> getRobotsEnemy() const;
    void setRobotsEnemy(const std::vector<Robot>& robots);

    Position getFieldCenter() const;
    void setFieldCenter(const Position& position);

private:
    Goal m_GoalLeft;
    Goal m_GoalRight;
    Ball m_Ball;
    std::vector<Robot> m_RobotsTeam;
    std::vector<Robot> m_RobotsEnemy;
    Position m_FieldCenter;

};

}

#endif // FIELDSTATE_H
