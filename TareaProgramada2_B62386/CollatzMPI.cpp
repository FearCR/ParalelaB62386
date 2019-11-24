//Compila con:
//mpic++ -n 4 CollatzMPI.cpp
//Corre con:
//mpiexec -n 4 ./a.out

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#define BLACK   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define TAMANO 100

using namespace std;



int main(int argc, char **argv) {
  int size, rank;
//se inicia mpi
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int globaldata[TAMANO];/*wants to declare array this way*/
  int localdata[TAMANO/size];/*without using pointers*/
  int i;
  //proceso 0 inicializa el vector global con numeros aleatorios
  if (rank == 0) {
    cout<<GREEN<<"CANTIDAD DE DATOS: "<<TAMANO<<BLACK<<endl<<endl;
    cout<<YELLOW<<"CANTIDAD DE DATOS POR PROCESO: "<<TAMANO/size<<BLACK<<endl<<endl;
    for (i=0; i<TAMANO; i++){
      globaldata[i] = rand()%1000+1;
    }


  }
//se reparte el vector global en vectores locales
  MPI_Scatter(globaldata, TAMANO/size, MPI_INT, &localdata, TAMANO/size, MPI_INT, 0, MPI_COMM_WORLD);


  for (i=0; i<TAMANO/size; i++){
    cout<<"Proceso "<< rank<<", dato en la posicion "<<i<<BLUE<<" local: "<<BLACK<<localdata[i]<<endl;
  }
  //se espera a que todos los procesos terminen
  MPI_Barrier(MPI_COMM_WORLD);
  if(rank==0){
    cout<<endl<<endl<<endl;
  }

  for (int i = 0; i <= TAMANO/size; i++) {
    while(localdata[i]>1){
      if(fmod(localdata[i],2)==0){
        localdata[i]=localdata[i]/2;
      }else{
        localdata[i]=(3*localdata[i])+1;
      }
    }
  }
//se envian los vectores locales al vector global en el proceso 0
  MPI_Gather(&localdata, TAMANO/size, MPI_INT, globaldata, TAMANO/size, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {

    for (i=0; i<TAMANO; i++){
      cout<<"Dato en la posicion "<<i<<RED<<" global: "<<BLACK<<globaldata[i]<<endl;
    }
  }


  MPI_Finalize();
  return 0;
}
