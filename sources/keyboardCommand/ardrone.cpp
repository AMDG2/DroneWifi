#include "ardrone.h"

ARDrone::ARDrone(QObject *parent) :
    QThread(parent)
{
    // Configuration de l'adresse IP du Drone et des ports de communications
    host = new QHostAddress("192.168.1.1");
    port_at = 5556;
    port_navdata = 5554;

    ident = 0;
    fly = false;
}

bool ARDrone::connectToDrone()
{
    // Création de la socket d'envoi des commande AT
    udpSocket_at = new QUdpSocket(this);

    // Création de la socket de réception des données
    udpSocket_navdata =  new QUdpSocket(this);
    // Écoute sur le port des données de navigation
    udpSocket_navdata->bind(port_navdata);

    return true;
}

bool ARDrone::sendAT(void)
{
    ident++;
    int length = bufferLeft.length() + bufferRight.length() + QString::number(ident).length();
    int tmp = length;
    tmp = udpSocket_at->writeDatagram(qPrintable(bufferLeft + QString::number(ident) + bufferRight),
                                       (qint64)length, *host, (qint16)port_at);
    if(tmp == -1 || tmp != length)
        return false;
    return true;
}

bool ARDrone::initDrone(void)
{
    // Configure la hauteur maximale du drone
    bufferLeft = "AT*CONFIG=";
    bufferRight = ",\"control:altitude_max\",\"3000\"\r";
    qDebug("#%5d - Config alt max - %s%d%s", ident+1, qPrintable(bufferLeft), ident+1, qPrintable(bufferRight));
    sendAT();

    // Indique au drone qu'il est à plat : le drone se calibre
    bufferLeft = "AT*FTRIM=";
    bufferRight = ",\r";
    qDebug("#%5d - Ftrim - %s%d%s", ident+1, qPrintable(bufferLeft), ident+1, qPrintable(bufferRight));

    return sendAT();
}

bool ARDrone::initNavData()
{
    // Send a packet to navdata port
    qDebug("Send 'Hello World !'\n");
    udpSocket_navdata->writeDatagram("Hello World !", 13, *host, (qint16)port_navdata);

    // Demande l'envoi de donnée de navigation
    bufferLeft = "AT*CONFIG=";
    bufferRight = ",\"general:navdata_demo\",\"FALSE\"\r";
    qDebug("#%5d - Ask for navdata flow - %s%d%s", ident+1, qPrintable(bufferLeft), ident+1, qPrintable(bufferRight));
    if(!sendAT())
        return false;

    // Démarre l'envoi de donnée de navigation
    udpSocket_at->writeDatagram("AT*CTRL=0\r", 10, *host, (qint16)port_at);

    // Demande l'envoi des données en MultiCast
    char un[14] = {'1','0','0','0','0','0','0','0','0','0','0','0','0','0'};
    udpSocket_navdata->writeDatagram(un, 14, *host, (qint16)port_navdata);

    return true;
}

bool ARDrone::takeoff(void)
{
    // Décollage du drone
    bufferLeft = "AT*REF=";
    bufferRight = ",290718208\r";
    qDebug("#%5d - Take off - %s%d%s", ident+1, qPrintable(bufferLeft), ident+1, qPrintable(bufferRight));
    sendAT();

    // Passe en mode vol
    bufferLeft = "AT*PCMD=";
    bufferRight = ",1,0,0,0,0\r";
    setGoUpDown(0);
    setTurnLeftRight(0);
    setTiltFrontBack(0);
    setTiltLeftRight(0);
    fly = true;
    this->start();

    return true;
}

bool ARDrone::land(void)
{
    fly = false;
    bufferLeft = "AT*REF=";
    bufferRight = ",290717696\r";
    qDebug("#%5d - Land - %s%d%s", ident+1, qPrintable(bufferLeft), ident+1, qPrintable(bufferRight));

    return sendAT();
}

bool ARDrone::aru(void)
{
    fly = false;
    bufferLeft = "AT*REF=";
    bufferRight = ",290717952\r";
    qDebug("#%5d - Arrêt d'urgence - %s%d%s", ident+1, qPrintable(bufferLeft), ident+1, qPrintable(bufferRight));

    return sendAT();
}

bool ARDrone::volCommand(float tiltLeftRight_, float tiltFrontBack_, float goUpDown_, float turnLeftRight_)
{
    char cStrBuff[50];
    sprintf(cStrBuff, ",1,%d,%d,%d,%d\r", *(int*)(&tiltLeftRight_),
                                        *(int*)(&tiltFrontBack_),
                                        *(int*)(&goUpDown_),
                                        *(int*)(&turnLeftRight_));
    QString strBuff;
    strBuff.operator =(cStrBuff);
    bufferLeft =  "AT*PCMD=";
    bufferRight = strBuff;
    qDebug("#%5d - PCMD - %s%d%s", ident+1, qPrintable(bufferLeft), ident+1, qPrintable(bufferRight));
    return sendAT();
}

void ARDrone::run(void)
{
    while(fly)
    {
        volCommand(tiltLeftRight, tiltFrontBack, goUpDown, turnLeftRight);
        msleep(30);
    }
}

int ARDrone::receiveNavData(QHostAddress *sender, qint64 senderPort, char *buffer, qint64 bufferLenght)
{
    int ret = 0;

    // Réveiller le watchdog pour demander l'envoi de données
    bufferLeft = "AT*COMWDG=";
    bufferRight = ",\r";
    if(!sendAT())
    {
        qDebug("##     - Unable to reset the Watchdog.\n");
        return -1;
    }

    ret = udpSocket_navdata->readDatagram(buffer, bufferLenght, sender, senderPort);
    if(ret <= 0)
        qDebug("Impossible de réceptionner les données : %d\n", ret);
    return ret;
}
