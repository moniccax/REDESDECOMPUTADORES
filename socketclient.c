/*MÔNICA EMEDIATO MENDES DE OLIVEIRA
Its job is to initiate communication with a server that is passively waiting to be contacted
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#define BUFSZ 1024

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
      int clientSocket, nBytes, i;
      char buffer[BUFSZ];
      char aux[] = "2";
      struct sockaddr_in6 serverAddr;
      socklen_t size;
      uint8_t num2 = 0 , num1 =0;
      uint16_t p;
      /* Checking the arguments */
      if(argc != 3) {
        printf("\n[TCP Client] Argument error.\n\n");
        exit(1);
      }
      //ipv4 32 bits ipv6 128bits

      /* Create TCP socket */
      //clientSocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET --> IPv4, SOCK_STREAM --> TCP
      clientSocket = socket(AF_INET6, SOCK_STREAM, 0); // AF_INET6 --> IPv6, SOCK_STREAM --> TCP

      if (clientSocket == -1) {
          printf("\n[TCP Client] Socket error.\n\n");
          exit(1);
      }

      //memset(serverAddr.sin_zero, 0, sizeof serverAddr.sin_zero); // for compatibility
      memset(&serverAddr, 0, sizeof (serverAddr));          // Cleaning

      /* Configure settings in address struct */
      //serverAddr.sin_family = AF_INET;                 // IPv4
      serverAddr.sin6_family = AF_INET6;                     // IPv6
      //serverAddr.sin_addr.s_addr = inet_addr(argv[1]); // Get the IP address by the argument
      //serverAddr.sin_port = htons(atoi(argv[2]));      // Get the port by the argument
      serverAddr.sin6_port = htons(atoi(argv[2]));           // Get the port by the argument
      inet_pton(AF_INET6, argv[1], &(serverAddr.sin6_addr)); // Get the IP address by the argument


      /* Initialize size variable to be used later on */
      size = sizeof(struct sockaddr_in6);

      printf("[TCP Client] Trying to connect to the server...\n");   // Trying to connect in the TCP Server
      if (connect(clientSocket, (struct sockaddr*) &serverAddr, size) < 0) {
          printf("\n[TCP Client] Connection Error.\n\n");
          exit(1);
      }

      printf("[TCP Client] CONNECTED! Welcome to the ***** FORCA ******.\n");

      nBytes = recv(clientSocket, buffer, 128, 0);
      num1 = buffer[1] - '0';
      printf("\n[TCP Client] Size word from server: %d\n\n", num1);
      for (int i = 0; i < (int)num1; i++) {
          printf(" _ ");
      }
      printf("[TCP Client] Enter the 1st palpite or zero to quit: ");
      fgets(buffer, BUFSZ, stdin);
      strcat(aux, buffer);


      nBytes = strlen(aux)+1;

      /* Sending data, without specifying a destination, because the TCP connection is already made. */
      while(num1 != 4 ) {
        while(num1 != 4 ) {
            write(clientSocket, aux, nBytes); // es

            nBytes = recv(clientSocket, buffer, 128, 0); //printf("\n[TCP Client] %s", buffer);
            num1= buffer[0] - '0'; //type
            num2 = buffer[1] - '0'; // how many

            if (num2>0) {
              printf("\n[TCP Client] Acertou: %d vezes. Posicoes: ", num2);
              for (i=0; i<num2; i++){
                p = buffer[2+i] - '0';
                printf("%d", p);
              }
            }
            else {
              printf("\n[TCP Client] Tente de novo. Nao teve acertos.\n\n");
            }
            if(num1 == 4 ) {
                printf("\n[TCP Client] Acertou a forca! \n\n");
                break;
            }

            printf("\n[TCP Client] Enter other letter: ");
            fgets(buffer, BUFSZ, stdin);
            sprintf(aux, "%d", 2); // type 2 palpite
            strcat(aux, buffer);
        }
      }
      printf("\n[TCP Client] Quitting...\n");
      close(clientSocket); // Releasing the socket.
      return 0;
    }
