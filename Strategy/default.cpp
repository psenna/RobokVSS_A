#include "default.h"

Default::Default()
{
}

void Default::play(Fieldstate *fs)
{
    goalkeeper(fs, 0);
    defender(fs, 0);
    attacker(fs, 0);

    distribuiPosicoes(fs);
}

void Default::goalkeeper(Fieldstate *fs, int id){
    Position ball, trave1, trave2;

    ball.x = fs->getBall().getPosition().x;
    ball.y = fs->getBall().getPosition().y;
    //std::cout << "ballx: " << ball.x << "  bally:  " << ball.y << std::endl;
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
        //std::cout  << " y: " << dy2 << std::endl;

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

        if(ball.y > 100 && ball.y < 380){
            float ang = atan2(fs->getFieldCenter().y - ball.y, fs->getGoalRight().GoalPost_1.x - ball.x);

            float hipo =  sqrt(pow(ball.y - fs->getFieldCenter().y, 2) + pow(fs->getGoalRight().GoalPost_1.x - ball.x, 2)) + 40;
            float dy2 = sin(ang) * hipo;
            float dx2 = cos(ang) * hipo;

            atacanteObj.x = (int) fs->getGoalRight().GoalPost_1.x - dx2;
            atacanteObj.y = (int) fs->getFieldCenter().y - dy2;

            //std::cout << ang << " " << hipo << " " << dy2 << " " << dx2 << fs->getFieldCenter().x << fs->getFieldCenter().y << std::endl;
        }else{
            atacanteObj.x = ball.x - 30;
            atacanteObj.y = ball.y;
        }
    }else{
        atacanteObj.x = fs->getFieldCenter().x;
        atacanteObj.y = fs->getFieldCenter().y;
    }
}

void Default::distribuiPosicoes(Fieldstate *fs){

    std::vector<Robot> robots = fs->getRobotsTeam();


    float menor = 9999;
    int goleiroId=-1, zagueiroId=-1, atacanteId=-1;

    for(int i=0; i<robots.size(); ++i){
         if(atacanteObj.distance(fs->getRobotTeamById(i).getPosition()) < menor && i != goleiroId && i!=zagueiroId  && fs->getRobotTeamById(i).getPosition().x > 0){
            menor = atacanteObj.distance(fs->getRobotTeamById(i).getPosition());
            atacanteId = i;
         }
    }
    if(atacanteId != -1){
        verificaAtacante(fs, atacanteId);
        Robot atacante = robots.at(atacanteId);
        atacante.setObjective(atacanteObj);
        //std::cout << "atac ID: " << goleiroId << std::endl;
        fs->setRobotTeamById(atacante, atacanteId);
    }

    menor = 9999;

    for(int i=0; i<robots.size(); ++i){
         if(goleiroObj.distance(fs->getRobotTeamById(i).getPosition()) < menor && i!= atacanteId && i!= zagueiroId && fs->getRobotTeamById(i).getPosition().x > 0){
            menor = goleiroObj.distance(fs->getRobotTeamById(i).getPosition());
            goleiroId = i;
         }
    }
    if(goleiroId != -1){
        Robot goleiro = robots.at(goleiroId);
        goleiro.setObjective(goleiroObj);
        //std::cout << "gol ID: " << goleiroId << std::endl;
        fs->setRobotTeamById(goleiro, goleiroId);
    }


    menor = 9999;
    for(int i=0; i<robots.size(); ++i){
         if(zagueiroObj.distance(fs->getRobotTeamById(i).getPosition()) < menor && i != goleiroId && i!=atacanteId && fs->getRobotTeamById(i).getPosition().x > 0){
            menor = zagueiroObj.distance(fs->getRobotTeamById(i).getPosition());
            zagueiroId = i;
         }
    }

    if(zagueiroId != -1){
        Robot zagueiro = robots.at(zagueiroId);
        zagueiro.setObjective(zagueiroObj);
        fs->setRobotTeamById(zagueiro, zagueiroId);
    }



    for(int i=0; i<robots.size(); ++i){
        if(fs->getRobotTeamById(i).getPosition().x > 0);
            //std::cout << i << " x: " << fs->getRobotTeamById(i).getObjective().x << "  y: " << fs->getRobotTeamById(i).getObjective().y << std::endl;
    }
}

void Default::verificaAtacante(Fieldstate *fs, int atacanteId){
    Robot robo = fs->getRobotTeamById(atacanteId);
    Position bola = fs->getBall().getPosition();
    if(robo.getPosition().x > atacanteObj.x-100 && robo.getPosition().x < atacanteObj.x+30){
        if((robo.getPosition().y < bola.y && bola.y < fs->getFieldCenter().y)
                || (robo.getPosition().y > bola.y && bola.y > fs->getFieldCenter().y)){
            atacanteObj.x = bola.x;
            atacanteObj.y = bola.y;
        }
    }
}
