#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string.h>
#define R 20
#define C 20
using namespace std;
char byte[5][20];
int  k = 0, n = 0;
void dec2bin(long decimal, char binary[80])
{
  
  int  neg_flag = 0;
  int  remain;
  int  old_decimal;  
  char temp[80];

  if (decimal < 0)
  {      
    decimal = -decimal;
    neg_flag = 1;
  }
  do 
  {
    old_decimal = decimal;  
    remain    = decimal % 2;
    decimal   = decimal / 2;
    temp[k++] = remain + '0';
  } while (decimal > 0);

  /*if (neg_flag)
    temp[k++] = '-';       
  else
    temp[k++] = ' ';       */
  while (k >= 0)
    binary[n++] = temp[--k];

 
	char qw[11];
	for(k=1;k<=n;k++)
		qw[k-1]=binary[k];
	for(k=0;k<n;k++)
		binary[k]=qw[k];

	int c=0,m=0;

	for(k=0;k<10;k++)
		binary[k]='0';
	binary[k]=0;
	//cout<<endl<<"k=:"<<k<<endl<<"n:"<<n<<endl;
	for(c=(k-n+2);c<k;c++)
	{
		binary[c]=qw[m];
		//cout<<"binary["<<c<<"]=qw["<<m++<<"];\n\n";
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

void testip(int a[20][20])
{               
	//cout <<"i ma here";
	reset(a);			
	a[19][0]=1;
}
void update(char a[80],int column)
{
	int i=0,c=1;
	for(i=0;a[i]!='\0';i++)
	{
		char t=0,t2;
		t=t<<1;
		t+=a[i];
		if((i+1)%8==0)
		{
			c++;
			byte[c][column]=t;
			t=0;
		}
	}
}

void encode(int a[R][C])
{
	char s1[80];
	char s2[80];
	int i,j;
	for(j=0;j<C;j+=2)
	{
		strcpy(s1,"");
		strcpy(s2,"");
		int temp1=0,temp2=0;
		for(i=0;i<R;i++)
		{
			if(a[i][j]==1)
			{	
				temp1+=pow(2,((R-1)-i));
			}
			if(((j+1)<C) &&  (a[i][j+1]==1) )
			{
				temp2+=pow(2,((R-1)-i));
			}
		}
		dec2bin((long)temp1,s1);
		dec2bin((long)temp2,s2);
		cout<<endl<<temp1<<":"<<s1<<endl<<temp2<<":"<<s2<<endl;
		strcat(s2,s1);
		cout<<s2<<endl;
		//update(s2,j);
	}
}	

main()
{
	char bin[80];
	long int x;
	
	
	int a[R][C];
	int i,j;
	/*dec2bin(255,bin);
	cout<<strlen(bin)<<"   "<<bin<<endl;*/
	reset(a);
	testip(a);
	for(i=0;i<5;i++)
	{
		for(j=0;j<20;j++)
			byte[i][j]='0';
		cout<<endl;
	}
	encode(a);
	for(i=0;i<5;i++)
	{
		for(j=0;j<20;j++)
			cout<<byte[i][j];
		cout<<endl;
	}

}
