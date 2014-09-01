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


int write_to_serial(char* ch, char *argv){
	printf("%s ", ch);

	int n=0;	//start	
	int mainfd=open(argv, O_WRONLY | O_NOCTTY | O_NDELAY);
	if (mainfd == -1){
	    printf("File not found %s\n\n\n%d", argv,n);
		return 1;
	}

	char chout;

	struct termios options, oldt;
	tcgetattr( STDIN_FILENO, &oldt );			//Previous settings
	
	fcntl(mainfd, F_SETFL, FNDELAY);                  	// Configure port reading
	                                     			// Get the current options for the port
	tcgetattr(mainfd, &options);
	cfsetispeed(&options, B38400);                 		// Set the baud rates to 1200
	cfsetospeed(&options, B38400);

                                   				// Enable the receiver and set local mode
	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~PARENB; 				// Mask the character size to 8 bits, no parity
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |=  CS8;                              	// Select 8 data bitsev
	options.c_cflag &= ~CRTSCTS;               		// Disable hardware flow control

                                 				// Enable data to be processed as raw input
	options.c_lflag &= ~(ICANON | ECHO | ISIG);

                                        			// Set the new options for the port
	tcsetattr(mainfd, TCSANOW, &options);



	cout<<"\nNo. of characters to send: "<<strlen(ch)<<endl;
	
	//loop	
	for(int i=0; i<strlen(ch); i++){
	  n+= write(mainfd, &ch[i], 1);
	}
	char v=48;
	write(mainfd, &v, 1);

	/*if (n < 0){
	  printf("Sending data to serial port failed!\n");
	  return 1;
	}*/
	
	/*
	ch=ch+1;
	n = write(mainfd, &ch, 1);
 	
	if (n < 0){
	  printf("Sending data to serial port failed!\n");
	  return 1;
	}
	*/
	
	//end
	//close(mainfd);
	
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);		//Reset terminal
	return 0;
}
int main(){

	char *argv="/dev/ttyUSB0";
	
	
	int serverfd;
	sockaddr_in server;
	
		serverfd=socket(AF_INET,SOCK_STREAM,0);
		cout<<"Serverfd:"<<serverfd<<endl;
		server.sin_family=AF_INET;
		server.sin_port=htons(5000);
		//inet_pton(AF_INET,"127.0.0.1",&server.sin_addr);
		server.sin_addr.s_addr=INADDR_ANY;

	char chin;
	int n=10;
	int mainfd=open(argv, O_WRONLY | O_NOCTTY | O_NDELAY);
	if (mainfd == -1){
	    printf("File not found %s\n\n\n%d", argv,n);
		return 1;
	}



	recv(serverfd,&chin,1,0);
	if(chin=='x')
	while(1)
	{	
		recv(serverfd,&chin,1,0);
		write(mainfd,&chin,1);
	}
	close(serverfd);			
	

	return 0;
}
