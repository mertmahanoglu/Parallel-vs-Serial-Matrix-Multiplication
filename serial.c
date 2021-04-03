#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc,char **argv)
{
    int boyut=1000;
    int i,j,k;
    int rank,size;
    double t1,t2,sonuc;
    // matrisleri tanimlama
    int ** Amatrisi=malloc(sizeof(int*)*boyut);
    for(i=0;i<boyut;i++)
      {
       Amatrisi[i]=malloc(sizeof(int*)*boyut);
      }

    int ** Bmatrisi=malloc(sizeof(int*)*boyut);
    for(i=0;i<boyut;i++)
      {
        Bmatrisi[i]=malloc(sizeof(int*)*boyut);
      }

    int** SonucMatrisi=malloc(sizeof(int*)*boyut);
    for(i=0;i<boyut;i++)
       {
          SonucMatrisi[i]=malloc(sizeof(int*)*boyut);
       }
     MPI_Status status;
     MPI_Init(&argc,&argv);
     MPI_Comm_size(MPI_COMM_WORLD,&size);
     MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if(rank==0)
        printf("\nÝþlemci Sayýsý=&d\n",size);
       // matrislere random eleman atama
    for ( i = 0; i < boyut; i++)
     {
	  for ( j = 0; j < boyut; j++)
	   {
	    Amatrisi[i][j] = rand() % 10;
	    Bmatrisi[i][j] = rand() % 10;
       }
     }
     if(rank==0)
        t1=MPI_Wtime();

    for ( i = rank; i < boyut; i+size)
	{
	  for (  j = 0; j < boyut; j++)
	    {
	      for ( k = 0; k < boyut; k++)
          {
		   SonucMatrisi[i][j] += Amatrisi[i][k] * Bmatrisi[k][j];
          }
        }
	}
      if(rank!=0)
       {
           for(i=rank;i<boyut;i+size)
            MPI_Send(&mat_res[i][0],boyut,MPI_INT,0,10+i,MPI_COMM_WORLD);
       }
     if(rank==0)
       {
           for(j=1;j<size;j++)
           {
                for(i=j;i<boyut;i=i+size)
                {
                    MPI_Recv(&mat_res[i][0],boyut,MPI_INT,0,10+i,MPI_COMM_WORLD,&status);
                }
           }
       }
      MPI_Barrier(MPI_COMM_WORLD);
     if (rank==0)
      t2=MPI_Wtime();
      sonuc=t2-t1;
      if(rank==0)
        printf("\n(&d x &d) boyutlu matrisin islem süresi : %f saniye\n",boyut,boyut,sonuc);
     MPI_Finalize();
     return 0;
}