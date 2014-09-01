#include <stdio.h>   // Standard input/output definitions 
#include <string.h>  // String function definitions 
#include <unistd.h>  // UNIX standard function definitions 
#include <fcntl.h>   // File control definitions 
#include <errno.h>   // Error number definitions 
#include <termios.h> // POSIX terminal control definitions 
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iostream>
//#include "conio.h"


#define u1 "/dev/ttyUSB0"
#define u2 "/dev/ttyUSB1"


using namespace std;

int getche() {
	  struct termios oldt,
		         newt;
	  int            ch;
	  tcgetattr( STDIN_FILENO, &oldt );
	  newt = oldt;
	  newt.c_lflag &= ~( ICANON | ECHO );
	  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	  ch = getchar();
	  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	  return ch;
	}
int main(){//int argc, char* argv[]){

	char ch = 'w';

	int mainfd=open(u1/*"/dev/ttyUSB0"*/, O_WRONLY | O_NOCTTY | O_NDELAY);
	if (mainfd == -1)
		return -1;


	char chout;

	struct termios options;
   
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
	options.c_cflag |=  CS8;                              	// Select 8 data bits 
	options.c_cflag &= ~CRTSCTS;               		// Disable hardware flow control   
 
                                 				// Enable data to be processed as raw input 
	options.c_lflag &= ~(ICANON | ECHO | ISIG);
      
                                        			// Set the new options for the port 
	tcsetattr(mainfd, TCSANOW, &options);
        


	//socket part
/*	int serverfd,status,clientfd,c; 
 	serverfd = socket(AF_INET,SOCK_STREAM,0);
 	sockaddr_in server,client;
	server.sin_family = AF_INET;
	server.sin_port = htons(5000);
	server.sin_addr.s_addr=INADDR_ANY;
 	status=connect(serverfd,(sockaddr*)&server,sizeof(server));
	char *message;
*/

	ch=0;
	char a[40];//={'1','2','3','4','5'},b[5];
	int aa=0,bb=0;


	//Input


	int i;
	for(i=0;i<40;i++)
	{
		a[i]=0;
	}
	for(i=0;i<40;i+=2)
	{
		a[i]=1;
	}
		
	



	while(1){
		printf("\nCharacter Sent: ");
		//ch = getche();
	
		printf("%x",ch);
		if(mainfd=open(u1/*"/dev/ttyUSB0"*/, O_WRONLY | O_NOCTTY | O_NDELAY)==-1)	
		{
			cout<<"ERROR!";
			break;
		}
		
		int n = write(mainfd, &ch, 1);
		ch++;
		//usleep(200000);
		sleep(1);
		mainfd=open("/dev/ttyUSB0", O_RDONLY | O_NOCTTY | O_NDELAY);
	
		read(mainfd,&chout,sizeof(chout));
		printf("\nRevieved Character : %x \n",chout);
		/*ch = 's';
		int n = write(mainfd, &ch, 1);
		*/
		//cout<<endl<<"Character recieved: "<<chout<<endl;
		//b[bb++]=chout;
//		printf("Character Recieved : %d \n",chout);

		}
	

	
	close(mainfd);

	return 0;
}

