#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

double randomizer(double high,double low);

FILE *fp=NULL;



int main(void)
{
int i;
double x1,x2;
	
	fp = fopen("clusteringPatterns.txt", "w");
	
	
	srand(time(NULL));
	
	for(i=0;i<100;i++)
	{
		x1=randomizer(0.3,-0.3);
		x2=randomizer(0.3,-0.3);
		fprintf(fp,"%3f \t",x1);
		fprintf(fp,"%3f \t",x2);
		fprintf(fp,"\n");
		
	}
	
	for(i=0;i<100;i++)
	{
		x1=randomizer(-0.5,-1.1);
		x2=randomizer(1.1,0.5);
		fprintf(fp,"%3f \t",x1);
		fprintf(fp,"%3f \t",x2);
		fprintf(fp,"\n");
	}
        
    for(i=0;i<100;i++)
	{
		x1=randomizer(-0.5,-1.1);
		x2=randomizer(-0.5,-1.1);
		fprintf(fp,"%3f \t",x1);
		fprintf(fp,"%3f \t",x2);
		fprintf(fp,"\n");
	}
        
    for(i=0;i<100;i++)
	{
		x1=randomizer(1.1,0.5);
		x2=randomizer(-0.5,-1.1);
		fprintf(fp,"%3f \t",x1);
		fprintf(fp,"%3f \t",x2);
		fprintf(fp,"\n");
	}
        
    for(i=0;i<100;i++)
	{
		x1=randomizer(1.1,0.5);
		x2=randomizer(1.1,0.5);
		fprintf(fp,"%3f \t",x1);
		fprintf(fp,"%3f \t",x2);
		fprintf(fp,"\n");
	}
	
	
	fclose(fp);
	
return(0);
}


double randomizer(double high, double low)
{ 	

	return ( (double)rand() * ( high - low ) ) / (double)RAND_MAX + low;
}	
