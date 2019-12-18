#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


#define M 7 // M = 2,3,5, or 7
#define N 500 

void load_points_from_file();
void first_centers();
void training();



FILE *fp = NULL; 
FILE *frl = NULL;

float points[N][2]; 
double center[M][2]; 
double nn=0.1;

int main(int argc, char** argv) {
    
    int i,j,position;
    double d[M],minimum,sum,total[M],previous;

    fp = fopen("clusteringPatterns.txt","r");
    if(fp==NULL)
    {
        fprintf(stderr,"can't open file! \n");
        exit(1);
    }
    
    
    frl = fopen("resultsLVQ.txt","w");
    if(frl==NULL)
    {
        fprintf(stderr,"can't open file! \n");
        exit(1);
    }
    
    fprintf(frl,"--- LVQ --- \n");
    fprintf(frl, "Number of Clusters : %d \n",M);
    
    // first 
    fprintf(frl, "\n Start training \n");
    load_points_from_file();
    first_centers();
    training();
    
    
    for(i=0;i<M;i++)
        total[i] = 0;
    
    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
        {
            d[j] = sqrt(pow((points[i][0] - center[j][0]),2.0) + pow((points[i][1] - center[j][1]),2.0));
        }
        
        minimum = d[0];
        position = 0;
        
        for(j=1;j<M;j++)
            if(d[j]<minimum)
            {
                minimum = d[j];
                position = j;
            }
       
        total[position]+=pow((points[i][0] - center[position][0]),2.0) + pow((points[i][1] - center[position][1]),2.0);
    }
 
    sum = 0.0;
    
    for(i=0;i<M;i++)
        sum+= total[i];
    
    
    do
    {
        previous = sum;
        nn = 0.95*nn;
        training();
        
        for(i=0;i<M;i++)
        total[i] = 0;

        for(i=0;i<N;i++)
        {
            for(j=0;j<M;j++)
            {
                d[j] = sqrt(pow((points[i][0] - center[j][0]),2.0) + pow((points[i][1] - center[j][1]),2.0));
            }

            minimum = d[0];
            position = 0;

            for(j=1;j<M;j++)
                if(d[j]<minimum)
                {
                    minimum = d[j];
                    position = j;
                }

            total[position]+=pow((points[i][0] - center[position][0]),2.0) + pow((points[i][1] - center[position][1]),2.0);
        }

        sum = 0.0;
        
        for(i=0;i<M;i++)
            sum+= total[i];

        }
        while(abs(sum-previous)>0.001);
 
    fprintf(frl,"\n Last Centers \n");
    for(i=0;i<M;i++)
        fprintf(frl, "%f , %f \n",center[i][0],center[i][1]);
    fprintf(frl,"\n Total variance %10f \n",sum);
    printf("\n Total variance %10f \n",sum);
    
    fclose(fp);
    fclose(frl);
    
    return (EXIT_SUCCESS);
}


void load_points_from_file()
{
    int i,j,l;

    for(i=0;i<N;i++)
    {   
       
        fscanf(fp,"%10f",&points[i][0]);
        fgetc(fp);
        fscanf(fp,"%10f",&points[i][1]);
        fgetc(fp);
    }
}


void first_centers()
{
    int random,i, previous_rand;
    
 
    
    fprintf(frl, "\n First centers \n");
    
  
    random = rand()%N;

    center[0][0] = points[random][0];
    center[0][1] = points[random][1];
    
    fprintf(frl, "%f , %f \n",center[0][0],center[0][1]);
    
    previous_rand = random;
    
    for(i=1;i<M;i++)
    {
      
      do
      {
        random = rand()%N + 1;
      }
      while(random == previous_rand);    
    
      center[i][0] = points[random][0];
      center[i][1] = points[random][1];
      
      fprintf(frl, "%f , %f \n",center[i][0],center[i][1]);
      
      previous_rand = random; 
    }
  
    
}


void training()
{
    int i,j,position;
    double d[M],minimum;
    
    
    
    for(i=0;i<N;i++)
    {
       
        for(j=0;j<M;j++)
        {
            d[j] = sqrt(pow((points[i][0] - center[j][0]),2.0) + pow((points[i][1] - center[j][1]),2.0));
        }
        
        
        minimum = d[0];
        position = 0;
        
        for(j=1;j<M;j++)
            if(d[j]<minimum)
            {
                minimum = d[j];
                position = j;
            }
        
       
        center[position][0] += nn*(points[i][0] - center[position][0]);
        center[position][1] += nn*(points[i][1] - center[position][1]);
        
    }
}

