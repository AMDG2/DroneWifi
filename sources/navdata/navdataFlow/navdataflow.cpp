#include "navdataflow.h"

navdataflow::navdataflow(QObject *parent) :
    QObject(parent)
{
    // Configuration de l'adresse IP du Drone et des ports de communications
    host = new QHostAddress("192.168.1.1");
    port_navdata = 5554;
}

bool navdataflow::connectToDrone()
{
    // Création de la socket d'envoi des commande AT
    udpSocket_at = new QUdpSocket(this);

    return true;
}

bool navdataflow::sendAT(void)
{
    ident++;
    int length = bufferLeft.length() + bufferRight.length() + QString::number(ident).length();
    int tmp = udpSocket_at->writeDatagram(qPrintable(bufferLeft + QString::number(ident) + bufferRight),
                                       (qint64)length, *host, (qint16)port_at);
    if(tmp == -1 || tmp != length)
        return false;
    return true;
}

bool navdataflow::initNavData()
{

    // Création de la socket de réception des données
    udpSocket_navdata =  new QUdpSocket(this);
    // Écoute sur le port des données de navigation
    udpSocket_navdata->bind(port_navdata);

    // Send a packet to navdata port
    udpSocket_navdata->writeDatagram("\r", 1, *host, (qint16)port_navdata);
    qint64 tmp;
    char buffer[500];
    //while(-1 == udpSocket_navdata->pendinwgDatagramSize());
    tmp = udpSocket_navdata->pendingDatagramSize();
    udpSocket_navdata->readDatagram(buffer, tmp);
    qDebug(buffer);

    // Demande l'envoi de donnée de navigation
    bufferLeft = "AT*CONFIG=";
    bufferRight = ",\"general:navdata_demo\",\"TRUE\"\r";
    qDebug("#%5d - Ask for navdata flow - %s%d%s", ident+1, qPrintable(bufferLeft), ident+1, qPrintable(bufferRight));
    sendAT();

    // Démarre l'envoi de donnée de navigation
    udpSocket_at->writeDatagram("AT*CTRL=0\r", 10, *host, (qint16)port_at);

    return true;
}
