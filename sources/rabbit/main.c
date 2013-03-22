/**
	\defgroup Rabbit
	
	\defgroup Connexion
	\ingroup Rabbit
	
	\defgroup UDP
	\ingroup Rabbit
	
	\defgroup IO
	\ingroup Rabbit
**/

#define MAX_UDP_SOCKET_BUFFERS 1
#define TCPCONFIG 5
#define IPDRONE "255.255.255.255"
#define PORTDRONE_NAVDATA 5554
#define PORTDRONE_VIDEO 5555
#define ALTITUDEMAX "2000"

#use "dcrtcp.lib"
#use "rcm56xxW.lib"
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
	\brief Initialise tous les ports d'entrées sorties pour la carte joystick
	\author Thibaut Marty
**/
void BRDInit(void);

/**	\fn void print_etat_commandes(etat_commandes *s)
	\ingroup IO
	\brief écris tous les éléments de la structure de l'état des commandes (pour debug)
	\param etat_commandes structure de l'état des commandes
	\author Thibaut Marty
**/
void print_etat_commandes(etat_commandes *s);

/**	\fn void init_etat_commandes(etat_commandes *s)
	\ingroup IO
	\brief initialise la structure de l'état des commandes
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

/**	\fn void SPI_delay(void)
	\ingroup IO
	\brief temps d'attente de 5 µs
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
	\brief fonction de callback qui traite les points d'accès trouvés pour trouver un point d'accès commençant par 'ardrone2_'. S'il y a plusieurs points d'accès, ce sera celui avec le signal le plus puissant qui sera choisi (donc en règle générale celui qui est le plus proche de la télécommande)
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

/*
 *	Début des fonctions de libARDrone
 */
typedef union
{
	float f;
	long l;
} joystick;

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
	joystick goUpDown;				// Buffer de la commande de vitesse verticale
	joystick turnLeftRight;			// Buffer de la commande de vitesse angulaire
} ardrone;

cppbool connectToDrone(ardrone* dr);
cppbool initDrone(ardrone* dr, etat_commandes *s);
cppbool takeoff(ardrone* dr);
cppbool land(ardrone* dr);
cppbool aru(ardrone* dr);
cppbool volCommand(ardrone* dr, joystick tiltLeftRight_, joystick tiltFrontBack_, joystick goUpDown_, joystick turnLeftRight_);
void setGoUpDown(ardrone* dr, float val) { dr->goUpDown.f = (val <= 1 && val >= -1) ? val:0; }
void setTurnLeftRight(ardrone* dr, float val) { dr->turnLeftRight.f = (val <= 1 && val >= -1) ? val:0; }
void setTiltFrontBack(ardrone* dr, float val) { dr->tiltFrontBack.f = (val <= 1 && val >= -1) ? val:0; }
void setTiltLeftRight(ardrone* dr, float val) { dr->tiltLeftRight.f = (val <= 1 && val >= -1) ? val:0; }
cppbool sendAT(ardrone* dr);
void newARDrone(ardrone* tmp);
void closeARDrone(ardrone* dr);

/** \fn void main(void)
	\ingroup Rabbit
	\brief Fonction principal : gestion du pilotage du drone en fonctions des entrées/sorties
	\author Baudouin Feildel
	\author Thibaut Marty
**/
void main(void)
{
	etat_commandes ec;
	ardrone droneV;
	ardrone* drone = &droneV;
	char cptPassErr = 0;
	wifi_status status;
	int initOk;
	
	/// Initialise les ports de la carte et les entrées/sorties
	brdInit(); // fonction de base de Rabbit
	BRDInit(); // fonction pour les bits utilisés
	init_etat_commandes(&ec);
	lireCommandes(&ec);

	/// Crée la structure du drone
	newARDrone(drone);
	
	/// Initialise la connexion (tant que toutes les étapes ne sont pas réussies) :
	do
	{
		printf("tentative d'initialisation\n");
		initOk = 0;
		
		/// .... Initialise le driver TCP
		if(sock_init())
			printf("erreur de sock_init()\n");
		else
			initOk++;
	
		/// .... Initialise la socket de connexion avec le drone
		if(!connectToDrone(drone))
			printf("erreur connectToDrone()\n");
		else
			initOk++;
		
		/// .... Se connecte au réseau WiFi
		connexion(&ec);
		
		/// .... Initialise le drone
		if(!initDrone(drone, &ec))
			printf("erreur initDrone()\n");
		else
			initOk++;
	} while(initOk < 3);
	printf("tentative d'initialisation reussie\n");
	
	/// Vérifie que l'on n'est pas déjà en position 'vol', sinon indique à l'utilisateur de changer le switch en faisant clignoter la LED erreur
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
	} while(ec.switch_land == 0);
	
	ec.led_erreur = 0;
	ecrireCommandes(&ec);

	
	/// Boucle principale (déroulement) :
	for(;;)
	{
		/// .... Tâche de gestion des entrées/sorties et de l'envoi de commandes au drone
		costate
		{
			tcp_tick(NULL);
			
			
			// Lit les entrées
			lireCommandes(&ec);

			/// ........ Si le bouton d'arrêt d'urgence est actif :
			if(!ec.bp_arret_urgence)
			{
				/// ............ Envoie la commande d'arrêt d'urgence
				aru(drone);
				drone->fly = false;
				/// ............ Attend le relâchement du bouton en faisant clignoter la LED debug
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
					yield; // Pour la tâche de vérification de la connexion WiFi
				} while(!ec.bp_arret_urgence);
				ec.led_debug = 0;
				ecrireCommandes(&ec);
				
				/// ............ Renvoie la commande (pour sortir du mode d'arrêt d'urgence)
				aru(drone);
			}
			/// ........ Sinon (traitement de tous les cas position du switch / vol en cours) :
			else
			{
				// pour plus tard :
				// if(ec.bp_video)	{ }
				
				/// ............ Si le bouton trim est actif et que l'on n'est pas en cours de vol, réinitialise le drone
				if(!ec.bp_trim && !drone->fly)
					initDrone(drone, &ec);
				
				/// ............ Si le switch est en position haute et que l'on ne vole pas
				if(ec.switch_land == 0 && !drone->fly)
				{
					/// ................ Fait décoler le drone. S'il y a une erreur : attend que l'utilisateur repasse le switch en position basse en faisant clignoter la LED erreur
					if(!(initDrone(drone, &ec) && takeoff(drone)))
					{
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
				/// ............ Si le switch est en position basse et que l'on vole
				else if(ec.switch_land == 1 && drone->fly)
				{
					/// ................ Fait atterrir le drone. S'il y a une erreur : attend que l'utilisateur passe le switch en position haute
					if(!land(drone))
					{
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
				/// ............ Les autres cas sont normaux et ne nécessitent pas de traitement particulier


				/// ............ Si on est en vol :
				if(drone->fly)
				{
					/// ................ Traite la valeur des joysticks et la stock dans la structure ardrone
					
					     setGoUpDown(drone, (float)(ec.joystick_2x - 2048) / 2048);
					setTurnLeftRight(drone, (float)(ec.joystick_2y - 2048) / 2048);
					setTiltFrontBack(drone, (float)(ec.joystick_1x - 2048) / 2048);
					setTiltLeftRight(drone, (float)(ec.joystick_1y - 2048) / 2048);

					/// ................ Envoie la commande avec les valeurs : s'il y a une erreur on incrémente un compteur d'erreurs. Au delà de dix erreurs de suite on tente de faire atterir le drone. S'il n'y a pas d'erreurs, on attend 30ms avant le prochain envoi
					if(!(volCommand(drone, drone->tiltLeftRight, drone->tiltFrontBack, drone->goUpDown, drone->turnLeftRight)))
					{
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
				else
					yield;
			}
		}
		/// .... Tâche de vérification de la connexion WiFi (toutes les 2 secondes
		costate
		{
			/// ........ Récupère le statut de la connexion
			ifconfig(IF_WIFI0, IFG_WIFI_STATUS, &status, IFS_END);
			while(!tcp_tick(NULL));
			
			/// ........ Si la connexion est OK, on attend 2 secondes (passe la main)
			if(status.state == WLN_ST_ASSOC_ESS)
				waitfor(DelayMs(2000));
			/// ........ Sinon :
			else
			{
				/// ............ Indique à l'utilisateur que la connexion est perdue en éteignant la LED connecte et en allumant les LED erreur et debug, puis tente de se reconnecter tant que toutes les étapes de la connexion ne sont pas réussies
				do
				{
					printf("tentative de reconnexion\n");
					initOk = 0;
					ec.led_connecte = 0;
					ec.led_erreur = 1;
					ec.led_debug = 1;
					ecrireCommandes(&ec);

					// tente de se reconnecter
					connexion(&ec);
					
					if(connectToDrone(drone))
						initOk++;
					if(initDrone(drone, &ec))
						initOk++;
				} while(initOk < 2);
				
				/// ............ Réinitialise l'état du drone dans le structure ardrone
				drone->ident = 0;
				drone->fly = false;
				
				/// ............ Indique à l'utilisateur que la connexion est OK (éteint les LED erreur et debug. La LED connecte s'est allumée dans la fonction de connexion)
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
	/// Applique la fonction normale (pas de bits spéciaux) pour tous les bits utilisés
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

	/// Applique le mode avec niveaux haut et bas (pas de drain ouvert)
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

	/// Applique le niveau logique haut pour les sorties (les bons niveaux seront appliqués dans init_etat_commandes())
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

	/// Configure les bits en entrée (0) ou en sortie (1)
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
	/// Les LED sont éteintes
	s->led_connecte = 0;
	s->led_erreur = 0;
	s->led_debug = 0;

	/// Les boutons sont inactifs
	s->bp_arret_urgence = 1;
	s->bp_video = 1;
	s->bp_trim = 1;
	s->bpj1 = 1;
	s->bpj2 = 1;
	
	/// Le switch est en position basse (mode attéri)
	s->switch_land = 1;

	/// Les valeurs des joysticks sont à 0
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
	asm ld a,3 $\
	sub 3 $\
	ld b,a $\
	db 0x10,-2
}


int SPIread(char addr)
{
	int i, r = 0;
	
	/// Traite le char en entrée pour ne prendre que les 2 derniers bits (les valeurs possibles sont dont 0, 1, 2 ou 3)
	addr &= 0x03; 

	/// Commence la communication
	SPI_CS(0); 

	/// Envoie le bit de start (1)
	SPI_DIN(1);

	SPI_delay();
	SPI_CLK(0);
	SPI_delay();
	SPI_CLK(1);

	/// Envoie le bit SGL/DIFF (1)
	SPI_DIN(1);

	SPI_delay();
	SPI_CLK(0);
	SPI_delay();
	SPI_CLK(1);

	/// Envoie l'addresse
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

	/// Attend la conversion

	SPI_delay();
	SPI_CLK(0);
	SPI_delay();
	SPI_CLK(1);

	/// Attend le temps pour le null bit
	SPI_delay();
	SPI_CLK(0);
	SPI_delay();
	SPI_CLK(1);

	/// Attend le temps pour le bit de signe
	SPI_delay();
	SPI_CLK(0);
	SPI_delay();
	SPI_CLK(1);

	/// Lit les données
	for(i = 12; i > 0; i--)
	{
		SPI_delay();
		SPI_CLK(0);
		SPI_delay();
		
		r += SPI_DOUT() << (i - 1);
		
		SPI_CLK(1);
	}

	/// Arrête la communication
	SPI_delay();
	SPI_DIN(0);
	SPI_CLK(0);
	SPI_CS(1);

	return r;
}

void lireCommandes(etat_commandes *s)
{
	/// Lit les valeurs numériques
	s->bp_arret_urgence = BitRdPortI(PDDR, 3);
	s->bp_video = BitRdPortI(PEDR, 3);
	s->bp_trim = BitRdPortI(PCDR, 3);
	s->bpj1 = BitRdPortI(PEDR, 1);
	s->bpj2 = BitRdPortI(PCDR, 1);
	s->switch_land = BitRdPortI(PEDR, 6);

	/// Lit les valeurs analogiques grâce au convertisseur SPI
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

	/// Place l'enum connect en mode RECHERCHE.
	connect = RECHERCHE;

	bss = data->bss;	// pointeur sur les points d'accès disponibles
	
	/// Trie les points d'accès trouvés en fonction de la puissance du signal.
	_f_qsort(bss, data->count, sizeof(bss[0]), rxsignal_cmp);	
	printf("\nResultat du scan des points d'acces :\n");
	for(i = 0; i < data->count; i++)	// affiche les points d'accès disponibles
	{
		wifi_ssid_to_str(ssid_str, bss[i].ssid, bss[i].ssid_len);
		printf("[%2d] trouve : %s\n", i, ssid_str);
	}

	printf("\nRecherche du drone :\n");
	/// Pour tous les points d'accès trouvés dans l'ordre décroissant de la puissance du signal :
	for(i = 0; i < data->count && connect != TROUVE; i++)
	{
		wifi_ssid_to_str(ssid_str, bss[i].ssid, bss[i].ssid_len);	// donne une chaîne de caractère imprimable

		printf("[%2d] comparaison de '%s' avec 'ardrone2_xxxxxx' : ", i, ssid_str);

		ctemp = ssid_str[9];	// sauvegarde du caractère
		ssid_str[9] = '\0';		// tronque à 9 caractères (ardrone2_)

		/// .... Si le début du ssid (chaîne de caractère nommant le point d'accès) correspond à 'ardrone2_' :
		if(!strcmp("ardrone2_", ssid_str))	// si le ssid correspond
		{
			printf("oui\n");
			ssid_str[9] = ctemp;	// remet du caractère enlevé

			// Need near copy of SSID to call ifconfig.  ssid will be promoted to
			// far for this call, but the results will be in ssid as a near
			// variable
			ssid_len = bss[i].ssid_len;
			_f_memcpy(ssid, bss[i].ssid, ssid_len);

			/// .... Demande la connexion à ce point d'accès.
			if(!ifconfig(IF_WIFI0, IFS_WIFI_SSID, ssid_len, ssid, IFS_END))
			{
				wifi_ssid_to_str(ssid, ssid, ssid_len);
				printf(" => demande de connexion a %s.....\n", ssid);
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
	char connect_timeout;

	/// Place l'enum connect en mode RECHERCHE.
	connect = RECHERCHE;

	/// Tant que celui-ci n'est pas en mode CONNECTE.
	while(connect != CONNECTE)
	{
		printf("* Essai de connexion *\n");
		
		connect_timeout = 0; // Réinitialise le compteur pour le timeout
		tcp_tick(NULL);
		
		/// .... Arrête l'interface en cours (n'a pas de conséquences s'il n'y en a pas).
		ifdown(IF_WIFI0);
		/// .... Attend que l'éventuelle ancienne connexion soit arrêtée.
		while(ifpending(IF_WIFI0) != IF_DOWN)	
		   tcp_tick(NULL);

		/// .... Donne la fonction de callback pour la requête de scan.
		ifconfig(IF_WIFI0, IFS_WIFI_SCAN, scan_assoc_callback, IFS_END);
		
		/// .... Fait fonctionner la pile TCP/IP (donc le scan) jusqu'à ce que l'enum connect soit en mode TROUVE avec un timeout de 5 secondes en faisant clignoter la LED connexion.
		do
		{
			costate
			{
				tcp_tick(NULL);
				waitfor(DelayMs(1));
			}
			costate // Fait clignoter la LED connecte
			{
				s->led_connecte = s->led_connecte ? 0 : 1;
				ecrireCommandes(s);
				connect_timeout++;
				waitfor(DelayMs(100)); // 50 * 100 ms = 5 secondes
			}
		} while(connect != TROUVE && connect_timeout < 50); // attend que le drone soit trouvé


		/// .... Si le drone est effectivement trouvé (sinon on recommence le scan) :
		if(connect == TROUVE) // i.e pas de timeout
		{
			printf("Attente pour que la connexion soit effective\n");
			connect_timeout = 0; // Réinitialise le compteur pour le timeout

			/// ........ Lance l'interface.
			ifup(IF_WIFI0);
			
			/// ........ Récupère le statut de la connexion WiFi tout en faisant fonctionner la pile TCP/IP jusqu'à ce que le statut indique que la connexion soit effective en faisant clignoter la LED connect.
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
					connect_timeout++;
					waitfor(DelayMs(100)); // 50 * 100 ms = 5 secondes
				}
			} while(status.state != WLN_ST_ASSOC_ESS && connect_timeout < 50); // attend que la connexion soit effective

			/// ........ Si la connexion est effective : place l'enum connect en mode CONNECTE.
			if(status.state == WLN_ST_ASSOC_ESS) // i.e pas de timeout
			{
				printf("connexion effective !\n");
				connect = CONNECTE;
			}
			/// ........ Sinon on recommence tout à partir du scan après avoir placé de nouveau l'enum connect en mode RECHERCHE.
			else
			{
				printf("la tentative de connexion a echoue... Nouvel essai\n");
				connect = RECHERCHE;
			}
		}
		else
			printf("\nTimeout de connexion...\n");
	}
	/// Et enfin allume la LED connecte.
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
	char outdoor[8];
	
	// Si le bouton est appuyé : mode outdoor
	if(s->bp_video)
		strcpy(outdoor, "FALSE\"\r");
	else
		strcpy(outdoor, "TRUE\"\r");
	
	
	strcpy(dr->bufferLeft, "AT*CONFIG=");
	// Configure la hauteur maximale du drone en fonction du bouton extérieur/intérieur
	
	strcpy(dr->bufferRight, ",\"control:altitude_max\",\"");
	
	if(s->bp_video)
		strcat(dr->bufferRight, ALTITUDEMAX);
	else
		strcat(dr->bufferRight, 10*ALTITUDEMAX);
	strcat(dr->bufferRight, "\"\r");
	if(!sendAT(dr))
		return false;
		
	// Configure si on est à l'intérieur / à l'extérieur
	strcpy(dr->bufferRight, ",\"control:outdoor\",\"");
	strcat(dr->bufferRight, outdoor);
	if(!sendAT(dr))
		return false;
		
	strcpy(dr->bufferRight, ",\"control:flight_without_shell\",\"");
	strcat(dr->bufferRight, outdoor);
	if(!sendAT(dr))
		return false;

	// Indique au drone qu'il est à plat : le drone se calibre
	strcpy(dr->bufferLeft, "AT*FTRIM=");
	strcpy(dr->bufferRight, ",\r");
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
