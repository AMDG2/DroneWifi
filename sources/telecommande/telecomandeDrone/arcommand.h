#ifndef ARCOMMAND_H
#define ARCOMMAND_H

#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <QThread>
#include <QTimer>
#include <QObject>

class arCommand : public QObject
{
    Q_OBJECT
public:
    arCommand(QString host0, int port0, QObject *parent);
    void changeData(QString dataLeft, QString dataRight);
    void connectToDrone(void);
    void initDrone();
    bool sendAT();

private:
    QUdpSocket *udpSocket;
    QHostAddress *host;
    int port;
    int ident;
    QString dataL;
    QString dataR;
    QTimer *clk0;

private slots:
    void end_clk0();

signals:
    void newTrame(QString /* data */);
};

#endif // ARCOMMAND_H
