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
    char comando[15];
    sprintf(comando, "<%d%d%03d%d%03d>", id, senEsq, velEsq, senDir, velDir);
    return QString::fromUtf8(comando);

}

bool Serial::connect(QString name)
{
    serialPort.setPortName(name);
    serialPort.setBaudRate(9600);
    if(serialPort.open(QIODevice::WriteOnly)){
        return true;
    }
    else{
        return false;
    }
}

bool Serial::send(QString command)
{
    serialPort.write(command.toUtf8().data(), command.length());
}

QStringList Serial::scan(){
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
