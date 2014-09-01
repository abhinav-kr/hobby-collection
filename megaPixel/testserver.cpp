#include <stdio.h>   // Standard input/output definitions
#include <string.h>  // String function definitions
#include <unistd.h>  // UNIX standard function definitions
#include <fcntl.h>   // File control definitions
#include <errno.h>   // Error number definitions
#include <termios.h> // POSIX terminal control definitions
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

#define NUM_WAITING 10

using namespace std;
/* /dev/ttyUSB0 */

//#define u1 "/dev/ttyUSB0"
//#define u2 "/dev/ttyUSB1"
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


int server1()
{
	int serverfd,status,clientfd,c;
	char message[50];
	serverfd = socket(AF_INET,SOCK_STREAM,0);
	cout<<"Socket fd: "<<serverfd<<endl;
	sockaddr_in server,client;
	server.sin_family = AF_INET;
	server.sin_port = htons(5000);
	server.sin_addr.s_addr=INADDR_ANY;
	status=bind(serverfd,(sockaddr*)&server,sizeof(server));
	cout<<"Bind: "<<status<<endl;
	status=listen(serverfd,NUM_WAITING);
	cout<<"Listen: "<<status<<endl;
	
	c=sizeof(sockaddr);
	if(clientfd=accept(serverfd,(sockaddr*)&client,(socklen_t*)&c))
	{
		cout<<"Incoming connection at "<<clientfd<<endl;
		return clientfd;
	}
	return 0;
}

int server2()
{
	int serverfd,status,clientfd,c;
	char message[50];
	serverfd = socket(AF_INET,SOCK_STREAM,0);
	cout<<"Socket fd: "<<serverfd<<endl;
	sockaddr_in server,client;
	server.sin_family = AF_INET;
	server.sin_port = htons(6000);
	server.sin_addr.s_addr=INADDR_ANY;
	status=bind(serverfd,(sockaddr*)&server,sizeof(server));
	cout<<"Bind: "<<status<<endl;
	status=listen(serverfd,NUM_WAITING);
	cout<<"Listen: "<<status<<endl;
	
	c=sizeof(sockaddr);
	if(clientfd=accept(serverfd,(sockaddr*)&client,(socklen_t*)&c))
	{
		cout<<"Incoming connection at "<<clientfd<<endl;
		return clientfd;
	}
	return 0;
}

		
	

int main()
{
	char a[48];
	char b[48];
		char x='x';
	int clientfd1=server1();
	send(clientfd1,&x,1,0);
	int clientfd2=server2();


	for(int i=0;i<48;i++)
	{
		a[i]=(char)(i+1);
		b[i]=1;
	}
	int mainfd;
	char chin='x';
	a[47]=b[47]=0;
	int c=0;

	
	send(clientfd2,&x,1,0);
	while(1)
	{
		//cout<<"enter:\n";
		//cin>>a;
		chin='x';
		if(c++%2==0)
		{
						//write_to_serial(a,"/dev/ttyUSB0");
		send(clientfd1,a,48,0);
		}
		else
		{
						//write_to_serial(b,"/dev/ttyUSB0");
		send(clientfd1,b,48,0);
		}

		//mainfd=open("/dev/ttyUSB0", O_RDONLY | O_NOCTTY | O_NDELAY);
		do
		{		
		recv(clientfd2,&chin,1,0);;
	//	cout<<"Character recieved : "<<(int)chin;
		}while(chin!='a');
				cout<<"Character recieved : "<<(int)chin;
	//	usleep(10000);	
		c=0;
		
	}
	
}	


