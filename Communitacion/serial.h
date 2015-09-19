#ifndef SERIAL_H
#define SERIAL_H

#include <string>
#include <QtSerialPort/QtSerialPort>
#include "fieldstate.h"

class Serial
{
private:
    QSerialPort serialPort;
    QSerialPortInfo info;
    QString finalCommand;

public:
    Serial();
    ~Serial();
    QStringList scan();
    QString createCommand(int id, int senEsq, int velEsq, int senDir, int velDir);
    void createSendNewCommands(robok::Fieldstate * fs);
    bool connect(QString name);
    bool send(QString command);
    void createSendCommands(robok::Fieldstate * fs);
    bool isConnected();
};

#endif // COMMAND_H
