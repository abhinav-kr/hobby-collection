#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#define R 20
#define C 20
using namespace std;
char byte[6][10];
char str[C][80];
int  k = 0, n = 0;

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

void testip(int a[R][C])
{               
	//cout <<"i ma here";
	reset(a);			
	/*a[19][0]=1;
	a[15][0]=1;
	a[0][0]=a[4][4]=a[9][8]=1;*/
	int i=0;
	for(i=0;i<R;i++)
		a[i][19]=a[i][1]=1;
	a[19][19]=0;
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


main()
{
	char bin[80];
	long int x;
	int a[R][C];
	int i,j;
//	convert(255,bin);
//	cout<<strlen(bin)<<"   "<<bin<<endl;
	reset(a);
	testip(a);
	for(i=0;i<5;i++)
	{
		for(j=0;j<10;j++)
			byte[i][j]=0;
		cout<<endl;
	}	
	cout<<endl<<endl;
	for(i=0;i<C;i++)
	{
		strcpy(str[i],"");
	}
	encode(a);
	
	for(i=0;i<R;i++)
	{
		for(j=0;j<C;j++)
		{
			cout<<a[i][j]<<"  ";
		}	
		cout<<endl;
	}
	int index=0;
	cout<<endl<<endl;
	for(i=0;i<C;i++)
	{
		
		for(j=0;j<20;j++)
		{
			index=((i)*20+j);
			
			cout<<str[i][j];

			if((index+1)%8==0 && index!=2)
				cout<<"  ";
			if((index+1)%40==0)
				cout<<"\n";
		}
		//cout<<endl;
	}
	cout<<endl<<endl;
	update();
	for(i=1;i<=10;i++)
		printf(" %9d ",i);
	cout<<"\n-------------------------------------------------------------------------------------------------------------\n\n";
	byte[0][0]=0x0;
	byte[0][1]=0x1;
	byte[0][2]=0x2;
	byte[0][3]=0x3;
	byte[0][4]=0x4;
	byte[0][5]=0x5;
	byte[0][6]=0x6;
	byte[0][7]=0x7;
	byte[0][8]=0x8;
	byte[0][9]=0x9;	
	
	for(i=0;i<=5;i++)
	{
//		printf(" %dth and %dth column : ",(2*(i+1))-1,(2*(i+1)));
		for(j=0;j<10;j++)
		{
			//cout<<byte[i][j];
			printf(" %9d",(unsigned int)byte[i][j]);
		}
		cout<<endl;
	}

	//cout<<"\n\n\n"<<(j==20);

}
