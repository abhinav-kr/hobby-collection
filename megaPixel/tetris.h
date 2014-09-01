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
	#define M 31
	#define N 16
	
	#define BEG 4
	#define END 27
	#define MID 8
	#define S 4
	#define DELAY 150000
	#define FAST  10000
	//#define delay 1400000
	using namespace std;
	#define R 20
	#define C 20

	time_t seconds;	

	unsigned long int Random=0;
	unsigned long int i;
	int a[M][N];
	int b[S][S];
	int x=3,y=MID;
	int mx,my;
	long int delay=DELAY;//3000000;
	char ch;
	int exitgame=0;
	int base[M][N];
	int historyX,historyY;
	int ss=0;
	int score=0;
	
	int super[R][C];	


unsigned long int c1=0,c2=0;


	//encoder ->
	char byte[6][10];
	char str[C][80];
	char argv[30];
	int  k = 0, n = 0;
	
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





	void convert(int a,char binary[80]);


	void *communicate(void*);//int a[][N]);
	void *move(void*);
	void *check(void*);

	void reset(int a[M][N]){
	    int i,j;

	    for(i=BEG;i<=END;i++)
	    {
		for(j=0;j<N;j++)
		{
		    if(base[i][j]==1)
			a[i][j]=1;
		    else
			a[i][j]=0;
		}
	    }
	}

	void reset(int a[S][S]){
	    int i,j;

	    for(i=0;i<S;i++)
	    {
		for(j=0;j<S;j++)
		{
		    a[i][j]=0;
		}
	    }
	}

	void reset(int a[R][C]){
	    int i,j;

	    for(i=0;i<R;i++)
	    {
		for(j=0;j<C;j++)
		{
		    a[i][j]=0;
		}
	    }
	}

	void print(int a[M][N])
	{
	    cout<<"Printing "<<++c1 <<"times \n";	
	    int i,j;
	    for(i=BEG;i<=END;i++)
	    {
		for(j=0;j<N;j++)
		{
		    cout<<a[i][j]<<"  ";
		}
		printf("\n");
	    }
	 
	    int bi=END-BEG;
	    int bj=N-1;	
	}

	void print(int a[S][S]){
	    int i,j;
	    for(i=0;i<S;i++)
	    {
		for(j=0;j<S;j++)
		{
		    cout<<a[i][j]<<"  ";
		}
		printf("\n");
	    }
	    printf("\n\n");
	}
	

	void swap(int &a, int &b){
		if(a!=b)
		{
			a=a+b;
			b=a-b;
			a=a-b;
		}
	}

	void transpose(int a[S][S], int type){ //Type 4 is a line.!
	    int i,j;
	    int n=S;
	    if(type!=4)
	   	n=S-1;
	    for(i=0;i<n;i++)
	    {
		for(j=0;j<i;j++)
		{
		    swap(a[j][i],a[i][j]);
		}
	    }
	}

	void input(int a[M][N]){
	    int i,j;
	    for(i=BEG;i<=END;i++)
	    {
		for(j=0;j<N;j++)
		{
		        cin>>a[i][j];
		}
	    }
	}
	void input(int a[S][S]){
	    int i,j;
	    for(i=0;i<S;i++)
	    {
		for(j=0;j<S;j++)
		{
		        cin>>a[i][j];
		}
	    }
	}

	class object
	{
	    public:
	    int otype;//1-"Square" 2-"7" 3-"Ulta 7" 4-"line" 5-"nln" 6-"S" 7-"Ulta S" 8-"L"
	    int rtype;//1,2,3,4
	    int matrix[S][S];
	    object(int type){
		otype=type;
		rtype=1;
		reset(matrix);
		if(type==1)
		    matrix[2][2]=matrix[2][1]=matrix[1][1]=matrix[1][2]=1;

		else if(type==2)
		    matrix[0][0]=matrix[0][1]=matrix[1][1]=matrix[2][1]=1;

		else if(type==3)
		    matrix[0][0]=matrix[1][0]=matrix[2][0]=matrix[0][1]=1;

		else if(type==4) //Donmt change the type
		    matrix[0][0]=matrix[1][0]=matrix[2][0]=matrix[3][0]=1;

		else if(type==5) 
		    matrix[0][0]=matrix[1][0]=matrix[2][0]=matrix[1][1]=1;

		else if(type==6)
		    matrix[0][0]=matrix[1][0]=matrix[1][1]=matrix[2][1]=1;

		else if(type==7)
		    matrix[0][1]=matrix[1][0]=matrix[1][1]=matrix[2][0]=1;

		else if(type==8)
		    matrix[0][0]=matrix[1][0]=matrix[1][1]=1;
	    }
	
	    void reflectY(int matrix[S][S]){
		int i,j=0;
		int n=3;
		if(otype==4)
			n=4;
		for(i=0;i<n;i++)
			for(j=0;j<(n/2);j++)
				swap(matrix[i][j],matrix[i][((n-1)-j)]);
	    }
			
	    void rotate(){
		rtype++;
		if(rtype>4)
		    rtype=1;
		if(otype!=0)
		{
			transpose(matrix,otype);	
			reflectY(matrix);			
		}
	    }
		
	    void assign(object t){
		otype=t.otype;
		rtype=1;
		int i,j;
		for(i=0;i<S;i++)
		{
			for(j=0;j<S;j++)
			{
				matrix[i][j]=t.matrix[i][j];
			}
		}
	    }

	}history(0),f((rand()%8)),future((rand()%8));

	void downShift(int a[M][N],int row){
		int i,j;
		for(i=row-1;i>=3;i--)
		{
			for(j=0;j<N;j++)
			{
				a[i+1][j]=a[i][j];
			}
		}
	}
	
	void permImpose(int a[M][N],int b[S][S],int bi, int bj){
		int i,j,k=0,l=0;
		int count=0;
	        for(i=(bi-S+1),k=0;i<=bi;i++,k++)
	        {
		    for(j=(bj-S+1),l=0;j<=bj;j++,l++)
		    {
		        if(a[i][j]||b[k][l])
		        {
		            a[i][j]=1;
		        }
		    }
	        }

		//Checking if a line is completed
		int check=1;
		count=0;
		for(i=(M-4);i>=(4);i--)
		{
			check=1;
			for(j=0;j<N;j++)
			{
				if(a[i][j]==0)	
				{
					check=0;
					break;
				}
			}
			if(check)
			{	
				downShift(a,i);
				count++;
			}	
		}
		//SCORING
		if(count==1)
			score+=1;
		if(count==2)
			score+=3;
		if(count==3)
			score+=8;
		if(count==4)
			score+=15;	
	}
	
	
	void newObject()
	{
		x=2;
		y=MID;
		delay=DELAY;//3000000;
		//seconds=time(NULL);
		object temp(c1%8);
		temp.assign(future);
		f.assign(temp);
	}
	
	int coincide(int a[M][N],int b[S][S],int bi,int bj)
	{
		int i,j,k,l;
		for(i=(bi-S+1),k=0;i<=bi;i++,k++)
			for(j=(bj-S+1),l=0;j<=bj;j++,l++)
				if(a[i][j]&&b[k][l]) //if both are 1s
					return 1;
		return 0;
	}
	
	int row_is_high(int a[S][S], int row) //Row is high if any of the elements in that row are high
	{
		int j; 
		for(j=0;j<S;j++)
			if(a[row][j]==1)
				return 1;
		return 0;
	}		
					

	void impose(int a[M][N],int b[S][S],int bi, int bj){
	    int i,j,k=0,l=0;
   	    int coin=0,temp;

		    for(i=(bi-S+1),k=0;i<=bi;i++,k++)
		    {
			for(j=(bj-S+1),l=0;j<=bj;j++,l++)
			{
			    if((b[k][l] && !base[i][j]))
			    {
				a[i][j]=1;
			    }
			    if(base[i][j]==1 && b[k][l]==1)
			    {
				coin=1;
				break;
			    }
			}
			if(coin)
				break;
		    }
		     	     
		int g=0;//No of empty rows from the bottom

		for(i=S-1;i>=0;i--)
		{
			if(row_is_high(b,i))
			{
				g=S-i-1;break; 
			}
		}
		
		if(x==(END+g) && !coin)
		{
			permImpose(base,b,bi,bj);
			newObject();
		}
		  
		    if(coin && my==0)
		    {
			coin=0;
			permImpose(base,history.matrix,historyX,historyY);	
			newObject();
		    }	
		my=0;
	      } 


