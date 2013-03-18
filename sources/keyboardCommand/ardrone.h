#ifndef ARDRONE_H
#define ARDRONE_H

#include <QThread>
#include <QtNetwork/QUdpSocket>



class ARDrone : public QThread
{
    Q_OBJECT
public:
    explicit ARDrone(QObject *parent = 0);

    /**
     * @fn bool connectToDrone(void)
     * @brief Initialise la connexion au drone
     * @return (bool) true : Connexion réussie ; false : Connexion échouée
     * @author Baudouin Feildel
     */
    bool connectToDrone(void);

    /**
     * @fn bool initDrone(void)
     * @brief Initialise le drone (à utiliser avant de lancer la navigation)
     * @return (bool) true : Initialisation réussie ; false : Initialisation échouée
     * @author Baudouin Feildel
     */
    bool initDrone(void);

    /**
     * @fn bool initNavData(void)
     * @brief Initialise le fux de données
     * @return (bool) true : Initialisation réussie ; false : Initialisation échouée
     * @author Baudouin Feildel
     */
    bool initNavData(void);

    /**
     * @fn bool takeoff(void)
     * @brief Demande de décollage
     * @return (bool) true : demande envoyée ; false : demande non-envoyée
     * @author Baudouin Feildel
     */
    bool takeoff(void);

    /**
     * @fn bool land(void)
     * @brief Demande d'atterrissage
     * @return (bool) true : demande envoyée ; false : demande non-envoyée
     */
    bool land(void);

    /**
     * @fn bool aru(void)
     * @brief Demande d'arrêt d'urgence
     * @return (bool) true : demande envoyée ; false : demande non-envoyée
     */
    bool aru(void);

    /**
     * @fn bool volCommand(float tiltLeftRight_, float tiltFrontBack_, float goUpDown_, float turnLeftRight_)
     * @brief Pilotage du drone en vol
     * @param float tiltLeftRight_ Inclinaison gauche droite ; [-1,1]
     * @param float tiltFrontBack_ Inclinaison avant arrière ; [-1;1]
     * @param float goUpDown_ Vitesse verticale ; [-1;1]
     * @param float turnLeftRight_ Vitesse angulaire ; [-1;1]
     * @return (bool) true : commande envoyée ; false : commande non-envoyée
     * @author Baudouin Feildel
     */
    bool volCommand(float tiltLeftRight_, float tiltFrontBack_, float goUpDown_, float turnLeftRight_);

    /**
     * @fn void setGoUpDown(float val)
     * @brief Setter sécurisé de la valeur de la vitesse verticale
     * @param float val : valeur à écrire
     * @author Baudouin Feildel
     */
    void setGoUpDown(float val) { goUpDown = (val <= 1 && val >= -1) ? val:0; }

    /**
     * @fn void setTurnLeftRight(float val)
     * @brief Setter sécurisé de la valeur de la vitesse angulaire
     * @param float val : valeur à écrire
     * @author Baudouin Feildel
     */
    void setTurnLeftRight(float val) { turnLeftRight = (val <= 1 && val >= -1) ? val:0; }

    /**
     * @fn void setTiltFrontBack(float val)
     * @brief Setter sécurisé de la valeur de l'inclinaison avant arrière
     * @param float val : valeur à écrire
     * @author Baudouin Feildel
     */
    void setTiltFrontBack(float val) { tiltFrontBack = (val <= 1 && val >= -1) ? val:0; }

    /**
     * @fn void setTiltLeftRight(float val)
     * @brief Setter sécurisé de la valeur de l'inclinaison gauche droite
     * @param float val : valeur à écrire
     * @author Baudouin Feildel
     */
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

    /**
     * @fn bool sendAT(void)
     * @brief Envoyer une commande AT
     * @return (bool) true : commande envoyée ; false : commande non-envoyée
     */
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
