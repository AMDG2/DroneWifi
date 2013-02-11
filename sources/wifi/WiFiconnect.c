/**
	\defgroup Connexion
	\defgroup UDP
	\defgroup IO
	\defgroup AT
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

//#define MODE_DEBUG 1


/** \typedef statut_connexion Permet de définir le statut de la connexion en cours
	\ingroup Connexion
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
	char switch_land;	// 1 : position bas
	
	char joystick_1x;
	char joystick_1y;
	char joystick_2x;
	char joystick_2y;
} etat_commandes;

#use "dcrtcp.lib"

#use "rcm56xxw.lib"


#define I2CSCLBit 1
#define I2CSDABit 0
#use "I2C_DEVICES.lib"





// adresse de l'esclave ADC : 000
#define ADCSLAVE 0

#memmap xmem
/**	\var connect variable globale qui défini le statut de la connexion en cours
	\ingroup Connexion
**/
statut_connexion connect = INOCCUPE;



/**	\fn void init_etat_commandes(etat_commandes *s)
	\ingroup IO
	\brief initialise la structure structure de l'état des commandes
	\param etat_commandes structure de l'état des commandes
	\author Thibaut Marty
**/
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

/**	\fn void print_etat_commandes(etat_commandes s)
	\ingroup IO
	\brief initialise la structure structure de l'état des commandes
	\param etat_commandes structure de l'état des commandes
	\author Thibaut Marty
**/
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

/**	\fn unsigned char lireCommandes(etat_commandes *s)
	\ingroup IO
	\brief remplie la structure de l'état des commandes avec de nouvelles valeurs lues
	\param etat_commandes structure de l'état des commandes
	\return erreurs sur la lecture des valeurs de joystick : {a b c d 0 EEE}.
		 a : erreur sur J1x
		 b : erreur sur J1y
		 c : erreur sur J2x
		 d : erreur sur J2y
		 EEE : nombre d'erreur au total (0 à 4)
	\author Thibaut Marty
**/
unsigned char lireCommandes(etat_commandes *s)
{
	unsigned char erreurs = 0;
	char buff; // todo : à voir
	s->bp_arret_urgence = BitRdPortI(PEDR, 2);
	s->bp_video = BitRdPortI(PEDR, 0);
	s->bp_trim = BitRdPortI(PEDR, 5);
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

/**	\fn void ecrireCommandes(etat_commandes s)
	\ingroup IO
	\brief écris les valeurs de la structure de l'état des commandes en sortie
	\param etat_commandes structure de l'état des commandes
	\author Thibaut Marty
**/
void ecrireCommandes(etat_commandes s)
{
	BitWrPortI(PEDR, &PEDRShadow, s.led_connecte, 1);
	BitWrPortI(PEDR, &PEDRShadow, s.led_erreur, 3);
	BitWrPortI(PEDR, &PEDRShadow, s.led_debug, 6);
}


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

/**	\fn int rxsignal_cmp(far _wifi_wln_scan_bss *a, far _wifi_wln_scan_bss *b)
	\ingroup Connexion
	\brief fonction qui permet le trie les points d'accès WiFi par puissance du signal
	\param a un _wifi_wln_scan_bss qui correspond à un point d'accès
	\param b un _wifi_wln_scan_bss qui correspond à un point d'accès
	\author Dynamic C
**/
int rxsignal_cmp(far _wifi_wln_scan_bss *a, far _wifi_wln_scan_bss *b)
{
	return b->rx_signal - a->rx_signal;
}

/**	\fn root void scan_assoc_callback(far wifi_scan_data* data)
	\ingroup Connexion
	\brief fonction de callback qui traite les points d'accès donnés :
		recherche le point d'accès du drone et se connecte
	\param data liste des points d'accès
	\author Thibaut Marty
**/
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

/**	\fn void connexion(void)
	\ingroup Connexion
	\brief fonction qui se connecte au drone. Elle se quitte uniquement si la connexion est effective
	\author Thibaut Marty
**/
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

void main(void)
{
	etat_commandes ec;
	unsigned char v, r;
	
	brdInit();
	
	init_etat_commandes(&ec);
	for(;;)
	{
		costate
		{
			ec.led_connecte = v & 0x01;
			ec.led_erreur = v & 0x02;
			ec.led_debug = v & 0x04;
			ecrireCommandes(ec);
			v++;
			r = lireCommandes(&ec);
			printf("\nnombre d'erreurs : %d (%s%s%s%s)\n", r & 0x07, (r & 0x80) ? "J1x " : "", (r & 0x40) ? "J1y " : "", (r & 0x20) ? "J2x " : "", (r & 0x10) ? "J2y " : "");
			print_etat_commandes(ec);
			waitfor(DelayMs(1000));
		}
	}
	/*
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
	}*/
	
	
	/*sprintf(buf, "AT*CONFIG=%d,\"control:altitude_max\",\"2000\"\r", ++ident);
	sprintf(buf, "AT*REF=%d,290718208\r", ++ident);
	sprintf(buf, "AT*REF=%d,290717696\r", ++ident);*/
	
	for(;;);
}

