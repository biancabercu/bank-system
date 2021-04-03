#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <string.h>
#include <errno.h>
#include <arpa/inet.h>


struct topicList {
    char topicName [256] ;
    int SF;
    struct topicList *nextTopic;
} topicList;

struct subsList{
    int idSub;
    struct topicList *topicsSub;
    struct subs *nextSub;
}subsList;

void usage(char *file) {
    fprintf(stderr, "Usage: %s server_port\n", file);
    exit(0);
}

 void reverse(char str[], int index, int size) {
    if (index == size / 2){
        return;
    }
    char aux;
    aux = str[index];
    str[index] = str[size - index];
    str[size - index] = aux;
    reverse(str, index + 1, size);
}

int main(int argc, char **argv)
{
    int BUFLEN=256;
	int fd, sock;
	socklen_t socklen;
	struct sockaddr_in from_station;
	char buf[BUFLEN]; 
    memset(buf, 0, BUFLEN);
	 //pana aici UDP

    int sockfd, newsockfd, portno;
	char buffer[BUFLEN];
	char notify_others[100];
	char aux[4];
	struct sockaddr_in serv_addr, cli_addr;
	int n, i, ret;
	socklen_t clilen;

	int clients[10];
	int clients_num = 0;

	fd_set read_fds;	// multimea de citire folosita in select()
	fd_set tmp_fds;		// multime folosita temporar
	int fdmax;			// valoare maxima fd din multimea read_fds
    //pana aici TCP

	char topic[BUFLEN];
    char topics[BUFLEN];

     if (argc < 1) {
        usage(argv[0]);
    }
// se goleste multimea de descriptori de citire (read_fds) si multimea temporara (tmp_fds)
	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);//tcp
 
	/* TODO deschidere socket */
    sock = socket(PF_INET, SOCK_DGRAM, 0);//udp
    if(sock<0)
    exit(EXIT_FAILURE);
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // se asteapta conexiuni --tcp
    if(sockfd<0)
    exit(EXIT_FAILURE);

    portno = atoi(argv[1]);
	if(portno==0)
    exit(EXIT_FAILURE);

	/* TODO setare struct sockaddr_in pentru a specifica unde trimit
	 * datele */
		from_station.sin_family=AF_INET;
		from_station.sin_port=htons(atoi(argv[1]));
		from_station.sin_addr.s_addr=INADDR_ANY;  //udp

     memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY; //tcp

	/* TODO legare proprietăți de socket */
    int retUdp= bind(sock, (struct sockaddr *) &from_station, sizeof(struct sockaddr_in));
	ret = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));//tcp

    if(ret<0 || retUdp<0)
    exit(EXIT_FAILURE);


    // se adauga noul file descriptor (socketul pe care se asculta conexiuni) in multimea read_fds
	FD_SET(sockfd, &read_fds);
	fdmax = sockfd;

	while (1) {
		tmp_fds = read_fds; 
		
		ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		if(ret<0)
        exit(EXIT_FAILURE);

		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &tmp_fds)) {
				if (i == sockfd) {
					// a venit o cerere de conexiune pe socketul inactiv (cel cu listen),
					// pe care serverul o accepta
					clilen = sizeof(cli_addr);
					newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
					if(newsockfd < 0)
                    exit(EXIT_FAILURE);

					// se adauga noul socket intors de accept() la multimea descriptorilor de citire
					FD_SET(newsockfd, &read_fds);
					if (newsockfd > fdmax) { 
						fdmax = newsockfd;
					}

					printf("New client %d connected from %s:%d.\n",
							newsockfd,inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

					// adauga-l in lista de clients
					clients[clients_num++] = newsockfd; //newsockfd reprezinta id-ul
					// sprintf(notify_others, "[UPDATE] New client: %d\n", newsockfd);
					
					//char others[100] = "Other clients are: ";

					// for (int j = 0; j < clients_num - 1; j++) {
					// 	sprintf(aux, "%d", clients[j]);
					// 	strcat(others, aux);
					// 	strcat(others, " ");
					// 	send(clients[j], notify_others, strlen(notify_others), 0);
					// }
					// if (clients_num > 1) {
					// 	strcat(others, "\n");
					// 	send(newsockfd, others, strlen(others), 0);
					// }

				} else {
					// s-au primit date pe unul din socketii de client,
					// asa ca serverul trebuie sa le receptioneze
					memset(buffer, 0, BUFLEN);
                    memset(topic, 0, BUFLEN);

					n = recv(i, buffer, sizeof(buffer), 0);
					if(n<0)
                    exit(EXIT_FAILURE);
                    

					if (n == 0) {
						// conexiunea s-a inchis
						printf("Client %d disconnected.\n", i);
						close(i);

						// sprintf(notify_others, "[UPDATE] Client %d closed connection\n", i);

						// for (int j = 0; j < clients_num; j++) {
						// 	if (j != i) {
						// 		send(clients[j], notify_others, strlen(notify_others), 0);
						// 	}
						// }
						
						// se scoate din multimea de citire socketul inchis 
						FD_CLR(i, &read_fds);
					} else {
						// int dest = atoi(buffer);
						// send(dest, buffer + 2, strlen(buffer) - 2, 0);
                            //printf ("S-a primit de la clientul de pe socketul %d mesajul: %s\n", i, buffer);
                      
                        // if (strncmp(buffer, "exit", 4) == 0) { //???
                        // close(sockfd);
                        // break;
                        // }
                        if(strncmp(buffer,"subscribe",9) == 0) {
                            // n = send(sockfd, buffer, strlen(buffer), 0);
                            // DIE(n < 0, "send");

                            strcpy(topic,buffer+9); //se copiaza topic+SF
                           if(strstr(topic,"00")) {
								//setez flag pe 0
							}else if(strstr(topic,"10"))
							{
								//setez flag pe 1
							}
									
                            // //topic SF
                            char mesaj[BUFLEN];
                            memset(mesaj, 0, BUFLEN);

                           // n = send(clients[i], mesaj, strlen(mesaj), 0);

                        }
                        if(strncmp(buffer,"unsubscribe",11) == 0) {
                            //unsubscribe topic
                            // n = send(sockfd, buffer, strlen(buffer), 0);
                            // DIE(n < 0, "send"); //serverul va verifica el
                            // memset(forPrintf, 0, BUFLEN);
                            // strcpy(forPrintf,buffer+11);
                            // printf("unsubscribe %s\n",forPrintf);
                        }
                    }
				}
			}
		}
	}
    //   /* TODO deschidere fișier pentru scriere */
    // fd = open(argv[2], O_WRONLY | O_CREAT, 0644);

    // /*
    //  * TODO
    //  * cat_timp  mai_pot_citi
    //  *		citeste din socket
    //  *		pune in fisier
    //  */
    // recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *) &from_station, &socklen)N�U;
    // printf("%s", buf);
    // fd = open(buf, O_WRONLY | O_CREAT, 0644);
    // memset(buf, 0, BUFLEN);
    // while (1) {
    //     recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *) &from_station, &socklen);
    //     if (strcmp(buf, "FINISH") == 0) {
    //         break;
    //     }
    //     int copiat = write(fd, buf, strlen(buf));
    //     memset(buf, 0, BUFLEN);
    // }


	/* TODO închidere socket */
	close(sock);//udp

    close(sockfd);
    	// /* TODO închidere fișier */ udp
	// close(fd);
	return 0;
}
