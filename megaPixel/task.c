#include <stdio.h>   // Standard input/output definitions 
#include <string.h>  // String function definitions 
#include <unistd.h>  // UNIX standard function definitions 
#include <fcntl.h>   // File control definitions 
#include <errno.h>   // Error number definitions 
#include <termios.h> // POSIX terminal control definitions 

//#include "conio.h"
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


int main(int argc, char* argv[]){

	char ch = 'w';

	int mainfd=open("/dev/ttyUSB0", O_WRONLY | O_NOCTTY | O_NDELAY);
	if (mainfd == -1)
		return -1;


	char chout;

	struct termios options;
   
	fcntl(mainfd, F_SETFL, FNDELAY);                  	// Configure port reading 
	                                     			// Get the current options for the port 
	tcgetattr(mainfd, &options);
	cfsetispeed(&options, B9600);                 		// Set the baud rates to 1200 
	cfsetospeed(&options, B9600);
    
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
   
	char i;
	char a[80];
	for(i=0;i<=255;i++){
		

		
		printf("\n character: ");
		ch = i;
		dec2bin((int)i,a);
		printf("%d : %s",ch,a);
		int n = write(mainfd, &ch, 1);
		sleep(1);
		
	}
	
	close(mainfd);

/*	if (n < 0)
     		return -1;
  	
*/
	return 0;
}

