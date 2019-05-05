#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// #include <errno.h>

int toInt(char a[]);
int main(int argc, char *argv[]){
	char message[255];
	int server, portNumber;
	socklen_t len;
	struct sockaddr_in servAdd;
	
	if(argc != 3){
		printf("Call model:%s <IP> <Port#>\n",argv[0]);
		exit(0);
	}
	if((server = socket(AF_INET, SOCK_STREAM, 0))<0){
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}
	
	servAdd.sin_family = AF_INET;
	sscanf(argv[2], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);
	if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr)<0){
		fprintf(stderr, " inet_pton() has failed\n");
		exit(2);
	}
	
	if(connect(server,(struct sockaddr *)&servAdd, sizeof(servAdd))<0){
		fprintf(stderr, "connect() has failed, exiting\n");
		exit(3);
	}

	int points=0;
	int points_server=0;
	int dice;
	long int ss=0;
	char a[255];
	
	while(1){
		if(read(server, message, 255)<=0){
			printf("Message from server: %s\n", message);
			close(server);
			exit(3);
		}
			
		if(strcmp(message, "Game over: you win the game")==0 || strcmp(message, "Game over: you lose the game")==0){
			printf("Message from server: %s\n", message);			
			close(server);
			exit(3);
		}
		
		if(strcmp(message, "Game on: you can now play your dice") != 0){
			points_server = points_server + toInt(message);
			printf("Server got %d points and so far %d points.\n", toInt(message), points_server);
		}
		
		printf("Hit enter to play dice\n");
		read(0, a, 255);
		
		dice = (int)time(&ss)%19 + 1;
		points += dice;
		printf("Client: I got %d points and so far %d points.\n\n\n", dice, points);
					
		sprintf(message, "%d", dice);
		write(server, message, strlen(message)+1);			
					
	}
}

int toInt(char a[]) {
  int c, sign, offset, n;
 
  if (a[0] == '-') {  // Handle negative integers
    sign = -1;
  }
 
  if (sign == -1) {  // Set starting position to convert
    offset = 1;
  }
  else {
    offset = 0;
  }
 
  n = 0;
 
  for (c = offset; a[c] != '\0'; c++) {
    n = n * 10 + a[c] - '0';
  }
 
  if (sign == -1) {
    n = -n;
  }
 
  return n;
}
