#ifndef ARDRONE_H
#define ARDRONE_H

#include <QThread>
#include <QtNetwork/QUdpSocket>



class ARDrone : public QThread
{
    Q_OBJECT
public:
    explicit ARDrone(QObject *parent = 0);
    bool connectToDrone(void);
    bool initDrone(void);
    bool initNavData(void);
    bool takeoff(void);
    bool land(void);
    bool aru(void);
    bool volCommand(float tiltLeftRight_, float tiltFrontBack_, float goUpDown_, float turnLeftRight_);

    void setGoUpDown(float val) { goUpDown = (val <= 1 && val >= -1) ? val:0; }
    void setTurnLeftRight(float val) { turnLeftRight = (val <= 1 && val >= -1) ? val:0; }
    void setTiltFrontBack(float val) { tiltFrontBack = (val <= 1 && val >= -1) ? val:0; }
    void setTiltLeftRight(float val) { tiltLeftRight = (val <= 1 && val >= -1) ? val:0; }

private:
    QUdpSocket *udpSocket_at;
    QUdpSocket *udpSocket_navdata;
    QHostAddress *host;
    int port_at;
    int port_navdata;
    QString bufferLeft;
    QString bufferRight;
    int ident;
    bool fly;
    bool sendAT(void);
    float tiltFrontBack;
    float tiltLeftRight;
    float goUpDown;
    float turnLeftRight;
    void run(void);

signals:

public slots:

};

#endif // ARDRONE_H
