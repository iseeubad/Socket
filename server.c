#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<winsock2.h>
#include<ws2tcpip.h>

#define Listening_Port 23
#define PENDING_QUEUE_MAXLENGTH 1
#define BUFFER_SIZE 1024

/****les structures : adress et contact ******/

typedef struct {
    char rue[50];
    char ville[30];
    char pays[30];
}Adresse;

typedef struct Contact{
    char    nom[50];
    char prenom[50];
    char  GSM[50];
    char  email[50];
    Adresse  adresse;
}Contact;

typedef struct {
    int fin; // 0 pour indiquer la fin de l'envoi, 1 sinon
    Contact contact; // Les informations du contact
} EnvoiContact;



typedef struct {
    int trouve; // 1 si le contact est trouvé, 0 sinon
    Contact contact; // Les informations du contact trouvé
} ResultatRecherche;
/**  FUNCTION AJOUTER ***/


void AJOUTER(char *nom_fich, Contact c) {
    FILE *f = fopen(nom_fich, "a");
    if (f != NULL) {
        fprintf(f, "%s %s %s %s %s %s %s\n", c.nom, c.prenom, c.GSM, c.email, c.adresse.rue, c.adresse.ville, c.adresse.pays);
        fclose(f);
        printf("\n------- Un Contact a été ajouté avec succès -------\n\n");
    } else
        printf("Impossible d'ouvrir le fichier !!!");
}


/*** fonction Rechercher en utilisons GSM  *****/

ResultatRecherche RECHERCHER(char *nom_fichier, char *gsm) {
    FILE *f = fopen(nom_fichier, "r");
    Contact c;
    ResultatRecherche resultat = {0, {0}}; // Initialisation à zéro

    if (f == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", nom_fichier);
        return resultat;
    }

    while (fscanf(f, "%s %s %s %s %s %s %s", c.nom, c.prenom, c.GSM, c.email, c.adresse.rue, c.adresse.ville, c.adresse.pays) == 7) {
        if (strcmp(gsm, c.GSM) == 0) {
            resultat.trouve = 1;
            resultat.contact = c; // Copie les informations du contact trouvé
            break;
        }
    }

    fclose(f);
    return resultat;
}

/*** fonction afficher tous les contacts ***/

Contact AFFICHER(char *nom_fichier){
	Contact c;

	FILE *f=fopen(nom_fichier,"r");
	 while(fscanf(f,"%s %s %s %s %s %s %s",c.nom,c.prenom,c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays)==7){
	 	 printf("%s %s %s %s %s %s %s  \n",c.nom,c.prenom,c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays);

	}
    fclose(f);
     return c;
}

/*** fonction modifier contact***/

int MODIFIER(char *nom_fichier,char *gsm, Contact c1) {
    Contact c;
    int r=0;
    FILE *F_in=fopen(nom_fichier,"r+");
    FILE *F_out=fopen("td.txt","w+");

    while(fscanf(F_in,"%s %s %s %s %s %s %s",c.nom,c.prenom,c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays)!=EOF){
        if (strcmp(gsm, c.GSM) == 0) {
            fprintf(F_out,"%s %s %s %s %s %s %s\n",c1.nom,c1.prenom,c1.GSM,c1.email,c1.adresse.rue,c1.adresse.ville,c1.adresse.pays);
            r=1;
        }
        else {
            fprintf(F_out,"%s %s %s %s  %s %s %s\n",c.nom,c.prenom,c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays);

        }
    }

    fclose(F_out);
    fclose(F_in);
    remove(nom_fichier);
  rename("td.txt",nom_fichier);

    return r;
}

/*** fonction supprimer un contact ***/
int SUPPRIMER_CONTACT(char *nom_fichier, char *gsm) {
    FILE *f = fopen(nom_fichier, "r");
    FILE *temp = fopen("temp.txt", "w");
    Contact c;
    int trouve = 0;

    if (f == NULL || temp == NULL) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return -1;
    }

    while (fscanf(f, "%s %s %s %s %s %s %s", c.nom, c.prenom, c.GSM, c.email, c.adresse.rue, c.adresse.ville, c.adresse.pays) == 7) {
        if (strcmp(gsm, c.GSM) != 0) {
            fprintf(temp, "%s %s %s %s %s %s %s\n", c.nom, c.prenom, c.GSM, c.email, c.adresse.rue, c.adresse.ville, c.adresse.pays);
        } else {
            trouve = 1;
        }
    }

    fclose(f);
    fclose(temp);

    if (trouve) {
        remove(nom_fichier);
        rename("temp.txt", nom_fichier);
    } else {
        remove("temp.txt");
    }

    return trouve;
}


int main(void) {

    //initialisation de winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de Winsock:\n\n");
        exit(1);
    }
    //creation du socket
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD == -1) {
        fprintf(stderr, "(SERVEUR) Echec d'initialisation de socket");
        exit(1);
    }
    //configuration et liaison  du socket
    struct sockaddr_in socketAddress;
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(Listening_Port);
    socketAddress.sin_addr.s_addr = INADDR_ANY;
    int socketAddresslength = sizeof(socketAddress);

    int bindReturnCode = bind(socketFD, (struct sockaddr *)&socketAddress, socketAddresslength);
    if (bindReturnCode == -1) {
        fprintf(stderr, "(SERVEUR) Echec d'initialisation de socket");
        exit(1);
    }
    // attente de nouvelles connections
    if (listen(socketFD, PENDING_QUEUE_MAXLENGTH) == -1) {
        fprintf(stderr, "(SERVEUR) Echec de démarrage de socket");
        exit(1);
    }
    printf("\n\n*********************************** Le_Serveur ***********************************\n\n");
    printf("\t\tLa socket %d est maintenant ouverte en mode TCP/IP\n\n", Listening_Port);
    printf("------>Listage du port %d ...\n", Listening_Port);
    printf("------>Patientez pendant que le client se connecte sur le port %d ...\n\n", Listening_Port);

    int connectedSocketFD = accept(socketFD, (struct sockaddr *)&socketAddress, (socklen_t *)&socketAddresslength);
    if (connectedSocketFD == -1) {
        fprintf(stderr, "(SERVEUR) Echec de connexion de socket");
        exit(1);
    }
    //cnx faite!!!

    // reception du message
    char buffer[BUFFER_SIZE] = {0};
    int receivedBytes = recv(connectedSocketFD, buffer, BUFFER_SIZE, 0);
    if (receivedBytes == -1) {
        fprintf(stderr, "(SERVEUR) Echec de reception de message\n");
        exit(1);
    }
    printf("client:%s \n", buffer);

    // envoi d'un message
    const char message[] = "bonjour client vous etes le bienvenue :) !!";
    int sentBytes = send(connectedSocketFD, message, strlen(message), 0);
    if (sentBytes == -1) {
        fprintf(stderr, "(SERVEUR) Echec d'envoi de message au client\n");
        exit(1);
    }

    printf("Un client  se connecte avec la socket :) %d de %s:%d\n\n\n", connectedSocketFD, inet_ntoa(socketAddress.sin_addr), ntohs(socketAddress.sin_port));

    int p = 0;
    char Nom[50], type[50], passwd[50];

    do {
        //reception d'un message
        char login[10] = {0};
        int receive1 = recv(connectedSocketFD, login, 10, 0);
        if (receive1 == -1) {
            fprintf(stderr, "(serveur) Echec de reception de le login du client \n");
        }
        printf(" AUTHENTIFICATION -----> Login : %s \n", login);
        FILE *f;
        f = fopen("comptes.txt", "r");
        if (f == NULL) {
            printf("erreur d'ouverture du fichier \n");
            exit(1);
        }
        char tab[100];
        int nbl = 0;
        while (fgets(tab, sizeof(tab), f)) {
            nbl++;
        }
        rewind(f);

        int i = 0;
        while (p == 0 && fscanf(f, "%s %s %s", Nom, passwd, type) == 3) {
            if (strcmp(Nom, login) == 0) {
                p = 1;
                printf("\t> login trouve !! \n\n");
            }
            i++;
        }

        //envoi du message
        int sent1 = send(connectedSocketFD, &p, sizeof(p), 0);
        if (sent1 == -1) {
            fprintf(stderr, "(serveur) echec d'envoi de reponse existe'\n");
            exit(1);
        }
    } while (p == 0);

    int right_pwd = 0;
    do {
        //reception du mot de passe
        char pwd_inser[10] = {0};
        int receive2 = recv(connectedSocketFD, pwd_inser, 10, 0);
        if (receive2 == -1) {
            fprintf(stderr, "(serveur) Echec de reception de la mot de passe du client \n");
        }
        printf("- AUTHENTIFICATION -> Mot de passe : %s \n", pwd_inser);
        if (strcmp(pwd_inser, passwd) == 0) {
            right_pwd = 1;
            printf("\t-> Mot de passe correct \n\n");

        } else {
            printf("Mot de passe incorrect !!\n ");
            printf("\n L'utilisateur  %s  connecte ", Nom);
        }

        //envoi du message
        int sent2 = send(connectedSocketFD, &right_pwd, sizeof(right_pwd), 0);
        if (sent2 == -1) {
            fprintf(stderr, "(serveur) echec d'envoi de reponse existe_mot_de_passe\n");
            exit(1);
        }
    } while (right_pwd == 0);

    int admin;
    if (strcmp(type, "admin") == 0) {
        //admin
        printf("Le client est connecté comme ADMIN \n");
        admin = 1;
        int sent3 = send(connectedSocketFD, &admin, sizeof(admin), 0);
        if (sent3 == -1) {
            fprintf(stderr, "(serveur) echec d'envoi de reponse de admin ou non\n");
            exit(1);
        }
        char f1[] = "contacts.txt";
        int ch1;
        int rep;
        int resultat;
        char gsm[50];
        do {
            int ch1;
            int receive3 = recv(connectedSocketFD, &ch1, sizeof(ch1), 0);
            if (receive3 == -1) {
                fprintf(stderr, "(serveur) Echec de reception de la choix de l'admin \n");
                exit(1);
            }

            switch (ch1) {

            case 1:
                //ajout contact
                printf("\n   %d Ajouter un contact\n", ch1);
                printf("\t> L'utilisateur a demandé d'ajouter un contact \n");

                Contact c1;
                int receive4 = recv(connectedSocketFD, &c1, sizeof(c1), 0);
                if (receive4 == -1) {
                    fprintf(stderr, "(serveur) Echec de reception contact a jouter \n");
                }

                printf("\n\t   -> Nom : %s", c1.nom);
                printf("\n\t   -> Prenom : %s", c1.prenom);
                printf("\n\t   -> GSM : %s", c1.GSM);
                printf("\n\t   -> Email : %s", c1.email);
                printf("\n\t   -> Adresse : %s %s %s", c1.adresse.rue, c1.adresse.ville, c1.adresse.pays);

                AJOUTER("contacts.txt", c1);
                printf("\n\t---> Contact ajouté avec succès !!\n");
                int rep = 1;
                int sent4 = send(connectedSocketFD, &rep, sizeof(rep), 0);
                break;

            case 2:
                //Rechercher un contact
                printf("\n  %d Rechercher un contact \n",ch1);
                printf("\t> L'utilisateur a demandé de rechercher un contact avec un GSM \n");


                recv(connectedSocketFD, gsm, sizeof(gsm), 0); // Recevoir le GSM du client

                printf("\n\t- Le GSM à rechercher : %s", gsm);

                ResultatRecherche resultat = RECHERCHER("contacts.txt", gsm);
                send(connectedSocketFD, &resultat.trouve, sizeof(resultat.trouve), 0); // Envoyer l'indicateur de recherche

                if (resultat.trouve) {
                     printf("\n\t>------Le contact a été trouvé.-------\n");
                    send(connectedSocketFD, &resultat.contact, sizeof(resultat.contact), 0); // Envoyer les informations du contact
                }
                else{
                    printf("\n\t>------Le contact n'a pas été trouvé.-------\n");
                }
                break;

            case 3:
                // Supprimer un contact
                printf("\n   %d Supprimer un contact\n", ch1);
                printf("\t> L'utilisateur a demandé de supprimer un contact \n");

                int receive = recv(connectedSocketFD, gsm, sizeof(gsm), 0);
                if (receive > 0) {
                    // Appeler la fonction de suppression
                    int resultat = SUPPRIMER_CONTACT("contacts.txt", gsm);
                    send(connectedSocketFD, &resultat, sizeof(resultat), 0);
                }
                break;

            case 4:
                // Modifier un contact

				printf("\n  %d modifier un contact \n", ch1);
                printf("\t> L'utilisateur a demandé de modifier un contact avec GSM \n");

                int receive7 = recv(connectedSocketFD, gsm, sizeof(gsm), 0);
                if (receive7 == -1) {
                    fprintf(stderr, "(serveur) Échec de réception du GSM\n");
                }
                printf("\n\t- Le GSM à Modifier : %s", gsm);
                Contact cModif;
                int receive8 = recv(connectedSocketFD, &cModif, sizeof(cModif), 0);
                if (receive8 == -1) {
                    fprintf(stderr, "(serveur) Échec de réception du contact à modifier \n");
                }

                printf("\n\t   - Nom : %s", cModif.nom);
                printf("\n\t   - Prénom : %s", cModif.prenom);
                printf("\n\t   - GSM : %s", cModif.GSM);
                printf("\n\t   - Email : %s", cModif.email);
                printf("\n\t   - Adresse : %s %s %s", cModif.adresse.rue, cModif.adresse.ville, cModif.adresse.pays);

                int r3 = MODIFIER("contacts.txt", gsm, cModif);
                printf("\n\t   - GSM : %s\n", cModif.GSM);

                int sent7 = send(connectedSocketFD, &r3, sizeof(r3), 0);
                if (sent7 == -1) {
                    fprintf(stderr, "(serveur) Échec d'envoi de la réponse de modification\n");
                    exit(1);
                }
                break;

            case 5:
                // Afficher tous les contacts
                printf("\n   %d Afficher tous les contacts\n", ch1);
                printf("\t> L'utilisateur a demandé d'afficher tous les contacts \n");

                FILE *f2 = fopen("contacts.txt", "r");
                EnvoiContact envoi;
                if (f2 == NULL) {
                    fprintf(stderr, "(serveur) Erreur lors de l'ouverture du fichier contacts.txt\n");
                    envoi.fin = -1; // Indicateur d'erreur
                    send(connectedSocketFD, &envoi, sizeof(envoi), 0);
                } else {
                    while (fscanf(f2, "%s %s %s %s %s %s %s\n", envoi.contact.nom, envoi.contact.prenom, envoi.contact.GSM, envoi.contact.email, envoi.contact.adresse.rue, envoi.contact.adresse.ville, envoi.contact.adresse.pays) == 7) {
                        envoi.fin = 1; // Indicateur de continuation
                        send(connectedSocketFD, &envoi, sizeof(envoi), 0);
                    }
                    fclose(f2);
                    envoi.fin = 0; // Indicateur de fin
                    send(connectedSocketFD, &envoi, sizeof(envoi), 0);
                }
                printf("\n\t---> Tous les contacts ont été affichés avec succès !!\n");
                break;

            case 6:
                close(socketFD);
                printf("\n    - %d -> Quitter\n", ch1);
                printf("\t> L'utilisateur a quitté \n");
                break;
            }
        } while (ch1 != 6);

    } else {
        //visiteur
        printf("comme invite \n");
        admin = 0;
        int sent6 = send(connectedSocketFD, &admin, sizeof(admin), 0);
        if (sent6 == -1) {
            fprintf(stderr, "(serveur) echec d'envoi de reponse de admin ou non\n");
            exit(1);
        }
        char f1[] = "contacts.txt";
        int ch3;
        char gsm1[50];
        int rep;

        do {
            int ch3;
            int receive3 = recv(connectedSocketFD, &ch3, sizeof(ch3), 0);
            if (receive3 == -1) {
                fprintf(stderr, "(serveur) Echec de reception de la choix de l'admin \n");
                exit(1);
            }

            switch (ch3) {

            case 1:
               //rechercher un contact
                 printf("\n  %d Rechercher un contact \n",ch3);
                printf("\t> L'utilisateur a demandé de rechercher un contact avec un GSM \n");

                char gsm[20]; // GSM en tant que chaîne de caractères

                recv(connectedSocketFD, gsm1, sizeof(gsm1), 0); // Recevoir le GSM du client

                printf("\n\t- Le GSM à rechercher : %s", gsm1);

                ResultatRecherche resultat = RECHERCHER("contacts.txt", gsm1);
                send(connectedSocketFD, &resultat.trouve, sizeof(resultat.trouve), 0); // Envoyer l'indicateur de recherche

                if (resultat.trouve) {
                    printf("\t>------Le contact n'a pas été trouvé.-------\n");
                    send(connectedSocketFD, &resultat.contact, sizeof(resultat.contact), 0); // Envoyer les informations du contact
                }
                break;

            case 2:
                // Afficher tous les contacts
                printf("\n   %d Afficher tous les contacts\n", ch3);
                printf("\t> L'utilisateur a demandé d'afficher tous les contacts \n");

                FILE *f2 = fopen("contacts.txt", "r");
                EnvoiContact envoi;
                if (f2 == NULL) {
                    fprintf(stderr, "(serveur) Erreur lors de l'ouverture du fichier contacts.txt\n");
                    envoi.fin = -1; // Indicateur d'erreur
                    send(connectedSocketFD, &envoi, sizeof(envoi), 0);
                } else {
                    while (fscanf(f2, "%s %s %s %s %s %s %s\n", envoi.contact.nom, envoi.contact.prenom, envoi.contact.GSM, envoi.contact.email, envoi.contact.adresse.rue, envoi.contact.adresse.ville, envoi.contact.adresse.pays) == 7) {
                        envoi.fin = 1; // Indicateur de continuation
                        send(connectedSocketFD, &envoi, sizeof(envoi), 0);
                    }
                    fclose(f2);
                    envoi.fin = 0; // Indicateur de fin
                    send(connectedSocketFD, &envoi, sizeof(envoi), 0);
                }
                printf("\n\t---> Tous les contacts ont été affichés avec succès !!\n");
                break;

            case 3:
                close(socketFD);
                printf("\n    - %d -> Quitter\n", ch3);
                printf("\t> L'utilisateur a quitté \n");
                break;
            }
        } while (ch3 != 3);
    }
    printf("fin du programme\n");
    close(socketFD);
    return 0;
}
