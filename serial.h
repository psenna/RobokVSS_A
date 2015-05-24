#ifndef SERIAL_H
#define SERIAL_H

#include <string>
#include <QtSerialPort/QtSerialPort>

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
    bool connect(QString name);
    bool send(QString command);
};

#endif // COMMAND_H
