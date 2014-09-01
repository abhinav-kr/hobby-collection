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
	
	#define BEG 3
	#define END 22
	#define MID 6
	#define S 3
	#define DELAY 750000
	#define FAST  500000
	//#define delay 1400000
	using namespace std;
	#define R 20
	#define C 20
	
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

	int score=0;
	
	int super[R][C];	


int c1=0,c2=0;


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


	void encode(int a[R][C]);
	void update();
	void testip(int a[R][C]);
	void convert(int a,char binary[80]);


	void communicate(int a[][N]);
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

	void simpose(int a[R][C],int b[M][N],int bi,int bj)
	{
		int i,j,k,l;
		for(i=(bi-M+1),k=0;i<=bi;i++,k++)
		    {
			for(j=(bj-N+1),l=0;j<=bj;j++,l++)
			{
				if(b[k][l])
					a[i][j]=1;	
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
		    //printf(" %3d ",a[i][j]);
		    cout<<a[i][j]<<"  ";
		}
		printf("\n");
	    }
	  //  communicate(a);//to compress into 48 bytes and send to SURE board	
	    int bi=END-BEG;
	    int bj=N-1;	
	    simpose(super,a,bi,bj);		    	
	    encode(super);
	    update();
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
	
	    void reflectY(int matrix[S][S]){
		int i,j=0;
		for(i=0;i<S;i++)
			for(j=0;j<(S/2);j++)
				swap(matrix[i][j],matrix[i][((S-1)-j)]);
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
	
	int bonus()
	{
		int i,j;
		for(i=BEG;i!=END;i++)
			for(j=0;j<N;j++)
				if(base[i][j])
					return 0;
		return 1;
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
		if(bonus())
			score+=30;
	}
	
	
	void newObject()
	{
		x=2;
		y=MID;
		delay=DELAY;//3000000;
		
		object temp((rand()%8));
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
	
	int messedUp(int a[S][S])
	{
		int j;
		for(j=0;j<S;j++)
			if(a[2][j]==1)
				return 0;
		return 1;
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
		     	     
		    if(!messedUp(b) && x==(M-4))
		    {
			permImpose(base,b,bi,bj);
			newObject();
		    }
		    else if(messedUp && x==(M-3))
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

void convert(int a,char binary[80])
{
	int r,s=0;
	int c=19;
	for(r=0;r<20;r++)
		binary[r]='0';
	while(a>0)
	{
		r=a%2;
		a=a/2;
		binary[c--]=r+'0';
	}
	binary[20]=0;
}
	

void update()
{
	char b[5]={0,0,0,0,0};
	int i,j;
	int count=0,cb=0;
	char temp=0;
	int index=0;
	for(i=0;i<C;i++)
	{
		for(j=0;j< R/*strlen(str[i])*/;j++)
		{
			index=((i*C)+j);
			
			temp=temp<<1;
			temp+=(str[i][j]=='1');	
			if((index+1)%8==0)
			{
				byte[((cb)%5)+1][count]=temp;
				cb++;				
				temp=0;
			}
			if((index+1)%40==0)
				count++;
			//cout<<str[i][j]<<"  ";
			
			
			
		}
		//cout<<endl;
	}
	cout<<"\n\n\n\n cb is :"<<cb<<"\n\n\n"<<"count is: "<<count<<"\n\n\n";;
}

void encode(int a[R][C])
{
	int i,j,k=0;
	for(i=0;i<R;i++)
	{
		k=0;
		for(j=C-1;j>=0;j--)
		{
				k+=(a[j][i] * pow(2,(C-1-j)));
				//cout<< a[j][i] <<"  ";
		}
		//update(k,i);
		char temp[80];
		convert(k,temp);
		strcpy(str[i],temp);
		//cout<<"\n k @ "<<i<< "is : "<<k;
		//cout<<endl;
	}
}


	

	
