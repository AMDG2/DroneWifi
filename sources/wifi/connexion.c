﻿/**
	\defgroup Connexion
	\defgroup UDP
	\defgroup IO
	\defgroup ATCommands
**/


/* what local UDP port to use - packets we send come from here */
#define LOCAL_PORT	5556

/*
 * Where to send the heartbeats. If it is set to all '255's,
 * it will be a broadcast packet.
 */
//#define REMOTE_IP			"192.168.1.1"
#define REMOTE_IP			"255.255.255.255" /*broadcast*/

/* the destination port to send to */
#define REMOTE_PORT	LOCAL_PORT

/* ******************************** */
/*
 * NETWORK CONFIGURATION
 * Please see the function help (Ctrl-H) on TCPCONFIG for instructions on
 * compile-time network configuration.
 */

#define TCPCONFIG 5

//#define printf

#use "dcrtcp.lib"

#use "rcm56xxW.lib"

#define IPDRONE "192.168.1.1"

#memmap xmem


/** \typedef statut_connexion Permet de définir le statut de la connexion en cours
	\ingroup Connexion
	\author Thibaut Marty
**/
typedef enum
{
	INOCCUPE,
	RECHERCHE,
	TROUVE,
	CONNECTE
} statut_connexion;

/** \typedef etat_commandes
	\ingroup IO
	\author Thibaut Marty
**/
typedef struct
{
	/* accessibles en écriture */
	char led_connecte;	// 1 : allumée
	char led_erreur;
	char led_debug;
	/* accessibles en lecture */
	char bp_arret_urgence;	// 1 : actif
	char bp_video;
	char bp_trim;
	char switch_land;	// 1 : position bas

	int joystick_1x;
	int joystick_1y;
	int joystick_2x;
	int joystick_2y;
} etat_commandes;

/** \typedef cppbool
	\ingroup ATCommands
	\brief Implementation du type c++ bool en c
	\author Baudouin Feildel
**/
#define true (1)
#define false (0)
#define cppbool char

/**	\var connect variable globale qui défini le statut de la connexion en cours
	\ingroup Connexion
	\author Thibaut Marty
**/
statut_connexion connect = INOCCUPE;

/**	\fn void BRDInit(void)
	\ingroup IO
	\brief Initialise tous les ports d'entreés sorties pour la carte joystick
	\author Thibaut Marty
**/
void BRDInit(void);

/**	\fn void print_etat_commandes(etat_commandes s)
	\ingroup IO
	\brief initialise la structure structure de l'état des commandes
	\param etat_commandes structure de l'état des commandes
	\author Thibaut Marty
**/
void print_etat_commandes(etat_commandes s);

/**	\fn void init_etat_commandes(etat_commandes *s)
	\ingroup IO
	\brief initialise la structure structure de l'état des commandes
	\param etat_commandes structure de l'état des commandes
	\author Thibaut Marty
**/
void init_etat_commandes(etat_commandes *s);

/**	\fn void SPI_CLK(char v)
	\ingroup IO
	\brief modifie l'état de la sortie CLK pour le convertisseur A/N SPI
	\param v 1 : +3.3V ; 0 : 0V
	\author Thibaut Marty
**/
void SPI_CLK(char v);

/**	\fn void SPI_DIN(char v)
	\ingroup IO
	\brief modifie l'état de la sortie DIN pour le convertisseur A/N SPI
	\param v 1 : +3.3V ; 0 : 0V
	\author Thibaut Marty
**/
void SPI_DIN(char v);

/**	\fn void SPI_CS(char v)
	\ingroup IO
	\brief modifie l'état de la sortie CS pour le convertisseur A/N SPI
	\param v 1 : +3.3V ; 0 : 0V
	\author Thibaut Marty
**/
void SPI_CS(char v);

/**	\fn int SPI_DOUT(void)
	\ingroup IO
	\brief lit l'état de l'entrée DOUT du convertisseur A/N SPI
	\return 1 : +3.3V ; 0 : 0V
	\author Thibaut Marty
**/
int SPI_DOUT(void);

/**	\fn cWAIT_5_us
	\ingroup IO
	\brief pause de 5 µs
	\author Dynamic C
**/
#define cWAIT_5_us asm ld a,3 $\
             sub 3 $\
             ld b,a $\
             db 0x10,-2

/**	\fn void SPI_delay(int i)
	\ingroup IO
	\brief temps d'attente de 'i' * 5 µs
	\param i : nombre d'attente de 5 µs
	\author Thibaut Marty
**/
void SPI_delay(int i);

/**	\fn int SPIread(char addr)
	\ingroup IO
	\brief demande au convertisseur A/N SPI de convertir le canal addr
	\param addr canal de lecture (0 à 3)
	\return le résultat de la conversion
	\author Thibaut Marty
**/
int SPIread(char addr);

/**	\fn void lireCommandes(etat_commandes *s)
	\ingroup IO
	\brief remplie la structure de l'état des commandes avec de nouvelles valeurs lues
	\param etat_commandes structure de l'état des commandes
	\author Thibaut Marty
**/
void lireCommandes(etat_commandes *s);

/**	\fn void ecrireCommandes(etat_commandes s)
	\ingroup IO
	\brief écris les valeurs de la structure de l'état des commandes en sortie
	\param etat_commandes structure de l'état des commandes
	\author Thibaut Marty
**/
void ecrireCommandes(etat_commandes s);

/**	\fn cppbool send_packet(char* str, const char far * ip, word port, udp_Socket *sock)
	\ingroup UDP
	\brief envoie une chaîne de caractère
	\param str chaîne à envoyé
	\param ip ip destination
	\param port port de l'envoi
	\param sock pointeur sur udp_Socket utilisée
	\return true : paquet envoyé ; false : erreur, la socket est fermée et ne se reouvre pas
	\author Thibaut Marty
**/
cppbool send_packet(char* str, const char far * ip, word port, udp_Socket *sock);

/**	\fn int rxsignal_cmp(far _wifi_wln_scan_bss *a, far _wifi_wln_scan_bss *b)
	\ingroup Connexion
	\brief fonction qui permet le trie les points d'accès WiFi par puissance du signal
	\param a un _wifi_wln_scan_bss qui correspond à un point d'accès
	\param b un _wifi_wln_scan_bss qui correspond à un point d'accès
	\author Dynamic C
**/
int rxsignal_cmp(far _wifi_wln_scan_bss *a, far _wifi_wln_scan_bss *b);

/**	\fn root void scan_assoc_callback(far wifi_scan_data* data)
	\ingroup Connexion
	\brief fonction de callback qui traite les points d'accès donnés :
		recherche le point d'accès du drone et se connecte
	\param data liste des points d'accès
	\author Thibaut Marty
**/
root void scan_assoc_callback(far wifi_scan_data* data);

/**	\fn void connexion(void)
	\ingroup Connexion
	\brief fonction qui se connecte au drone. Elle se quitte uniquement si la connexion est effective
	\author Thibaut Marty
**/
void connexion(void);

/** \typedef ardrone
	\ingroup ATCommands
	\brief Structure contenant les informations nécéssaires à la commande d'un drone
	\author Baudouin Feildel
**/
typedef struct
{
	char buff[1025];				// Buffer de Stockage de trame
	udp_Socket udpSocket_at;		// Handle de la socket d'envoi de commande AT
	int port_at;					// Port de commande du drone
	char bufferLeft[20];			// Partie gauche du buffer
	char bufferRight[50];			// Partie droite du buffer
	int ident;						// Identifiant de la commande
	cppbool fly;						// Mode vol
	float tiltFrontBack;			// Buffer de la commande d'inclinaison avant arrière
	float tiltLeftRight;			// Buffer de la commande d'inclinaison gauche droite
	float goUpDown;					// Buffer de la commande de vitesse verticale
	float turnLeftRight;			// Buffer de la commande de vitesse angulaire
} ardrone;

/** \fn cppbool connectToDrone(far ardrone* dr)
	\ingroup ATCommands
	\brief Initie la connexion avec le drone dr
	\param far ardrone* dr : handle de drone
	\return true : Connexion réussie ; false : Connexion échouée
	\author Baudouin Feildel
**/
cppbool connectToDrone(far ardrone* dr);

/** \fn cppbool initDrone(far ardrone* dr)
	\ingroup ATCommands
	\brief Initialisation du drone dr pour permettre un décollage en toute sécurité.
	\param far ardrone* dr : Handle du drone
	\return true : initialisation réussie ; false : initialisation échouée
	\author Baudouin Feildel
**/
cppbool initDrone(far ardrone* dr);

/** \fn cppbool takeoff(far ardrone* dr)
	\ingroup ATCommands
	\brief Commande le décollage du drone dr
	\param far ardrone* dr : Handle du drone
	\return true : commande envoyée ; false : commande non-envoyée
	\author Baudouin Feildel
**/
cppbool takeoff(far ardrone* dr);

/** \fn cppbool land(far ardrone* dr)
	\ingroup ATCommands
	\brief Commande l'atterissage du drone dr
	\param far ardrone* dr : Handle du drone
	\return true : commande envoyée ; false : commande non-envoyée
	\author Baudouin Feildel
**/
cppbool land(far ardrone* dr);

/** \fn cppbool aru(far ardrone* dr)
	\ingroup ATCommands
	\brief Envoi un arrêt d'urgence au drone dr
	\param far ardrone* dr : Handle du drone
	\return true : arrêt d'urgence envoyé ; false : arrêt d'urgence non-envoyé
	\author Baudouin Feildel
**/
cppbool aru(far ardrone* dr);

/** \fn cppbool volCommand(far ardrone* dr, float tiltLeftIrght_, float tiltFrontBack_, float goUpDown_, float turnLeftRight_)
	\ingroup ATCommands
	\brief Envoi de la commande de vol au drone dr
	\param far ardrone* dr : Handle du drone
	\param float tiltLeftRight : commande l'inclinaison avant/arrière du drone
	\param float tiltFrontBack : commande l'inclinaison gauche/droite du drone
	\param float goUpDown : commande vitesse verticale du drone
	\param float turnLeftRight : commande vitesse angulaire du drone
	\return true : commande envoyée ; false : commande non-envoyée
	\author Baudouin Feildel
**/
cppbool volCommand(far ardrone* dr, float tiltLeftRight_, float tiltFrontBack_, float goUpDown_, float turnLeftRight_);

/** \fn void setGoUpDown(far ardrone* dr, float val)
	\ingroup ATCommands
	\brief Mettre une valeur de manière sécurisée dans le buffer de la vitesse verticale du drone dr
	\param far ardrone* dr : Handle du drone
	\param float val : valeur à inserer
	\author Baudouin Feildel
**/
void setGoUpDown(far ardrone* dr, float val) { dr->goUpDown = (val <= 1 && val >= -1) ? val:0; }

/** \fn void setTurnLeftRight(far ardrone* dr, float val)
	\ingroup ATCommands
	\brief Mettre une valeur dans de manière sécurisée dans le buffer de la vitesse angulaire du drone dr
	\param far ardrone* dr: Handle du drone
	\param float val : valeur à insérer
	\author Baudouin Feildel
**/
void setTurnLeftRight(far ardrone* dr, float val) { dr->turnLeftRight = (val <= 1 && val >= -1) ? val:0; }

/** \fn void setTiltFrontBack(far ardrone* dr, float val)
	\ingroup ATCommands
	\brief Mettre une valeur de manière sécurisé dans le buffer de l'inclinaison avant/arrière du drone dr
	\param far ardrone* dr : Handle du drone
	\param float val : valeur à insérer
	\author Baudouin Feildel
**/
void setTiltFrontBack(far ardrone* dr, float val) { dr->tiltFrontBack = (val <= 1 && val >= -1) ? val:0; }

/** \fn void setTiltLeftRight(far ardrone* dr, float val)
	\ingroup ATCommands
	\brief Mettre une valeur de manière sécurisée dans le buffer de l'inclinaison gauche/droite du drone dr
	\param far ardrone* dr : Handle du drone
	\param float val : valeur à insérer
	\author Baudouin Feildel
**/
void setTiltLeftRight(far ardrone* dr, float val) { dr->tiltLeftRight = (val <= 1 && val >= -1) ? val:0; }

/** \fn cppbool sendAT(far ardrone* dr)
	\ingroup ATCommands
	\brief Envoyer la commande AT qui est dans le buffer du drone dr
	\param far ardrone* dr : Handle du drone
	\return true : buffer envoyé ; false buffer non-envoyé
	\author Baudouin Feildel
**/
cppbool sendAT(far ardrone* dr);

/** \fn far ardrone* newARDrone(void)
	\ingroup ATCommands
	\brief Constructeur de l'objet ardrone
	\return Handle sur drone
	\author Baudouin Feildel
**/
far ardrone* newARDrone(void);

/** \fn void closeARDrone(far ardrone* dr)
	\ingroup ATCommands
	\brief Destructeur de l'objet ardrone
	\param far ardrone* dr : Handle du objet drone
	\author Baudouin Feildel
**/
void closeARDrone(far ardrone* dr);

/*********************
 *                   *
 *      main         *
 *                   *
 *********************/

/** \fn void main(void)
	\brief Fonction principal : gestion du pilotage du drone en fonctions des entrées/sorties
	\author Baudouin Feildel
**/
void main(void)
{
	etat_commandes ec;
	far ardrone* drone;
	char cptPassErr = 0;
	wifi_status status;

	// Initialisation de la carte
	brdInit();
	BRDInit();
	init_etat_commandes(&ec);

	// Connexion au réseau WiFi
	connexion();

	// Connexion et Initialisation du drone
	drone = newARDrone();
	connectToDrone(drone);
	initDrone(drone);

	ec.led_connecte = 1;
	ecrireCommandes(ec);


	for(;;)
	{
		// Tache de gestion des entrées/sorties
		//		et de l'envoi de commande au drone
		costate
		{
			// Read Inputs
			lireCommandes(&ec);

			// Si ARRÊT D'URGENCE
			if(ec.bp_arret_urgence)
				aru(drone);
			else
			{

				/*if(ec.bp_video)
				{

				}*/

				if(ec.bp_trim && !drone->fly)
					initDrone(drone);

				if(ec.switch_land == 0 && !drone->fly)	// Si on est en haut et que l'on ne vole pas
				{
					if( !(initDrone(drone) && takeoff(drone)) )
					{
						while(ec.switch_land == 0)
						{
							costate
							{
								lireCommandes(&ec);
								yield;
							}
							costate
							{
								ec.led_erreur = 1;
								ecrireCommandes(ec);
								waitfor(DelayMs(300));

								ec.led_erreur = 0;
								ecrireCommandes(ec);
								waitfor(DelayMs(300));
							}
						}
					}
				}
				else if(ec.switch_land == 1 && drone->fly)	// Si on est en bas et que l'on vole
				{
					if(!land(drone))
					{
						while(ec.switch_land == 1)
						{
							costate
							{
								lireCommandes(&ec);
								yield;
							}
							costate
							{
								ec.led_erreur = 1;
								ecrireCommandes(ec);
								waitfor(DelayMs(300));

								ec.led_erreur = 0;
								ecrireCommandes(ec);
								waitfor(DelayMs(300));
							}
						}
					}
				}
				// Les autres cas sont :
				//			- haut et pas en vol
				//			- bas et en vol
				// Ces cas sont interdit, et il n'y a rien à traiter dans ces cas.


				// Si on est en vol
				if(drone->fly)
				{
					// Traiter la valeur des joystick
					// Et stocker cette valeur dans l'objet drone
					     setGoUpDown(drone, ((float)(ec.joystick_1x / 128) - 1) );
					setTurnLeftRight(drone, ((float)(ec.joystick_1y / 128) - 1) );
					setTiltFrontBack(drone, ((float)(ec.joystick_2x / 128) - 1) );
					setTiltLeftRight(drone, ((float)(ec.joystick_2y / 128) - 1) );

					// Envoyer la commande
					if(!(volCommand(drone, drone->goUpDown, drone->turnLeftRight, drone->tiltFrontBack, drone->tiltLeftRight)))
					{
						// Si il y a une erreur on incrémente un compteur
						// Au dela de dix erreurs de suite on fait atterrir le drone
						if(cptPassErr > 10)
							land(drone);
						else
							cptPassErr++;
					}
					else
						cptPassErr = 0; // Remise à zéro du compteur d'erreur en cas de réussite
				}
			}

			// On passe la main
			yield;
		}
		costate
		{
			// Vérification de la connexion WiFi toute les 5 secondes
			ifconfig(IF_WIFI0, IFG_WIFI_STATUS, &status, IFS_END);	// récupere le statut
			while(!tcp_tick(NULL));
			if(status.state == WLN_ST_ASSOC_ESS) // test de la connexion
			{
				waitfor(DelayMs(5000));
			}
			else
			{
				ec.led_connecte = 0;
				ec.led_erreur = 1;
				ec.led_debug = 1;
				ecrireCommandes(ec);

				connexion();

				ec.led_connecte = 1;
				ec.led_erreur = 0;
				ec.led_debug = 0;
				ecrireCommandes(ec);
			}
		}
	}
}

/*********************
 *                   *
 *     functions     *
 *                   *
 *********************/


void BRDInit(void)
{
	// Fonction normale pour tous les bits
	BitWrPortI(PCFR,  &PCFRShadow, 0, 1);

	BitWrPortI(PDFR,  &PDFRShadow, 0, 0);
	BitWrPortI(PDFR,  &PDFRShadow, 0, 2);
	BitWrPortI(PDFR,  &PDFRShadow, 0, 3);

	BitWrPortI(PEFR,  &PEFRShadow, 0, 1);
	BitWrPortI(PEFR,  &PEFRShadow, 0, 3);
	BitWrPortI(PEFR,  &PEFRShadow, 0, 6);

	// Niveaux haut et bas (pas de drain ouvert)
	BitWrPortI(PCDCR, &PCDCRShadow, 0, 1);

	BitWrPortI(PDDCR, &PDDCRShadow, 0, 0);
	BitWrPortI(PDDCR, &PDDCRShadow, 0, 2);
	BitWrPortI(PDDCR, &PDDCRShadow, 0, 3);

	BitWrPortI(PEDCR, &PEDCRShadow, 0, 1);
	BitWrPortI(PEDCR, &PEDCRShadow, 0, 3);
	BitWrPortI(PEDCR, &PEDCRShadow, 0, 6);

	// Niveaux logiques de sortie : tout à 1
	BitWrPortI(PCDR, &PCDRShadow, 1, 1);

	BitWrPortI(PDDR, &PDDRShadow, 1, 0);
	BitWrPortI(PDDR, &PDDRShadow, 1, 2);
	BitWrPortI(PDDR, &PDDRShadow, 1, 3);

	BitWrPortI(PEDR, &PEDRShadow, 1, 1);
	BitWrPortI(PEDR, &PEDRShadow, 1, 3);
	BitWrPortI(PEDR, &PEDRShadow, 1, 6);

	// Entrées / Sorties
	BitWrPortI(PCDDR, &PCDDRShadow, 0, 1); // sorties : DOUT

	BitWrPortI(PDDDR, &PDDDRShadow, 1, 0); // sorties : /CS, DIN, CLK
	BitWrPortI(PDDDR, &PDDDRShadow, 1, 2);
	BitWrPortI(PDDDR, &PDDDRShadow, 1, 3);

	BitWrPortI(PEDDR, &PEDDRShadow, 1, 1); // sorties : 3 LEDs ; entrées : 4 boutons
	BitWrPortI(PEDDR, &PEDDRShadow, 1, 3);
	BitWrPortI(PEDDR, &PEDDRShadow, 1, 6);
}

void print_etat_commandes(etat_commandes s)
{
	printf("\nLED connecte : %s\nLED erreur : %s\nLED debug : %s\nBP arret d'urgence : %s\nBP video : %s\nBP trim : %s\nSWITCH land : %s\nJ1 x : %d\nJ1 y : %d\nJ2 x : %d\nJ2 y : %d\n",

	s.led_connecte ? "allumee" : "eteinte",
	s.led_erreur ? "allumee" : "eteinte",
	s.led_debug ? "allumee" : "eteinte",

	s.bp_arret_urgence ? "inactif" : "actif",
	s.bp_video ? "inactif" : "actif",
	s.bp_trim ? "inactif" : "actif",
	s.switch_land ? "bas" : "haut",

	s.joystick_1x,
	s.joystick_1y,
	s.joystick_2x,
	s.joystick_2y);
}

void init_etat_commandes(etat_commandes *s)
{
	s->led_connecte = 0;
	s->led_erreur = 0;
	s->led_debug = 0;

	s->bp_arret_urgence = 1;
	s->bp_video = 1;
	s->bp_trim = 1;
	s->switch_land = 1;

	s->joystick_1x = 0;
	s->joystick_1y = 0;
	s->joystick_2x = 0;
	s->joystick_2y = 0;
}

void SPI_DIN(char v) { BitWrPortI(PDDR, &PDDRShadow, v & 1, 3); }
void SPI_CLK(char v) { BitWrPortI(PDDR, &PDDRShadow, v & 1, 2); }
void SPI_CS(char v)  { BitWrPortI(PDDR, &PDDRShadow, v & 1, 0); }
int SPI_DOUT(void)   { return BitRdPortI(PCDR, 1); }

/*#define SPI_CLK(0) BitWrPortI(PDDR, &PDDRShadow, 0, 2)
#define SPI_CLK(1) BitWrPortI(PDDR, &PDDRShadow, 1, 2)
#define SPI_CS(0)  BitWrPortI(PDDR, &PDDRShadow, 0, 0)
#define SPI_CS(1)  BitWrPortI(PDDR, &PDDRShadow, 1, 0)*/

void SPI_delay(int i)
{                         
	for(i = 0; i < i; i++)
		cWAIT_5_us;
}


int SPIread(char addr)
{
	int i, r;
	addr &= 0x03; // ne prend que les 2 derniers bits


	SPI_CS(0); // commence la communication

	// bit de start
	SPI_DIN(1);

	SPI_delay(20);
	SPI_CLK(0);
	SPI_delay(20);
	SPI_CLK(1);

	// SGL/DIFF
	SPI_DIN(1);

	SPI_delay(20);
	SPI_CLK(0);
	SPI_delay(20);
	SPI_CLK(1);

	// addresse
	SPI_DIN(0);

	SPI_delay(20);
	SPI_CLK(0);
	SPI_delay(20);
	SPI_CLK(1);

	SPI_DIN((addr & 0x02) >> 1);

	SPI_delay(20);
	SPI_CLK(0);
	SPI_delay(20);
	SPI_CLK(1);

	SPI_DIN(addr & 0x01);

	SPI_delay(20);
	SPI_CLK(0);
	SPI_delay(20);
	SPI_CLK(1);

	SPI_DIN(0);

	// attente conversion

	SPI_delay(20);
	SPI_CLK(0);
	SPI_delay(20);
	SPI_CLK(1);

	// null bit
	SPI_delay(20);
	SPI_CLK(0);
	SPI_delay(20);
	SPI_CLK(1);

	// bit de signe
	SPI_delay(20);
	SPI_CLK(0);
	SPI_delay(20);
	SPI_CLK(1);

	SPI_delay(20);
	SPI_CLK(0);
	SPI_delay(20);
	SPI_CLK(1);

	// données
	for(i = 12; i > 0; i--)
	{
		r += SPI_DOUT() << i;

		SPI_delay(20);
		SPI_CLK(0);
		SPI_delay(20);
		SPI_CLK(1);
	}

	// arrête la communication
	SPI_delay(20);
	SPI_DIN(0);
	SPI_CLK(0);
	SPI_CS(1);

	return r;
}

void lireCommandes(etat_commandes *s)
{
	s->bp_arret_urgence = BitRdPortI(PEDR, 2);
	s->bp_video = BitRdPortI(PEDR, 0);
	s->bp_trim = BitRdPortI(PEDR, 5);
	s->switch_land = BitRdPortI(PEDR, 7);

	s->joystick_1x = SPIread(0);
	s->joystick_1y = SPIread(1);
	s->joystick_2x = SPIread(2);
	s->joystick_2y = SPIread(3);
}

void ecrireCommandes(etat_commandes s)
{
	BitWrPortI(PEDR, &PEDRShadow, s.led_connecte, 1);
	BitWrPortI(PEDR, &PEDRShadow, s.led_erreur, 3);
	BitWrPortI(PEDR, &PEDRShadow, s.led_debug, 6);
}

cppbool send_packet(char* str, const char far * ip, word port, udp_Socket *sock)
{
	// envoi le paquet
	if(udp_send(sock, str, strlen(str) + 1) < 0) // s'il y a une erreur
	{
		printf("Erreur d'envoi du paquet :\n;;;%s\n", str);
		sock_close(sock);	// ferme la socket puis la re-ouvre
		if(!udp_open(sock, port, resolve(ip), port, NULL))
		{
			printf("Erreur de reouverture de la socket\n");
			return false;
		}
		else
		{
			printf("Reouverture de la socket reussien nouvel essai\n");
			return send_packet(str, ip, port, sock);
		}
	}
   else
		printf("Paquet envoye :\n;;;%s\n", str);

	tcp_tick(NULL);
	return true;
}

int rxsignal_cmp(far _wifi_wln_scan_bss *a, far _wifi_wln_scan_bss *b)
{
	return b->rx_signal - a->rx_signal;
}

root void scan_assoc_callback(far wifi_scan_data* data)
{
	far _wifi_wln_scan_bss *bss;
	int i, ssid_len;
	char ssid_str[WLN_SSID_SIZE+1], ssid[WLN_SSID_SIZE+1], ctemp;

	connect = RECHERCHE;

	bss = data->bss;	// pointeur sur les points d'accès disponibles

	_f_qsort(bss, data->count, sizeof(bss[0]), rxsignal_cmp);	// trie en fonction de la puissance du signal
	printf("\nResultat du scan des points d'acces :\n");
	for(i = 0; i < data->count; i++)	// affiche les points d'accès disponibles
	{
		wifi_ssid_to_str(ssid_str, bss[i].ssid, bss[i].ssid_len);
		printf("[%2d] trouve : %s\n", i, ssid_str);
	}

	printf("\nRecherche du drone :\n");
	for(i = 0; i < data->count && connect != TROUVE; i++)
	{
		wifi_ssid_to_str(ssid_str, bss[i].ssid, bss[i].ssid_len);	// donne une chaîne de caractère imprimable

		printf("[%2d] comparaison de '%s' avec 'ardrone2_xxxxxx' : ", i, ssid_str);

		ctemp = ssid_str[9];	// sauvegarde du caractère
		ssid_str[9] = '\0';		// tronque à 9 caractères (ardrone2_)

		if(!strcmp("ardrone2_", ssid_str))	// si le ssid correspond
		{
			printf("oui\n");
			ssid_str[9] = ctemp;	// remet du caractère enlevé

			// Need near copy of SSID to call ifconfig.  ssid will be promoted to
			// far for this call, but the results will be in ssid as a near
			// variable
			ssid_len = bss[i].ssid_len;
			_f_memcpy(ssid, bss[i].ssid, ssid_len);

			if(!ifconfig(IF_WIFI0, IFS_WIFI_SSID, ssid_len, ssid, IFS_END))	// demande la connexion
			{
				wifi_ssid_to_str(ssid, ssid, ssid_len);
				printf(" => demande de connexion a %s...\n", ssid);
				connect = TROUVE;	// indique que le drone est trouvé
				// puis il faut attendre que la connexion soit effective (dans connexion())
			}
			else
				printf("=> erreur de demande de connexion a %s", ssid_str);
		}
		else
			printf("non");
		putchar('\n');
	}
	if(connect != TROUVE)
	{
		printf("Aucun point d'acces de drone trouve\n");
	}
}

void connexion(void)
{
	wifi_status status;
	word waitms;
	char connect_timeout; // pour la connexion

	connect = RECHERCHE;
	sock_init();

	waitms = _SET_SHORT_TIMEOUT(300);

	while(connect != CONNECTE)	// status.state != WLN_ST_ASSOC_ESS)	// tant que la connexion n'est pas effective
	{
		printf("* Essai de connexion *\n");
		connect_timeout = 1;	// reset
		tcp_tick(NULL);
		ifdown(IF_WIFI0);
		while(ifpending(IF_WIFI0) != IF_DOWN)	// attend que l'éventuelle ancienne connexion soit arrêtée
		   tcp_tick(NULL);

		// Donne la fonction de callback avant la requête de scan
		ifconfig(IF_WIFI0, IFS_WIFI_SCAN, scan_assoc_callback, IFS_END);
		do
		{
			costate
			{
				tcp_tick(NULL);
				waitfor(DelayMs(1));
			}
			costate
			{
				connect_timeout++; // jusqu'à l'overflow => sort de la boucle
				waitfor(DelayMs(20)); // 5,10 secondes de tentatives de connexion
			}
		} while(connect != TROUVE && connect_timeout); // attend que le drone soit trouvé


		if(connect == TROUVE)	// (pas de timeout)
		{
			printf("Attente pour que la connexion soit effective\n");
			connect_timeout = 1;	// reset

			ifup(IF_WIFI0);
			do
			{
				costate
				{
					ifconfig(IF_WIFI0, IFG_WIFI_STATUS, &status, IFS_END);	// récupere le statut
					while(!tcp_tick(NULL));
					waitfor(DelayMs(1));
				}
				costate
				{
					connect_timeout++; // jusqu'à l'overflow => sort de la boucle
					waitfor(DelayMs(20)); // 5,10 secondes de tentatives de connexion
				}
			} while(status.state != WLN_ST_ASSOC_ESS && connect_timeout); // attend que la connexion soit effective

			if(status.state == WLN_ST_ASSOC_ESS)	// (pas de timeout)
			{
				printf("connexion effective !\n");
				connect = CONNECTE;
			}
			else
			{
				printf("la tentative de connexion a echoue... Nouvel essai\n");
				connect = RECHERCHE;
			}
		}
		else
			printf("\nTimeout de connexion...\n");
	}
}

cppbool connectToDrone(far ardrone* dr)
{
    // Création de la socket d'envoi des commande AT
    if(!udp_open((udp_Socket*) &(dr->udpSocket_at), LOCAL_PORT, resolve(IPDRONE), 5556/*dr->port_at*/, NULL))
	{
		printf("Failed to open udp socket with the drone on port %d!\n", 5556/*dr->port_at*/);
		return false;
	}

    return true;
}

cppbool sendAT(far ardrone* dr)
{
	char lenght;
	char tmp;
    (dr->ident)++;
	sprintf(dr->buff, "%s%d%s", dr->bufferLeft, dr->ident, dr->bufferRight);
	return send_packet(dr->buff, IPDRONE, dr->port_at, &dr->udpSocket_at);
	cppbool send_packet(char* str, const char far * ip, word port, udp_Socket *sock);
}

cppbool initDrone(far ardrone* dr)
{
    // Configure la hauteur maximale du drone
    strcpy(dr->bufferLeft, "AT*CONFIG=");
    strcpy(dr->bufferRight, ",\"control:altitude_max\",\"3000\"\r");
    printf("#%5d - Config alt max - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
    sendAT(dr);

    // Demande l'envoi de donnée de navigation
    strcpy(dr->bufferLeft, "AT*CONFIG=");
    strcpy(dr->bufferRight, ",\"general:navdata_demo\",\"TRUE\"\r");
    printf("#%5d - Start navdata flow - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
    sendAT(dr);

    // Indique au drone qu'il est à plat : le drone se calibre
    strcpy(dr->bufferLeft, "AT*FTRIM=");
    strcpy(dr->bufferRight, ",\r");
    printf("#%5d - Ftrim - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);

    return sendAT(dr);
}

cppbool takeoff(far ardrone* dr)
{
    // Décollage du drone
    strcpy(dr->bufferLeft, "AT*REF=");
    strcpy(dr->bufferRight, ",290718208\r");
    printf("#%5d - Take off - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
    sendAT(dr);

    // Passe en mode vol
    strcpy(dr->bufferLeft, "AT*PCMD=");
    strcpy(dr->bufferRight, ",1,0,0,0,0\r");
    setGoUpDown(dr, 0);
    setTurnLeftRight(dr, 0);
    setTiltFrontBack(dr, 0);
    setTiltLeftRight(dr, 0);
    dr->fly = true;
    // Voir comment lancer le thread du mode vol.
	//this->start();

    return true;
}

cppbool land(far ardrone* dr)
{
    dr->fly = false;
    strcpy(dr->bufferLeft, "AT*REF=");
    strcpy(dr->bufferRight, ",290717696\r");
    printf("#%5d - Land - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);

    return sendAT(dr);
}

cppbool aru(far ardrone* dr)
{
    dr->fly = false;
    strcpy(dr->bufferLeft, "AT*REF=");
    strcpy(dr->bufferRight, ",290717952\r");
    printf("#%5d - Arrêt d'urgence - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);

    return sendAT(dr);
}

cppbool volCommand(far ardrone* dr, float tiltLeftRight_, float tiltFrontBack_, float goUpDown_, float turnLeftRight_)
{
    char strBuff[50];
    sprintf(strBuff, ",1,%d,%d,%d,%d\r", *(int*)(&tiltLeftRight_),
                                         *(int*)(&tiltFrontBack_),
                                         *(int*)(&goUpDown_),
                                         *(int*)(&turnLeftRight_));
    strcpy(dr->bufferLeft, "AT*PCMD=");
    strcpy(dr->bufferRight, strBuff);
    printf("#%5d - PCMD - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
    return sendAT(dr);
}

far ardrone* newARDrone(void)
{
	far ardrone* tmp;
	tmp = malloc(sizeof(ardrone));

    tmp->port_at = 5556;

    tmp->ident = 0;
    tmp->fly = false;

    return tmp;
}
