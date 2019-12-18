#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


#define M 7 // M = 2,3,5, or 7
#define N 500 

void load_points_from_file();
void first_centers();
void training();
void new_center();



FILE *fp = NULL;
FILE *frk = NULL;

float points[N][2]; 
double center[M][2]; 
int point_cluster[N]; 
int previous_point_cluster[N]; 

int cntr[M]; 

double dsum[M][2]; 

int main(int argc, char** argv) {

    int i,j,f;
    double sum;

    fp = fopen("clusteringPatterns.txt","r");
    if(fp==NULL)
    {
        fprintf(stderr,"can't open file! \n");
        exit(1);
    }

    frk = fopen("resultsKmeans.txt","w");
    if(frk==NULL)
    {
        fprintf(stderr,"can't open file! \n");
        exit(1);
    }

    fprintf(frk,"--- K  means --- \n");
    fprintf(frk, "Number of Clusters : %d \n",M);

    
    for(i=0;i<N;i++)
    {
        point_cluster[i] = -1;
        previous_point_cluster[i] = -1;
    }

    // first 
    fprintf(frk, "\n Start training \n");
    load_points_from_file();
    first_centers();
    for(i=0;i<M;i++)
    {
        
        cntr[i] = 0;

        for(j=0;j<2;j++)
            dsum[i][j] = 0;
    }

    training();

    f = 0;
	
    // rest 
	
    while(f==0)
    {
        
        for(i=0;i<N;i++)
            previous_point_cluster[i] = point_cluster[i];


        training();

		f = 1; 
        for(i=0;i<N;i++)
        {
            if(point_cluster[i]!=previous_point_cluster[i])
            {
                f = 0;
                break;
            }
        }
    }

    sum = 0.0;
    
    for(i=0;i<M;i++)
    {
        for(j=0;j<N;j++)
        {
            if(point_cluster[j] == i)
                sum += pow((points[j][0]-center[i][0]),2.0) + pow((points[j][1]-center[i][1]),2.0);
        }
    }

    fprintf(frk,"\n Last Centers \n");
    for(i=0;i<M;i++)
        fprintf(frk, "%10f , %10f \n",center[i][0],center[i][1]);

    fprintf(frk,"\n Total variance %10f \n",sum);
    printf("\n Total variance %10f \n",sum);

    fclose(fp);
    fclose(frk);

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

 

    fprintf(frk, "\n First centers \n");

    
    random = rand()%N;

    center[0][0] = points[random][0];
    center[0][1] = points[random][1];

    fprintf(frk, "%f , %f \n",center[0][0],center[0][1]);

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

      fprintf(frk, "%f , %f \n",center[i][0],center[i][1]);

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

        
        point_cluster[i] = position;
        cntr[position]++; 
        dsum[position][0] += points[i][0];
        dsum[position][1] += points[i][1];

       

    }

    new_center();
}


void new_center()
{
    int i;

    for(i=0;i<M;i++)
    {
        
        center[i][0] = dsum[i][0]/cntr[i];
        center[i][1] = dsum[i][1]/cntr[i];
    }
}
