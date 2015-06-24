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

    defender(fs, 0);

}

void Default::goalkeeper(Fieldstate *fs, int id){
    Position ball, trave1, trave2;

    ball.x = fs->getBall().getPosition().x;
    ball.y = fs->getBall().getPosition().y;
    trave1 = fs->getGoalLeft().GoalPost_1;
    trave2 = fs->getGoalRight().GoalPost_2;

    goleiroObj.x = fs->getGoalLeft().GoalPost_1.x + 15;             ///goleirox recebe um x fixo, um pouco a frente do gol

    if(ball.y > trave1.y && ball.y < trave2.y){                     /// verifica se bolay esta entre as traves.
        goleiroObj.y = ball.y;                                      /// se estiver o obj do goleiro é o y da bola
    }else{
        goleiroObj.y = (ball.y < trave1.y ? trave1.y : trave2.y);   /// caso contrário, seta o y do goleiro como o y de uma das traves.
    }
}

void Default::defender(Fieldstate *fs, int id){
    Position ball;

    ball.x = fs->getBall().getPosition().x;
    ball.y = fs->getBall().getPosition().y;

    if(ball.x > fs->getFieldCenter().x){                             ///verifica se a bola está no campo de ataque

        zagueiroObj.x = (fs->getFieldCenter().x - fs->getArea())/4 + fs->getArea(); ///seta objetivo do zag um pouco a frente da area

        int dy1 = fs->getFieldCenter().y - ball.y;
        int dx1 = (ball.x - fs->getGoalLeft().GoalPost_1.x);
        int dx2 = (ball.x - zagueiroObj.x);
        int dy2 = (int) (dy1*dx2)/dx1;

        zagueiroObj.y = dy2 + ball.y;
        std::cout  << " y: " << dy2 << std::endl;

    }else{
        zagueiroObj.x = ball.x;
        zagueiroObj.y = ball.y;
    }
}

void Default::attacker(Fieldstate *fs, int id){
    Position ball;

    ball.x = fs->getBall().getPosition().x;
    ball.y = fs->getBall().getPosition().y;

    if(ball.x > fs->getFieldCenter().x){                             ///verifica se a bola está no campo de ataque
        atacanteObj.x = ball.x;
        atacanteObj.y = ball.y;
    }else{
        atacanteObj.x = fs->getFieldCenter().x;
        atacanteObj.y = fs->getFieldCenter().y;
    }
}
