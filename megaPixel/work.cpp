#include "tetris.h"

/*
void show_op(int &x,int &y,int mx,int my)
{
	x++;
	y+=my;  
	my=0;
	system("clear");
	reset(a);
}*/



void game()
{
    unsigned long int i;
    int a[N][N];
    int b[S][S];
    object f(6);
    pthread_t threads;
    int x=3,y=5;
    int mx,my;
    long int delay=1400000;
    char ch;

	int tf=0,j=1;
    while(x<=9)
    {
	if(tf==1)
	{
		j++;
		break;
	}
        ch=getch();
	mx=my=0;
	if(ch==75 || ch=='d' || ch=='D')/*LEFT*/
		my=1;
	if(ch==77 || ch=='a' || ch=='A')/*RIGHT*/
		my=-1;
	if(ch==72 || ch=='w' || ch=='W')/*UP*/
		f.rotate();
	if(ch==80 || ch=='s' || ch=='S')/*DOWN*/
		delay=300000;

        while(!kbhit() &&( (x<=9 && y<=9)))
        {
		x++;
		y+=my;  
		my=0;
		system("clear");
		reset(a);		   
		impose(a,f.matrix,x,y);
		print(a);
		//usleep(delay);  
		for(i=0;i<=300000000;i++)
		{
			/*if(kbhit())	
			{
				//ch=getch();
				mx=my=0;
				if(ch==75 || ch=='d' || ch=='D')//LEFT
					my=1;
				if(ch==77 || ch=='a' || ch=='A')//RIGHT
					my=-1;
				if(ch==72 || ch=='w' || ch=='W')//UP
					f.rotate();
				if(ch==80 || ch=='s' || ch=='S')//DOWN
					delay=300000;
				//cout<<"KBHIT !!!!!!!!!!!";
				//tf=1;
				//break;
				//continue;
				for(;i<300000000;i++);
			}*/
		}
				
        }

    }
}

int main()
{
    int arr[N][N],temp[S][S];
    reset(arr);
    //input(arr,N);
    //transpose(arr,N);
/*
    print(arr);
    object o1(8);
    o1.rtype=1;
    o1.rotate();
    //impose(arr,o1.matrix,9,9);
    print(o1.matrix);*/

    game();
    while(!kbhit())
    {
            cout<<"KeyBoard HIT !!";
            break;
    }
    return 0;
}

