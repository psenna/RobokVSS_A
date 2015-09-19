#include "serial.h"
#include <string.h>
#include <iostream>

Serial::Serial()
{
}

Serial::~Serial()
{

}

QString Serial::createCommand(int id, int senEsq, int velEsq, int senDir, int velDir)
{
    char command[15];
    sprintf(command, "<%d%d%03d%d%03d>", id, senEsq, velEsq, senDir, velDir);
    return QString::fromUtf8(command);

}

void Serial::createSendCommands(robok::Fieldstate * fs)
{
    for (int i = 0; i < fs->getRobotsTeam().size(); ++i) {
        if(fs->getRobotTeamById(i).getPosition().x != -1){
            //std::cout << "entrou finalmente" << std::endl;
            robok::Robot robo = fs->getRobotTeamById(i);
            Command comando = robo.getCommand();
            std::cout << comando.ID << " " << comando.OrientationLeft << " " << comando.SpeedLeft << " " << comando.OrientationRight << " " << comando.SpeedRight << std::endl;

            char command[15];
            sprintf(command, "<%d%d%03d%d%03d>", comando.ID, comando.OrientationLeft, comando.SpeedLeft, comando.OrientationRight, comando.SpeedRight);
            send(QString::fromUtf8(command));
        }
    }

}

void Serial::createSendNewCommands(robok::Fieldstate * fs)
{
    char command[15];
    char command0[10] = "0000", command1[10] = "0000", command2[10] = "0000";

    for (int i = 0; i < fs->getRobotsTeam().size(); ++i) {
        if(fs->getRobotTeamById(i).getPosition().x != -1){
            //std::cout << "entrou finalmente" << std::endl;
            robok::Robot robo = fs->getRobotTeamById(i);
            Command comando = robo.getCommand();
            std::cout << comando.ID << " " << comando.OrientationLeft << " " << comando.SpeedLeft << " " << comando.OrientationRight << " " << comando.SpeedRight << std::endl;

            comando.SpeedLeft = (comando.SpeedLeft*49)/100 ;
            if(comando.OrientationLeft == 1){
                comando.SpeedLeft += 50;
            }

            comando.SpeedRight = (comando.SpeedRight*49)/100 ;
            if(comando.OrientationRight == 1){
                comando.SpeedRight += 50;
            }

            if(comando.ID == 0)
                sprintf(command0, "%02d%02d", comando.SpeedLeft, comando.SpeedRight);
            if(comando.ID == 1)
                sprintf(command1, "%02d%02d", comando.SpeedLeft, comando.SpeedRight);
            if(comando.ID == 2)
                sprintf(command2, "%02d%02d", comando.SpeedLeft, comando.SpeedRight);
        }
    }
    sprintf(command, "<%s%s%s>", command0, command1, command2);
    std::cout << "comando novo: " << command << std::endl;
    send(QString::fromUtf8(command));
}


bool Serial::connect(QString name)
{
    serialPort.setPortName(name);
    serialPort.setBaudRate(9600);

    if (serialPort.open(QIODevice::WriteOnly)) {

        return true;
    }
    else {

        return false;
    }
}

bool Serial::send(QString command)
{
    std::cout << "comando " << command.toStdString() << std::endl;
    serialPort.write(command.toUtf8().data(), command.length());
}

QStringList Serial::scan()
{
    QList<QSerialPortInfo> list;
    list = QSerialPortInfo::availablePorts();
    QStringList stringList;
    for(int i = 0; i<list.length(); i++)
    {
        std::cout << list.at(i).portName().toStdString() << std::endl;
        stringList.push_back(list.at(i).portName());
    }
    return stringList;
}

bool Serial::isConnected(){
    return serialPort.isOpen();
}
