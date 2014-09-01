#include <stdio.h>
	#include <stdlib.h>
	#include <iostream>
	#include <termios.h>
	#include <unistd.h>
	#include <sys/select.h>
	#include <sys/types.h>
	#include <pthread.h>


using namespace std;

void *print_message_function( void *ptr );
void *hello(void*);
int i,j;
unsigned long int delay=100000000;
main()
{
     pthread_t thread1, thread2;
     char *message1 = "Thread 1";
     char *message2 = "Thread 2";
     int  iret1, iret2;

    /* Create independent threads each of which will execute function */

     iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1);
     iret2 = pthread_create( &thread2, NULL, hello, (void*) message2);
     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */
     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL); 
     printf("Thread 1 returns: %d\n",iret1);
     printf("Thread 2 returns: %d\n",iret2);
     exit(0);
}

void *print_message_function( void *ptr )
{
  /*   char *message;

	cout<<"one\n\n";
	for(i=0;i<10;i++)
		cout<<" 1 : "<<i<<endl;
     message = (char *) ptr;
     printf("%s \n", message);*/

	cout<<"1"<<endl;
		cout<<"\nwaiting..\n";
		usleep(1000000);
	cout<<"\nwaiting..\n";
		cout<<"2"<<endl;
	cout<<"\nwaiting..\n";
		usleep(1000000);
		cout<<"3"<<endl;
}

void *hello( void *p)
{
	/*cout<<"two\n\n";
	for(j=0;j<10;j++)
		cout<<" 2 : "<<j<<endl;*/
	cout<<"4"<<endl;
	cout<<"\nwaiting..\n";
		usleep(1000000);
		cout<<"5"<<endl;
	cout<<"\nwaiting..\n";
		usleep(1000000);
		cout<<"6"<<endl;
}	
     

