#include "doubleattack.h"

DoubleAttack::DoubleAttack()
{

}

void DoubleAttack::play(Fieldstate *fs){
    goalkeeper(fs, 0);
    roboEsquerda(fs, 0);
    roboDireita(fs, 0);
}

void DoubleAttack::goalkeeper(Fieldstate *fs, int id){
    Position ball, trave1, trave2;

    ball.x = fs->getBall().getPosition().x;
    ball.y = fs->getBall().getPosition().y;
    std::cout << "ballx: " << ball.x << "  bally:  " << ball.y << std::endl;
    trave1 = fs->getGoalLeft().GoalPost_1;
    trave2 = fs->getGoalRight().GoalPost_2;

    goleiroObj.x = fs->getGoalLeft().GoalPost_1.x + 15;             ///goleirox recebe um x fixo, um pouco a frente do gol

    if(ball.y > trave1.y && ball.y < trave2.y){                     /// verifica se bolay esta entre as traves.
        goleiroObj.y = ball.y;                                      /// se estiver o obj do goleiro é o y da bola
    }else{
        goleiroObj.y = (ball.y < trave1.y ? trave1.y : trave2.y);   /// caso contrário, seta o y do goleiro como o y de uma das traves.
    }
}

void DoubleAttack::roboEsquerda(Fieldstate *fs, int id){
    Position ball;
    int centroY = fs->getFieldCenter().y;

    ball.x = fs->getBall().getPosition().x;
    ball.y = fs->getBall().getPosition().y;

    //traveY = (fs->getGoalLeft().GoalPost_1.y + fs->getGoalRight().GoalPost_1.y)/2;

    if(ball.y < centroY){
        float ang = atan2(fs->getFieldCenter().y - ball.y, fs->getGoalRight().GoalPost_1.x - ball.x);

        float hipo =  sqrt(pow(ball.y - fs->getFieldCenter().y, 2) + pow(fs->getGoalRight().GoalPost_1.x - ball.x, 2)) + 40;
        float dy2 = sin(ang) * hipo;
        float dx2 = cos(ang) * hipo;

        esquerdaObj.x = (int) fs->getGoalRight().GoalPost_1.x - dx2;
        esquerdaObj.y = (int) fs->getFieldCenter().y - dy2;
    }else{
        esquerdaObj.x = fs->getGoalRight().GoalPost_1.x - (fs->getGoalRight().GoalPost_1.x - ball.x)*1.4;
        esquerdaObj.y = centroY;
    }

    if(esquerdaObj.x < fs->getGoalLeft().GoalPost_1.x + 10)
        esquerdaObj.x = fs->getGoalLeft().GoalPost_1.x + 10;
}

void DoubleAttack::roboDireita(Fieldstate *fs, int id){
    Position ball, trave1, trave2;
    int centroY = fs->getFieldCenter().y;

    ball.x = fs->getBall().getPosition().x;
    ball.y = fs->getBall().getPosition().y;

    //traveY = (fs->getGoalLeft().GoalPost_1.y + fs->getGoalRight().GoalPost_1.y)/2;

    if(ball.y > centroY){
        float ang = atan2(fs->getFieldCenter().y - ball.y, fs->getGoalRight().GoalPost_1.x - ball.x);

        float hipo =  sqrt(pow(ball.y - fs->getFieldCenter().y, 2) + pow(fs->getGoalRight().GoalPost_1.x - ball.x, 2)) + 40;
        float dy2 = sin(ang) * hipo;
        float dx2 = cos(ang) * hipo;

        direitaObj.x = (int) fs->getGoalRight().GoalPost_1.x - dx2;
        direitaObj.y = (int) fs->getFieldCenter().y - dy2;
    }else{
        direitaObj.x = fs->getGoalRight().GoalPost_1.x - (fs->getGoalRight().GoalPost_1.x - ball.x)*1.4;
        direitaObj.y = centroY;
    }

    if(direitaObj.x < fs->getGoalLeft().GoalPost_1.x + 10)
        direitaObj.x = fs->getGoalLeft().GoalPost_1.x + 10;
}
