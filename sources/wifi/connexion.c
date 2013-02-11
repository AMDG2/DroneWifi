/**
	\defgroup Connexion
	\defgroup UDP
	\defgroup IO
	\defgroup ATCommands
**/

/*
 * Define the number of socket buffers that will be allocated for
 * UDP sockets.  We only need one UDP socket, so one socket buffer
 * will be enough.
 */
#define MAX_UDP_SOCKET_BUFFERS 1

/*
 * UDP demo configuration
 */
/* How often (in seconds) to send out UDP packet. */
#define  HEARTBEAT_RATE 	10

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

//#define MODE_DEBUG

#use "dcrtcp.lib"

// pour ledDS1()
//#use "rcm56xxw.lib"


/* I2C.LIB a été modifié :
	SCL Port D 0
	SDA Port D 1
*/
#use "I2C_DEVICES.lib"
// adresse de l'esclave ADC : 000
#define ADCSLAVE 0

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
	
	char joystick_1x;
	char joystick_1y;
	char joystick_2x;
	char joystick_2y;
} etat_commandes;

/** \typedef bool
	\ingroup ATCommands
	\brief Implementation du type c++ bool en c
	\author Baudouin Feildel
**/
#define true (1)
#define false (0)
typedef bool char;

/**	\var connect variable globale qui défini le statut de la connexion en cours
	\ingroup Connexion
	\author Thibaut Marty
**/
statut_connexion connect = INOCCUPE;

/**	\fn unsigned char lireCommandes(etat_commandes *s)
	\ingroup IO
	\brief remplie la structure de l'état des commandes avec de nouvelles valeurs lues
	\param etat_commandes structure de l'état des commandes
	\return erreurs sur la lecture des valeurs de joystick : {a b c d 00 EE}.
		 0xa : erreur sur J1x
		 0xb : erreur sur J1y
		 0xc : erreur sur J2x
		 0xd : erreur sur J2y
		 0xEE : nombre d'erreur au total (0 à 4)
	\author Thibaut Marty
**/
unsigned char lireCommandes(etat_commandes *s);

/**	\fn void ecrireCommandes(etat_commandes *s)
	\ingroup IO
	\brief écris les valeurs de la structure de l'état des commandes en sortie
	\param etat_commandes structure de l'état des commandes
	\author Thibaut Marty
**/
void ecrireCommandes(etat_commandes *s);

/**	\fn int send_packet(char* str, const char far * ip, word port, udp_Socket *sock)
	\ingroup UDP
	\brief envoie une chaîne de caractère
	\param str chaîne à envoyé
	\param ip ip destination
	\param port port de l'envoi
	\param sock pointeur sur udp_Socket utilisée*
	\return 0 : paquet envoyé ; -1 : erreur mais la socket est ouverte ; -2 : erreur, la socket est ouverte
	\author Thibaut Marty
**/
int send_packet(char* str, const char far * ip, word port, udp_Socket *sock);

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

/** \typedef bool
	\ingroup ATCommands
	\brief Implementation du type c++ bool en c
	\author Baudouin Feildel
**/
#define true (1)
#define false (0)
typedef bool char;

/** \typedef ardrone
	\ingroup ATCommands
	\brief Structure contenant les informations nécéssaires à la commande d'un drone
	\author Baudouin Feildel
**/
typedef struct
{
	char buff[1025];				// Buffer de Stockage de trame
	udp_socket udpSocket_at;		// Handle de la socket d'envoi de commande AT
	char host[13] = "192.168.1.1";	// Adresse IP du drone
	int port_at = 5556;				// Port de commande du drone
	char bufferLeft[20];			// Partie gauche du buffer
	char bufferRight[50];			// Partie droite du buffer
	int ident;						// Identifiant de la commande
	bool fly;						// Mode vol
	float tiltFrontBack;			// Buffer de la commande d'inclinaison avant arrière
	float tiltLeftRight;			// Buffer de la commande d'inclinaison gauche droite
	float goUpDown;					// Buffer de la commande de vitesse verticale
	float turnLeftRight;			// Buffer de la commande de vitesse angulaire
} ardrone;

/** \fn bool connectToDrone(ardrone* dr)
	\ingroup ATCommands
	\brief Initie la connexion avec le drone dr
	\param ardrone* dr : handle de drone
	\return true : Connexion réussie ; false : Connexion échouée
	\author Baudouin Feildel
**/
bool connectToDrone(ardrone* dr);

/** \fn bool initDrone(ardrone* dr)
	\ingroup ATCommands
	\brief Initialisation du drone dr pour permettre un décollage en toute sécurité.
	\param ardrone* dr : Handle du drone
	\return true : initialisation réussie ; false : initialisation échouée
	\author Baudouin Feildel
**/
bool initDrone(ardrone* dr);

/** \fn bool takeoff(ardrone* dr)
	\ingroup ATCommands
	\brief Commande le décollage du drone dr
	\param ardrone* dr : Handle du drone
	\return true : commande envoyée ; false : commande non-envoyée
	\author Baudouin Feildel
**/
bool takeoff(ardrone* dr);

/** \fn bool land(ardrone* dr)
	\ingroup ATCommands
	\brief Commande l'atterissage du drone dr
	\param ardrone* dr : Handle du drone
	\return true : commande envoyée ; false : commande non-envoyée
	\author Baudouin Feildel
**/
bool land(ardrone* dr);

/** \fn bool aru(ardrone* dr)
	\ingroup ATCommands
	\brief Envoi un arrêt d'urgence au drone dr
	\param ardrone* dr : Handle du drone
	\return true : arrêt d'urgence envoyé ; false : arrêt d'urgence non-envoyé
	\author Baudouin Feildel
**/
bool aru(ardrone* dr);

/** \fn bool volCommand(ardrone* dr, float tiltLeftIrght_, float tiltFrontBack_, float goUpDown_, float turnLeftRight_)
	\ingroup ATCommands
	\brief Envoi de la commande de vol au drone dr
	\param ardrone* dr : Handle du drone
	\param float tiltLeftRight : commande l'inclinaison avant/arrière du drone
	\param float tiltFrontBack : commande l'inclinaison gauche/droite du drone
	\param float goUpDown : commande vitesse verticale du drone
	\param float turnLeftRight : commande vitesse angulaire du drone
	\return true : commande envoyée ; false : commande non-envoyée
	\author Baudouin Feildel
**/
bool volCommand(ardrone* dr, float tiltLeftRight_, float tiltFrontBack_, float goUpDown_, float turnLeftRight_);

/** \fn void setGoUpDown(ardrone* dr, float val)
	\ingroup ATCommands
	\brief Mettre une valeur de manière sécurisée dans le buffer de la vitesse verticale du drone dr
	\param ardrone* dr : Handle du drone
	\param float val : valeur à inserer
	\author Baudouin Feildel
**/
void setGoUpDown(ardrone* dr, float val) { dr->goUpDown = (val <= 1 && val >= -1) ? val:0; }

/** \fn void setTurnLeftRight(ardrone* dr, float val)
	\ingroup ATCommands
	\brief Mettre une valeur dans de manière sécurisée dans le buffer de la vitesse angulaire du drone dr
	\param ardrone* dr: Handle du drone
	\param float val : valeur à insérer
	\author Baudouin Feildel
**/
void setTurnLeftRight(ardrone* dr, float val) { dr->turnLeftRight = (val <= 1 && val >= -1) ? val:0; }

/** \fn void setTiltFrontBack(ardrone* dr, float val)
	\ingroup ATCommands
	\brief Mettre une valeur de manière sécurisé dans le buffer de l'inclinaison avant/arrière du drone dr
	\param ardrone* dr : Handle du drone
	\param float val : valeur à insérer
	\author Baudouin Feildel
**/
void setTiltFrontBack(ardrone* dr, float val) { dr->tiltFrontBack = (val <= 1 && val >= -1) ? val:0; }

/** \fn void setTiltLeftRight(ardrone* dr, float val)
	\ingroup ATCommands
	\brief Mettre une valeur de manière sécurisée dans le buffer de l'inclinaison gauche/droite du drone dr
	\param ardrone* dr : Handle du drone
	\param float val : valeur à insérer
	\author Baudouin Feildel
**/
void setTiltLeftRight(ardrone* dr, float val) { dr->tiltLeftRight = (val <= 1 && val >= -1) ? val:0; }

/** \fn bool sendAT(ardrone* dr)
	\ingroup ATCommands
	\brief Envoyer la commande AT qui est dans le buffer du drone dr
	\param ardrone* dr : Handle du drone
	\return true : buffer envoyé ; false buffer non-envoyé
	\author Baudouin Feildel
**/
bool sendAT(ardrone* dr);

/** \fn void run(ardrone* dr)
	\ingroup ATCommands
	\brief Thread du mode vol
	\param ardrone* dr : Handle du drone
	\author Baudouin Feildel
**/
void run(ardrone* dr);

/*********************
 *                   * 
 *      main         *
 *                   *
 *********************/
void main(void)
{
	udp_Socket sock;
	char buff[1025];
	
	i2c_init();
	
#ifdef MODE_DEBUG
	printf("\n\nDEMARRAGE\n\n");
#endif
	connexion();
#ifdef MODE_DEBUG
	printf("\n\nCONNECTE\n\n");
#endif
	

	
	strcpy(buff, "AT*CONFIG=1,\"control:altitude_max\",\"2000\"\r");
	
	
	if(!udp_open(&sock, LOCAL_PORT, resolve(REMOTE_IP), REMOTE_PORT, NULL))
	{
#ifdef MODE_DEBUG
		printf("udp_open failed!\n");
#endif
	}
	for(;;)
	{
		tcp_tick(NULL);
		costate
		{
			waitfor(IntervalSec(HEARTBEAT_RATE));
			send_packet(buff, REMOTE_IP, LOCAL_PORT, &sock);
		}
	}
	
	
	/*sprintf(buf, "AT*CONFIG=%d,\"control:altitude_max\",\"2000\"\r", ++ident);
	sprintf(buf, "AT*REF=%d,290718208\r", ++ident);
	sprintf(buf, "AT*REF=%d,290717696\r", ++ident);*/
	
	for(;;);
}

/*********************
 *                   * 
 *     functions     *
 *                   *
 *********************/
unsigned char lireCommandes(etat_commandes *s)
{
	unsigned char erreurs = 0;
	char buff; // todo : à voir
	s->bp_arret_urgence = BitRdPortI(PEDR, 3);
	s->bp_video = BitRdPortI(PEDR, 6);
	s->bp_trim = BitRdPortI(PEDR, 1);
	s->switch_land = BitRdPortI(PEDR, 7);
	
	if(!I2CRead(ADCSLAVE, 0, &buff, 1)) // pas d'erreur
		s->joystick_1x = buff;
	else
		erreurs += 0x80;
	if(!I2CRead(ADCSLAVE, 1, &buff, 1)) // pas d'erreur
		s->joystick_1y = buff;
	else
		erreurs += 0x40;
	if(!I2CRead(ADCSLAVE, 2, &buff, 1)) // pas d'erreur
		s->joystick_2x = buff;
	else
		erreurs += 0x20;
	if(!I2CRead(ADCSLAVE, 3, &buff, 1)) // pas d'erreur
		s->joystick_2y = buff;
	else
		erreurs += 0x10;
	
	erreurs += ((erreurs & 0x10) >> 3) + ((erreurs & 0x20) >> 4) + ((erreurs & 0x40) >> 5) + ((erreurs & 0x80) >> 6); // nombre d'erreur
}

void ecrireCommandes(etat_commandes *s)
{
	BitWrPortI(PEDR, &PEDRShadow, s->led_connecte, 0);
	BitWrPortI(PEDR, &PEDRShadow, s->led_erreur, 2);
	BitWrPortI(PEDR, &PEDRShadow, s->led_debug, 5);
}

int send_packet(char* str, const char far * ip, word port, udp_Socket *sock)
{	
	// envoi le paquet
	if(udp_send(sock, str, strlen(str) + 1) < 0) // s'il y a une erreur
	{
#ifdef MODE_DEBUG
		printf("Erreur d'envoi du paquet :\n;;;%s\n", str);
#endif
		sock_close(sock);	// ferme la socket puis la re-ouvre
		if(!udp_open(sock, port, resolve(ip), port, NULL))
		{
#ifdef MODE_DEBUG
			printf("Erreur de reouverture de la socket\n");
#endif
			return -1;
		}
		else
		{
#ifdef MODE_DEBUG
			printf("Erreur de reouverture de la socket\n");
#endif
			return -2;
		}
	}
#ifdef MODE_DEBUG
   else
		printf("Paquet envoye :\n;;;%s\n", str);
#endif

	tcp_tick(NULL);
	return 0;
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
#ifdef MODE_DEBUG
	printf("\nResultat du scan des points d'acces :\n");
#endif
	for(i = 0; i < data->count; i++)	// affiche les points d'accès disponibles
	{
		wifi_ssid_to_str(ssid_str, bss[i].ssid, bss[i].ssid_len);
#ifdef MODE_DEBUG
		printf("[%2d] trouve : %s\n", i, ssid_str);
#endif
	}
	
#ifdef MODE_DEBUG
	printf("\nRecherche du drone :\n");
#endif
	for(i = 0; i < data->count && connect != TROUVE; i++)
	{
		wifi_ssid_to_str(ssid_str, bss[i].ssid, bss[i].ssid_len);	// donne une chaîne de caractère imprimable
		
#ifdef MODE_DEBUG
		printf("[%2d] comparaison de '%s' avec 'ardrone2_xxxxxx' : ", i, ssid_str);
#endif
		
		ctemp = ssid_str[9];	// sauvegarde du caractère
		ssid_str[9] = '\0';		// tronque à 9 caractères (ardrone2_)
		
		if(!strcmp("ardrone2_", ssid_str))	// si le ssid correspond
		{
#ifdef MODE_DEBUG
			printf("oui\n");
#endif
			ssid_str[9] = ctemp;	// remet du caractère enlevé
			
			// Need near copy of SSID to call ifconfig.  ssid will be promoted to
			// far for this call, but the results will be in ssid as a near
			// variable
			ssid_len = bss[i].ssid_len;
			_f_memcpy(ssid, bss[i].ssid, ssid_len);
			
			if(!ifconfig(IF_WIFI0, IFS_WIFI_SSID, ssid_len, ssid, IFS_END))	// demande la connexion
			{
				wifi_ssid_to_str(ssid, ssid, ssid_len);
#ifdef MODE_DEBUG
				printf(" => demande de connexion a %s...\n", ssid);
#endif
				connect = TROUVE;	// indique que le drone est trouvé
				// puis il faut attendre que la connexion soit effective (dans connexion())
			}
#ifdef MODE_DEBUG
			else
				printf("=> erreur de demande de connexion a %s", ssid_str);
#endif
		}
#ifdef MODE_DEBUG
		else
			printf("non");
		putchar('\n');
#endif
	}
#ifdef MODE_DEBUG
	if(connect != TROUVE)
	{
		printf("Aucun point d'acces de drone trouve\n");
	}
#endif
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
#ifdef MODE_DEBUG
		printf("* Essai de connexion *\n");
#endif
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
#ifdef MODE_DEBUG
			printf("Attente pour que la connexion soit effective\n");
#endif
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
#ifdef MODE_DEBUG
				printf("connexion effective !\n");
#endif
				connect = CONNECTE;
			}
			else
			{
#ifdef MODE_DEBUG
				printf("la tentative de connexion a echoue... Nouvel essai\n");
#endif
				connect = RECHERCHE;
			}
		}
#ifdef MODE_DEBUG
		else
			printf("\nTimeout de connexion...\n");
#endif
	}
}

bool connectToDrone(ardrone* dr)
{
    // Création de la socket d'envoi des commande AT
    if(!udp_open(&udpSocket_at, LOCAL_PORT, resolve(dr->port_at), dr->port_at, NULL))
	{
#ifdef MODE_DEBUG
		printf("Failed to open udp socket with the drone on port %d!\n", dr->port_at);
#endif
		return false;
	}

    return true;
}

bool sendAT(ardrone* dr)
{
	char lenght;
	char tmp;
    dr->ident++;
	sprintf(dr->buff, "%s%d%s", dr->bufferLeft, dr->ident, dr->bufferRight);
	tmp = send_packet(dr->buff, dr->host, dr->port_at, &dr->udpSocket_at);
    if(tmp == -1 || tmp == -2)
        return false;
    return true;
}

bool initDrone(ardrone* dr)
{
    // Configure la hauteur maximale du drone
    dr->bufferLeft = "AT*CONFIG=";
    dr->bufferRight = ",\"control:altitude_max\",\"3000\"\r";
#ifdef MODE_DEBUG
    printf("#%5d - Config alt max - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
#endif
    sendAT(dr);

    // Demande l'envoi de donnée de navigation
    dr->bufferLeft = "AT*CONFIG=";
    dr->bufferRight = ",\"general:navdata_demo\",\"TRUE\"\r";
#ifdef MODE_DEBUG
    printf("#%5d - Start navdata flow - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
#endif
    sendAT(dr);

    // Indique au drone qu'il est à plat : le drone se calibre
    dr->bufferLeft = "AT*FTRIM=";
    dr->bufferRight = ",\r";
#ifdef MODE_DEBUG
    printf("#%5d - Ftrim - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
#endif

    return sendAT(dr);
}

bool takeoff(ardrone* dr)
{
    // Décollage du drone
    dr->bufferLeft = "AT*REF=";
    dr->bufferRight = ",290718208\r";
#ifdef MODE_DEBUG
    printf("#%5d - Take off - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
#endif
    sendAT(dr);

    // Passe en mode vol
    dr->bufferLeft = "AT*PCMD=";
    dr->bufferRight = ",1,0,0,0,0\r";
    setGoUpDown(dr, 0);
    setTurnLeftRight(dr, 0);
    setTiltFrontBack(dr, 0);
    setTiltLeftRight(dr, 0);
    dr->fly = true;
    // Voir comment lancer le thread du mode vol.
	//this->start();

    return true;
}

bool land(ardrone* dr)
{
    dr->fly = false;
    dr->bufferLeft = "AT*REF=";
    dr->bufferRight = ",290717696\r";
#ifdef MODE_DEBUG
    printf("#%5d - Land - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
#endif

    return sendAT(dr);
}

bool aru(ardrone* dr)
{
    dr->fly = false;
    dr->bufferLeft = "AT*REF=";
    dr->bufferRight = ",290717952\r";
#ifdef MODE_DEBUG
    printf("#%5d - Arrêt d'urgence - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
#endif

    return sendAT(dr);
}

bool volCommand(ardrone* dr, float tiltLeftRight_, float tiltFrontBack_, float goUpDown_, float turnLeftRight_)
{
    char strBuff[50];
    sprintf(strBuff, ",1,%d,%d,%d,%d\r", *(int*)(&tiltLeftRight_),
                                         *(int*)(tiltFrontBack_),
                                         *(int*)(goUpDown_),
                                         *(int*)(turnLeftRight_));
    dr->bufferLeft =  "AT*PCMD=";
    dr->bufferRight = strBuff;
#ifdef MODE_DEBUG
    printf("#%5d - PCMD - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
#endif
    return sendAT(dr);
}

void run(ardrone* dr)
{
    while(dr->fly)
    {
        volCommand(dr, dr->tiltLeftRight, dr->tiltFrontBack, dr->goUpDown, dr->turnLeftRight);
    }
}


