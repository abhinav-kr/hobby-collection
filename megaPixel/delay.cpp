#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <pthread.h>

using namespace std;

int main()
{	
	long int i,j=0;
	for(i=1;i<30000000000;i++)
	{
		if(i%200000000==0)
			cout<<j++<<endl;
	}
	return 0;
}

