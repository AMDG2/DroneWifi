/*
 * NETWORK CONFIGURATION
 * Please see the function help (Ctrl-H) on TCPCONFIG for instructions on
 * compile-time network configuration.
 */

#define TCPCONFIG 5
//#define MODE_DEBUG 1

typedef enum
{
  INOCCUPE,
	RECHERCHE,
	TROUVE,
	CONNECTE
} connect_status;

#use "dcrtcp.lib"

// pour ledDS1()
#use "rcm56xxw.lib"

#memmap xmem

connect_status connect = INOCCUPE;

void ledDS1(char state)
{
	BitWrPortI(PDDR, &PDDRShadow, !state, 0/*ds1*/);
}



/****************************************************************************
	rxsignal_cmp

	qsort comparison, based on rx signal strength.

   Inputs:
      a, b  -- far pointers to _wifi_wln_scan_bss, a struct populated by
               the WIFI SCAN, including rx_signal (relative receive signal
               strength).

	Return value: > 0 if a > b
					  < 0 if a < b
					  0   if a == b

****************************************************************************/
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
		
		while(!tcp_tick(NULL));	// attend que tout ce qu'il faut soit fait
		
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

void main(void)
{
	brdInit();
	ledDS1(0);
#ifdef MODE_DEBUG
	printf("\n\nDEMARRAGE\n\n");
#endif
	connexion();
#ifdef MODE_DEBUG
	printf("\n\nCONNECTE\n\n");
#endif
	ledDS1(1);
	for(;;);
	/*{
		ifconfig(IF_WIFI0, IFG_WIFI_STATUS, &status, IFS_END);
		while(!tcp_tick(NULL));	// attend que tout ce qu'il faut soit fait
		print_status(&status);
		printf(" => affichage du statut mac > SSID : %*s", _wifi_macStatus.ssid_len, _wifi_macStatus.ssid);
		while(!kbhit())
			tcp_tick(NULL);
		getchar();
	}*/
}

