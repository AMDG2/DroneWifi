#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ardrone.h"

int main(int argc, char** argv)
{
	ardrone* drone = newARDrone();
	struct sockaddr_in sender;
	socklen_t senderLen = sizeof(sender);
	unsigned char b[500];
	int tmp, i, j;
	navdata_t trame;
	navdata_demo_t navdata;
	
	if(!connectToDrone(drone))
	{
		fprintf(stderr, "Impossible d'ouvrir la socket de connexion au drone.\n");
		return -1;
	}
	fprintf(stdout, "[OK] Connexion au drone réussie.\n");
	
	/*if(!initDrone(drone))
		fprintf(stderr, "Impossible d'initialiser le drone.\n");
	else
		fprintf(stdout, "[OK] Initialisation du drone réussie.\n");*/
	if(initNavData(drone))
		fprintf(stdout, "[OK] Initialisation du drone réussie !\n");
	else
	{
		fprintf(stderr, "[ER] Initialisation du drone échouée... \nAbort...\n");
		return -2;
	}
	
	fprintf(stdout, "Tentative de récupération des données de navigation.\n");
	
	//do
	//{
		senderLen = sizeof(sender);
		//fprintf(stdout, "Launch receiveNavData function.\n");
		tmp = receiveNavData(drone, &sender, &senderLen, b, 24);
		//fprintf(stdout, "receiveNavData just finish, result : \n");
		fprintf(stdout, "Header : 0x");
		for(i = 0 ; i < 4 ; fprintf(stdout, "%02x", b[i++]));
		fprintf(stdout, "\n");
		j = i;
		
		fprintf(stdout, "Drone state : 0x");
		for(i = 0 ; i < 4 ; fprintf(stdout, "%02x", b[j + i++]));
		fprintf(stdout, "\n");
		j += i;
		
		fprintf(stdout, "Sequence number : 0x");
		for(i = 0 ; i < 4 ; fprintf(stdout, "%02x", b[j + i++]));
		fprintf(stdout, "\n");
		j += i;
		
		fprintf(stdout, "Vision flag : 0x");
		for(i = 0 ; i < 4 ; fprintf(stdout, "%02x", b[j + i++]));
		fprintf(stdout, "\n");
		j += i;
		
		for(i = 0 ; i < 24-j ; fprintf(stdout, "%02x ", b[j + i++]));
		fprintf(stdout, "\n");
		
		//
		//
		//
		
		fprintf(stdout, "\n\nAffichage d'un nouvelle trame.\n");
		tmp = receiveNavData(drone, &sender, &senderLen, b, 500);
		fprintf(stdout, "Header : 0x");
		for(i = 0 ; i < 4 ; fprintf(stdout, "%02x", b[i++]));
		fprintf(stdout, "\n");
		j = i;
		
		fprintf(stdout, "Drone state : 0x");
		for(i = 0 ; i < 4 ; fprintf(stdout, "%02x", b[j + i++]));
		fprintf(stdout, "\n");
		j += i;
		
		fprintf(stdout, "Sequence number : 0x");
		for(i = 0 ; i < 4 ; fprintf(stdout, "%02x", b[j + i++]));
		fprintf(stdout, "\n");
		j += i;
		
		fprintf(stdout, "Vision flag : 0x");
		for(i = 0 ; i < 4 ; fprintf(stdout, "%02x", b[j + i++]));
		fprintf(stdout, "\n");
		j += i;
		
		/*for(i = 0 ; i < 500-j ; fprintf(stdout, "%02x ", b[j + i++]));
		fprintf(stdout, "\n");*/
		
		fprintf(stdout, "Performing a memcpy\n");
		memcpy(&trame, b, 128);
		fprintf(stdout, "information sur la trame:\n");
		fprintf(stdout, "Header : 0x%08x - 0x%08x\n", trame.header, ntohl(trame.header));
		fprintf(stdout, "ARDrone State : 0x%08x - 0x%08x\n", trame.ardrone_state, ntohl(trame.ardrone_state));
		fprintf(stdout, "Sequence number : 0x%08x %d\n", trame.sequence, trame.sequence);
		fprintf(stdout, "Vision flag : 0x%08x\n", trame.vision_defined);
		
		memcpy(&navdata, &b[129], 372 /*500-128*/);
		fprintf(stdout, "\n\nDonnées de navigation : \n");
		fprintf(stdout, "tag : 0x%08x - %d\n", navdata.tag, navdata.tag);
		fprintf(stdout, "size : 0x%08x - %d\n", navdata.size, navdata.size);
		fprintf(stdout, "ctrl_state : 0x%08x - %d\n", navdata.ctrl_state, navdata.ctrl_state);
		
		fprintf(stdout, "vbat_flying_percentage : 0x%08x - %d\n", navdata.vbat_flying_percentage, navdata.vbat_flying_percentage);
		fprintf(stdout, "theta : 0x%08x - %f\n", navdata.theta, navdata.theta);
		fprintf(stdout, "phi : 0x%08x - %f\n", navdata.phi, navdata.phi);
		fprintf(stdout, "psi : 0x%08x - %f\n", navdata.psi, navdata.psi);
		fprintf(stdout, "altitude : 0x%08x - %d\n", navdata.altitude, navdata.altitude);
		fprintf(stdout, "vx : 0x%08x - %f\n", navdata.vx, navdata.vx);
		fprintf(stdout, "vy : 0x%08x - %f\n", navdata.vy, navdata.vy);
  		fprintf(stdout, "vz : 0x%08x - %f\n", navdata.vz, navdata.vz);
		
		//sleep(1);
	//} while (1);
		
	closeARDrone(drone);
	return 1;
}
