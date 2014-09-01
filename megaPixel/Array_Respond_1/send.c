#include <stdio.h>   // Standard input/output definitions
#include <string.h>  // String function definitions
#include <unistd.h>  // UNIX standard function definitions
#include <fcntl.h>   // File control definitions
#include <errno.h>   // Error number definitions
#include <termios.h> // POSIX terminal control definitions
#include <iostream>
using namespace std;
/* /dev/ttyUSB0 */

int write_to_serial(char* ch, char *argv){
	printf("%s ", ch);

	//start	
	int mainfd=open(argv, O_WRONLY | O_NOCTTY | O_NDELAY);
	if (mainfd == -1){
	    printf("File not found %s\n", argv);
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
	options.c_cflag |=  CS8;                              	// Select 8 data bits
	options.c_cflag &= ~CRTSCTS;               		// Disable hardware flow control

                                 				// Enable data to be processed as raw input
	options.c_lflag &= ~(ICANON | ECHO | ISIG);

                                        			// Set the new options for the port
	tcsetattr(mainfd, TCSANOW, &options);


	int n;
	cout<<"\nNo. of characters to send: "<<strlen(ch)<<endl;
	
	//loop	
	for(int i=0; i<strlen(ch); i++){
	  n += write(mainfd, &ch[i], 1);
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
	close(mainfd);
	
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);		//Reset terminal
	return 0;
}

main()
{
	char ch;
	/*cout<<"Enter character:\n";
	cin>>ch;
	printf("%c ", ch);*/

	//start	
	int mainfd=open("/dev/ttyUSB0", O_WRONLY | O_NOCTTY | O_NDELAY);
	if (mainfd == -1){
	    printf("File not found \n");
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
	options.c_cflag |=  CS8;                              	// Select 8 data bits
	options.c_cflag &= ~CRTSCTS;               		// Disable hardware flow control

                                 				// Enable data to be processed as raw input
	options.c_lflag &= ~(ICANON | ECHO | ISIG);

                                        			// Set the new options for the port
	tcsetattr(mainfd, TCSANOW, &options);


	int n=10;
	char x[n];
	int i;
	for(i=0;i<n;i++)
		x[i]=(char)i+1;
	char result[n];
	int c=0;
	//cout<<"\nNo. of characters to send: "<<endl;
	ch=0;
	while(c<n)
	{
		mainfd=open("/dev/ttyUSB0", O_WRONLY | O_NOCTTY | O_NDELAY);
		write(mainfd, &x[c++], 1);
		ch=x[c-1];
		cout<<"\n\nSending Character : "<<(int)ch;
	/*	mainfd=open("/dev/ttyUSB0", O_RDONLY | O_NOCTTY | O_NDELAY);
		read(mainfd, &result[c++], 1);
		cout<<"\nCharacter Received : "<<(int)result[c-1];
	//	usleep(500000);
	*/
		ch++;
	}
	c=0;
	while(c<2)
	{
		mainfd=open("/dev/ttyUSB0", O_RDONLY | O_NOCTTY | O_NDELAY);
		read(mainfd, &chout, 1);
		result[c++]=chout;
		cout<<"Character recieved : "<<(int)chout;
	}


	cout<<"\n\nArray recieved";
	for(c=0;c<2;c++)
	{
		cout<<result[c]<<endl;
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
	close(mainfd);
	
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);		//Reset terminal
}
