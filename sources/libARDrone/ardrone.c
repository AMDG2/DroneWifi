#include "ardrone.h"

bool connectToDrone(ardrone* dr)
{
	int reuse = 1;
    // Création de la socket d'envoi des commande AT
    //if(!udp_open((udp_Socket*) &(dr->udpSocket_at), LOCAL_PORT, resolve(dr->host), dr->port_at, NULL))
    dr->udpSocket_at = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(dr->udpSocket_at < 0)
	{
		fprintf(stderr, "## Error %d : Failed to create udp socket for AT commands.\n", dr->udpSocket_at);
		return false;
	}
	
	dr->udpSocket_navData = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(dr->udpSocket_navData < 0)
	{
		fprintf(stderr, "## Error %d : Failed to create udp socket for nav-data.\n", dr->udpSocket_navData);
		return false;
	}
	
	/// Initialisation de la structure serverAddr
	dr->serverAddr.sin_family = dr->hostInfo->h_addrtype;
	memcpy(	(char *) &(dr->serverAddr.sin_addr.s_addr),
			dr->hostInfo->h_addr_list[0],
			dr->hostInfo->h_length);
	dr->serverAddr.sin_port = htons(dr->port_at);
	
	/// Initialisation de la structure serverListS
	dr->serverListS.sin_family = dr->hostInfo->h_addrtype;
	memcpy(	(char*)&(dr->serverListS.sin_addr.s_addr),
			dr->hostInfo->h_addr_list[0],
			dr->hostInfo->h_length);
	dr->serverListS.sin_port = htons(dr->port_navdata);
	
	/// Initialisation de la structure serverListen
	dr->serverListen.sin_family = PF_INET;
	dr->serverListen.sin_addr.s_addr = htonl(INADDR_ANY);
	dr->serverListen.sin_port = htons(dr->port_navdata);
	if(bind(dr->udpSocket_navData, (struct sockaddr*)&(dr->serverListen), sizeof(dr->serverListen)) == -1)
	{
		fprintf(stderr, "## Error : bind failed\n");
		close(dr->udpSocket_navData);
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
	/*tmp = send_packet(dr->buff, dr->host, dr->port_at, &dr->udpSocket_at);
    if(tmp == -1 || tmp == -2)
        return false;
    return true;*/
     if (sendto(dr->udpSocket_at, dr->buff, strlen(dr->buff), 0,
				(struct sockaddr *) &(dr->serverAddr),
				sizeof(dr->serverAddr)) < 0)
     {
     	fprintf(stderr, "## Error AT : Impossible d'émettre la trame %d\n", dr->ident);
     	return false;
     }
     return true;
}

bool sendNavData(ardrone* dr, const char* buffer)
{
     if (sendto(dr->udpSocket_navData, buffer, strlen(buffer), 0,
				(struct sockaddr *) &(dr->serverListS),
				sizeof(dr->serverListS)) < 0)
     {
     	fprintf(stderr, "## Error NavData : Impossible d'émettre la trame '%s'\n", buffer);
     	return false;
     }
     return true;
}

bool initDrone(ardrone* dr)
{
    // Configure la hauteur maximale du drone
    strcpy(dr->bufferLeft, "AT*CONFIG=");
    strcpy(dr->bufferRight, ",\"control:altitude_max\",\"3000\"\r");
    fprintf(stdout, "#%5d - Config alt max - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
    if(!sendAT(dr))
    	return false;

    // Indique au drone qu'il est à plat : le drone se calibre
    strcpy(dr->bufferLeft, "AT*FTRIM=");
    strcpy(dr->bufferRight, ",\r");
    fprintf(stdout, "#%5d - Ftrim - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);

    return sendAT(dr);
}

bool initNavData(ardrone* dr)
{
	static char atCtrl[] = "AT*CTRL=0\r", un[14] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0};
	// Envoi d'un paquet sur le port de données de navigation
	fprintf(stdout, "#      - Send some bytes on navdata port\n");
	if(!sendNavData(dr, "Hello World !"))
		return false;
	
	// Demande l'envoi de donnée de navigation
    strcpy(dr->bufferLeft, "AT*CONFIG=");
    strcpy(dr->bufferRight, ",\"general:navdata_demo\",\"TRUE\"\r");
    fprintf(stdout, "#%5d - Start navdata flow %s%d%s\n", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);    
    if(!sendAT(dr))
    	return false;

	if (sendto(dr->udpSocket_at, atCtrl, strlen(atCtrl), 0,
		(struct sockaddr *) &(dr->serverAddr),
		sizeof(dr->serverAddr)) < 0)
	{
		fprintf(stderr, "## Error : Impossible d'émettre la trame AT*CTRL=0\n", dr->ident);
		return false;
	}
	else
		fprintf(stdout, "#      - AT*CTRL=0 emitted.\n");
	
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
	// Do something with the wathdog
    strcpy(dr->bufferLeft, "AT*COMWDG=");
    strcpy(dr->bufferRight, ",\r");
    //fprintf(stdout, "#%5d - Do something with the watchdog for Navdata flow - %s%d%s\n", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
    if(!sendAT(dr))
    {
    	fprintf(stderr, "##     - Unable to reset the watchdog.\n");
    	return -1;
    }
    
	//fprintf(stdout, "Wait for data ...\n");
    ret = recvfrom(dr->udpSocket_navData, buffer, bufferLenght, 0, (struct sockaddr*)sender, senderLen);
    if(ret <= 0)
    	fprintf(stderr, "Impossible de réceptionner des données : %d\n", ret);
    return ret;
}

bool takeoff(ardrone* dr)
{
    // Décollage du drone
    strcpy(dr->bufferLeft, "AT*REF=");
    strcpy(dr->bufferRight, ",290718208\r");
    fprintf(stdout, "#%5d - Take off - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
    if(!sendAT(dr))
    	return false;

    // Passe en mode vol
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
    dr->fly = false;
    strcpy(dr->bufferLeft, "AT*REF=");
    strcpy(dr->bufferRight, ",290717696\r");
    fprintf(stdout, "#%5d - Land - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);

    return sendAT(dr);
}

bool aru(ardrone* dr)
{
    dr->fly = false;
    strcpy(dr->bufferLeft, "AT*REF=");
    strcpy(dr->bufferRight, ",290717952\r");
    fprintf(stdout, "#%5d - Arrêt d'urgence - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);

    return sendAT(dr);
}

bool volCommand(ardrone* dr, float tiltLeftRight_, float tiltFrontBack_, float goUpDown_, float turnLeftRight_)
{
    char strBuff[50];
    sprintf(strBuff, ",1,%d,%d,%d,%d\r", *(int*)(&tiltLeftRight_),
                                         *(int*)(&tiltFrontBack_),
                                         *(int*)(&goUpDown_),
                                         *(int*)(&turnLeftRight_));
    strcpy(dr->bufferLeft, "AT*PCMD=");
    strcpy(dr->bufferRight, strBuff);
    fprintf(stdout, "#%5d - PCMD - %s%d%s", dr->ident+1, dr->bufferLeft, dr->ident+1, dr->bufferRight);
    return sendAT(dr);
}

void      setGoUpDown(ardrone* dr, float val) {      dr->goUpDown = (val <= 1 && val >= -1) ? val:0; }
void setTurnLeftRight(ardrone* dr, float val) { dr->turnLeftRight = (val <= 1 && val >= -1) ? val:0; }
void setTiltFrontBack(ardrone* dr, float val) { dr->tiltFrontBack = (val <= 1 && val >= -1) ? val:0; }
void setTiltLeftRight(ardrone* dr, float val) { dr->tiltLeftRight = (val <= 1 && val >= -1) ? val:0; }

ardrone* newARDrone(void)
{
	ardrone* tmp;
	tmp = malloc(sizeof(ardrone));
	
	tmp->hostInfo = gethostbyname("192.168.1.1");
    tmp->port_at = 5556;
    tmp->port_navdata = 5554;

    tmp->ident = 0;
    tmp->fly = false;
    
    return tmp;
}

void closeARDrone(ardrone* dr)
{
	close(dr->udpSocket_at);
	close(dr->udpSocket_navData);
	free(dr);
}
