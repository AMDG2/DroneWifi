#ifndef __ARDRONE__
#define __ARDRONE__
/**
	\defgroup libARDrone
	
	\defgroup Navdata
	\ingroup libARDrone
	
	\defgroup ATCommands
	\ingroup libARDrone
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
// Socket libraries
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

/**	\typedef joystick
	\ingroup libARDrone
	\brief Union permettant d'écrire des valeurs flottantes, et de lire des int ayant la même empreinte.
	\author Thibaut Marty
**/
typedef union
{
	float f;
	long l; /*!< Le type long est utilisé sur le rabbit */
	int i;
} joystick;

/** \typedef bool
	\ingroup libARDrone
	\brief Implémentation du type c++ bool en c.
	\author Baudouin Feildel
**/
#define true (1)
#define false (0)
typedef int bool;

/** \macro VOL_INTERIEUR
 *  \ingroup libARDrone
 *  \brief Configure la librairie pour le vol en intérieur.
 *  \author Baudouin Feildel
 */
#define VOL_INTERIEUR

/** \macro ALTITUDEMAX
 *  \ingroup libARDrone
 *  \brief Configure la hauteur maximale de vol en intérieur, la hauteur maximale de vol en extérieur est 100 fois celle-ci. Cette valeur est en millimètre.
 *  \author Baudouin Feildel
 */
#define ALTITUDEMAX "2000"

/** \typedef ardrone
	\ingroup libARDrone
	\brief Structure contenant les informations nécessaires au pilotage d'un drone.
	\author Baudouin Feildel
**/
typedef struct ardrone
{
	char buff[1025];				/*!< Buffer de Stockage de trame de commande */
	int udpSocket_at;				/*!< Handle de la socket d'envoi de commande AT */
	int udpSocket_navData;			/*!< Handle de la socket de récupération des données de navigation */
	struct sockaddr_in addrDroneAT;	/*!< Information sur le récepteur des trames de commandes AT */
	struct sockaddr_in addrDroneNavDataR;/*!< Information sur l'émetteur des trames de données de navigation */
	struct sockaddr_in addrDroneNavDataW; /*!< Information sur le récepteur des trames à destination du port navdata */
	struct hostent* hostInfo;		/*!< Adresse IP du drone */
	unsigned short int port_at;		/*!< Port de commande du drone */
	unsigned short int port_navdata;/*!< Port d'écoute des données de navigation */
	char bufferLeft[20];			/*!< Partie gauche du buffer */
	char bufferRight[50];			/*!< Partie droite du buffer */
	int ident;						/*!< Identifiant de la commande */
	bool fly;						/*!< Mode vol */
	joystick tiltFrontBack;			/*!< Buffer de la commande d'inclinaison avant arrière */
	joystick tiltLeftRight;			/*!< Buffer de la commande d'inclinaison gauche droite */
	joystick goUpDown;				/*!< Buffer de la commande de vitesse verticale */
	joystick turnLeftRight;			/*!< Buffer de la commande de vitesse angulaire */
} ardrone;

// Some typedef to be compatible with ardrone SDK Code
typedef float float32_t;

/**
 *	@typedef _navdata_option_t
 *	@ingroup Navdata
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
 * @ingroup Navdata
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
 * @ingroup Navdata
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


/** \fn bool connectToDrone(ardrone* dr)
	\ingroup libARDrone
	\brief Crée et initialise les sockets de communication avec le drone, ainsi que les structures qui identifient le drone pour la lecture et l'écriture sur le port des navdata (5554), et l'écriture sur le port des commandes AT (5556). Cette fonction est dépendante de l'environnement d'exécution.
	\param ardrone* dr : handle de drone
	\return true : Connexion réussie ; false : Connexion échouée
	\author Baudouin Feildel
**/
bool connectToDrone(ardrone* dr);

/** \fn bool initDrone(ardrone* dr)
	\ingroup libARDrone
	\brief Initialisation du drone dr, indique qu'il est à l'horizontale (AT*FTRIM), configure l'altitude max (3000 mm), indique au drone qu'il vole en intérieur ou extérieur suivant la configuration réalisée à l'aide d'une define (#define VOL_INTERIEUR) ou d'un bouton sur la télécommande. N.B. : Cette fonction retourne false dès que l'envoi d'une commande a échoué.
	\param ardrone* dr : Handle du drone
	\return true : initialisation réussie ; false : initialisation échouée
	\author Baudouin Feildel
**/
bool initDrone(ardrone* dr);

/** \fn bool initNavData(ardrone* dr)
	\ingroup Navdata
	\brief Initialise de la communication des données de navigation. Retourne false dès qu'une partie du protocole d'initialisation a échoué, pour plus de détails voir le déroulement de la fonction ci-dessous.
	\param ardrone* dr : Handle du drone
	\return true : initialisation réussie ; false : initialisation échouée
	\author Baudouin Feildel
**/
bool initNavData(ardrone* dr);

/** \fn bool takeoff(ardrone* dr)
	\ingroup ATCommands
	\brief Commande le décollage du drone dr.
	\param ardrone* dr : Handle du drone
	\return true : commande envoyée ; false : commande non-envoyée
	\author Baudouin Feildel
**/
bool takeoff(ardrone* dr);

/** \fn bool land(ardrone* dr)
	\ingroup ATCommands
	\brief Commande l'atterrissage du drone dr.
	\param ardrone* dr : Handle du drone
	\return true : commande envoyée ; false : commande non-envoyée
	\author Baudouin Feildel
**/
bool land(ardrone* dr);

/** \fn bool aru(ardrone* dr)
	\ingroup ATCommands
	\brief Envoie un arrêt d'urgence au drone dr.
	\param ardrone* dr : Handle du drone
	\return true : arrêt d'urgence envoyé ; false : arrêt d'urgence non-envoyé
	\author Baudouin Feildel
**/
bool aru(ardrone* dr);

/** \fn bool volCommand(ardrone* dr, joystick tiltLeftIrght_, joystick tiltFrontBack_, joystick goUpDown_, joystick turnLeftRight_)
	\ingroup ATCommands
	\brief Envoie de la commande AT*PCMD au drone dr. Cette commande indique au drone la vitesse angulaire, et la vitesse verticale qu'il doit adopter, et lui indique comment il doit s'incliner sur deux axes (gauche/droite et avant/arrière).
	\param ardrone* dr : Handle du drone
	\param joystick tiltLeftRight : commande l'inclinaison avant/arrière du drone
	\param joystick tiltFrontBack : commande l'inclinaison gauche/droite du drone
	\param joystick goUpDown : commande vitesse verticale du drone
	\param joystick turnLeftRight : commande vitesse angulaire du drone
	\return true : commande envoyée ; false : commande non-envoyée
	\author Baudouin Feildel
**/
bool volCommand(ardrone* dr, joystick tiltLeftRight_, joystick tiltFrontBack_, joystick goUpDown_, joystick turnLeftRight_);

/** \fn void setGoUpDown(ardrone* dr, float val)
	\ingroup ATCommands
	\brief Met une valeur de manière sécurisée dans le buffer de la vitesse verticale du drone dr. Ce buffer est une union qui permet de stocker des valeurs flottantes (membre f) et de les écrire en tant que int (utilisé dans l'envoi des commandes AT).
	\param ardrone* dr : Handle du drone
	\param float val : valeur à inserer
	\author Baudouin Feildel
**/
void setGoUpDown(ardrone* dr, float val);

/** \fn void setTurnLeftRight(ardrone* dr, float val)
	\ingroup ATCommands
	\brief Met une valeur de manière sécurisée dans le buffer de la vitesse angulaire du drone dr. Ce buffer est une union qui permet de stocker des valeurs flottantes (membre f) et de les écrire en tant que int (utilisé dans l'envoi des commandes AT).
	\param ardrone* dr: Handle du drone
	\param float val : valeur à insérer
	\author Baudouin Feildel
**/
void setTurnLeftRight(ardrone* dr, float val);

/** \fn void setTiltFrontBack(ardrone* dr, float val)
	\ingroup ATCommands
	\brief Met une valeur de manière sécurisée dans le buffer de l'inclinaison avant/arrière du drone dr. Ce buffer est une union qui permet de stocker des valeurs flottantes (membre f) et de les écrire en tant que int (utilisé dans l'envoi des commandes AT).
	\param ardrone* dr : Handle du drone
	\param float val : valeur à insérer
	\author Baudouin Feildel
**/
void setTiltFrontBack(ardrone* dr, float val);

/** \fn void setTiltLeftRight(ardrone* dr, float val)
	\ingroup ATCommands
	\brief Met une valeur de manière sécurisée dans le buffer de l'inclinaison gauche/droite du drone dr. Ce buffer est une union qui permet de stocker des valeurs flottantes (membre f) et de les écrire en tant que int (utilisé dans l'envoi des commandes AT).
	\param ardrone* dr : Handle du drone
	\param float val : valeur à insérer
	\author Baudouin Feildel
**/
void setTiltLeftRight(ardrone* dr, float val);

/** \fn bool sendAT(ardrone* dr)
	\ingroup ATCommands
	\brief Envoie la commande AT qui est dans le buffer du drone dr. Le buffer est construit à l'aide de bufferLeft et de bufferRight, l'identifiant de commande est inséré entre ces deux buffers. N.B. : Cette fonction incrémente l'identifiant de commande à chaque appel.
	\param ardrone* dr : Handle du drone
	\return true : buffer envoyé ; false buffer non-envoyé
	\author Baudouin Feildel
**/
bool sendAT(ardrone* dr);

/** \fn bool sendNavData(ardrone* dr, const char* buffer)
	\ingroup Navdata
	\brief Envoie le buffer au drone sur le port de données de navigation.
	\param ardrone* dr : Handle du drone
	\param const char* buffer : Trame à envoyer.
	\return true : buffer envoyé ; false buffer non-envoyé
	\author Baudouin Feildel
**/
bool sendNavData(ardrone* dr, const char* buffer);

/** \fn int receiveNavData(ardrone* dr, struct sockaddr_in* sender, socklen_t* senderLen, char *buffer, int bufferLenght)
	\ingroup Navdata
	\brief Réceptionne une trame de données de navigation venant du drone dr. Cette trame est stockée dans le buffer. Les informations concernant l'expéditeur sont stockées dans la structure sender.
	\param ardrone* dr : Handle du drone
	\param struct sockaddr_in* sender : structure recevant les informations concernant l'expéditeur
	\param socklen_t* senderLen : taille de la structure précédente, généralement "sizeof(sender)"
	\param char* buffer : buffer de réception
	\param int bufferLenght : taille du buffer
	\return Nombre d'octets reçus.
	\author Baudouin Feildel
**/
int receiveNavData(ardrone* dr, struct sockaddr_in* sender, socklen_t* senderLen, char *buffer, int bufferLenght);

/** \fn ardrone* newARDrone(void)
	\ingroup libARDrone
	\brief Constructeur de l'objet ardrone. Initialise l'adresse IP du drone, les ports de communication, l'identifiant de commande, désactive le mode de vol. N.B. : Cette fonction est dépendante de l'environnement d'exécution et le langage utilisé.
	\return Handle sur drone
	\author Baudouin Feildel
**/
ardrone* newARDrone(void);

/** \fn void closeARDrone(ardrone* dr)
	\ingroup libARDrone
	\brief Destructeur de l'objet ardrone. Ferme les sockets, et libère la mémoire allouée dynamiquement.
	\param ardrone* dr : Handle du objet drone
	\author Baudouin Feildel
**/
void closeARDrone(ardrone* dr);

#endif
