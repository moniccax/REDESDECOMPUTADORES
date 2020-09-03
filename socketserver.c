/*MÔNICA EMEDIATO MENDES DE OLIVEIRA
The server’s job is to set up a communication endpoint and passively wait for a connection from the client. There are four general
steps for basic TCP server communication:
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Function to concatenate two integers into one*/
int concat(int a, int b){
      char s1[10];
      char s2[10];
      /* Convert both the integers to string */
      sprintf(s1, "%d", a);
      sprintf(s2, "%d", b);
      /* Concatenate both strings */
      strcat(s1, s2);
      /* Convert the concatenated string to integer */
      int c = atoi(s1);
      /* Return the formed integer */
      return c;
}

int main(int argc, char** argv) {
      struct sockaddr_in6 serverAddr, clientAddr;
      int serverSocket, clientSocket, nBytes, i, fim=0, encontrei, v6only;
      socklen_t size;
      char buffer[128], client_ip[150];
      uint8_t num1, num2, teste[7];
      int n_tentativas=0, corretas=0, auxcorretas=0;
      char palavra[7], tentativa[7], copiapalavra[7];
      strcpy(palavra, "momox");
      strcpy(copiapalavra, palavra);
      //limite_tentativas = strlen(palavra);
      char chute, c;

      num2=strlen(palavra); // how many
      num1=1;//type

      uint32_t protocolo = (concat(num1, num2)); //o que preciso enviar para o cliente inicialmente

      sprintf(buffer, "%d", protocolo);

      /* Checking the arguments*/
      if (argc != 2) {
            printf("\n[TCP Server] Argument error.\n\n");
            exit(1);
      }

      /* Creating TCP socket*/
      serverSocket = socket(AF_INET6, SOCK_STREAM, 0);  // AF_INET --> IPv4, SOCK_STREAM --> TCP

      if (serverSocket == -1) {
            printf("\n[TCP Server] Socket error.\n\n");
            exit(1);
      }

      /* socket IPv4 e IPv6 */
      v6only = 0;
      if (setsockopt(serverSocket, IPPROTO_IPV6, IPV6_V6ONLY, &v6only, sizeof(v6only)) != 0) {
            printf("\n[TCP Server] TIMEOUT - Socket IPv6/IPv4 error.\n\n");
            exit(1);
      }

    /* for compatibility */
      memset(&serverAddr, 0, sizeof(serverAddr));
      /* Configure settings in address struct */
      serverAddr.sin6_family = AF_INET6;            // IPv6 aceita ipv4
      serverAddr.sin6_addr = in6addr_any;    // Any IP address for the server
      serverAddr.sin6_port = htons(atoi(argv[1])); // Gets the port number by the first argument

    /* Let reusing port */
      int enable = 1;
      if ( 0 != setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))){
            printf("\n[TCP Server] Setsockopt Error\n\n" );
      }

    /* Bind socket with address struct */
      if( bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
            printf("\n[TCP Server] Bind Error\n\n");
            exit(1);
      }

      struct sockaddr_storage cstorage; //client address
      struct sockaddr *caddr = (struct sockaddr *) (&cstorage);
      socklen_t caddrlen = sizeof(cstorage);

      /* Initialize size variable to be used later on */
      size = sizeof(struct sockaddr_in6);

      listen(serverSocket, 5);// Transform the socket in an TCP listener, waiting for TCP connections. */

      while(1) {
          // If a new client try to connect, ANOTHER socket is created just for it (clientSocket).
          printf("[TCP Server] Waiting for clients...\n\n");

          clientSocket = accept(serverSocket, caddr, &caddrlen); // Blocks the execution waiting por clients
          if (clientSocket == -1) { // Test the new socket.
              perror("[TCP Server] Connection Error.\n");
              exit(1);
          }
          getpeername(clientSocket, (struct sockaddr *) &clientAddr, &size);

          if(inet_ntop(AF_INET6, &clientAddr.sin6_addr, client_ip, sizeof(client_ip))) {
              printf("[TCP Server] Client [%s:%d] Connected!\n\n", client_ip, ntohs(clientAddr.sin6_port));
          }

          //printf("[TCP Server] Client [%s:%d] Connected!\n\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
          nBytes = write(clientSocket, buffer, 128);

          //armazena string com espacos com tamanho da palavra sorteada
          for (i=0; i< (int)(strlen(palavra)); i++)
            tentativa[i]= ' ';

          printf("\n FORCA EM C \n");
          printf("____________________\n\n");
          do {

                /* apresenta posiçoes ja encontradas */
                for (i=0; i< (int)strlen(palavra); i++)
                      printf("%c", tentativa[i]);
                printf("\n");

                printf("__________________________\n");
                //printf(" Corretas:%d ", auxcorretas);

                //buffer= {0};
                nBytes = read(clientSocket, buffer, 128); // Receiving first
                if (nBytes < 1) {
                    printf("\n[TCP Server]Nada chegou! ");
                }
                //printf("[TCP Server]Palpite %s", buffer);
                num1 = (buffer[0] - '0'); // type from client
                if(num1 != 2 ) break;

                strcpy (&chute, (buffer)+1);
                strcpy(&c, (buffer)+1);


                /*testa se a letra informada encontra-se na palavra escolhida*/ //protocolo = (concat(num1, num2));
                encontrei =0;
                for (i=0; i< (int)(strlen(palavra)); i++)
                    if (copiapalavra[i] == chute){
                      copiapalavra[i] = '*';
                      tentativa[i] = chute;
                      teste[corretas] = i;
                      corretas++;
                      auxcorretas++;
                      encontrei = 1;
                       //concaetando posicoes inteiras em aux
                    }
                if (encontrei == 0 )
                    n_tentativas++;

                num1 = 3; // type answer palpite
                num2 = corretas; // how many right
                protocolo = 0;

                if (auxcorretas == (int)strlen(palavra) || (chute == '*')) {//|| (n_tentativas >= limite_tentativas)) {
                    num1 = 4;
                    fim = 1;
                }

                protocolo = concat (num1, num2); // type, how many, positions

                if (corretas>0) { //user is right
                    for (i=0;i<corretas; i++)
                      protocolo = concat(protocolo, teste[i]);
                }

                sprintf(buffer, "%d", protocolo); //convert to char in buffer
                corretas=0;
                nBytes = write(clientSocket, buffer, 128); // Sending back
                if(nBytes < 0) break;
        } while((fim == 0) || (num1 != 4));

         n_tentativas=0;
         corretas=0;
         auxcorretas=0;
         strcpy(palavra, "momox");
         strcpy(copiapalavra, palavra);
         num2=strlen(palavra); // how many
         num1=1;//type
         printf("[TCP Server] Client [%s:%d] Connection Closed.\n\n", client_ip, ntohs(clientAddr.sin6_port));
         close(clientSocket); // Releasing the socket for the client
      }
      close(serverSocket); // Releasing the socket of the server
      return (0);
}
