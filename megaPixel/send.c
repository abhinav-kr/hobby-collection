#include <stdio.h>   // Standard input/output definitions
#include <string.h>  // String function definitions
#include <unistd.h>  // UNIX standard function definitions
#include <fcntl.h>   // File control definitions
#include <errno.h>   // Error number definitions
#include <termios.h> // POSIX terminal control definitions
#include <iostream>
using namespace std;
/* /dev/ttyUSB0 */

#define u1 "/dev/ttyUSB0"
#define u2 "/dev/ttyUSB1"
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



int write_to_serial(char* ch, char *argv){
	printf("%s ", ch);

	int n=0;	//start	
	int mainfd=open(u1/*argv*/, O_WRONLY | O_NOCTTY | O_NDELAY);
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

int main()
{
	char a[2]="1";

	while(1)
	{
		write_to_serial(a,"/dev/ttyUSB0");
		//sleep(2);
	}
}	


