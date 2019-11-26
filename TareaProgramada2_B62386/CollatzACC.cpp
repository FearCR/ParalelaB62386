//Compila con:
//g++  CollatzOMP.cpp -fopenacc

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
#define TAMANO 1000000
#define THREADS 10
using namespace std;

int main(int argc, char **argv) {
  int globaldata[TAMANO];/*wants to declare array this way*/
  int i;
  double inicio, finalizacion;
  int my_rank =0;


  cout<<GREEN<<"CANTIDAD DE DATOS: "<<TAMANO<<BLACK<<endl<<endl;
  //se paraleliza el llenado del array
  # pragma acc parallel loop
  for (i=0; i<TAMANO; i++){
    globaldata[i] = rand()%100000+1;
  }
  //no se paraleliza el cout
  for (i=0; i<TAMANO; i++){;
    cout<<"Dato en la posicion "<<i<<BLUE<<" local: "<<BLACK<<globaldata[i]<<endl;
  }
  inicio=omp_get_wtime();
  //se paraleliza el proceso de la conjetura de collatz
  # pragma acc parallel loop
  for (int i = 0; i <= TAMANO; i++) {
    while(globaldata[i]>1){
      if(fmod(globaldata[i],2)==0){
        globaldata[i]=globaldata[i]/2;
      }else{
        globaldata[i]=(3*globaldata[i])+1;
      }
    }
  }
  finalizacion=omp_get_wtime()-inicio;
  //no se paraleliza el cout
  for ( i=0; i<TAMANO; i++){

    cout<<" el dato en la posicion "<<i<<RED<<" global "<<BLACK<<"es: "<<globaldata[i]<<endl;
  }
  cout<<"Duracion de la conjetura de Collatz en "<<TAMANO<<" datos: "<<finalizacion<<" segundos"<<endl;

  return 0;
}
