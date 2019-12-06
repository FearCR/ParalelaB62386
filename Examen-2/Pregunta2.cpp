
//en el examen se compiló con:
// g++ -fopenacc -Ofast -c Pregunta2.cpp
// g++ -fopenacc Pregunta2.o -o salida
//tambien funciona con pgi


#include <cstdlib>
#include <iostream>
#include <omp.h>
using namespace std;


int main( int argc, char ** argv ) {
  double t_start;
  double t_end;
  long terminos, inicio, fin;
  double casiPi = 0;
  double alterna = 4;
  long divisor = 0;

  terminos = 1000000;
  if ( argc > 1 ) {
    terminos = atol( argv[ 1 ] );
  }



  t_start=omp_get_wtime();
  for ( int i = 0; i < terminos; i++ ) {
    divisor = i + i + 1;		// 2 x termino + 1
    casiPi += alterna/divisor;		// 4 / (2xi + 1)
    alterna *= -1;				// Pasa de 4 a -4 y viceversa, para realizar la aproximacion de los terminos
  }
  t_end=omp_get_wtime()-t_start;
  cout<<"Aproximacion de pi: "<<casiPi<<" con "<<terminos<<" terminos serial"<<endl;
  cout<<"Tiempo: "<<t_end<<" segundos "<<endl;

  casiPi=0;
  cout<<endl;


  t_start=omp_get_wtime();
  #pragma acc parallel loop reduction(+:casiPi)
  for ( int i = 0; i < terminos; i++ ) {
    divisor = i + i + 1;		// 2 x termino + 1
    casiPi += alterna/divisor;		// 4 / (2xi + 1)
    alterna *= -1;				// Pasa de 4 a -4 y viceversa, para realizar la aproximacion de los terminos
  }
  t_end=omp_get_wtime()-t_start;
  cout<<"Aproximacion de pi: "<<casiPi<<" con "<<terminos<<" terminos usando Open ACC"<<endl;
  cout<<"Tiempo: "<<t_end<<" segundos "<<endl;

}
