#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <pthread.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

#include "board.c"

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"
#define GREEN  "\x1B[32m"

int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent * server;
pthread_t tid[1];


void * on_signal(void * sockfd) {//xử lý dl server trả về 
  char buffer[64];
  int n;
  int socket = *(int *)sockfd;
  int * player = (int *)malloc(sizeof(int *));

  while (1) {
    bzero(buffer, 64);
    n = read(socket, buffer, 64);

    if (n < 0) {
       perror("ERROR reading from socket");
       exit(1);
    }

    if (buffer[0] == 'i' || buffer[0] == 'e' || buffer[0] == '\0') {
      if (buffer[0] == 'i') {
        if (buffer[2] == 't') {
          printf("\nMake your move: \n");
        }
        if (buffer[2] == 'n') {
          printf("\nWaiting for opponent...\n");
        }
        if (buffer[2] == 'p') {
          *player = atoi(&buffer[3]);
          if (*player == 2) {
            printf("You're blacks (%c)\n", buffer[3]);
          } else {
            printf("You're whites (%c)\n", buffer[3]);
          }
        }
      }
      else if (buffer[0] == 'e') {
        // Syntax errors
        if (buffer[2] == '0') {
          switch (buffer[3]) {
            case '0':
              printf(RED "  ↑ ('-' missing)\n" RESET);
              break;
            case '1':
              printf(RED "↑ (should be letter)\n" RESET);
              break;
            case '2':
              printf(RED "   ↑ (should be letter)\n" RESET);
              break;
            case '3':
              printf(RED " ↑ (should be number)\n" RESET);
              break;
            case '4':
              printf(RED " ↑ (out of range)\n" RESET);
              break;
            case '5':
              printf(RED "   ↑ (should be number)\n" RESET);
              break;
            case '6':
              printf(RED "   ↑ (out of range)\n" RESET);
              break;
            case '7':
              printf(RED "(out of range)\n" RESET);
              break;
          }
        }
        printf("\nerror %s\n", buffer);
      }
      // Check if it's an informative or error message
    } else if (buffer[0] == 'r') {
      printf("%s\n", buffer);
    } else {
      // Print the board
      system("clear");
      if (*player == 1) {
        print_board_buff(buffer);
      } else {
        print_board_buff_inverted(buffer);
      }
    }

    bzero(buffer, 64);
  }
}

int login(){

}

int signup(){

}

int responseRank = 0;

void * handleRank(void * sockfd) {
  char buffer[64];
  int n;
  int socket = *(int *)sockfd;
  int * player = (int *)malloc(sizeof(int *));

  while (1) {
    bzero(buffer, 64);
    n = read(socket, buffer, 64);

    if (n < 0) {
       perror("ERROR reading from socket");
       exit(1);
    }

    printf("%s\n", buffer);

    bzero(buffer, 64);
    responseRank = 1;
  }
}

int rank(){
    int n = -1;
    char buffer[] = "rank";
    pthread_create(&tid[0], NULL, &handleRank, &sockfd);
    n = write(sockfd, buffer, strlen(buffer));//gửi dl lên server 
    printf("%s\n", buffer);
    if (n < 0) {//kiem tra gui thanh cong hay k 
      perror("ERROR writing to socket");
      exit(1);
   }
   printf("nWaiting...\n");
   while(responseRank == 0) {

   }
}

void play() {
   char buffer[64];

   // Response thread
   pthread_create(&tid[0], NULL, &on_signal, &sockfd);//tạo ra 1 luồng khi server gửi dl cho client thì nó sẽ chạy vào hàm on_signal 
    n = write(sockfd, "play", strlen("play"));//gửi dl lên server 

     if (n < 0) {//kiem tra gui thanh cong hay k 
        perror("ERROR writing to socket");
        exit(1);
     }
   while (1) {
     bzero(buffer, 64);
     fgets(buffer, 64, stdin);
     fflush(stdin);
     printf("buffer: %s\n", buffer);
     /* Send message to the server */
     n = write(sockfd, buffer, strlen(buffer));//gửi dl lên server 

     if (n < 0) {//kiem tra gui thanh cong hay k 
        perror("ERROR writing to socket");
        exit(1);
     }
   }
}

int main(int argc, char *argv[]) {//97-136:kết nối đến server 
  setlocale(LC_ALL, "en_US.UTF-8");

  portno = 8080;

  printf("Connecting to %s:%d\n", "localhost", portno);

  /* Create a socket point */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    perror("ERROR opening socket");
    exit(1);
  }

  server = gethostbyname("localhost");

  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);

  // Now connect to the server 
  if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("ERROR connecting");
    exit(1);
  }

   /* Now ask for a message from the user, this message
      * will be read by server
   */

  char username[256];
  char password[256];
  char menu[64];
  printf("1.LOGIN\n");
  printf("2.SIGNUP\n");
  printf("3.START PLAY\n");
  printf("4.RANK\n\n");
  printf("Enter your choose: ");
  fgets(menu, 64, stdin);
  switch(menu[0]){
    case '1':{
      printf("Username\n");
      scanf( "%s", username);
      printf("password\n");
      scanf("%s", password);

      printf("%s...%s\n", username, password);
      break;
      }
    case '2':{
        
    }
    case '3':{
      play();
      break;
    }
    case '4':{
      rank();
      break;
    }
  }
  fflush(stdin);
  return 0;
}
