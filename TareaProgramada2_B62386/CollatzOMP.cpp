//Compila con:
//g++  CollatzOMP.cpp -fopenmp

#include <omp.h>
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
#define THREADS 10
using namespace std;

int main(int argc, char **argv) {
  int size, rank;
  int globaldata[TAMANO];/*wants to declare array this way*/
  int i;

  int my_rank =0;


  cout<<GREEN<<"CANTIDAD DE DATOS: "<<TAMANO<<BLACK<<endl<<endl;
  cout<<YELLOW<<"CANTIDAD DE DATOS POR PROCESO: "<<TAMANO/THREADS<<BLACK<<endl<<endl;
  //se paraleliza el llenado del array
  # pragma omp parallel for num_threads(THREADS) schedule(static,1)
  for (i=0; i<TAMANO; i++){
    globaldata[i] = rand()%1000+1;
    # pragma omp critical
    cout<<"Dato en la posicion "<<i<<BLUE<<" local: "<<BLACK<<globaldata[i]<<endl;
  }
  //se paraleliza el proceso de la conjetura de collatz
  # pragma omp parallel for num_threads(THREADS) schedule(static,1)
  for (int i = 0; i <= TAMANO; i++) {
    while(globaldata[i]>1){
      if(fmod(globaldata[i],2)==0){
        globaldata[i]=globaldata[i]/2;
      }else{
        globaldata[i]=(3*globaldata[i])+1;
      }
    }
  }
  //se paraleliza el print final, se usa my rank para saber el numero del thread
  # pragma omp parallel for num_threads(THREADS) schedule(static,1)
  for (i=0; i<TAMANO; i++){
    my_rank=omp_get_thread_num ( );
    // se usa un critical para hacer los prints mas ordenados
    # pragma omp critical
    cout<<"Soy el thread "<<my_rank<<", el dato en la posicion "<<i<<RED<<" global "<<BLACK<<"es: "<<globaldata[i]<<endl;
  }


  return 0;
}
