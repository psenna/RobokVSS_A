#include "motion.h"

#define ANGLIMITE 25
#define VELMAX 70
#define VELMAXGIRO 10
#define DISTMAX 0

void Motion::movimenta(robok::Fieldstate *fs){
    std::vector<robok::Robot> robots;
    Position ballPosition;

    robots = fs->getRobotsTeam();
    ballPosition = fs->getBall().getPosition();

    for(int i=0; i<robots.size(); ++i){

        if(fs->getRobotTeamById(i).getPosition().x != -1){
            float anguloRoboBola = (atan2(fs->getRobotTeamById(i).getPosition().y - fs->getRobotTeamById(i).getObjective().y,
                                         fs->getRobotTeamById(i).getObjective().x - fs->getRobotTeamById(i).getPosition().x))*180/3.1415;


            float angRelativo = anguloRoboBola - fs->getRobotTeamById(i).getOrientation();
            if(angRelativo < -180) angRelativo += 360;
            if(angRelativo > 180) angRelativo -= 360;

            std::cout << i << " " << angRelativo << std::endl;
            angRelativo = previsaoAngulo(fs, angRelativo, i);
            std::cout << i << " " << angRelativo << std::endl;

            if(angRelativo < -180) angRelativo += 360;
            if(angRelativo > 180) angRelativo -= 360;

            verificarAngulo(angRelativo, i, fs);
        }
    }
}

void Motion::verificarAngulo(float angRelativo, int id, robok::Fieldstate *fs){


    int distRoboObj = sqrt(pow(fs->getRobotTeamById(id).getPosition().x - fs->getBall().getPosition().x, 2) +
                           pow(fs->getRobotTeamById(id).getPosition().y - fs->getBall().getPosition().y ,2));

    std::cout << "dist: " << distRoboObj << std::endl;

    if(fabs(angRelativo) < ANGLIMITE || fabs(angRelativo) > (180-ANGLIMITE)){
        fazCurva(angRelativo, id, fs, distRoboObj);
        return;
    }

    Command comando;

    if((angRelativo > 0 && angRelativo < 90) || (angRelativo > -180 && angRelativo < -90)){
        comando.SpeedLeft = VELMAXGIRO;
        comando.OrientationLeft = 1;
        comando.SpeedRight = VELMAXGIRO;
        comando.OrientationRight = 0;
    }else{
        comando.SpeedRight = VELMAXGIRO;
        comando.OrientationRight = 1;
        comando.SpeedLeft = VELMAXGIRO;
        comando.OrientationLeft = 0;
    }


    comando.ID = fs->getRobotTeamById(id).getId();
    robok::Robot robot = fs->getRobotTeamById(id);

    robot.setCommand(comando);
    fs->setRobotTeamById(robot, id);
}

void Motion::fazCurva(float angRelativo, int id, robok::Fieldstate *fs, int dist){
    Command comando;

    std::cout << "faz curva" << std::endl;
    if(angRelativo > 0){
        if(angRelativo < 90){
            comando.SpeedLeft = (int) fabs(VELMAX);
            comando.OrientationLeft = 0;
            comando.SpeedRight = VELMAX-5;
            comando.OrientationRight = 0;
        }else{
            comando.SpeedLeft = (int) fabs(VELMAX); // - (180-angRelativo*0.1));
            comando.OrientationLeft = 1;
            comando.SpeedRight = VELMAX-15;
            comando.OrientationRight = 1;
        }
    }else{
        if(fabs(angRelativo) < 90){
            comando.SpeedRight = (int) fabs(VELMAX); // + angRelativo*0.1);
            comando.OrientationRight = 0;
            comando.SpeedLeft = VELMAX;
            comando.OrientationLeft = 0;
        }else{
            comando.SpeedRight = (int) fabs(VELMAX); //+ (fabs(angRelativo*0.1)-180));
            comando.OrientationRight = 1;
            comando.SpeedLeft = VELMAX;
            comando.OrientationLeft = 1;
        }

    }

    comando.ID = fs->getRobotTeamById(id).getId();
    robok::Robot robot = fs->getRobotTeamById(id);


    robot.setCommand(comando);
    fs->setRobotTeamById(robot, id);
}

float Motion::previsaoAngulo(robok::Fieldstate * fs, float angRelativo, int id){
    return fs->getAng(id, angRelativo);
}
