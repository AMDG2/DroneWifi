#include "arcommand.h"

arCommand::arCommand(QString host0, int port0, QObject *parent) :
    QObject(parent)
{
    host = new QHostAddress(host0);
    port = port0;
    ident = 0;

    connectToDrone();

    clk0 = new QTimer(this);
    connect(clk0, SIGNAL(timeout()), this, SLOT(end_clk0));
    //clk0->start(100);

}

bool arCommand::sendAT()
{
    ident++;
    int length = dataL.length() + dataR.length() + QString::number(ident).length();
    int tmp = udpSocket->writeDatagram(qPrintable(dataL + QString::number(ident) + dataR),
                                       (qint64)length, *host, (qint16)port);
    if(tmp == -1 || tmp != length)
        return false;
    return true;
}

void arCommand::changeData(QString dataLeft, QString dataRight)
{
    dataL = dataLeft;
    dataR = dataRight;
}

void arCommand::end_clk0()
{
    sendAT();
    emit newTrame(dataL + QString::number(ident) + dataR);
}

void arCommand::connectToDrone()
{
    udpSocket = new QUdpSocket(this);
}

void arCommand::initDrone()
{
    changeData("AT*CONFIG=", ",\"control:altitude_max\",\"2000\"\r");
    snedAT();

    changeData("AT*CONFIG=", ",\"general:navdata_demo\",\"TRUE\"\r");
    sendAT();

    changeData("AT*FTRIM=", ",\r");
    sendAT();
}
