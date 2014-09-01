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

#define u1 "/dev/ttyUSB0"
#define u2 "/dev/ttyUSB1"


int open_port(char* argv){
   int fd = open(argv, O_RDONLY | O_NOCTTY | O_NDELAY);

   if (fd == -1){                                      	 // Could not open the port 
     printf("File not found %s\n", argv);
   }

   return (fd);
}

void dec2bin(long decimal, char *binary)
{
  int  k = 0, n = 0;
  int  neg_flag = 0;
  int  remain;
  int  old_decimal;  
  char temp[80];

  if (decimal < 0)
  {      
    decimal = -decimal;
    neg_flag = 1;
  }
  do 
  {
    old_decimal = decimal;  
    remain    = decimal % 2;
    decimal   = decimal / 2;
    temp[k++] = remain + '0';
  } while (decimal > 0);

  if (neg_flag)
    temp[k++] = '-';       // add - sign
  else
    temp[k++] = ' ';       // space
  // reverse the spelling
  while (k >= 0)
    binary[n++] = temp[--k];

  binary[n-1] = 0;         // end with NULL
}



int main(int argc, char *argv[]){
 
 int mainfd=0;                                            // File descriptor 
 char chout;

 struct termios options, oldt;
 tcgetattr( STDIN_FILENO, &oldt );			//Previous settings  
 
 mainfd = open_port(u1);//"/dev/ttyUSB0");

 fcntl(mainfd, F_SETFL, FNDELAY);                  	// Configure port reading 
	                                     		// Get the current options for the port 
 tcgetattr(mainfd, &options);
 cfsetispeed(&options, B38400);                 		// Set the baud rates to 9600 
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


//SOCKETs
 /*int serverfd,status,clientfd,c,mainfd; 
 serverfd = socket(AF_INET,SOCK_STREAM,0);
 sockaddr_in server,client;
	server.sin_family = AF_INET;
	server.sin_port = htons(5000);
	server.sin_addr.s_addr=INADDR_ANY;
 status=bind(serverfd,(sockaddr*)&server,sizeof(server));
 listen(serverfd,5);
 mainfd=accept(serverfd,(sockaddr*)&server,sizeof(server));*/

char st[80];
 while(1){
   read(mainfd, &chout, sizeof(chout));          	// Read character from ABU 
  // sleep(1);
   if (chout != 0){
	//dec2bin((int)chout,st);
      printf("Got %d : %s.\n", chout,st);
	//char *temp=new char(chout);
	//send(serverfd,temp,50,0);	
      //break;
   }  
 }
 
 close(mainfd);		                                 // Close the serial port 
  
 tcsetattr(STDIN_FILENO, TCSANOW, &oldt);		 //Reset terminal
 
 return chout;

}

