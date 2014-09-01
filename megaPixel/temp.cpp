	#include <stdio.h>
	#include <stdlib.h>
	#include <iostream>
	#include <termios.h>
	#include <unistd.h>
	#include <sys/select.h>
	#include <sys/types.h>
	#include <pthread.h>
	#define M 16
	#define N 10
	#define BEG 3
	#define END 12
	#define S 3
	#define DELAY 3000000
	using namespace std;

	
	unsigned long int i;
	int a[M][N];
	int b[S][S];
	int x=3,y=5;
	int mx,my;
	long int delay=DELAY;
	char ch;
	int exitgame=0;
	int base[M][N];
	int historyX,historyY;
	
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

	void print(int a[M][N]){
	    int i,j;
	    for(i=BEG;i<=END;i++)
	    {
		for(j=0;j<N;j++)
		{
		    //printf(" %3d ",a[i][j]);
		    cout<<a[i][j]<<"  ";
		}
		printf("\n");
	    }

	    printf("\n\n");
	}

	void print(int a[S][S]){
	    int i,j;
	    for(i=0;i<S;i++)
	    {
		for(j=0;j<S;j++)
		{
		    //printf(" %3d ",a[i][j]);
		    cout<<a[i][j]<<"  ";
		}
		printf("\n");
	    }
	    printf("\n\n");
	}
	int swappable(int a,int b){
		if((a&&!b)||(b&&!a)) // if(a XOR b ==1) => only if a and b are different
			return 1;
		else
			return 0;
	}
	void swap(int &a, int &b){
		if(swappable(a,b))
		{
			a=a+b;
			b=a-b;
			a=a-b;
		}
	}

	void transpose(int a[S][S]){
	    int i,j;
	    int n=S;
	    for(i=0;i<n;i++)
	    {
		for(j=0;j<i;j++)
		{
		    if(swappable(a[j][i],a[i][j]))
			    swap(a[j][i],a[i][j]);
		}
	    }
	}
	
	void reflectY(int matrix[S][S]){
		int i,j=0;
		for(i=0;i<S;i++)
			for(j=0;j<(S/2);j++)
				swap(matrix[i][j],matrix[i][((S-1)-j)]);
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
	    int otype;//1-"Square" 2-"7" 3-"Ulta 7" 4-"line" 5-"nlm" 6-"S" 7-"Ulta S" 8-"L"
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

		else if(type==4)
		    matrix[0][0]=matrix[1][0]=matrix[1][0]=1;

		else if(type==5)
		    matrix[0][0]=matrix[1][0]=matrix[2][0]=matrix[1][1]=1;

		else if(type==6)
		    matrix[0][0]=matrix[1][0]=matrix[1][1]=matrix[2][1]=1;

		else if(type==7)
		    matrix[0][1]=matrix[1][0]=matrix[1][1]=matrix[2][0]=1;

		else if(type==8)
		    matrix[0][0]=matrix[1][0]=matrix[1][1]=1;
	    }
		
	    void rotate(){
		rtype++;
		if(rtype>4)
		    rtype=1;
		if(otype!=0)
		{
			transpose(matrix);	
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
	}history(0),f((rand()%8));

	void permImpose(int a[M][N],int b[S][S],int bi, int bj){
		int i,j,k=0,l=0;

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
	}
	
	
	void newObject()
	{
		x=2;
		y=5;
		delay=DELAY;
		object temp((rand()%8));
		f.assign(temp);
	}
	
	int coincide(int a[M][N],int b[M][N])
	{
		int i,j;
		for(i=BEG;i<=END;i++)
			for(j=0;j<N;j++)
				if(a[i][j]&&b[i][j]) //if both are 1s
					return 1;
		return 0;
	}
			
					

	void impose(int a[M][N],int b[S][S],int bi, int bj){
	    int i,j,k=0,l=0;
   	    int coin=0;
	    for(i=(bi-S+1),k=0;i<=bi;i++,k++)
	    {
		for(j=(bj-S+1),l=0;j<=bj;j++,l++)
		{
		    if(b[k][l])
		    {
		        a[i][j]=1;
		    }
		    
		    if(base[i][j])for(i=0;i<S;i++)
		    {
			for(j=0;j<S;j++)
			{
			    a[i][j]=0;
			}
		    }
		    {
			coin=1;
			break;
		    }
		}
		if(coin)
			break;
	    }
	     	     
	     if(x==(M-4))
	    {
		permImpose(base,b,bi,bj);
		newObject();
	    }
	    if(coin)
	    {
		coin=0;
		permImpose(base,history.matrix,historyX,historyY);	
		newObject();
	    }	 
	}
	
