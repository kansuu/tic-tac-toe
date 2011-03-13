/* serveur.c : Serveur de mon petit tic-tac-toe :)
 * Date de création : 12 mars 2011
 * Auteur : Jean-Philippe Boudreau
 */ 
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#define MON_PORT 3500
#define TAMPON_MAXIMAL 100
#define NB_MAX_EN_ATTENTE 2

/*****
 * QuitterEnErreur : Ferme le programme en affichant le mssage d'erreur recu
 *					 en paramètre.
 */
void QuitterEnErreur(char* p_message)
	{
	printf("%s\nErreur : %d", p_message, errno);
	exit(0);
	}

/*****
 * PROGRAMME PRINCIPAL
 */
int main(int argc, char* argv[])
	{
	int sdServeur; /* Descripteur de Socket */
	struct sockaddr_in monAdr; /* Informations sur le socket */
	
	sdServeur= socket(AF_INET, /* Type de Socket, IPv4*/
			    	  SOCK_STREAM, /* TCP */
					  0);	/* Protocole */

	if (sdServeur == -1)
		QuitterEnErreur("socket à échoué");

	monAdr.sin_family= AF_INET;
	monAdr.sin_port= htons(MON_PORT);	/* short NBO(Network Byte Order) */
	monAdr.sin_addr.s_addr= inet_addr("127.0.0.1");
	memset(monAdr.sin_zero, 0, sizeof(monAdr.sin_zero));

	/* On lis le socket aux port */
	if (-1 == bind(sdServeur, (struct sockaddr *)& monAdr, sizeof(struct sockaddr)))
		QuitterEnErreur("bind() à échoué");

	if (listen(sdServeur, NB_MAX_EN_ATTENTE) < 0)
		QuitterEnErreur("listen() à échoué");

	for(;;)
		{
		int sockClient = 0;
		struct sockaddr_in adrClient = {0};
		unsigned int tailleClient= sizeof(adrClient);

		if ((sockClient= (int)accept(sdServeur, (struct sockaddr*)& adrClient, &tailleClient)) < 0)
			QuitterEnErreur("accept() à échoué");

		printf("Gestion du client %s\n", inet_ntoa(adrClient.sin_addr));
		}

	return 1;
	}

