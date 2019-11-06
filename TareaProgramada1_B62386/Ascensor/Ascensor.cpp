// Compile with g++ Ascensor.cpp -lpthread
//pthread_mutex_lock ( &mutex );
//pthread_mutex_unlock ( &mutex );
//sem_post( &sC );
//sem_wait( &sC );
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

#define NUMTHRDS 6
#define PISOS 16


#define BLACK   "\033[0m"
#define RED     "\033[1;31m"
#define BLUE    "\033[1;34m"

int status;
int n = 256;
pthread_t thds[NUMTHRDS];
int subidas[PISOS];
int bajadas[PISOS];
int pisoMaximo=0;
int pisoMinimo=0;
int pisoActual=0;
bool subiendo=true;
pthread_mutex_t mutex;

sem_t sAscensor, sPersona;

int cPersonaFuera = 0, cPersonaDentro = 0;

int main ( int argc, char *argv[] );
void *ascensor ( void *arg );
void *persona ( void *arg );


int main ( int argc, char *argv[] ) {
  for (int j = 0; j < PISOS; j++) {
    subidas[j]=0;
    bajadas[j]=0;
  }
  long i;
  sem_init( &sPersona, 0, 0 );
  sem_init( &sAscensor, 0, 0 );
  pthread_attr_t attr;
  pthread_mutex_init ( &mutex, NULL );
  pthread_attr_init ( &attr );
  pthread_attr_setdetachstate ( &attr, PTHREAD_CREATE_JOINABLE );
  for ( i = 0; i < NUMTHRDS; i++ ) {
    if ( i == 0 )
    pthread_create ( &thds[i], &attr, ascensor, ( void * ) i );
    else
    pthread_create ( &thds[i], &attr, persona, ( void * ) i );
  }
  pthread_attr_destroy ( &attr );
  for ( i = 0; i < NUMTHRDS; i++ ) {
    pthread_join ( thds[i], ( void ** ) &status );
  }
  pthread_mutex_destroy ( &mutex );

  cout << "\n";
  cout << "----------Normal end of execution.----------\n";

  return 0;
}


void *ascensor ( void *arg ) {
  long i;
  i = (long) arg;
  pthread_mutex_lock ( &mutex );
  cout<<"Soy el thread Ascensor "<<i<<endl;
  pthread_mutex_unlock ( &mutex );

  while(true){

    if(bajadas[pisoActual]>1){//bajan del ascensor
      for (int j = 0; j < bajadas[pisoActual]; j) {
        if(cPersonaDentro>0){
          pthread_mutex_lock ( &mutex );
          cPersonaDentro--;
          cout<<"Bajo uno en el piso "<<pisoActual<<" adentro del ascensor hay "<<cPersonaDentro<<endl;
          bajadas[pisoActual]--;
          pthread_mutex_unlock ( &mutex );
          if(subidas[pisoActual]>1){
            sem_post(&sPersona);
          }

        }else{
          break; //si se vacía el ascensor
        }
      }
    }
    if(subidas[pisoActual]>1){//suben al ascensor
      for (int j = 0; j < subidas[pisoActual]; j) {
        if(cPersonaDentro<=8){//solo suben si hay espacio
          sem_post( &sPersona );
          pthread_mutex_lock ( &mutex );
          cPersonaDentro++;
          cout<<"subio uno en el piso "<<pisoActual<<" adentro del ascensor hay "<<cPersonaDentro<<endl;
          subidas[pisoActual]--;
          pthread_mutex_unlock ( &mutex );

        }else{
          break;//si no hay espacio sale del ciclo
        }
      }
    }

    if(cPersonaFuera==0 && cPersonaDentro==0){
      pthread_mutex_lock ( &mutex );
      cout<<"Nadie necesita subir o bajar del ascensor, entonces espero. "<<endl;
      pthread_mutex_unlock ( &mutex );
      sem_wait( &sAscensor );
      pthread_mutex_lock ( &mutex );
      cout<<"señal recibida, alguien necesita subir al ascensor"<<endl;
      pthread_mutex_unlock ( &mutex );

    }
    for (int j = 0; j < PISOS; j++) {//se fija cual es el piso maximo y minimo al que tiene que ir siempre
      if(subidas[j]>1&&j>pisoMaximo){
        pisoMaximo=j;
      }
      if(bajadas[j]>1&&j>pisoMaximo){
        pisoMaximo=j;
      }
      if(subidas[j]>1&&j<pisoMinimo){
        pisoMinimo=j;
      }
      if(bajadas[j]>1&&j>pisoMinimo){
        pisoMinimo=j;
      }
    }


    if(subiendo){
      if(pisoActual<pisoMaximo){
        subiendo=true;
        pisoActual++;
      }else{
        subiendo=false;
      }
    }else{
      if(pisoActual>pisoMinimo){
        subiendo=false;
        pisoActual--;
      }else{
        subiendo=true;
      }
    }

  }
  pthread_exit ( ( void * ) 0 );
}


void *persona ( void *arg ) {
  long i;
  i = (long) arg;
  int meSuboEn=rand()%16 +1;
  int meBajoEn=rand()%16 +1;
  pthread_mutex_lock ( &mutex );
  cPersonaFuera++;
  subidas[meSuboEn]++;
  pthread_mutex_unlock ( &mutex );
  sem_post(&sAscensor);
  if(cPersonaDentro>=8){
    pthread_mutex_lock ( &mutex );
    cout<<"El ascensor esta lleno, entonces espero "<<endl;
    pthread_mutex_unlock ( &mutex );
    sem_wait( &sPersona );
  }
  if(pisoActual!=meSuboEn){
    sem_wait( &sPersona );
  }
  pthread_mutex_lock ( &mutex );
  cout<<"Entré y presioné el boton para bajar en el piso "<<meBajoEn<<endl;
  bajadas[meBajoEn]++;
  pthread_mutex_unlock ( &mutex );
  sem_wait( &sPersona );
  pthread_exit ( ( void * ) 0 );	// Finish this thread
}
