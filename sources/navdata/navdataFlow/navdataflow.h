#ifndef NAVDATAFLOW_H
#define NAVDATAFLOW_H

#include <QObject>
#include <QtNetwork/QUdpSocket>

class navdataflow : public QObject
{
    Q_OBJECT
public:
    explicit navdataflow(QObject *parent = 0);
    bool connectToDrone(void);
    bool initNavData(void);

private:
    QUdpSocket *udpSocket_at;
    QUdpSocket *udpSocket_navdata;
    QHostAddress *host;
    int port_at;
    int port_navdata;
    QString bufferLeft;
    QString bufferRight;
    int ident;
    bool sendAT(void);

signals:

public slots:

};

#endif // NAVDATAFLOW_H
