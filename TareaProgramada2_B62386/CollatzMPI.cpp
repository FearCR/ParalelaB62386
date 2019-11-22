#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;



int main(int argc, char **argv) {
  int size, rank;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int globaldata[100];/*wants to declare array this way*/
  int localdata[100/size];/*without using pointers*/
  
  int i;
  if (rank == 0) {

    for (i=0; i<100; i++){
      globaldata[i] = rand()%1000+1;
    }


  }

  MPI_Scatter(globaldata, 100/size, MPI_INT, &localdata, 100/size, MPI_INT, 0, MPI_COMM_WORLD);


  for (i=0; i<100/size; i++){
    cout<<"proceso "<< rank<<" dato en la posicion "<<i<<" local: "<<localdata[i]<<endl;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if(rank==0){
    cout<<endl<<endl<<endl;
  }

  for (int i = 0; i <= 100/size; i++) {
    while(localdata[i]>1){
      if(fmod(localdata[i],2)==0){
        localdata[i]=localdata[i]/2;
      }else{
        localdata[i]=(3*localdata[i])+1;
      }
    }
  }

  MPI_Gather(&localdata, 100/size, MPI_INT, globaldata, 100/size, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {

    for (i=0; i<100; i++){
      cout<<"dato en la posicion "<<i<<" global: "<<globaldata[i]<<endl;
    }
  }


  MPI_Finalize();
  return 0;
}
