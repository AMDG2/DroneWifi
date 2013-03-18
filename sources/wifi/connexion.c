/**
	\defgroup Connexion
	\defgroup UDP
	\defgroup IO
	\defgroup ATCommands // Non commenté ici !!
**/

/* ******************************** */
/*
 * NETWORK CONFIGURATION
 * Please see the function help (Ctrl-H) on TCPCONFIG for instructions on
 * compile-time network configuration.
 */

#define MAX_UDP_SOCKET_BUFFERS 1

#define TCPCONFIG 5

#define IPDRONE "255.255.255.255"

#define PORTDRONE_NAVDATA 5554
#define PORTDRONE_VIDEO 5555

#define ALTITUDEMAX "2000"


#use "dcrtcp.lib"

#use "rcm56xxW.lib"

#memmap xmem

//#define printf

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
	char bp_arret_urgence;	// 0 : actif
	char bp_video;
	char bp_trim;
	char bpj1;
	char bpj2;
	char switch_land;	// 1 : position basse

	int joystick_1x;
	int joystick_1y;
	int joystick_2x;
	int joystick_2y;
} etat_commandes;

/* \typedef cppbool
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

/**	\fn void print_etat_commandes(etat_commandes *s)
	\ingroup IO
	\brief initialise la structure structure de l'état des commandes
	\param etat_commandes structure de l'état des commandes
	\author Thibaut Marty
**/
void print_etat_commandes(etat_commandes *s);

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

/**	\fn void SPI_delay(void)
	\ingroup IO
	\brief temps d'attente de 'i' * 5 µs
	\author Thibaut Marty
**/
void SPI_delay(void);

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

/**	\fn void ecrireCommandes(etat_commandes *s)
	\ingroup IO
	\brief écris les valeurs de la structure de l'état des commandes en sortie
	\param etat_commandes structure de l'état des commandes
	\author Thibaut Marty
**/
void ecrireCommandes(etat_commandes *s);

/**	\fn int send_packet(char* str, udp_Socket *sock)
	\ingroup UDP
	\brief envoie une chaîne de caractère sur une socket (udp)
	\param str chaîne à envoyé
	\param sock pointeur sur udp_Socket utilisée
	\return >=0 nombre d'octets envoyés ; -1 : échec ; -2 échec à cause d'adresse non résolue
	\author Thibaut Marty
**/
int send_packet(char* str, udp_Socket *sock);

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

/**	\fn void connexion(etat_commandes *s)
	\ingroup Connexion
	\brief fonction qui se connecte au drone. Elle se quitte uniquement si la connexion est effective
	\param etat_commandes structure de l'état des commandes
	\author Thibaut Marty
**/
void connexion(etat_commandes *s);

/* \typedef joystick
	\ingroup ATCommands
	\brief union permettant l'envoie d'une valeur flottante en tant que long (empreinte binaire selon un long)
	\author Thibaut Marty
**/
typedef union
{
	float f;
	long l;
} joystick;

/* \typedef ardrone
	\ingroup ATCommands
	\brief Structure contenant les informations nécéssaires à la commande d'un drone
	\author Baudouin Feildel
**/
typedef struct
{
	char buff[200];					// Buffer de Stockage de trame
	udp_Socket udpSocket_at;		// Handle de la socket d'envoi de commande AT
	unsigned int port_at;			// Port de commande du drone
	char bufferLeft[20];			// Partie gauche du buffer
	char bufferRight[150];			// Partie droite du buffer
	int ident;						// Identifiant de la commande
	cppbool fly;					// Mode vol
	joystick tiltFrontBack;			// Buffer de la commande d'inclinaison avant arrière
	joystick tiltLeftRight;			// Buffer de la commande d'inclinaison gauche droite
	joystick goUpDown;					// Buffer de la commande de vitesse verticale
	joystick turnLeftRight;			// Buffer de la commande de vitesse angulaire
} ardrone;

/* \fn cppbool connectToDrone(ardrone* dr)
	\ingroup ATCommands
	\brief Initie la connexion avec le drone dr
	\param ardrone* dr : handle de drone
	\return true : Connexion réussie ; false : Connexion échouée
	\author Baudouin Feildel
**/
cppbool connectToDrone(ardrone* dr);

/* \fn cppbool initDrone(ardrone* dr, etat_commandes *s)
	\ingroup ATCommands
	\brief Initialisation du drone dr pour permettre un décollage en toute sécurité.
	\param ardrone* dr : Handle du drone
	\param etat_commandes *s structure de l'état des commandes
	\return true : initialisation réussie ; false : initialisation échouée
	\author Baudouin Feildel
**/
cppbool initDrone(ardrone* dr, etat_commandes *s);

/* \fn cppbool takeoff(ardrone* dr)
	\ingroup ATCommands
	\brief Commande le décollage du drone dr
	\param ardrone* dr : Handle du drone
	\return true : commande envoyée ; false : commande non-envoyée
	\author Baudouin Feildel
**/
cppbool takeoff(ardrone* dr);

/* \fn cppbool land(ardrone* dr)
	\ingroup ATCommands
	\brief Commande l'atterissage du drone dr
	\param ardrone* dr : Handle du drone
	\return true : commande envoyée ; false : commande non-envoyée
	\author Baudouin Feildel
**/
cppbool land(ardrone* dr);

/* \fn cppbool aru(ardrone* dr)
	\ingroup ATCommands
	\brief Envoi un arrêt d'urgence au drone dr
	\param ardrone* dr : Handle du drone
	\return true : arrêt d'urgence envoyé ; false : arrêt d'urgence non-envoyé
	\author Baudouin Feildel
**/
cppbool aru(ardrone* dr);

/* \fn cppbool volCommand(ardrone* dr, joystick tiltLeftIrght_, joystick tiltFrontBack_, joystick goUpDown_, joystick turnLeftRight_)
	\ingroup ATCommands
	\brief Envoi de la commande de vol au drone dr
	\param ardrone* dr : Handle du drone
	\param joystick tiltLeftRight : commande l'inclinaison avant/arrière du drone
	\param joystick tiltFrontBack : commande l'inclinaison gauche/droite du drone
	\param joystick goUpDown : commande vitesse verticale du drone
	\param joystick turnLeftRight : commande vitesse angulaire du drone
	\return true : commande envoyée ; false : commande non-envoyée
	\author Baudouin Feildel
**/
cppbool volCommand(ardrone* dr, joystick tiltLeftRight_, joystick tiltFrontBack_, joystick goUpDown_, joystick turnLeftRight_);

/* \fn void setGoUpDown(ardrone* dr, float val)
	\ingroup ATCommands
	\brief Mettre une valeur de manière sécurisée dans le buffer de la vitesse verticale du drone dr
	\param ardrone* dr : Handle du drone
	\param float val : valeur à inserer
	\author Baudouin Feildel
**/
void setGoUpDown(ardrone* dr, float val) { dr->goUpDown.f = (val <= 1 && val >= -1) ? val:0; }

/* \fn void setTurnLeftRight(ardrone* dr, float val)
	\ingroup ATCommands
	\brief Mettre une valeur dans de manière sécurisée dans le buffer de la vitesse angulaire du drone dr
	\param ardrone* dr: Handle du drone
	\param float val : valeur à insérer
	\author Baudouin Feildel
**/
void setTurnLeftRight(ardrone* dr, float val) { dr->turnLeftRight.f = (val <= 1 && val >= -1) ? val:0; }

/* \fn void setTiltFrontBack(ardrone* dr, float val)
	\ingroup ATCommands
	\brief Mettre une valeur de manière sécurisé dans le buffer de l'inclinaison avant/arrière du drone dr
	\param ardrone* dr : Handle du drone
	\param float val : valeur à insérer
	\author Baudouin Feildel
**/
void setTiltFrontBack(ardrone* dr, float val) { dr->tiltFrontBack.f = (val <= 1 && val >= -1) ? val:0; }

/* \fn void setTiltLeftRight(ardrone* dr, float val)
	\ingroup ATCommands
	\brief Mettre une valeur de manière sécurisée dans le buffer de l'inclinaison gauche/droite du drone dr
	\param ardrone* dr : Handle du drone
	\param float val : valeur à insérer
	\author Baudouin Feildel
**/
void setTiltLeftRight(ardrone* dr, float val) { dr->tiltLeftRight.f = (val <= 1 && val >= -1) ? val:0; }

/* \fn cppbool sendAT(ardrone* dr)
	\ingroup ATCommands
	\brief Envoyer la commande AT qui est dans le buffer du drone dr
	\param ardrone* dr : Handle du drone
	\return true : buffer envoyé ; false buffer non-envoyé
	\author Baudouin Feildel
**/
cppbool sendAT(ardrone* dr);

/* \fn void newARDrone(ardrone* tmp)
	\ingroup ATCommands
	\brief Constructeur de l'objet ardrone
	\param pointeur sur ardrone
	\author Baudouin Feildel
**/
void newARDrone(ardrone* tmp);

/* \fn void closeARDrone(ardrone* dr)
	\ingroup ATCommands
	\brief Destructeur de l'objet ardrone
	\param ardrone* dr : Handle du objet drone
	\author Baudouin Feildel
**/
void closeARDrone(ardrone* dr);

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
	ardrone droneV;
	ardrone* drone = &droneV;
	char cptPassErr = 0;
	wifi_status status;
	int initOk;
	
	// Initialisation de la carte
	brdInit();
	BRDInit();
	init_etat_commandes(&ec);
	lireCommandes(&ec);
	
	// crée la structure
	newARDrone(drone);
	
	do
	{
		printf("tentative d'initialisation\n");
		initOk = 0;
		if(sock_init())
			printf("erreur de sock_init()\n");
		else
			initOk++;
	
		// Connexion au drone
		if(!connectToDrone(drone))
			printf("erreur connectToDrone()\n");
		else
			initOk++;
		
		// Connexion au réseau WiFi
		connexion(&ec);
		
		// Initialisation du drone
		if(!initDrone(drone, &ec))
			printf("erreur initDrone()\n");
		else
			initOk++;
	} while(initOk < 2);
	
	printf("tentative d'initialisation reussie\n");
	
	// Vérifie que l'on est pas déjà en position 'vol', sinon indique à l'utilisateur de changer le switch
	do
	{
		costate
		{
			lireCommandes(&ec);
			yield;
		}
		costate // Fait clignoter la LED d'erreur
		{
			ec.led_erreur = ec.led_erreur ? 0 : 1;
			ecrireCommandes(&ec);
			waitfor(DelayMs(300));
		}
	}
	while(ec.switch_land == 0);
	
	ec.led_erreur = 0;
	ecrireCommandes(&ec);

	for(;;)
	{
		// Tache de gestion des entrées/sorties
		// et de l'envoi de commande au drone
		costate
		{
			tcp_tick(NULL);
			
			
			// Lit les entrées
			lireCommandes(&ec);

			// Si ARRÊT D'URGENCE
			if(!ec.bp_arret_urgence)
			{
				aru(drone);
				drone->fly = false;
				// attend le relachement du bouton d'arrêt d'urgence
				do
				{
					costate
					{
						lireCommandes(&ec);
						yield;
					}
					costate // Fait clignoter la LED de debug
					{
						ec.led_debug = ec.led_debug ? 0 : 1;
						ecrireCommandes(&ec);
						waitfor(DelayMs(100));
					}
				}
				while(!ec.bp_arret_urgence);
				ec.led_debug = 0;
				ecrireCommandes(&ec);
				aru(drone);
			}
			else
			{

				// if(ec.bp_video)
				// {

				// }

				if(!ec.bp_trim && !drone->fly)
					initDrone(drone, &ec);

				if(ec.switch_land == 0 && !drone->fly)	// Si le switch est en position haute et que l'on ne vole pas
				{
					if(!(initDrone(drone, &ec) && takeoff(drone)) )
					{
						// attend que l'utilisateur passe le switch en position bas
						do
						{
							costate
							{
								lireCommandes(&ec);
								yield;
							}
							costate // Fait clignoter la LED d'erreur
							{
								ec.led_erreur = ec.led_erreur ? 0 : 1;
								ecrireCommandes(&ec);
								waitfor(DelayMs(100));
							}
						} while(ec.switch_land == 0);
					}
				}
				else if(ec.switch_land == 1 && drone->fly)	// Si le switch est en position basse et que l'on vole
				{
					if(!land(drone))
					{
						// attend que l'utilisateur passe le switch en position haute
						do
						{
							costate
							{
								lireCommandes(&ec);
								yield;
							}
							costate // Fait clignoter la LED d'erreur
							{
								ec.led_erreur = ec.led_erreur ? 0 : 1;
								ecrireCommandes(&ec);
								waitfor(DelayMs(300));
							}
						} while(ec.switch_land == 1);
					}
				}
				// Les autres cas sont :
				//			- haut et pas en vol
				//			- bas et en vol
				// Ces cas sont interdit (n'arriveront pas), et il n'y a rien à traiter dans ces cas.


				// Si on est en vol
				if(drone->fly)
				{
					// Traiter la valeur des joystick
					// Et stocker cette valeur dans l'objet drone
					
					     setGoUpDown(drone, (float)(ec.joystick_2x - 2048) / 2048);
					setTurnLeftRight(drone, (float)(ec.joystick_2y - 2048) / 2048);
					setTiltFrontBack(drone, (float)(ec.joystick_1x - 2048) / 2048);
					setTiltLeftRight(drone, (float)(ec.joystick_1y - 2048) / 2048);

					// Envoyer la commande
					if(!(volCommand(drone, drone->tiltLeftRight, drone->tiltFrontBack, drone->goUpDown, drone->turnLeftRight)))
					{
						// Si il y a une erreur on incrémente un compteur
						// Au dela de dix erreurs de suite on fait atterrir le drone
						if(cptPassErr > 10)
							land(drone);
						else
							cptPassErr++;
					}
					else
					{
						cptPassErr = 0; // Remise à zéro du compteur d'erreur en cas de réussite
						waitfor(DelayMs(30)); // prochain envoi de commande dans 30 ms
					}
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
			if(status.state == WLN_ST_ASSOC_ESS) // si connecté
			{
				waitfor(DelayMs(5000));
			}
			else // sinon
			{
				// indique à l'utilisateur que la connexion est perdue
				do
				{
					printf("tentative de reconnexion\n");
					initOk = 0;
					ec.led_connecte = 0;
					ec.led_erreur = 1;
					ec.led_debug = 1;
					ecrireCommandes(&ec);

					// se reconnecte
					connexion(&ec);
					
					if(connectToDrone(drone))
						initOk++;
					if(initDrone(drone, &ec))
						initOk++;
				}
				while(initOk < 2);
				
				// Reset de l'état du drone
				drone->ident = 0;
				drone->fly = false;
				
				// indique que c'est bon
				ec.led_erreur = 0;
				ec.led_debug = 0;
				ecrireCommandes(&ec);
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
	BitWrPortI(PCFR,  &PCFRShadow, 0, 0);
	BitWrPortI(PCFR,  &PCFRShadow, 0, 1);
	BitWrPortI(PCFR,  &PCFRShadow, 0, 2);
	BitWrPortI(PCFR,  &PCFRShadow, 0, 3);

	BitWrPortI(PDFR,  &PDFRShadow, 0, 2);
	BitWrPortI(PDFR,  &PDFRShadow, 0, 3);

	BitWrPortI(PEFR,  &PEFRShadow, 0, 0);
	BitWrPortI(PEFR,  &PEFRShadow, 0, 1);
	BitWrPortI(PEFR,  &PEFRShadow, 0, 2);
	BitWrPortI(PEFR,  &PEFRShadow, 0, 3);
	BitWrPortI(PEFR,  &PEFRShadow, 0, 5);
	BitWrPortI(PEFR,  &PEFRShadow, 0, 6);
	BitWrPortI(PEFR,  &PEFRShadow, 0, 7);

	// Niveaux haut et bas (pas de drain ouvert)
	BitWrPortI(PCDCR, &PCDCRShadow, 0, 0);
	BitWrPortI(PCDCR, &PCDCRShadow, 0, 1);
	BitWrPortI(PCDCR, &PCDCRShadow, 0, 2);
	BitWrPortI(PCDCR, &PCDCRShadow, 0, 3);

	BitWrPortI(PDDCR, &PDDCRShadow, 0, 2);
	BitWrPortI(PDDCR, &PDDCRShadow, 0, 3);


	BitWrPortI(PEDCR, &PEDCRShadow, 0, 0);
	BitWrPortI(PEDCR, &PEDCRShadow, 0, 1);
	BitWrPortI(PEDCR, &PEDCRShadow, 0, 2);
	BitWrPortI(PEDCR, &PEDCRShadow, 0, 3);
	BitWrPortI(PEDCR, &PEDCRShadow, 0, 5);
	BitWrPortI(PEDCR, &PEDCRShadow, 0, 6);
	BitWrPortI(PEDCR, &PEDCRShadow, 0, 7);

	// Niveaux logiques de sortie : tout à 1
	BitWrPortI(PCDR, &PCDRShadow, 1, 0);
	BitWrPortI(PCDR, &PCDRShadow, 1, 1);
	BitWrPortI(PCDR, &PCDRShadow, 1, 2);
	BitWrPortI(PCDR, &PCDRShadow, 1, 3);

	BitWrPortI(PDDR, &PDDRShadow, 1, 2);
	BitWrPortI(PDDR, &PDDRShadow, 1, 3);

	BitWrPortI(PEDR, &PEDRShadow, 1, 0);
	BitWrPortI(PEDR, &PEDRShadow, 1, 1);
	BitWrPortI(PEDR, &PEDRShadow, 1, 2);
	BitWrPortI(PEDR, &PEDRShadow, 1, 3);
	BitWrPortI(PEDR, &PEDRShadow, 1, 5);
	BitWrPortI(PEDR, &PEDRShadow, 1, 6);
	BitWrPortI(PEDR, &PEDRShadow, 1, 7);

	// Entrées (0) / Sorties (1)
	BitWrPortI(PCDDR, &PCDDRShadow, 1, 0); // sortie : leder
	BitWrPortI(PCDDR, &PCDDRShadow, 0, 1); // entrée : bpj2
	BitWrPortI(PCDDR, &PCDDRShadow, 1, 2); // sortie : ledde
	BitWrPortI(PCDDR, &PCDDRShadow, 0, 3); // entrée : bptr

	BitWrPortI(PDDDR, &PDDDRShadow, 1, 2); // sortie : ledco
	BitWrPortI(PDDDR, &PDDDRShadow, 0, 3); // entrée : bpau

	BitWrPortI(PEDDR, &PEDDRShadow, 0, 0); // entrée : SPIDOUT
	BitWrPortI(PEDDR, &PEDDRShadow, 0, 1); // entrée : bpj1
	BitWrPortI(PEDDR, &PEDDRShadow, 1, 2); // sortie : SPICLK
	BitWrPortI(PEDDR, &PEDDRShadow, 0, 3); // entrée : bpvi
	BitWrPortI(PEDDR, &PEDDRShadow, 1, 5); // sortie : SPICS
	BitWrPortI(PEDDR, &PEDDRShadow, 0, 6); // entrée : land
	BitWrPortI(PEDDR, &PEDDRShadow, 1, 7); // sortie : SPIDIN
}

void print_etat_commandes(etat_commandes *s)
{
	printf("\nLED connecte : %s\nLED erreur : %s\nLED debug : %s\nBP arret d'urgence : %s\nBP video : %s\nBP trim : %s\nBP bpj1 : %s\nBP bpj2 : %s\nSWITCH land : %s\nJ1 x : %d\nJ1 y : %d\nJ2 x : %d\nJ2 y : %d\n",

	s->led_connecte ? "allumee" : "eteinte",
	s->led_erreur ? "allumee" : "eteinte",
	s->led_debug ? "allumee" : "eteinte",

	s->bp_arret_urgence ? "inactif" : "actif",
	s->bp_video ? "inactif" : "actif",
	s->bp_trim ? "inactif" : "actif",
	s->bpj1 ? "inactif" : "actif",
	s->bpj2 ? "inactif" : "actif",
	s->switch_land ? "bas" : "haut",

	s->joystick_1x,
	s->joystick_1y,
	s->joystick_2x,
	s->joystick_2y);
}

void init_etat_commandes(etat_commandes *s)
{
	// leds éteintes
	s->led_connecte = 0;
	s->led_erreur = 0;
	s->led_debug = 0;

	// boutons inactifs
	s->bp_arret_urgence = 1;
	s->bp_video = 1;
	s->bp_trim = 1;
	s->bpj1 = 1;
	s->bpj2 = 1;
	
	// switch land en position basse
	s->switch_land = 1;

	// valeurs analogiques à 0
	s->joystick_1x = 0;
	s->joystick_1y = 0;
	s->joystick_2x = 0;
	s->joystick_2y = 0;
}

void SPI_DIN(char v) { BitWrPortI(PEDR, &PEDRShadow, v & 1, 7); }
void SPI_CLK(char v) { BitWrPortI(PEDR, &PEDRShadow, v & 1, 2); }
void SPI_CS(char v)  { BitWrPortI(PEDR, &PEDRShadow, v & 1, 5); }
int SPI_DOUT(void)   { return BitRdPortI(PEDR, 0); }

void SPI_delay(void)
{                         
	cWAIT_5_us;
}


int SPIread(char addr)
{
	int i, r = 0;
	addr &= 0x03; // ne prend que les 2 derniers bits


	SPI_CS(0); // commence la communication

	// bit de start
	SPI_DIN(1);

	SPI_delay();
	SPI_CLK(0);
	SPI_delay();
	SPI_CLK(1);

	// SGL/DIFF
	SPI_DIN(1);

	SPI_delay();
	SPI_CLK(0);
	SPI_delay();
	SPI_CLK(1);

	// addresse
	SPI_DIN(0);

	SPI_delay();
	SPI_CLK(0);
	SPI_delay();
	SPI_CLK(1);

	SPI_DIN((addr & 0x02) >> 1);

	SPI_delay();
	SPI_CLK(0);
	SPI_delay();
	SPI_CLK(1);

	SPI_DIN(addr & 0x01);

	SPI_delay();
	SPI_CLK(0);
	SPI_delay();
	SPI_CLK(1);

	SPI_DIN(0);

	// attente conversion

	SPI_delay();
	SPI_CLK(0);
	SPI_delay();
	SPI_CLK(1);

	// null bit
	SPI_delay();
	SPI_CLK(0);
	SPI_delay();
	SPI_CLK(1);

	// bit de signe
	SPI_delay();
	SPI_CLK(0);
	SPI_delay();
	SPI_CLK(1);

	/*SPI_delay();
	SPI_CLK(0);
	SPI_delay();
	SPI_CLK(1);*/

	// données
	for(i = 12; i > 0; i--)
	{
		SPI_delay();
		SPI_CLK(0);
		SPI_delay();
		
		r += SPI_DOUT() << (i - 1);
		
		SPI_CLK(1);
	}

	// arrête la communication
	SPI_delay();
	SPI_DIN(0);
	SPI_CLK(0);
	SPI_CS(1);

	return r;
}

void lireCommandes(etat_commandes *s)
{
	s->bp_arret_urgence = BitRdPortI(PDDR, 3);
	s->bp_video = BitRdPortI(PEDR, 3);
	s->bp_trim = BitRdPortI(PCDR, 3);
	s->bpj1 = BitRdPortI(PEDR, 1);
	s->bpj2 = BitRdPortI(PCDR, 1);
	s->switch_land = BitRdPortI(PEDR, 6);

	s->joystick_1x = SPIread(0);
	s->joystick_1y = SPIread(1);
	s->joystick_2x = SPIread(2);
	s->joystick_2y = SPIread(3);
}

void ecrireCommandes(etat_commandes *s)
{
	BitWrPortI(PDDR, &PDDRShadow, s->led_connecte, 2);
	BitWrPortI(PCDR, &PCDRShadow, s->led_erreur, 0);
	BitWrPortI(PCDR, &PCDRShadow, s->led_debug, 2);
}

int send_packet(char* str, udp_Socket *sock)
{
	int ret;
	
	ret = udp_send(sock, str, strlen(str) + 1); // envoi le paquet
	
	tcp_tick(NULL);
	return ret;
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

void connexion(etat_commandes *s)
{
	wifi_status status;
	char connect_timeout; // pour la connexion

	connect = RECHERCHE;

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
				s->led_connecte = s->led_connecte ? 0 : 1;
				ecrireCommandes(s);
				connect_timeout++;
				waitfor(DelayMs(100)); // 5 secondes de tentatives de connexion
			}
		} while(connect != TROUVE && connect_timeout < 50); // attend que le drone soit trouvé


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
					s->led_connecte = s->led_connecte ? 0 : 1;
					ecrireCommandes(s);
					connect_timeout++; // jusqu'à l'overflow => sort de la boucle
					waitfor(DelayMs(100)); // 5 secondes de tentatives de connexion
				}
			} while(status.state != WLN_ST_ASSOC_ESS && connect_timeout < 50); // attend que la connexion soit effective

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
	s->led_connecte = 1;
	ecrireCommandes(s);
}

cppbool connectToDrone(ardrone* dr)
{
	// Création de la socket d'envoi des commande AT
	sock_close(&(dr->udpSocket_at));
	return udp_open(&(dr->udpSocket_at), dr->port_at, resolve(IPDRONE), dr->port_at, NULL);//true;
}

cppbool sendAT(ardrone* dr)
{
	(dr->ident)++;
	sprintf(dr->buff, "%s%d%s", dr->bufferLeft, dr->ident, dr->bufferRight);
	//printf("\n\nenvoi de :\n%s\n\n", dr->buff);
	while(send_packet(dr->buff, &(dr->udpSocket_at)) < 0)
	{
		// todo : faire clignoter une led d'erreur ici...
		sock_close(&(dr->udpSocket_at));	// ferme la socket puis la re-ouvre
		if(!udp_open(&(dr->udpSocket_at), dr->port_at, resolve(IPDRONE), dr->port_at, NULL))
		{
			printf("Erreur de reouverture de la socket\n");
			return false;
		}
		else
			printf("Reouverture de la socket reussie. Nouvel essai\n");
	}
	return true;
}

cppbool initDrone(ardrone* dr, etat_commandes *s)
{
	/*
	// Configure l'application ID pour pouvoir modifier les paramètres de la vidéo
	strcpy(dr->bufferLeft, "AT*CONFIG_IDS=");
	strcpy(dr->bufferRight, ",\"5686c78e\",\"2aff28b9\",\"355fda13\"");
	if(!sendAT(dr))
		return false;
	strcpy(dr->bufferLeft, "AT*CONFIG=");
	strcpy(dr->bufferRight, ",\"custom:session_id\",\"5686c78e\"");
	if(!sendAT(dr))
		return false;
	strcpy(dr->bufferLeft, "AT*CONFIG_IDS=");
	strcpy(dr->bufferRight, ",\"5686c78e\",\"2aff28b9\",\"355fda13\"");
	if(!sendAT(dr))
		return false;
	strcpy(dr->bufferLeft, "AT*CONFIG=");
	strcpy(dr->bufferRight, ",\"custom:profile_id\",\"2aff28b9\"");
	if(!sendAT(dr))
		return false;
	strcpy(dr->bufferLeft, "AT*CONFIG_IDS=");
	strcpy(dr->bufferRight, ",\"5686c78e\",\"2aff28b9\",\"355fda13\"");
	if(!sendAT(dr))
		return false;
	strcpy(dr->bufferLeft, "AT*CONFIG=");
	strcpy(dr->bufferRight, ",\"custom:application_id\",\"355fda13\"");
	if(!sendAT(dr))
		return false;
	

	// Configure la hauteur maximale du drone
	strcpy(dr->bufferLeft, "AT*CONFIG_IDS=");
	strcpy(dr->bufferRight, ",\"5686c78e\",\"2aff28b9\",\"355fda13\"");
	if(!sendAT(dr))
		return false;*/
	strcpy(dr->bufferLeft, "AT*CONFIG=");
	strcpy(dr->bufferRight, ",\"control:altitude_max\",\"");
	strcat(dr->bufferRight, ALTITUDEMAX);
	strcat(dr->bufferRight, "\"\r");
	if(!sendAT(dr))
		return false;
	/*
	// configure à l'intérieur / à l'extérieur
	strcpy(dr->bufferLeft, "AT*CONFIG_IDS=");
	strcpy(dr->bufferRight, ",\"5686c78e\",\"2aff28b9\",\"355fda13\"");
	if(!sendAT(dr))
		return false;*/
	strcpy(dr->bufferLeft, "AT*CONFIG=");
	strcpy(dr->bufferRight, ",\"control:outdoor\",\"");
	if(s->bp_video)
		strcat(dr->bufferRight, "FALSE\"\r");
	else
		strcat(dr->bufferRight, "TRUE\"\r");
	if(!sendAT(dr))
		return false;
	//printf("\n\n%s%d%s\n\n", dr->bufferLeft, dr->ident, dr->bufferRight);
	/*
	// configure les fps du flux vidéo
	strcpy(dr->bufferLeft, "AT*CONFIG_IDS=");
	strcpy(dr->bufferRight, ",\"5686c78e\",\"2aff28b9\",\"355fda13\"");
	if(!sendAT(dr))
		return false;
	strcpy(dr->bufferLeft, "AT*CONFIG=");
	strcpy(dr->bufferRight, ",\"video:codec_fps\",\"15\"\r");
	if(!sendAT(dr))
		return false;
	strcpy(dr->bufferLeft, "AT*CONFIG_IDS=");
	strcpy(dr->bufferRight, ",\"5686c78e\",\"2aff28b9\",\"355fda13\"");
	if(!sendAT(dr))
		return false;
	strcpy(dr->bufferLeft, "AT*CONFIG=");
	strcpy(dr->bufferRight, ",\"video:camif_fps\",\"15\"\r");
	if(!sendAT(dr))
		return false;
	/*strcpy(dr->bufferRight, ",\"video:max_bitrate\",\"1000\"\r");
	if(!sendAT(dr))
		return false;
	strcpy(dr->bufferRight, ",\"video:bitrate\",\"1000\"\r");
	if(!sendAT(dr))
		return false;*/

	// Indique au drone qu'il est à plat : le drone se calibre
	strcpy(dr->bufferLeft, "AT*FTRIM=");
	strcpy(dr->bufferRight, ",\r");
	//printf("#%5d - Ftrim - %s%d%s\n", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);

	return sendAT(dr);
}

cppbool takeoff(ardrone* dr)
{
	// Décollage du drone
	strcpy(dr->bufferLeft, "AT*REF=");
	strcpy(dr->bufferRight, ",290718208\r");
	//printf("#%5d - Take off - %s%d%s\n", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
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

cppbool land(ardrone* dr)
{
	dr->fly = false;
	strcpy(dr->bufferLeft, "AT*REF=");
	strcpy(dr->bufferRight, ",290717696\r");
	//printf("#%5d - Land - %s%d%s\n", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);

	return sendAT(dr);
}

cppbool aru(ardrone* dr)
{
	dr->fly = false;
	strcpy(dr->bufferLeft, "AT*REF=");
	strcpy(dr->bufferRight, ",290717952\r");
	//printf("#%5d - Arret d'urgence - %s%d%s\n", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);

	return sendAT(dr);
}

cppbool volCommand(ardrone* dr, joystick tiltLeftRight_, joystick tiltFrontBack_, joystick goUpDown_, joystick turnLeftRight_)
{
	tiltFrontBack_.f = -tiltFrontBack_.f;
	tiltLeftRight_.f = -tiltLeftRight_.f;
	turnLeftRight_.f = -turnLeftRight_.f;
	sprintf(dr->bufferRight, ",1,%ld,%ld,%ld,%ld\r",	tiltLeftRight_.l,
														tiltFrontBack_.l,
														goUpDown_.l,
														turnLeftRight_.l);
	strcpy(dr->bufferLeft, "AT*PCMD=");
	//printf("#%5d - PCMD - %s%d%s\n", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
	return sendAT(dr);
}

void newARDrone(ardrone* tmp)
{
	tmp->port_at = 5556;

	tmp->ident = 0;
	tmp->fly = false;
}

