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
#include "serveur.h"
#define MON_PORT 3500
#define NB_MAX_EN_ATTENTE 2
#define TAILLE_STRING_GRILLE 50

void TestJoueur(struct TypeJoueur *p_joueur)
	{
	printf("Adresse : %s\nSocket : %d\nPort : %d\n",inet_ntoa(p_joueur->adresse.sin_addr), p_joueur->socket, p_joueur->adresse.sin_port);
	}

/*****
 * QuitterEnErreur : Ferme le programme en affichant le mssage d'erreur recu
 *					 en paramètre.
 */
void QuitterEnErreur(char *p_message)
	{
	printf("%s\nErreur : %d", p_message, errno);
	exit(0);
	}

void EnvoyerGrille(int p_socket1, int p_socket2, char p_grille[3][3])
	{
	char grille[TAILLE_STRING_GRILLE];
	sprintf(grille, "%c | %c | %c\n"
					"---------\n"
					"%c | %c | %c\n"
					"---------\n"
					"%c | %c | %c\n", 
					p_grille[0][0], p_grille[0][1], p_grille[0][2],
					p_grille[1][0], p_grille[1][1], p_grille[1][2],
					p_grille[2][0], p_grille[2][1], p_grille[2][2]);

	send(p_socket1, grille, TAILLE_STRING_GRILLE, 0);
	send(p_socket2, grille, TAILLE_STRING_GRILLE, 0);
	}

/*****
 * TraiterJoueur : 
 */
void TraiterJoueur(struct TypeJoueur *p_pJoueurs[2])
	{
	char grille[3][3];
	int i,j;
	for (i= 0; i < 3; ++i)
		for (j= 0; j < 3; ++j)
			grille [i][j]= 'X';

	EnvoyerGrille(p_pJoueurs[0]->socket, p_pJoueurs[1]->socket, grille);	

	/*send(p_pJoueurs[0]->socket, p_pJoueurs[0]->tampon, p_pJoueurs[0]->tailleMessage, 0);	*/
	/*recv(p_pJoueurs[0]->socket, p_pJoueurs[0]->tampon, TAMPON_MAXIMAL - 1, 0);*/
	/*printf("%s", p_pJoueurs[0]->tampon);

	send(p_pJoueurs[1]->socket, p_pJoueurs[1]->tampon, p_pJoueurs[1]->tailleMessage, 0);*/
	/*recv(p_pJoueurs[1]->socket, p_pJoueurs[1]->tampon, TAMPON_MAXIMAL - 1, 0);*/
	/*printf("%s", p_pJoueurs[1]->tampon);*/

	printf("On ferme...");
	close(p_pJoueurs[0]->socket);
	close(p_pJoueurs[1]->socket);
	}

/*****
 * PROGRAMME PRINCIPAL
 */
int main(int argc, char *argv[])
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

	struct TypeJoueur *pJoueurs[2];
	
	for(;;)
		{
		int i= 0;
		for (; i < 2; ++i)
			{
			struct TypeJoueur *pJoueur = NULL;
			pJoueur = malloc(sizeof(TypeJoueur));

			if (pJoueur == NULL)
				QuitterEnErreur("Allocation dynamique échou.");

			pJoueur->tailleMessage= sizeof(pJoueur->adresse);

			if ((pJoueur->socket= (int)accept(sdServeur, (struct sockaddr*) &pJoueur->adresse, &pJoueur->tailleMessage)) < 0)
				QuitterEnErreur("accept() à échoué");

			printf("Gestion du client %s\n", inet_ntoa(pJoueur->adresse.sin_addr));
			
			pJoueurs[i]= pJoueur;

			TestJoueur(pJoueurs[i]);
			}

		/*if (fork() == 0)
			{
			close(sdServeur);*/
			TraiterJoueur(pJoueurs);
			/*}*/
		}
	
	return 1;
	}

