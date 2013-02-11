#ifndef ARCOMMAND_H
#define ARCOMMAND_H

#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <QThread>
#include <QTimer>
#include <QObject>
#include <QMutex>

class arCommand : public QThread
{
    Q_OBJECT
public:
    arCommand(QString host0, int port0, QObject *parent);
    void changeData(QString dataLeft, QString dataRight);
    void connectToDrone(void);
    void initDrone();
    bool sendAT();
    void setARU(bool value = true) { aru = value; }

protected:
    void run();

private:
    QUdpSocket *udpSocket;
    QHostAddress *host;
    int port;
    int ident;
    QString dataL;
    QString dataR;
    QMutex dataMtx;
    bool aru;

private slots:
    void end_clk0();

signals:
    void newTrame(QString /* data */);
};

#endif // ARCOMMAND_H
