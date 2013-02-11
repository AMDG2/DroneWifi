#include "arcommand.h"

arCommand::arCommand(QString host0, int port0, QObject *parent) :
    QThread(parent)
{
    host = new QHostAddress(host0);
    port = port0;
    ident = 0;

    connectToDrone();

    aru = true;

}

bool arCommand::sendAT()
{
    dataMtx.lock();
    ident++;
    int length = dataL.length() + dataR.length() + QString::number(ident).length();
    int tmp = udpSocket->writeDatagram(qPrintable(dataL + QString::number(ident) + dataR),
                                       (qint64)length, *host, (qint16)port);
    dataMtx.unlock();
    if(tmp == -1 || tmp != length)
        return false;
    return true;
}

void arCommand::changeData(QString dataLeft, QString dataRight)
{
    dataMtx.lock();
    dataL = dataLeft;
    dataR = dataRight;
    dataMtx.unlock();
}

void arCommand::connectToDrone()
{
    udpSocket = new QUdpSocket(this);
}

void arCommand::initDrone()
{
    changeData("AT*CONFIG=", ",\"control:altitude_max\",\"2000\"\r");
    sendAT();

    changeData("AT*CONFIG=", ",\"general:navdata_demo\",\"TRUE\"\r");
    sendAT();

    changeData("AT*FTRIM=", ",\r");
    sendAT();
}

void arCommand::run()
{
    while(!aru)
    {
        sendAT();
        // wait 100ms
        msleep(100);
    }
}
