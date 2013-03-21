#include "ardrone.h"

bool connectToDrone(ardrone* dr)
{
    /// Commence par créer la socket d'envoi des commande AT.
    dr->udpSocket_at = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(dr->udpSocket_at < 0)
	{
		fprintf(stderr, "## Error %d : Failed to create udp socket for AT commands.\n", dr->udpSocket_at);
		return false;
	}
	
	/// Ensuite crée la socket d'envoi et d'écoute des données de navigation.
	dr->udpSocket_navData = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(dr->udpSocket_navData < 0)
	{
		fprintf(stderr, "## Error %d : Failed to create udp socket for nav-data.\n", dr->udpSocket_navData);
		return false;
	}
	
	/* Puis initialise la structure addrDroneAT qui identifie le drone par son adresse IP et le port des commandes AT.
		Cette structure permet d'envoyer des commandes AT en l'utilisant avec la socket udpSocket_at.
	*/
	dr->addrDroneAT.sin_family = dr->hostInfo->h_addrtype;
	memcpy(	(char *) &(dr->addrDroneAT.sin_addr.s_addr),
			dr->hostInfo->h_addr_list[0],
			dr->hostInfo->h_length);
	dr->addrDroneAT.sin_port = htons(dr->port_at);
	
	/* Ensuite initialise la structure addrDroneNavDataW qui identifie le drone par son adresse IP et le port des données de navigation.
		Cette structure permet d'envoyer des données sur le port des données de navigation en l'utilisant avec les socket udpSocket_navData.
		Le W à la fin du nom signifie qu'elle permet d'envoyer des données W=WRITE.
	*/
	dr->addrDroneNavDataW.sin_family = dr->hostInfo->h_addrtype;
	memcpy(	(char*)&(dr->addrDroneNavDataW.sin_addr.s_addr),
			dr->hostInfo->h_addr_list[0],
			dr->hostInfo->h_length);
	dr->addrDroneNavDataW.sin_port = htons(dr->port_navdata);
	
	/* Enfin initialise la structure addrDroneNavDataR qui identifie le drone par son adresse IP et le port des données de navigation.
		Cette structure permet de recevoir des données sur le port des données de navigation en l'utilisant avec la socket udpSocket_navData.
		Le R à la fin du nom signifie qu'elle permet la lecture des données R=READ.		
	*/
	dr->addrDroneNavDataR.sin_family = PF_INET;
	dr->addrDroneNavDataR.sin_addr.s_addr = htonl(INADDR_ANY);
	dr->addrDroneNavDataR.sin_port = htons(dr->port_navdata);
	
	/// Pour terminer ouvre la socket udpSocket_navData pour qu'elle écoute les données venant depuis l'hôte identifié par la structure addrDroneNavDataR (bind).
	/// Si le bind échoue : indique à l'utilisateur l'erreur, puis retourne false.
	if(bind(dr->udpSocket_navData, (struct sockaddr*)&(dr->addrDroneNavDataR), sizeof(dr->addrDroneNavDataR)) == -1)
	{
		fprintf(stderr, "## Error : bind failed\n");
		close(dr->udpSocket_navData);
		return false;
	}
	/// Sinon : retourne true.
    return true;
}

bool sendAT(ardrone* dr)
{
	char lenght;
	char tmp;
	
	/** On commence par incrémenter l'identifiant de commande.
		Puis on prépare la trame à envoyer en joignant les deux buffers et en ajoutant l'identifiant de commande.
	*/
    dr->ident++;
	sprintf(dr->buff, "%s%d%s", dr->bufferLeft, dr->ident, dr->bufferRight);
	
	/// Enfin envoie la commande.
	/// Si l'envoi a échoué : indique dans la sortie standard d'erreur le problème et retourne false.
	if(sendto(dr->udpSocket_at, dr->buff, strlen(dr->buff), 0,
			  (struct sockaddr *) &(dr->addrDroneAT),
			  sizeof(dr->addrDroneAT)) < 0)
	{
		fprintf(stderr, "## Error AT : Impossible d'émettre la trame %d\n", dr->ident);
		return false;
	}
	/// Sinon : retourne vrai.
	return true;
}

bool sendNavData(ardrone* dr, const char* buffer)
{
	/// Envoie un datagramme sur le réseau, simplement en utilisant la socket de navdata, et la structure addrDroneNavDataW.
	
	/// Si l'envoi a échoué : indique dans la sortie standard d'erreur le problème et retourne false.
	if (sendto(dr->udpSocket_navData, buffer, strlen(buffer), 0,
		(struct sockaddr *) &(dr->addrDroneNavDataW),
		sizeof(dr->addrDroneNavDataW)) < 0)
	{
		fprintf(stderr, "## Error NavData : Impossible d'émettre la trame '%s'\n", buffer);
		return false;
	}
	/// Sinon : retourne vrai.
	return true;
}

bool initDrone(ardrone* dr)
{
    /// Commence par configurer la hauteur maximale du drone.
    strcpy(dr->bufferLeft, "AT*CONFIG=");
    strcpy(dr->bufferRight, ",\"control:altitude_max\",\"3000\"\r");
    fprintf(stdout, "#%5d - Config alt max - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
    if(!sendAT(dr))
    	return false;

    /// Puis indique au drone qu'il est à plat : le drone se calibre.
    strcpy(dr->bufferLeft, "AT*FTRIM=");
    strcpy(dr->bufferRight, ",\r");
    fprintf(stdout, "#%5d - Ftrim - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
    if(!sendAT(dr))
    	return false;

	/// Enfin, suivant la configuration réalisée avec un #define, indique au drone qu'il vole en extérieur, ou en intérieur, avec la carène correspondante.
	
	/// N.B. : Cette configuration est réalisée à l'aide d'un bouton sur la télécommande.
	strcpy(dr->bufferLeft, "AT*CONFIG=");
#ifdef VOL_INTERIEUR
	strcpy(dr->bufferRight, ",\"control:outdoor\", \"FALSE\"\r");
	if(!sendAT(dr))
		return false;
	
	strcpy(dr->bufferRight, ",\"control:flight_without_shell\", \"FALSE\"\r");
#else
	strcpy(dr->bufferRight, ",\"control:outdoor\", \"TRUE\"\r");
	if(!sendAT(dr))
		return false;
	
	strcpy(dr->bufferRight, ",\"control:flight_without_shell\", \"TRUE\"\r");
#endif
    return sendAT(dr);
}

bool initNavData(ardrone* dr)
{
	static char atCtrl[] = "AT*CTRL=0\r", un[14] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0};
	/// Commence par envoyer un paquet sur le port des données de navigation, pour que le drone commence à écouter.
	fprintf(stdout, "#      - Send some bytes on navdata port\n");
	if(!sendNavData(dr, "Hello World !"))
		return false;
	
	/// Puis demande l'envoi de donnée de navigation.
    strcpy(dr->bufferLeft, "AT*CONFIG=");
    strcpy(dr->bufferRight, ",\"general:navdata_demo\",\"TRUE\"\r");
    fprintf(stdout, "#%5d - Start navdata flow %s%d%s\n", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);    
    if(!sendAT(dr))
    	return false;

	/// Ensuite envoie une trame de contrôle pour indiquer au drone qu'il peut envoyer des données.
	if (sendto(dr->udpSocket_at, atCtrl, strlen(atCtrl), 0,
		(struct sockaddr *) &(dr->addrDroneAT),
		sizeof(dr->addrDroneAT)) < 0)
	{
		fprintf(stderr, "## Error : Impossible d'émettre la trame AT*CTRL=0\n", dr->ident);
		return false;
	}
	else
		fprintf(stdout, "#      - AT*CTRL=0 emitted.\n");
	
	/// Puis envoie une configuration pour demander au drone d'envoyer les données de navigation en multicast.
	if(!sendNavData(dr, un))
	{
		fprintf(stderr, "## Error : Impossible d'émettre un 1\n");
		return false;
	}
	else
		fprintf(stdout, "#      - 1 envoyé.\n");
	return true;
}

int receiveNavData(ardrone* dr, struct sockaddr_in* sender, socklen_t* senderLen, char* buffer, int bufferLenght)
{
	int ret = 0;
	/// Commence par "réveiller" le drone en envoyant un paquet pour redémarrer le WatchDog.
    strcpy(dr->bufferLeft, "AT*COMWDG=");
    strcpy(dr->bufferRight, ",\r");
    //fprintf(stdout, "#%5d - Restart the watchdog for Navdata flow - %s%d%s\n", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
    if(!sendAT(dr))
    {
    	fprintf(stderr, "##     - Unable to reset the watchdog.\n");
    	return -1;
    }
    
    /// Puis attend de récupérer les données de navigation.
	//fprintf(stdout, "Wait for data ...\n");
    ret = recvfrom(dr->udpSocket_navData, buffer, bufferLenght, 0, (struct sockaddr*)sender, senderLen);
    /// Une fois que les données sont reçues, vérifie que la fonction a bien écrit des caractères dans le buffer.
    if(ret <= 0)
    	fprintf(stderr, "Impossible de réceptionner des données : %d\n", ret);

	/// Enfin retourne le nombre de caractères écris dans le buffer.
    return ret;
}

bool takeoff(ardrone* dr)
{
    /// Demande le décollage du drone.
    strcpy(dr->bufferLeft, "AT*REF=");
    strcpy(dr->bufferRight, ",290718208\r");
    fprintf(stdout, "#%5d - Take off - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
    if(!sendAT(dr))
    	return false;

    /// Si la demande a réussi passe en mode vol.
    strcpy(dr->bufferLeft, "AT*PCMD=");
    strcpy(dr->bufferRight, ",1,0,0,0,0\r");
    setGoUpDown(dr, 0);
    setTurnLeftRight(dr, 0);
    setTiltFrontBack(dr, 0);
    setTiltLeftRight(dr, 0);
    dr->fly = true;

    return true;
}

bool land(ardrone* dr)
{
	/// On commence par sortir du mode vol.
    dr->fly = false;
    
    /// Puis demande l'atterrissage.
    strcpy(dr->bufferLeft, "AT*REF=");
    strcpy(dr->bufferRight, ",290717696\r");
    fprintf(stdout, "#%5d - Land - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);

    return sendAT(dr);
}

bool aru(ardrone* dr)
{
	/// Commence par sortir du mode de vol.
    dr->fly = false;
    /// Ensuite envoie un paquet d'arrêt d'urgence.
    strcpy(dr->bufferLeft, "AT*REF=");
    strcpy(dr->bufferRight, ",290717952\r");
    fprintf(stdout, "#%5d - Arrêt d'urgence - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);

    return sendAT(dr);
}

bool volCommand(ardrone* dr, joystick tiltLeftRight_, joystick tiltFrontBack_, joystick goUpDown_, joystick turnLeftRight_)
{
    char strBuff[50];
    /// Commence par préparer la trame à envoyer en écrivant dans un buffer les valeurs flottantes, récupérées en paramètres, sous forme de int. Cela revient à écrire un int dont l'empreinte binaire est celle du flottant correspondant passé en paramètre.
    sprintf(strBuff, ",1,%d,%d,%d,%d\r", tiltLeftRight_.i,
                                         tiltFrontBack_.i,
                                         goUpDown_.i,
                                         turnLeftRight_.i);
    /// Envoie le datagramme.
    strcpy(dr->bufferLeft, "AT*PCMD=");
    strcpy(dr->bufferRight, strBuff);
    fprintf(stdout, "#%5d - PCMD - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
    return sendAT(dr);
}

void      setGoUpDown(ardrone* dr, float val) {      dr->goUpDown.f = (val <= 1 && val >= -1) ? val:0; }
void setTurnLeftRight(ardrone* dr, float val) { dr->turnLeftRight.f = (val <= 1 && val >= -1) ? val:0; }
void setTiltFrontBack(ardrone* dr, float val) { dr->tiltFrontBack.f = (val <= 1 && val >= -1) ? val:0; }
void setTiltLeftRight(ardrone* dr, float val) { dr->tiltLeftRight.f = (val <= 1 && val >= -1) ? val:0; }

ardrone* newARDrone(void)
{
	ardrone* tmp;
	/// Réserve de l'espace mémoire pour notre structure.
	tmp = malloc(sizeof(ardrone));
	
	/// Remplit la structure avec l'adresse du drone, les différents ports de communication avec le drone.
	tmp->hostInfo = gethostbyname("192.168.1.1");
    tmp->port_at = 5556;
    tmp->port_navdata = 5554;

	/// Initialise l'identifiant de commande et sort du mode de vol.
    tmp->ident = 0;
    tmp->fly = false;
    
    /// Enfin retourne l'adresse mémoire de l'espace précédemment alloué.
    return tmp;
}

void closeARDrone(ardrone* dr)
{
	/// Commence par fermer les sockets, puis libère l'espace mémoire alloué dynamiquement.
	close(dr->udpSocket_at);
	close(dr->udpSocket_navData);
	free(dr);
}
