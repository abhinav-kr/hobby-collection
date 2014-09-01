#include "tetris.h"



int num=0;
main()
{

     srand (time(NULL));
	
	strcpy(argv,"/dev/ttyUSB0");

     pthread_t thread1, thread2, thread3;
     char *message1 = "Thread 1";
     char *message2 = "Thread 2";
     char *message3 = "Thread 3";
     int  iret1, iret2,iret3;
     //reset(base);
	int i,j;
     	for(i=BEG;i<=END;i++)
	{
		for(j=0;j<N;j++)
		{
		    base[i][j]=0;
		}
	}


     iret1 = pthread_create( &thread1, NULL, move, (void*) message1);
     iret2 = pthread_create( &thread2, NULL, check, (void*) message2);
   //  iret3 = pthread_create( &thread3, NULL, communicate, (void*) message3);

     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */
     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL); 
  //   pthread_join( thread3, NULL); 
}

void *move( void *ptr )
{
	while(!exitgame)
		while(1)
		{
			x++;
			system("clear");
			reset(a);
			if(x<=M)		   
				impose(a,f.matrix,x,y);
			
			print(a);

			communicate(a);

			usleep(delay);  
		}		
}

void *check( void *ptr )
{
	while(!exitgame)
	{
		ch=getch();
		mx=my=0;
		delay=DELAY;//3000000;
		if(ch==75 || ch=='d' || ch=='D')/*RIGHT*/
			my=1;
		if(ch==77 || ch=='a' || ch=='A')/*LEFT*/
			my=-1;
		if(ch==72 || ch=='w' || ch=='W')/*UP*/
			f.rotate();
		if(ch==80 || ch=='s' || ch=='S')/*DOWN*/
			delay=FAST;
		while(!kbhit())
		{
			//x++;
			y+=my;
			system("clear");
			reset(a);	
			if(coincide(base,f.matrix,x,y) && my!=0)
			{
				y-=my;
			}	   
			impose(a,f.matrix,x,y);
			historyX=x;
			historyY=y;
			history.assign(f);
			print(a);

			//communicate(a);
			object temp((rand()%8));
			future.assign(temp);
			usleep(delay);
		}
	}

}

void communicate(int a[M][N])
{
	cout<<"Communicating "<<++c2<<"times \n";
	char it[48];
	for(i=0;i<47;i++)
	{
		it[i]=0;
	}
	int j,c=0;

	for(i=0;i<M;i++)
	{
		for(j=N/2;j<N;j++)
		{
			if(a[i][j]==1)
				it[c]=(char)(((int)it[c]*2) + 1);
			else
				it[c]=(char)(((int)it[c]*2) );
		}
		c++;
		for(j=0;j<N/2;j++)
		{
			if(a[i][j]==1)
				it[c]=(char)(((int)it[c]*2) + 1);
			else
				it[c]=(char)(((int)it[c]*2) );
		}
		c++;
	}

	int mainfd=open(argv, O_WRONLY | O_NOCTTY | O_NDELAY);
	if (mainfd == -1){
	    printf("File not found %s\n\n\n%d", argv,n);
		//return 1;
	}
	cout<<"Writing 48 characters:\n";
	for(i=0;i<48;i++)
	{
		write(mainfd,&it[i],1);
	}
	char chin='x';
	int tr=0;
	mainfd=open(argv, O_RDONLY | O_NOCTTY | O_NDELAY);
	do
	{		
		read(mainfd, &chin, 1);
		tr++;
	}while(chin!='a');
	cout<<"Character recieved : "<<(int)chin <<"\nand reading "<<tr<<"times.";
//	usleep(10000);		
}			

