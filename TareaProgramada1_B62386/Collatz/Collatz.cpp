#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <math.h>
using namespace std;

#define BLACK   "\033[0m"
#define RED     "\033[1;31m"
#define BLUE    "\033[1;34m"

double inicio;
double final;
double tiempo;
time_t timer;
int pasos=0;
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
  double entrada=0;
  cout<< "Ingrese el numero:"<<endl;
  cin>>entrada;

  inicio=clock();
  while(entrada!=1){
    cout<<"El numero es:  "<<BLUE<<entrada<<BLACK "   Paso realizados: "<<pasos<<endl;
    entrada=col(entrada);
  }
  cout<<"El numero es:  "<<RED<<entrada<<BLACK "   Paso realizados: "<<pasos<<endl;
  final=clock();
  tiempo=(final-inicio)/CLOCKS_PER_SEC;
  cout<<"El programa tardó: "<<tiempo<<" segundos"<<endl;
  return 0;
}
