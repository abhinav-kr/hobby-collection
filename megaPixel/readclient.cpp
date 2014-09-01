#include <stdio.h>   // Standard input/output definitions
#include <string.h>  // String function definitions
#include <unistd.h>  // UNIX standard function definitions
#include <fcntl.h>   // File control definitions
#include <errno.h>   // Error number definitions
#include <termios.h> // POSIX terminal control definitions
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

int main(){

	char *argv="/dev/ttyUSB0";
	int mainfd=open(argv, O_RDONLY | O_NOCTTY | O_NDELAY);
	
	int serverfd;
	sockaddr_in server;
	char chin;
		serverfd=socket(AF_INET,SOCK_STREAM,0);
		cout<<"Serverfd:"<<serverfd<<endl;
		server.sin_family=AF_INET;
		server.sin_port=htons(6000);
		//inet_pton(AF_INET,"127.0.0.1",&server.sin_addr);
		server.sin_addr.s_addr=INADDR_ANY;

	recv(serverfd,&chin,1,0);
	if(chin=='x')
	do
	{		
		read(mainfd, &chin, 1);
		send(serverfd,&chin,1,0);
	}while(1);
		//		cout<<"Character recieved : "<<(int)chin;
	close(serverfd);

	return 0;
}
