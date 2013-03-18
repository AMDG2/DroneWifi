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

    /**
     * @fn int receiveNavData()
     * @brief Reçoit les données de navigation et remplie le buffer de réception.
     * @param QHostAddress* sender : Adresse de l'expéditeur
     * @param qint64* senderPort : port de l'expéditeur
     * @param char* buffer : Buffer de stockage de la trame reçue
     * @param bufferLenght : Taille du buffer
     * @return Nombre de bytes reçu
     */
    int receiveNavData(QHostAddress* sender, qint64* senderPort, char* buffer, qint64 bufferLenght);

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
    void updateNavData(char *data, )

public slots:

};

// Some typedef to be compatible with ardrone SDK Code
typedef float float32_t;

/**
 *	@typedef _navdata_option_t
 *	@ingroup ardrone Navdata
 *	@brief Données de navigation
 *	@author Parrot
 */
typedef struct _navdata_option_t {
    uint16_t  tag;
    uint16_t  size;
    #if defined _MSC_VER || defined (__ARMCC_VERSION)
        /* Do not use flexible arrays (C99 feature) with these compilers */
        uint8_t   data[1];
    #else
        uint8_t   data[];
    #endif
} navdata_option_t;

/**
 * @typedef _navdata_t
 * @ingroup ardrone Navdata
 * @brief Navdata structure sent over the network.
 * @author Parrot
 */
typedef struct _navdata_t {
    uint32_t    header;			/*!< Always set to NAVDATA_HEADER */
    uint32_t    ardrone_state;    /*!< Bit mask built from def_ardrone_state_mask_t */
    uint32_t    sequence;         /*!< Sequence number, incremented for each sent packet */
    bool      vision_defined;

    navdata_option_t  options[1];
}/*_ATTRIBUTE_PACKED_*/ navdata_t;

/*----------------------------------------------------------------------------*/
/**
 * @typedef _navdata_demo_t
 * @ingroup ardrone Navdata
 * @brief Minimal navigation data for all flights.
 * @author Parrot
 */
typedef struct _navdata_demo_t {
    uint16_t    tag;					  /*!< Navdata block ('option') identifier */
    uint16_t    size;					  /*!< set this to the size of this structure */

    uint32_t    ctrl_state;             /*!< Flying state (landed, flying, hovering, etc.) defined in CTRL_STATES enum. */
    uint32_t    vbat_flying_percentage; /*!< battery voltage filtered (mV) */

    float32_t   theta;                  /*!< UAV's pitch in milli-degrees */
    float32_t   phi;                    /*!< UAV's roll  in milli-degrees */
    float32_t   psi;                    /*!< UAV's yaw   in milli-degrees */

    int32_t     altitude;               /*!< UAV's altitude in centimeters */

    float32_t   vx;                     /*!< UAV's estimated linear velocity */
    float32_t   vy;                     /*!< UAV's estimated linear velocity */
    float32_t   vz;                     /*!< UAV's estimated linear velocity */

    uint32_t    num_frames;			  /*!< streamed frame index */ // Not used -> To integrate in video stage.

    // Camera parameters compute by detection
    //  matrix33_t  detection_camera_rot;   /*!<  Deprecated ! Don't use ! */
    //  vector31_t  detection_camera_trans; /*!<  Deprecated ! Don't use ! */
    //  uint32_t	  detection_tag_index;    /*!<  Deprecated ! Don't use ! */

    //  uint32_t	  detection_camera_type;  /*!<  Type of tag searched in detection */

    //  // Camera parameters compute by drone
    //  matrix33_t  drone_camera_rot;		  /*!<  Deprecated ! Don't use ! */
    //  vector31_t  drone_camera_trans;	  /*!<  Deprecated ! Don't use ! */
}/*_ATTRIBUTE_PACKED_*/ navdata_demo_t;

#endif // ARDRONE_H
