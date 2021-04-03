#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
void usage(char *file) {
    fprintf(stderr, "Usage: %s id_client server_address server_port\n", file);
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
int main(int argc, char *argv[]) {
    int BUFLEN=256;
    int sockfd, n, ret;
    struct sockaddr_in serv_addr;
    char buffer[BUFLEN];
    char forPrintf[BUFLEN];

    fd_set read_fds;
    fd_set tmp_fds;

    int fdmax;
    char flag0=(char) 00;
    char flag1=(char) 01;
    FD_ZERO(&tmp_fds);
    FD_ZERO(&read_fds);

    if (argc < 4) {
        usage(argv[0]);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0)
    exit(EXIT_FAILURE);

    FD_SET(sockfd, &read_fds);
    fdmax = sockfd;
    FD_SET(0, &read_fds);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    ret = inet_aton(argv[1], &serv_addr.sin_addr);
    if(ret==0)
    exit(EXIT_FAILURE);

    ret = connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if(ret<0)
    exit(EXIT_FAILURE);

    while (1) {
        tmp_fds = read_fds;

        ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
        if(ret<0)
        exit(EXIT_FAILURE);

        if (FD_ISSET(0, &tmp_fds)) {  // citeste de la tastatura
            memset(buffer, 0, BUFLEN);
            fgets(buffer, BUFLEN - 1, stdin);

            if (strncmp(buffer, "exit", 4) == 0) {
                break;
            }
            if(strncmp(buffer,"subscribe",9) == 0) {
                n = send(sockfd, buffer, strlen(buffer), 0);
                if(n<0)
                exit(EXIT_FAILURE);
                memset(forPrintf, 0, BUFLEN);
                strcpy(forPrintf,buffer+9); //se copiaza topic+SF
            
                char *pflag0=strstr(forPrintf,"00");
                char *pflag1=strstr(forPrintf,"10");
                if(!pflag0)
                pflag0=0;
                else
                if(!pflag1)
                pflag1;
                //automat ramane doar topic-ul in forPrintf
                printf("subscribed %s\n",forPrintf);
                // //topic SF

            }
            if(strncmp(buffer,"unsubscribe",11) == 0) {
                //unsubscribe topic
                n = send(sockfd, buffer, strlen(buffer), 0);
                if(n<0)
                exit(EXIT_FAILURE);
                memset(forPrintf, 0, BUFLEN);
                strcpy(forPrintf,buffer+11);
                printf("unsubscribe %s\n",forPrintf);
           }
            // se trimite mesaj la server? topic
        } else {
            memset(buffer, 0, BUFLEN);
            recv(sockfd, buffer, BUFLEN, 0);
          
            //server-ul va trimite el mesaj gata cu format
            printf("%s\n", buffer);

        }
    }

    close(sockfd);
    return 0;
}
