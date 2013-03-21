#ifndef ARDRONE_H
#define ARDRONE_H

/**
  @defgroup libARDrone

  @defgroup Navdata
  @ingroup libARDrone

  @defgroup ATCommands
  @ingroup libARDrone
  */

#include <QThread>
#include <QtNetwork/QUdpSocket>


// Some typedef to be compatible with ardrone SDK Code
typedef float float32_t;
typedef quint8 uint8_t;
typedef quint16 uint16_t;
typedef quint32 uint32_t;

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

typedef struct _navdata_t {
    uint32_t    header;			/*!< Always set to NAVDATA_HEADER */
    uint32_t    ardrone_state;    /*!< Bit mask built from def_ardrone_state_mask_t */
    uint32_t    sequence;         /*!< Sequence number, incremented for each sent packet */
    int      vision_defined;

    navdata_option_t  options[1];
}/*_ATTRIBUTE_PACKED_*/ navdata_t;

/*----------------------------------------------------------------------------*/
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
    void getNavData();

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
    float tiltFrontBack;
    float tiltLeftRight;
    float goUpDown;
    float turnLeftRight;
    _navdata_demo_t navdata;
    _navdata_t infoDrone;

    bool sendAT(void);
    int receiveNavData(QHostAddress* sender, quint16 *senderPort, char* buffer, qint64 bufferLenght);
    void run(void);

signals:

public slots:

};

#endif // ARDRONE_H
