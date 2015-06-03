#include "default.h"

Default::Default()
{
}

void Default::play(Fieldstate *fs)
{
    std::vector<Robot> robots = fs->getRobotsTeam();

    for(int i=0; i<robots.size(); i++){
//        std::cout << "posx: " << robots.at(i).getPosition().x << " posy: " << robots.at(i).getPosition().y << std::endl;
    }

//    std::cout << "l1: " << fs->getGoalLeft().GoalPost_1.x << std::endl;
//    std::cout << "l2: " << fs->getGoalLeft().GoalPost_2.x << std::endl;
//    std::cout << "r1: " << fs->getGoalRight().GoalPost_1.x << std::endl;
//    std::cout << "r2: " << fs->getGoalRight().GoalPost_2.x << std::endl;

}

void Default::goalkeeper(Fieldstate *fs){

}

void Default::defender(Fieldstate *fs){

}

void Default::attacker(Fieldstate *fs){

}
