#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8 /*maximum number of client connections */
struct signin{
		char username[MAXLINE];
		char password[MAXLINE];
	};
struct signup{
		char username[MAXLINE];
		char password[MAXLINE];
		char email[MAXLINE];
	};
int readFile_findKey(char str[255]){
	char *p;
    FILE* fp;
    char buffer[255];

    fp = fopen("infor.txt", "r");

    while(fgets(buffer, 255, (FILE*) fp)) {
    	p=strtok(buffer,"\n");
    	//printf("%lu",strlen(p));
    	if(strcmp(p,str)==0){
    		puts("Đăng nhập thành công");
    		return 0;
    	}
    }
    puts("Đăng nhập thất bại");
    fclose(fp);
    return 0;
}
void checkBuf(char buf[MAXLINE]){
	char *p;
	int index = 0;
	char **b = (char **)malloc(100*sizeof(char));
	char signal[MAXLINE];

	FILE *fptr;
	fptr=fopen("infor.txt","r");
	if(fptr==NULL){
		printf("Error\n" );
		exit(1);
	}
	struct signin sign_in;
	struct signup sign_up;

	puts(buf);
	p=strtok(buf," ");
	while(p!=NULL){
		b[index]=p;
		index++;
		p=strtok(NULL," ");
	}
	//printf("%s\n", buf);
	char login[255];
	char space[2]=" ";
	
	strcpy(signal,buf);
	puts(signal);
	// mess.signal=b[0];
	// mess.username=b[1];
	// mess.password=b[2];
	if(strcmp(signal,"login")==0){
		strcpy(sign_in.username,b[1]);
		strcpy(sign_in.password,b[2]);
		//Doc file de so sanh xem co tai khoan khong sau do moi cho dang nhap
		strcpy(login,b[1]);
		strcat(login,space);
		strcat(login,b[2]);
		puts(login);
		readFile_findKey(login);
		//fprintf(fptr, "%s %s\n", sign_in.username,sign_in.password);
		//printf("Kieu: %s username: %s password: %s",signal,sign_in.username,sign_in.password);
	}
	else if(strcmp(signal,"signup")==0){
		strcpy(sign_up.username,b[1]);
		strcpy(sign_up.password,b[2]);
		strcpy(sign_up.email,b[3]);
		fprintf(fptr, "%s %s %s\n", sign_up.username,sign_up.password,sign_up.email);
		printf("Kieu: %s username: %s password: %s email:%s",signal,sign_up.username,sign_up.password,sign_up.email);
	}
	fclose(fptr);
}

int main (int argc, char **argv)
{
 int listenfd, connfd, n;
 pid_t childpid;
 socklen_t clilen;
 char buf[MAXLINE];
 char buf1[MAXLINE];
 struct sockaddr_in cliaddr, servaddr;
	
 //creation of the socket
 listenfd = socket (AF_INET, SOCK_STREAM, 0);
	
 //preparation of the socket address 
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 servaddr.sin_port = htons(SERV_PORT);
	
 bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
 listen (listenfd, LISTENQ);
	
 printf("%s\n","Server running...waiting for connections.");
	
 for ( ; ; ) {
		
  clilen = sizeof(cliaddr);
  connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
  printf("%s\n","Received request...");
				
  while ( (n = recv(connfd, buf, MAXLINE,0)) > 0)  {
  	buf[n-1]='\0';
   	printf("%s","String received from and resent to the client:");
   	puts(buf);
   	strcpy(buf1,buf);
   	//printf("%s",buf1);
   	puts(buf1);
   	checkBuf(buf1);
   	send(connfd, buf, n-1, 0);
  }
			
 if (n < 0) {
  perror("Read error"); 
  exit(1);
 }
 close(connfd);
	
 }
 //close listening socket
 close (listenfd); 
}
