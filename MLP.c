
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define d 2 
#define K 3 
#define H1 8 // 5,7,8
#define H2 6 // 3,4,5
#define AF 0 // 1 for logistic and 0 for tanh
#define n 0.1 
#define L  300 // 1, 300, 3000

void load();
void init();
void initWeights();
void initB();
float random_value();
void forwardPass(float a[5]);
void forwardPassForTestPatterns(float a[5]);
void reverse(float a[5]);
void backPropagation(float a[5]);
void gradientDescent();
void openfiles();
void calculateSquareErrors(float a[5]);
void calculateGeneralError(float a[5]);
void calculateWeights();
void finalResults(int r,int w);



float trainPatterns[3000][5]; 
float testPatterns[3000][5]; 

float weightsInsertHidden1[H1][d]; 
float weightsHidden1Hidden2[H2][H1]; 
float weightsHidden2Exit[K][H2]; 

float bHidden1[H1]; 
float bHidden2[H2]; 
float bel[K]; 

float exitHidden1[H1]; 
float exitHidden2[H2]; 
float exitLevel[K]; 
float error[K]; 

float deltaHidden1[H1]; 
float deltaHidden2[H2]; 
float delta[K]; 


float squareError[3000]; 

float generalationError[3000]; 
int right = 0; 
int wrong = 0; 
int cntrTrain = 0; 
int cntrTest = 0; 

FILE *fdtrain=NULL; 
FILE *fdtest=NULL; 
FILE *fp = NULL;
FILE *fright = NULL;
FILE *fwrong = NULL;

int main(int argc, char** argv) {

    int i,j,flag;
    float x[5];

    openfiles();
    load();
    gradientDescent();

    cntrTest=0;
    
    // ig
    for(i=0;i<3000;i++)
    {
        for(j=0;j<5;j++)
            x[j] = testPatterns[i][j];

        forwardPassForTestPatterns(x);
        
        flag = 1; 
        for(j=0;j<K;j++)
        {
            if((x[j+2]==0 && exitLevel[j]>=0.5) || (x[j+2]==1 && exitLevel[j]<0.5))
                flag = 0;  

        }

        if(flag==1)
        {
            right++;
            fprintf(fright,"%f  %f \n",x[0],x[1]);
        }
        else
        {
            wrong++;
            fprintf(fwrong,"%f  %f \n",x[0],x[1]);
        }

        cntrTest++;
    }

    finalResults(right,wrong);


    fclose(fdtrain);
    fclose(fdtest);
    return (EXIT_SUCCESS);
}

void finalResults(int r,int w)
{
	int i;
	double errorFinal;

	 for(i=0;i<3000;i++)
        errorFinal += generalationError[i];

    errorFinal /= 3000;
    fprintf(fp,"Average Error At Testing: %f \n",errorFinal);

    fprintf(fp," Right patterns : %d, percentage: %f. Wrong patterns : %d, percentage: %f \n",r,((100*r) / 3000.0),w,100-((100*r) / 3000.0));
}

void openfiles()
{
	fp = fopen("MLPresults.txt","w");
    if(fp==NULL)
    {
        fprintf(stderr,"can't open file! \n");
        exit(1);
    }

    fdtrain = fopen("trainPatterns.txt","r");
    if(fdtrain==NULL)
    {
        fprintf(stderr,"can't open file! \n");
        exit(1);
    }

    fdtest = fopen("testPatterns.txt","r");
    if(fdtest==NULL)
    {
        fprintf(stderr,"can't open file! \n");
        exit(1);
    }

        fwrong = fopen("wrong.txt","w");
    if(fwrong==NULL)
    {
        fprintf(stderr,"can't open file! \n");
        exit(1);
    }

            fright = fopen("right.txt","w");
    if(fright==NULL)
    {
        fprintf(stderr,"can't open file! \n");
        exit(1);
    }

    fprintf(fp,"Entries: %d  - Hidden layer 1 neurons: %d - hidden layer 2 neurons: %d , L:%d\n",d,H1,H2,L);
    if(AF==0)
        fprintf(fp,"Tanh activation function \n");
    else
        fprintf(fp,"Logistic activation function \n");

    fprintf(fp,"\n\n");
}


void load()
{
    int i,j,l;

    for(i=0;i<3000;i++)
    {

        fscanf(fdtrain,"%10f",&trainPatterns[i][0]);
        fgetc(fdtrain);
        fscanf(fdtrain,"%10f",&trainPatterns[i][1]);
        fgetc(fdtrain);

        fscanf(fdtrain,"%10f",&trainPatterns[i][2]);
        fgetc(fdtrain);
        fscanf(fdtrain,"%10f",&trainPatterns[i][3]);
        fgetc(fdtrain);
        fscanf(fdtrain,"%10f",&trainPatterns[i][4]);

        //get "\n"
        fgetc(fdtrain);

        fscanf(fdtest,"%10f",&testPatterns[i][0]);
        fgetc(fdtest);
        fscanf(fdtest,"%10f",&testPatterns[i][1]);
        fgetc(fdtest);

        fscanf(fdtest,"%10f",&testPatterns[i][2]);
        fgetc(fdtest);
        fscanf(fdtest,"%10f",&testPatterns[i][3]);
        fgetc(fdtest);
        fscanf(fdtest,"%10f",&testPatterns[i][4]);

         //get "\n"
        fgetc(fdtest);
    }
}


float random_value()
{
    int Random_Num;
    float randnum;

	randnum=(RAND_MAX - rand())/(float)(RAND_MAX); 
  	Random_Num=rand()%2;
	if(Random_Num==1) return randnum;
	return (-1*randnum);
}

void init()
{
	initWeights();
	initB();
}

//[-1,1]
void initWeights()
{
    int  i,j;


    for(i=0;i<H1;i++)
        for(j=0;j<d;j++)
            weightsInsertHidden1[i][j] = random_value();

    for(i=0;i<H2;i++)
        for(j=0;j<H1;j++)
            weightsHidden1Hidden2[i][j] = random_value();

    for(i=0;i<K;i++)
        for(j=0;j<H2;j++)
            weightsHidden2Exit[i][j] = random_value();
}

void initB()
{
    int i;

    for(i=0;i<H1;i++)
        bHidden1[i] = random_value();

    for(i=0;i<H2;i++)
        bHidden2[i] = random_value();

    for(i=0;i<K;i++)
        bel[i] = random_value();
}

void gradientDescent()
{
    float a[5],squareErrorAfter=0.0,squareErrorBefore=0.0;
    double r;
    int i, j, k,season, l, s;

  

   	init();

   cntrTrain=0;
   s=0;
   for(k=1;k<=(3000/L);k++)
   {

    for(i=s;i<s+L;i++)
    {
        for(j=0;j<5;j++)
            a[j] = trainPatterns[i][j];
        backPropagation(a);

        squareErrorAfter = squareErrorAfter + squareError[i];
        cntrTrain++;
    }
      calculateWeights(); 
      s=i;
	}
     season = 1;
     fprintf(fp," season %d - square error  : %15f \n",season,squareErrorAfter);
    cntrTrain = 0;



    // rest
    for(l=2;l<=500;l++)
    {
        squareErrorBefore = squareErrorAfter;
        squareErrorAfter = 0;
        cntrTrain=0;
        s=0;
        for(k=1;k<=(3000/L);k++)
   		{
	        for(i=s;i<s+L;i++)
	        {
	            for(j=0;j<5;j++)
	                a[j] = trainPatterns[i][j];
	            backPropagation(a);


	            squareErrorAfter = squareErrorAfter + squareError[i];
	            cntrTrain++;
	        }
	        calculateWeights(); 
			s=i;
    	}
        season++;
     	fprintf(fp," season %d - square error  : %15f \n",season,squareErrorAfter);
        cntrTrain = 0;
    }

    if((r=fabs(squareErrorAfter-squareErrorBefore))<=0.000000001)
    {
    	return;
	}

///////////////////////////
    while(r>0.0000000001)
    {
        squareErrorBefore = squareErrorAfter;
        squareErrorAfter = 0;
        cntrTrain=0;
        s=0;
        for(k=1;k<=(3000/L);k++)
   		{
	        for(i=s;i<s+L;i++)
	        {
	            for(j=0;j<5;j++)
	                a[j] = trainPatterns[i][j];
	            backPropagation(a);


	            squareErrorAfter = squareErrorAfter + squareError[i];
	            cntrTrain++;
	        }
	        calculateWeights(); 
    	}
        season++;
     	fprintf(fp," season %d - square error  : %f \n",season,squareErrorAfter);

        r = abs(squareErrorAfter - squareErrorBefore);
    }
}


void backPropagation(float a[5])
{

    forwardPass(a);
    reverse(a);

}


void forwardPass(float a[5])
{
    int i,j;
    float tmp;

    
    for(i=0;i<H1;i++)
    {
        tmp = bHidden1[i];
        for(j=0;j<d;j++)
           tmp = tmp + weightsInsertHidden1[i][j]*a[i] ;
        if(AF==1)
        	exitHidden1[i] = 1/(1+exp(-tmp));
        else
        	exitHidden1[i] = (exp(tmp) - exp(-tmp))/(exp(tmp) + exp(-tmp));
    }

    
    for(i=0;i<H2;i++)
    {
        tmp = bHidden2[i];
        for(j=0;j<H1;j++)
           tmp = tmp + weightsHidden1Hidden2[i][j]*exitHidden1[j] ;
        if(AF==1)
        	exitHidden2[i] = 1/(1+exp(-tmp));
        else
        	exitHidden2[i] = (exp(tmp) - exp(-tmp))/(exp(tmp) + exp(-tmp));
    }

    
    for(i=0;i<K;i++)
    {
        tmp = bel[i];
        for(j=0;j<H2;j++)
            tmp = tmp + weightsHidden2Exit[i][j]*exitHidden2[j];

        exitLevel[i] = 1/(1+exp(-tmp));

    }

    calculateSquareErrors(a);

}


void calculateSquareErrors(float a[5])
{
	int i;
	
    squareError[cntrTrain] = 0;
    for(i=0;i<K;i++)
    {
        error[i] = a[i+2]- exitLevel[i];
        squareError[cntrTrain] += error[i]*error[i] ;
    }
    squareError[cntrTrain] = squareError[cntrTrain]/2.0;
}


void reverse(float a[5])
{


	int i,j;
	double tmp;
	// exit 
    for(i=0;i<K;i++)
    {
        

            delta[i] = exitLevel[i]*(1 - exitLevel[i])*(exitLevel[i]-a[i+2]);

    }

    
    for(i=0;i<H2;i++)
    {

        tmp = 0;
        for(j=0;j<K;j++)
            tmp = tmp + weightsHidden1Hidden2[i][j]*delta[j];

        if(AF==1) 
        	deltaHidden2[i] = exitHidden2[i]*(1 - exitHidden2[i] )* tmp;
        else 
        {
        	deltaHidden2[i] = (1+exitHidden2[i])*(1 - exitHidden2[i] )* tmp;

		}
    }

    
    for(i=0;i<H1;i++)
    {

        tmp = 0;
        for(j=0;j<H2;j++)
            tmp = tmp + weightsInsertHidden1[i][j]*deltaHidden2[j];
        if(AF==1)
        	deltaHidden1[i] = exitHidden1[i]*(1 - exitHidden1[i] )* tmp;
        else
        	deltaHidden1[i] = (1 + exitHidden1[i])*(1 - exitHidden1[i] )* tmp;

    }

}


void calculateWeights()
{
	int i,j;
   // exit 
    for(i=0;i<K;i++)
    {
        for(j=0;j<H2;j++)
        {
            
            weightsHidden2Exit[i][j] = weightsHidden2Exit[i][j] - n*delta[i]*exitHidden2[j];
        }
       
        bel[i] = bel[i] - n*delta[i];
    }

     
    for(i=0;i<H2;i++)
    {
        for(j=0;j<H1;j++)
        {

            weightsHidden1Hidden2[i][j] = weightsHidden1Hidden2[i][j] - n*deltaHidden2[i]*exitHidden1[j];
        }

        bHidden2[i] = bHidden2[i] - n*deltaHidden2[i];
    }

    
    for(i=0;i<H1;i++)
    {
        for(j=0;j<d;j++)
        {

            weightsInsertHidden1[i][j] = weightsInsertHidden1[i][j] - n*deltaHidden1[i]; 
        }

        bHidden1[i] = bHidden1[i] - n*deltaHidden1[i];
    }
}








void forwardPassForTestPatterns(float a[5])
{
    int i,j;
    float tmp;

     
    for(i=0;i<H1;i++)
    {
        tmp = bHidden1[i];
        for(j=0;j<d;j++)
           tmp = tmp + weightsInsertHidden1[i][j]*a[i] ;
        if(AF==1)
        	exitHidden1[i] = 1/(1+exp(-tmp));
        else
        	exitHidden1[i] = (exp(tmp) - exp(-tmp))/(exp(tmp) + exp(-tmp));
    }

    
    for(i=0;i<H2;i++)
    {
        tmp = bHidden2[i];
        for(j=0;j<H1;j++)
           tmp = tmp + weightsHidden1Hidden2[i][j]*exitHidden1[j] ;
        if(AF==1)
        	exitHidden2[i] = 1/(1+exp(-tmp));
        else
        	exitHidden2[i] = (exp(tmp) - exp(-tmp))/(exp(tmp) + exp(-tmp));
    }

    
    for(i=0;i<K;i++)
    {
        tmp = bel[i];
        for(j=0;j<H2;j++)
            tmp = tmp + weightsHidden2Exit[i][j]*exitHidden2[j];

        exitLevel[i] = 1/(1+exp(-tmp));

    }

 	calculateGeneralError(a);
}

void calculateGeneralError(float a[5])
{
	int i;
	   // calculate errors
    generalationError[cntrTest] = 0;
    for(i=0;i<K;i++)
    {
        error[i] = a[i+2]- exitLevel[i];
        generalationError[cntrTest] += error[i]*error[i] ;
    }
    generalationError[cntrTest] = generalationError[cntrTest]/2.0;
}
