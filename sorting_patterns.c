#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


FILE *ftrain=NULL;
FILE *ftest=NULL;

double randomizer(double high, double low);
void makeTrain();
void makeTest();

int main(void)
{
int i;
double x1,x2;
	
	ftrain=fopen("trainPatterns.txt", "w");
	ftest=fopen("testPatterns.txt", "w");
	
	srand(time(NULL));
	
	makeTrain();
	makeTest();
	
	
	
	
	
	fclose(ftrain);
	fclose(ftest);
	
return(0);
}




double randomizer(double high, double low)
{ 	

	return ( (double)rand() * ( high - low ) ) / (double)RAND_MAX + low;
}


void makeTrain()
{
	int i;
	double x1, x2;
	
	for(i=0;i<1500;i++)
	{
		x1=randomizer(2,0);
		x2=randomizer(2,0);
		fprintf(ftrain,"%3f \t",x1);
		fprintf(ftrain,"%3f \t",x2);
		if(((x1-1)*(x1-1)+(x2-1)*(x2-1))<=0.16 || ((x1+1)*(x1+1)+(x2+1)*(x2+1))<=0.16)
		{
                        fprintf(ftrain,"0\t");
			fprintf(ftrain,"0\t");
			fprintf(ftrain,"1\t");
			fprintf(ftrain,"\n");
		}
		else if(((x1-1)*(x1-1)+(x2-1)*(x2-1))<0.64 || ((x1+1)*(x1+1)+(x2+1)*(x2+1))<0.64)
		{
                        fprintf(ftrain,"0\t");
			fprintf(ftrain,"1\t");
			fprintf(ftrain,"0\t");
			fprintf(ftrain,"\n");
		}
		else 
		{
			fprintf(ftrain,"1\t");
			fprintf(ftrain,"0\t");
                        fprintf(ftrain,"0\t");
			fprintf(ftrain,"\n");
		}
		
	}
	
	for(i=0;i<1500;i++)
	{
		x1=randomizer(0,-2);
		x2=randomizer(0,-2);
		fprintf(ftrain,"%3f \t",x1);
		fprintf(ftrain,"%3f \t",x2);
		if(((x1-1)*(x1-1)+(x2-1)*(x2-1))<=0.16 || ((x1+1)*(x1+1)+(x2+1)*(x2+1))<=0.16)
		{
                        fprintf(ftrain,"0\t");
			fprintf(ftrain,"0\t");
			fprintf(ftrain,"1\t");
			fprintf(ftrain,"\n");
		}
		else if(((x1-1)*(x1-1)+(x2-1)*(x2-1))<0.64 || ((x1+1)*(x1+1)+(x2+1)*(x2+1))<0.64)
		{
                        fprintf(ftrain,"0\t");
			fprintf(ftrain,"1\t");
			fprintf(ftrain,"0\t");
			fprintf(ftrain,"\n");
		}
		else 
		{
			fprintf(ftrain,"1\t");
			fprintf(ftrain,"0\t");
                        fprintf(ftrain,"0\t");
			fprintf(ftrain,"\n");
		}
		
	}
}

void makeTest()
{
	int i;
	double x1,x2;
	
	for(i=0;i<1500;i++)
	{
		x1=randomizer(0,-2);
		x2=randomizer(0,-2);
		fprintf(ftest,"%3f \t",x1);
		fprintf(ftest,"%3f \t",x2);
		if(((x1-1)*(x1-1)+(x2-1)*(x2-1))<=0.16 || ((x1+1)*(x1+1)+(x2+1)*(x2+1))<=0.16)
		{
                        fprintf(ftest,"0\t");
			fprintf(ftest,"0\t");
			fprintf(ftest,"1\t");
			fprintf(ftest,"\n");
		}
		else if(((x1-1)*(x1-1)+(x2-1)*(x2-1))<0.64 || ((x1+1)*(x1+1)+(x2+1)*(x2+1))<0.64)
		{
                        fprintf(ftest,"0\t");
			fprintf(ftest,"1\t");
			fprintf(ftest,"0\t");
			fprintf(ftest,"\n");
		}
		else 
		{
			fprintf(ftest,"1\t");
			fprintf(ftest,"0\t");
                        fprintf(ftest,"0\t");
			fprintf(ftest,"\n");
		}
		
	}
	
	for(i=0;i<1500;i++)
	{
		x1=randomizer(2,0);
		x2=randomizer(2,0);
		fprintf(ftest,"%3f \t",x1);
		fprintf(ftest,"%3f \t",x2);
		if(((x1-1)*(x1-1)+(x2-1)*(x2-1))<=0.16 || ((x1+1)*(x1+1)+(x2+1)*(x2+1))<=0.16)
		{
                        fprintf(ftest,"0\t");
			fprintf(ftest,"0\t");
			fprintf(ftest,"1\t");
			fprintf(ftest,"\n");
		}
		else if(((x1-1)*(x1-1)+(x2-1)*(x2-1))<0.64 || ((x1+1)*(x1+1)+(x2+1)*(x2+1))<0.64)
		{
                        fprintf(ftest,"0\t");
			fprintf(ftest,"1\t");
			fprintf(ftest,"0\t");
			fprintf(ftest,"\n");
		}
		else 
		{
			fprintf(ftest,"1\t");
			fprintf(ftest,"0\t");
                        fprintf(ftest,"0\t");
			fprintf(ftest,"\n");
		}
		
	}
	
	
}
