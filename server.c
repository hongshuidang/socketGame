#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int toInt(char a[]);
void serviceClient(int client, int server);
int main(int argc, char *argv[]){
	int sd, client, portNumber;
	socklen_t len;
	struct sockaddr_in servAdd;
	
	if(argc != 2){
		printf("Call model: %s <Port #>\n", argv[0]);
		exit(0);
	}
	
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}
	
	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	sscanf(argv[1], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);
	bind(sd, (struct sockaddr*)&servAdd, sizeof(servAdd));
	listen(sd, 5);
	
	
	while(1){		
		client=accept(sd, (struct sockaddr*)NULL, NULL);
		printf("The player connects\n");
		if(!fork()){			
			serviceClient(client, sd);
		}
		close(client);
		//close(sd);
		//exit(0);
	}
}
 
void serviceClient(int client, int server){
	char message[255];	
	int points=0;
	int points_client=0;
	int dice;
	long int ss = 0;
	char a[255];
	
	strncpy(message, "Game on: you can now play your dice", 100);
	write(client, message, strlen(message)+1);
	
	while(1){
		
		if(read(client, message, 255)<0){
			close(server);
			exit(0);
		}
		
		points_client = points_client + toInt(message);
		printf("Client got %d points and so far %d points.\n", toInt(message), points_client);
		
		if(points_client >= 100){
			strncpy(message, "Game over: you win the game", 100);
			printf("Game over: the client wins\n");
			write(client, message, strlen(message)+1);

			close(server);
			exit(0);			
		}else{
			printf("Hit enter to play dice\n");
			read(0, a, 255);
			
			dice =(int)time(&ss)%17 + 1;
			points += dice;
			printf("Server: I got %d points and so far %d points.\n\n\n", dice, points);
			
			if(points >= 100){
				strncpy(message, "Game over: you lose the game", 100);
				printf("Game over: I win\n");
				write(client, message, strlen(message)+1);

				close(server);
				exit(0);				
			}else{
				sprintf(message, "%d", dice);
				write(client, message, strlen(message)+1);
			}
		}		
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

