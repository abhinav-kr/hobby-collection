#include <stdio.h>
	#include <stdlib.h>
	#include <iostream>
	#include <termios.h>
	#include <unistd.h>
	#include <sys/select.h>
	#include <sys/types.h>
	#include <pthread.h>

	#define N 10
	#define S 3

	using namespace std;


	struct termios oldterm, newterm;
	void set_unbuffered ( void ) {
	  tcgetattr( STDIN_FILENO, &oldterm );
	  newterm = oldterm;
	  newterm.c_lflag &= ~( ICANON | ECHO );
	  tcsetattr( STDIN_FILENO, TCSANOW, &newterm );
	}
	void set_buffered ( void ) {
	  tcsetattr( STDIN_FILENO, TCSANOW, &oldterm );
	}

	int getch() {
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

	int kbhit ( void ) {
	    int result;
	    fd_set  set;
	    struct timeval tv;

	    FD_ZERO(&set);
	    FD_SET(STDIN_FILENO,&set);  /* watch stdin */
	    tv.tv_sec = 0;
	    tv.tv_usec = 0;             /* don't wait */

	    /* quick peek at the input, to see if anything is there */
	    set_unbuffered();
	    result = select( STDIN_FILENO+1,&set,NULL,NULL,&tv);
	    set_buffered();

	    return result == 1;
	}
	
int main()
{
	if(fork()==0)
	{
		cout<<"1"<<endl;
		usleep(500000);
		cout<<"2"<<endl;
		usleep(500000);
		cout<<"3"<<endl;
	}
	else
	{
		cout<<"4"<<endl;
		usleep(500000);
		cout<<"5"<<endl;
		usleep(500000);
		cout<<"6"<<endl;
	}
	return 0;
}
	
	


	

