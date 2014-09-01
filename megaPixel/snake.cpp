#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/select.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <sys/socket.h>
#include <unistd.h>  // UNIX standard function definitions
#include <fcntl.h>   // File control definitions
#include <errno.h>   // Error number definitions
#include <termios.h> // POSIX terminal control definitions
#define M 24
#define N 16
#define BLINK 100000
#define DELAY 200000
using namespace std;

char *argv="/dev/ttyUSB0";
int num=0,c2=98,c1=0;

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
	    FD_SET(STDIN_FILENO,&set);  // watch stdin 
	    tv.tv_sec = 0;
	    tv.tv_usec = 0;             // don't wait 

	    // quick peek at the input, to see if anything is there
	    set_unbuffered();
	    result = select( STDIN_FILENO+1,&set,NULL,NULL,&tv);
	    set_buffered();

	    return result == 1;
	}

int base[M][N];
int length=4,i,foodx,foody,score=10,speed=DELAY;

int sc[10][5][3];

char ch,scr[5];
int row[1000],col[1000],x=0,y=0,xpos,ypos,mx=0,my=0;

void *communicate( void *);
void *blink(void *);

void setScr()
{

	sc={{{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}},{{1,0,0},{1,0,0},
	       {1,0,0},
		{1,0,0},
		{1,0,0}},{{1,1,1},
               {0,0,1},
	       {0,1,1},
		{1,0,1},
		{1,1,1}},{{1,1,1},
               {0,0,1},
	       {0,1,1},
		{0,0,1},
		{1,1,1}},{{1,0,1},
               {1,0,1},
	       {1,0,1},
		{0,0,1},
		{0,0,1}},{{1,1,1},
               {1,0,0},
	       {1,1,1},
		{0,0,1},
		{1,1,1}},{{1,1,1},
               {1,0,0},
	       {1,1,1},
		{1,0,1},
		{1,1,1}},{{1,1,1},
               {0,0,1},
	       {0,0,1},
		{0,0,1},
		{0,0,1}},{{1,1,1},
               {1,0,1},
	       {1,1,1},
		{1,0,1},
		{1,1,1}},{{1,1,1},
               {1,0,1},
	       {1,1,1},
		{0,0,1},
		{1,1,1}}};	

	/*sc[0]={{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}};

	sc[1]={{1,0,0},
               {1,0,0},
	       {1,0,0},
		{1,0,0},
		{1,0,0}};

	sc[2]={{1,1,1},
               {0,0,1},
	       {0,1,1},
		{1,0,1},
		{1,1,1}};

	sc[3]={{1,1,1},
               {0,0,1},
	       {0,1,1},
		{0,0,1},
		{1,1,1}};

	sc[4]={{1,0,1},
               {1,0,1},
	       {1,0,1},
		{0,0,1},
		{0,0,1}};

	sc[5]={{1,1,1},
               {1,0,0},
	       {1,1,1},
		{0,0,1},
		{1,1,1}};

	sc[6]={{1,1,1},
               {1,0,0},
	       {1,1,1},
		{1,0,1},
		{1,1,1}};

	sc[7]={{1,1,1},
               {0,0,1},
	       {0,0,1},
		{0,0,1},
		{0,0,1}};

	sc[8]={{1,1,1},
               {1,0,1},
	       {1,1,1},
		{1,0,1},
		{1,1,1}};

	sc[9]={{1,1,1},
               {1,0,1},
	       {1,1,1},
		{0,0,1},
		{1,1,1}};*/
}
	

void print(int a[M][N])
	{
		 int i,j;

		for(i=0;i<5;i++)
		{
			for(j=0;j<3;j++)
				cout<<sc[0][i][j];
			cout<<endl;
		}
		
		

	    cout<<"\n\nPrinting "<<++c1 <<"times \n";	
	   
	    for(i=0;i<M;i++)
	    {
		for(j=0;j<N;j++)
		{
		    cout<<a[i][j]<<"  ";
		}
		printf("\n");
	    }
	 }

int initialX=7, initialY=10,exit_game=1;
void generate(int &x, int &y)
{
	int i;
	while(1)
	{
		x=rand()%(N-4)+2;
		y=rand()%(M-6)+3;
		for(i=0;i<length;i++)
		{
			if(row[i]==y && col[i]==x)
				break;
		}
		if(i==length)
			break;
	}
}

int main()
{
	int i,j;
     	for(i=0;i<M;i++)
	{
		for(j=0;j<N;j++)
		{
		    base[i][j]=0;
		}
	}
	pthread_t thread1,thread2; char *message="hello";
	int iret1=pthread_create( &thread1, NULL, communicate, (void*) message);
	int iret2=pthread_create( &thread2, NULL, blink, (void*) message);
	srand(time(NULL));
	
	for(xpos=initialX,ypos=initialY;xpos<length+initialX;xpos++)
	{
		base[ypos][xpos]=1;
		col[x++]=xpos;
		row[y++]=ypos;
	}
	setScr();
	foodx=rand()%(N-4)+2;
	foody=rand()%(M-6)+3;
	base[foody][foodx]=2;
	while(exit_game==1)
	{
		ch=getch();
		mx=my=0;
		if(ch=='d' || ch=='D' || ch=='6') //right
			mx=1;
		if(ch=='a' || ch=='A' || ch=='4') //left
			mx=-1;
		if(ch=='w' || ch=='W' || ch=='8') //up
			my=-1;
		if(ch=='s' || ch=='S' || ch=='2' || ch=='5') //down
			my=1;

		if(ch=='q' || ch=='Q' || ch=='7'){ //top left
			mx=-1;my=-1;}
		if(ch=='e' || ch=='E' || ch=='9'){ //top right
			mx=1;my=-1;}
		if(ch=='c' || ch=='C' || ch=='3'){ //bottom right
			mx=1;my=1;}
		if(ch=='z' || ch=='Z' || ch=='1'){ //bottom left
			mx=-1;my=1;}
		
		//communicate();
		while(!kbhit())
		{
			xpos+=mx;
			ypos+=my;
			if(base[ypos][xpos]==1 )
			{
				exit_game=0;
				break;
			}
			//communicate();
			usleep(speed);
			base[ypos][xpos]=1;
			base[row[0]][col[0]]=0;
			print(base);
			//communicate();
			if(xpos==foodx && ypos==foody)
			{
				length+=2;
				score+=1;
				//foodx=rand()%(N-4)+2;
				//foody=rand()%(M-6)+3;
				generate(foodx,foody);
				base[foody][foodx]=2;
				if(score%2==0 )
					if(speed-=10000 < 0) speed+=10000;
			}
			for(i=0;i<length;i++)
			{
				col[i]=col[i+1];
				row[i]=row[i+1];
				if(xpos==col[i] && ypos==row[i])
				{
					exit_game=0;
				}
			}
			col[length]=xpos;
			row[length]=ypos;
			base[foody][foodx]=2;
		}
		
		if(!((xpos<(N-3) && xpos >0) && (ypos<M && ypos>0)))
		{
			exit_game=0;
		}
	}
     return 0;
}

/*Co-ordinate system : 
The matrix is one of the quadrants.
Top-left corner : 0,0
X increases to the right (X= No of columns of the base matrix)
and Y increases towards bottom(Y= No of Rows of the base matrix)
*/

	
void* blink(void* message)
{
	while(exit_game)
	{
		if(base[foody][foodx])
		{
			usleep(BLINK);
			base[foody][foodx]=0;
		}
		else
		{
			usleep(BLINK);
			base[foody][foodx]=2;
		}
	}
}

void* communicate(void* message)			//int a[M][N])
{
	while(exit_game)
	{	

	
	//imposing the score
int i,j,k,l;
for(i=0,k=1;i<6,k<7;i++,k++)
	{
		for(j=0,l=13;j<3,l<16;j++,l++)
		{
			///if(sc[aa][i][j]==1)
			base[k][l]=0;
		}
	}
	for(i=0,k=1;i<6,k<7;i++,k++)
	{
		for(j=0,l=9;j<3,l<12;j++,l++)
		{
			base[k][l]=0;
		}
	}
	int aa=score%10; int aax=16;
	int bb=score/10; int bby=6;
	
	for(i=0,k=1;i<5,k<6;i++,k++)
	{
		for(j=0,l=13;j<3,l<16;j++,l++)
		{
			if(sc[aa][i][j]==1)
			base[k][l]=1;
		}
	}
	for(i=0,k=8;i<5,k<13;i++,k++)
	{
		for(j=0,l=13;j<3,l<16;j++,l++)
		{
			if(sc[bb][i][j]==1)
			base[k][l]=1;
		}
	}
		






	system("clear");
	cout<<"Communicating "<<++c2<<"times \n";
	print(base);
	char it[48];
	for(i=0;i<47;i++)
	{
		it[i]=0;
	}
	int c=0;

	for(i=0;i<M;i++)
	{
		for(j=N/2;j<N;j++)
		{
			if(base[i][j]>=1)
				it[c]=(char)(((int)it[c]*2) + 1);
			else
				it[c]=(char)(((int)it[c]*2) );
		}
		c++;
		for(j=0;j<N/2;j++)
		{
			if(base[i][j]>=1)
				it[c]=(char)(((int)it[c]*2) + 1);
			else
				it[c]=(char)(((int)it[c]*2) );
		}
		c++;
	}

	int mainfdWRITE=open(argv, O_WRONLY | O_NOCTTY | O_NDELAY);
	if (mainfdWRITE == -1){
	    printf("File not found %s\n\n\n%d", argv,10);
		//return 1;
	}
	cout<<"Writing 48 characters:\n";
	for(i=0;i<48;i++)
	{
		write(mainfdWRITE,&it[i],1);
	}
	//close(mainfdWRITE);
	char chin='x';
	int tr=0;
	int mainfdREAD=open(argv, O_RDONLY | O_NOCTTY | O_NDELAY);
	do
	{		
		read(mainfdREAD, &chin, 1);
		tr++;
	}while(chin!='a');
	close(mainfdREAD);
	cout<<"Character recieved : "<<(int)chin <<"\nand reading "<<tr<<"times.";
	}		
}
