#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define CONNECTION_HOST "127.0.0.1"
#define  Listening_Port 23
#define BUFFER_SIZE 1024

////////////////////////Code_Client///////////////////////

///////////////////////Menu_Admin////////////////////////

int Menu_Admin() {
    int choix;
    do {
        printf("\n\t********************************Menu********************************:\n");
        printf("1. Ajouter un Contact\n");
        printf("2. Rechercher Un Contact\n");
        printf("3. Supprimer Un Contact\n");
        printf("4. Modifier Un Contact\n");
        printf("5. Afficher tous les Contacts\n");
        printf("6. Quitter\n");

        printf("\n\tEntrer votre choix: ");
        scanf("%d", &choix);
    } while (choix < 1 || choix > 6);

    return choix;
}

///////////////////Menu_Visiteur//////////////////

int Menu_Visiteur() {
    int choix;
    do {
        printf("\n\t********************************Menu********************************:\n");
        printf("1. Rechercher Un Contact\n");
        printf("2. Afficher tous les Contacts\n");
        printf("3. Quitter\n");

        printf("\n\tEntrer votre choix: ");
        scanf("%d", &choix);
    } while (choix < 1 || choix > 3);

    return choix;
}

//////////////////les Structures//////////////////

typedef struct {
    char rue[50];
    char ville[30];
    char pays[30];
} Adresse;

typedef struct Contact {
    char nom[50];
    char prenom[50];
    char GSM[50];
    char email[50];
    Adresse adresse;
} Contact;


typedef struct {
    int fin; // 0 pour indiquer la fin de l'envoi, 1 sinon
    Contact contact; // Les informations du contact
} EnvoiContact;


typedef struct {
    int trouve; // 1 si le contact est trouv�, 0 sinon
    Contact contact; // Les informations du contact trouv�
} ResultatRecherche;
/////////////////la fonction saisir_contact////////////////

Contact saisir_contact() {
    Contact c;
    printf("Donner le nom du contact : ");
    scanf("%s", c.nom);
    printf("Entrez le pr�nom du contact: ");
    scanf("%s", c.prenom);
    printf("Entrez le GSM : ");
    scanf("%s", c.GSM);
    printf("Entrez l'email : ");
    scanf("%s", c.email);
    printf("\nSaisie de l'adresse :\n");
    printf("Veuillez entrer le nom de la rue : ");
    scanf("%s", c.adresse.rue);
    printf("Veuillez entrer la ville du contact : ");
    scanf("%s", c.adresse.ville);
    printf("Veuillez entrer le pays du contact : ");
    scanf("%s", c.adresse.pays);
    //printf("\n\n Contact : %s ## %s ## %s ## %s ## %s ## %s ## %s \n\n", c.nom, c.prenom, c.email, c.GSM, c.adresse.rue, c.adresse.ville, c.adresse.pays);
    return c;
}

int main() {
    //initialisation de winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        fprintf(stderr, "(client) Echec d'initialisation de WinSock \n");
        exit(1);
    }

    //////1- Creation du socket//////
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD == -1) {
        fprintf(stderr, "erreur de creation de socket , programme termin� \n");
        exit(1);
    }

    //////2- Connect� le client au serveur/////
    struct sockaddr_in socketAddress;
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(Listening_Port);

    int inetReturnCode = inet_pton(AF_INET, CONNECTION_HOST, &socketAddress.sin_addr);
    if (inetReturnCode == -1) {
        fprintf(stderr, "Adresse invalide , programme termin�\n\n");
        exit(1);
    }

    int socketaddresslength = sizeof(socketAddress);
    int connectionStatus = connect(socketFD, (struct sockaddr*) &socketAddress, socketaddresslength);
    if (connectionStatus == -1) {
        fprintf(stderr, "Unable to connect to server!\n");
        exit(1);
    }

    //////3- envoy� les donnees//////
    const char message[] = "bonjour je suis client";
    int sentBytes = send(socketFD, message, strlen(message), 0);
    if (sentBytes == -1) {
        fprintf(stderr, "Erreur d'envoi du message , programme termin�\n\n");
        exit(1);
    }

    //////4- Recevoir les donnees//////
    char buffer[BUFFER_SIZE] = {0};
    int receivedBytes = recv(socketFD, buffer, BUFFER_SIZE, 0);
    if (receivedBytes == -1) {
        fprintf(stderr, "Erreur de reception du message du serveur \n");
        exit(1);
    }
    printf("serveur : %s \n", buffer);

    printf("\n*********************************** Bienvenu ***********************************\n");
    printf("\n\tRQ: vous avez le droit d'entrer comme -ADMIN ou INVITE seulement :)\n");

    int u;

    do {
        char utili[20];
        printf("\tS'il vous plait Saisir votre login: ");
        scanf("%s", utili);
        int sent1 = send(socketFD, utili, strlen(utili), 0);
        if (sent1 == -1) {
            fprintf(stderr, "(Client) erreur de l'envoi de utili'\n");
            exit(1);
        }

        int receive1 = recv(socketFD, &u, sizeof(u), 0);
        if (receive1 == -1) {
            fprintf(stderr, "(Client)Echec de reception de u \n");
            exit(1);
        }
    } while (u == 0);

    int n;
    int tentatives = 0;
    do {
        if (tentatives < 3) {
            char mot_passe[20];
            printf("\tS'il vous plait Saisir votre de passe: ");
            scanf("%s", mot_passe);
            int sent2 = send(socketFD, mot_passe, strlen(mot_passe), 0);
            if (sent2 == -1) {
                fprintf(stderr, "(Client) erreur de l'envoi de mot_passe'\n");
                exit(1);
            }
            tentatives++;

            int receive2 = recv(socketFD, &n, sizeof(n), 0);
            if (receive2 == -1) {
                fprintf(stderr, "(Client)Echec de reception de n \n");
                exit(1);
            }
        } else {
            printf("vous avez depassez le nombres de tentatives .byyyy  \n");
            exit(1);
        }
    } while (n == 0);

    // admin ou invite ;
    int type;
    int receive3 = recv(socketFD, &type, sizeof(type), 0);
    if (receive3 == -1) {
        fprintf(stderr, "(Client)Echec de reception de type ou non \n");
        exit(1);
    }

    if (type == 1) {
        printf("\t-------------------------VOUS ETES ADMINISTRATEUR-------------------------\n");

        int ch;

        do {

            ch = Menu_Admin();
            int sent3 = send(socketFD, &ch, sizeof(ch), 0);
            if (sent3 == -1) {
                fprintf(stderr, "(Client) erreur de l'envoi de choix_admin \n");
                exit(1);
            }

            Contact c;
            Contact cModif;
            char gsm[50];
            int resultat;
            int recvSize;
            int rep;
            char buffer[BUFFER_SIZE];
            switch (ch) {

            case 1:
                //Ajouter Un contact
                c = saisir_contact();
                int sent4 = send(socketFD, &c, sizeof(c), 0);
                if (sent4 == -1) {
                    fprintf(stderr, "(serveur) echec d'envoi du contact'\n");
                    exit(1);
                }
                int rep = 0;
                int receve4 = recv(socketFD, &rep, sizeof(rep), 0);   // recv reponce
                // envoyer les nouveaux donnees au serveur pour les ajouter
                printf("\nContact saisi : %s#%s#%s#%s#%s#%s#%s\n", c.nom, c.prenom, c.GSM, c.email, c.adresse.rue, c.adresse.ville, c.adresse.pays);
                printf("\nContact ajout� avec succ�s !\n");
                break;

            case 2:
                //Rechercher Un contact
                printf("Entrez le GSM du contact � rechercher : ");
                scanf("%s", gsm);
                send(socketFD, gsm, sizeof(gsm), 0); // Envoyer le GSM au serveur

                int trouve;
                recv(socketFD, &trouve, sizeof(trouve), 0); // Recevoir l'indicateur de recherche

                if (trouve) {
                    Contact contact;
                    recv(socketFD, &contact, sizeof(contact), 0); // Recevoir les informations du contact
                    // Afficher les informations du contact
                    printf("Nom: %s", contact.nom);
                    printf("\nPr�nom: %s\n", contact.prenom);

                } else {
                    printf("Le contact n'existe pas.\n");
                }
                break;


            case 3:
                // Supprimer un contact
                printf("Entrez le GSM du contact � supprimer : ");
                scanf("%s", gsm);
                send(socketFD, gsm, sizeof(gsm), 0);

                int resultat;
                recv(socketFD, &resultat, sizeof(resultat), 0);
                if (resultat == 1) {
                    printf("Le contact a �t� supprim� avec succ�s.\n");
                } else {
                    printf("Le contact n'a pas �t� trouv�.\n");
                }
                break;

            case 4:
                // Modifier un contact
                printf("Entrez le GSM du contact � modifier : ");
                scanf("%s", gsm);
                send(socketFD, gsm, sizeof(gsm), 0); // Envoyer le GSM au serveur

                // Pr�parer les nouvelles informations pour le contact
                printf("Entrez le nouveau nom : ");
                scanf("%s", cModif.nom);
                printf("Entrez le nouveau pr�nom : ");
                scanf("%s", cModif.prenom);
                printf("Entrez le nouveau GSM : ");
                scanf("%s", cModif.GSM);
                printf("Entrez le nouvel email : ");
                scanf("%s", cModif.email);
                printf("Entrez la nouvelle adresse :\n");
                printf("Rue : ");
                scanf("%s", cModif.adresse.rue);
                printf("Ville : ");
                scanf("%s", cModif.adresse.ville);
                printf("Pays : ");
                scanf("%s", cModif.adresse.pays);

                // Envoyer les nouvelles informations au serveur
                send(socketFD, &cModif, sizeof(cModif), 0);

                // Recevoir la r�ponse du serveur
                int reponse;
                recv(socketFD, &reponse, sizeof(reponse), 0);
                if (reponse == 1) {
                    printf("Le contact a �t� modifi� avec succ�s.\n");
                } else {
                    printf("Le contact n'a pas �t� trouv� ou une erreur s'est produite.\n");
                }
                break;


            case 5:
                printf("Demande d'affichage de tous les contacts...\n");

                EnvoiContact envoi;

                // Boucle pour recevoir les contacts un par un
                while (1) {
                    int receive = recv(socketFD, &envoi, sizeof(envoi), 0);
                    if (receive <= 0) {
                        fprintf(stderr, "Erreur lors de la r�ception des donn�es.\n");
                        break;
                    }

                    // V�rifier si c'est un signal de fin ou d'erreur
                    if (envoi.fin == 0) {
                        printf("Tous les contacts ont �t� re�us.\n");
                        break;
                    } else if (envoi.fin == -1) {
                        fprintf(stderr, "Le serveur a signal� une erreur lors de l'ouverture du fichier.\n");
                        break;
                    }

                    // Afficher les informations du contact
                    printf("\n---------------------- Contact -----------------------\n");
                    printf("Nom: %s\n", envoi.contact.nom);
                    printf("Pr�nom: %s\n", envoi.contact.prenom);
                    printf("GSM: %s\n", envoi.contact.GSM);
                    printf("Email: %s\n", envoi.contact.email);
                    printf("Adresse: %s, %s, %s\n", envoi.contact.adresse.rue, envoi.contact.adresse.ville, envoi.contact.adresse.pays);
                }

                break;


            case 6: {
                close(socketFD);
                break;
            }

            default:
                printf("votre choix %d  est invalide\n\n ", ch);
                break;
            }

        } while (ch != 6);
    }

    else {
        printf("\t-------------------------VOUS ETES INVITE-------------------------\n");

        int ch2;

        do {

            ch2 = Menu_Visiteur();
            int sent6 = send(socketFD, &ch2, sizeof(ch2), 0);
            if (sent6 == -1) {
                fprintf(stderr, "(Client) erreur de l'envoi de choix_visiteur \n");
                exit(1);
            }

            char gsm1[50];
            char buffer[BUFFER_SIZE];
            int recvSize;
            switch (ch2) {

            case 1:
                printf("Entrez le GSM du contact � rechercher : ");
                scanf("%s", gsm1);

                // Envoyer le GSM au serveur
                send(socketFD, gsm1, sizeof(gsm1), 0);

                // Recevoir l'indicateur du serveur pour savoir si le contact a �t� trouv�
                int trouve;
                recv(socketFD, &trouve, sizeof(trouve), 0);

                // Si le serveur a trouv� le contact, il enverra les d�tails du contact
                if (trouve) {
                    // Cr�er une structure Contact pour recevoir les d�tails du contact
                    Contact contact;
                    recv(socketFD, &contact, sizeof(contact), 0); // Recevoir les d�tails du contact

                    // Afficher les d�tails du contact
                    printf("\n---------------------- Contact trouv� ! -----------------------\n");
                    printf("Nom: %s\n", contact.nom);
                    printf("Pr�nom: %s\n", contact.prenom);
                    printf("GSM: %s\n", contact.GSM);
                    printf("Email: %s\n", contact.email);
                    printf("Adresse: %s, %s, %s\n", contact.adresse.rue, contact.adresse.ville, contact.adresse.pays);
                } else {
                    // Si le contact n'est pas trouv�, afficher un message appropri�
                    printf("Le contact avec le GSM %s n'existe pas.\n", gsm1);
                }
                break;

            case 2: {


               printf("Demande d'affichage de tous les contacts...\n");

                EnvoiContact envoi;

                // Boucle pour recevoir les contacts un par un
                while (1) {
                    int receive = recv(socketFD, &envoi, sizeof(envoi), 0);
                    if (receive <= 0) {
                        fprintf(stderr, "Erreur lors de la r�ception des donn�es.\n");
                        break;
                    }

                    // V�rifier si c'est un signal de fin ou d'erreur
                    if (envoi.fin == 0) {
                        printf("Tous les contacts ont �t� re�us.\n");
                        break;
                    } else if (envoi.fin == -1) {
                        fprintf(stderr, "Le serveur a signal� une erreur lors de l'ouverture du fichier.\n");
                        break;
                    }

                    // Afficher les informations du contact
                    printf("\n---------------------- Contact -----------------------\n");
                    printf("Nom: %s\n", envoi.contact.nom);
                    printf("Pr�nom: %s\n", envoi.contact.prenom);
                    printf("GSM: %s\n", envoi.contact.GSM);
                    printf("Email: %s\n", envoi.contact.email);
                    printf("Adresse: %s, %s, %s\n", envoi.contact.adresse.rue, envoi.contact.adresse.ville, envoi.contact.adresse.pays);
                }

                break;

            }

            case 3: {
                close(socketFD);
                break;
            }

            default:
                printf("votre choix %d  est invalide\n\n ", ch2);
                break;
            }

        } while (ch2 != 3);
    }

    //Fermeture des sockets et liberation des ressources
    closesocket(socketFD);
    WSACleanup();

    return 0;
}

