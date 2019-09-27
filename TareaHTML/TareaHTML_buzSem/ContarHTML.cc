//compila con g++ ContarHTML.cc
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include "Semaforo.h"
#include "buzon.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;
#define WORKERS 5;
ifstream file;
map<char, int> etiquetas;
buzon buz;
Semaforo sem;

void cuentaEnLinea(){
char * cWord;
  if(file.good())
  {
      string s;
      string word;
      getline(file, s);
      int pos = s.find_first_of('>');
      int ini = s.find_first_of('>');
      while(s.size() > 0)
      {
          word="";
          ini = s.find_first_of('<');
          pos = s.find_first_of('>');
          if(pos < 0 )
              pos = s.size();
          if(ini>=0){
            word = s.substr(ini, (pos+1)-ini);//substr(posicionInicial, cantidad de caracteres a la derecha)
          }else{
            ini=0;
          }
          if(word != "")
          cWord=(char *)word.c_str();
          cout<<cWord<<endl;
          buz.Enviar(cWord);
          s = s.erase(0, (pos+1));//erase(posicionInicial, cantidad de caracteres a la derecha)
      }
  }
}

int main(int argc, char **argv)
{
  int pid;
  file.open("archivo.html");
  if(!file.is_open()) return 1;


  for ( int proceso = 0; proceso < 5; proceso++ ) {

    pid = fork();
    if ( ! pid ) {
      while(file.good()){
        cuentaEnLinea();
      }
    }
  }
  char w;
  for ( int proceso = 0; proceso < 5; proceso++ ) {
  buz.Recibir(&w);
  cout<<w<<endl;
  etiquetas[w]++;
}

    for(map<char, int>::iterator wit = etiquetas.begin(); wit != etiquetas.end(); ++wit)
        cout << "etiqueta: " << wit->first << " | Encontro: " << wit->second << endl;
    return 0;
}
