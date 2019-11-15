
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include <mpi.h>     /* For MPI functions, etc */
using namespace std;

#define BLACK   "\033[0m"
#define RED     "\033[1;31m"
#define BLUE    "\033[1;34m"
#define TAMANO 107
MPI_Comm comm;
double inicio;
double final;
double tiempo;
time_t timer;
int pasos=0;
int arreglo[TAMANO];



double col(double x){
  double numero=x;
  if(fmod(numero,2)==0){
    numero=numero/2;
    pasos++;
  }else{
    numero=(3*numero)+1;
    pasos++;
  }
  return numero;
}




int main(int argc, char **argv)
{
  int comm_sz;               /* Number of processes    */
  int my_rank;               /* My process rank        */
  int myStart;
  int myEnd;
  /* Start up MPI */
  MPI_Init(NULL, NULL);
  comm = MPI_COMM_WORLD;
  /* Get the number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  /* Get my rank among all the processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


  if(my_rank==0){
    for (int i = 0; i <TAMANO; i++) {
      arreglo[i]=rand()%1000+100;
    }
  }

  myStart=my_rank*(TAMANO/comm_sz);
  if(my_rank!=comm_sz-1){
    myEnd=((my_rank+1)*(TAMANO/comm_sz))-1;
  }else{
    myEnd=TAMANO-1;
  }


cout<<myStart<<" "<<myEnd<<endl;


  for (int i = myStart; i <= myEnd; i++) {
    while(arreglo[i]>1){
      if(fmod(arreglo[i],2)==0){
        arreglo[i]=arreglo[i]/2;
      }else{
        arreglo[i]=(3*arreglo[i])+1;
      }
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);


  if(my_rank==1){
    for (int i = 0; i < TAMANO; i++) {
      cout<<arreglo[i]<<endl;
    }
  }





  /*
  inicio=clock();

  final=clock();
  tiempo=(final-inicio)/CLOCKS_PER_SEC;
  cout<<"El programa tardó: "<<tiempo<<" segundos"<<endl;*/
  return 0;
}
