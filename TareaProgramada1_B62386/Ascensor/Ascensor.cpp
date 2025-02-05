// Compile with g++ Ascensor.cpp -lpthread
//pthread_mutex_lock ( &mutex );
//pthread_mutex_unlock ( &mutex );
//sem_post( &sC );
//sem_wait( &sC );
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

#define NUMTHRDS 100
#define PISOS 16

#define BLACK   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"

int status;
int n = 256;
bool moving=true;
pthread_t thds[NUMTHRDS];
int subidas[PISOS+1];
int bajadas[PISOS+1];
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
  // /usleep(1000);//solo para asegurarse de que al ascensor entre la cantidad maxima de personas que puede entrar al inicio, si no puede que trabaje muy rapido y no se aprecie.
  //con este metodo se puede apreciar bien al inicio del programa. Al final vuelve a alcanzar la velocidad de los threads atendiendolos justo cuando llegan.
  //tambien se puede poner un sleep entre la subida y bajada de cada piso para que el ascensor no supere la velocidad de los otros threads  y se sobrecargue
  long i;
  i = (long) arg;
  pthread_mutex_lock ( &mutex );
  cout<<"Soy el thread Ascensor "<<i<<endl;
  pthread_mutex_unlock ( &mutex );

  while(true){

    if(bajadas[pisoActual]>0){//bajan del ascensor
      for (int j = 0; j < bajadas[pisoActual]; j) {
        if(cPersonaDentro>0){
          sem_post(&sPersona);
          pthread_mutex_lock ( &mutex );
          cPersonaDentro--;
          bajadas[pisoActual]--;
          cout<<"Bajo uno en el piso "<<pisoActual<<" adentro del ascensor hay "<<cPersonaDentro<<" personas."<<endl;
          pthread_mutex_unlock ( &mutex );
        }else{
          break; //si se vacía el ascensor
        }
      }
    }
    if(subidas[pisoActual]>0){//suben al ascensor
      for (int j = 0; j < subidas[pisoActual]; j) {
        if(cPersonaDentro<8){//solo suben si hay espacio
          sem_post( &sPersona );
          pthread_mutex_lock ( &mutex );
          cPersonaDentro++;
          cPersonaFuera--;
          cout<<"subio uno en el piso "<<pisoActual<<" adentro del ascensor hay "<<cPersonaDentro<<" personas."<<endl;
          subidas[pisoActual]--;
          pthread_mutex_unlock ( &mutex );

        }else{
          pthread_mutex_lock ( &mutex );
          cout<<RED"El ascensor esta lleno no puede entrar nadie mas hasta que al menos uno baje."<<BLACK<<endl;
          pthread_mutex_unlock ( &mutex );
          break;//si no hay espacio sale del ciclo
        }
      }
    }

    if(cPersonaFuera==0 && cPersonaDentro==0){
      pthread_mutex_lock ( &mutex );
      cout<<RED"Nadie necesita subir o bajar del ascensor, entonces espero. "<<BLACK<<endl;
      pthread_mutex_unlock ( &mutex );
      moving=false;
      sem_wait( &sAscensor );

      pthread_mutex_lock ( &mutex );
      moving=true;
      cout<<GREEN"señal recibida, alguien necesita subir al ascensor"<<BLACK<<endl;
      pthread_mutex_unlock ( &mutex );

    }
    pisoMaximo=1;
    pisoMinimo=1;
    for (int j = 0; j <= PISOS; j++) {//se fija cual es el piso maximo y minimo al que tiene que ir siempre

      if(subidas[j]>0&&j>pisoMaximo){
        pisoMaximo=j;
      }
      if(bajadas[j]>0&&j>pisoMaximo){
        pisoMaximo=j;
      }
      if(subidas[j]>0&&j<pisoMinimo){
        pisoMinimo=j;
      }
      if(bajadas[j]>0&&j<pisoMinimo){
        pisoMinimo=j;
      }
    }

    if(subiendo){
      if(pisoActual<pisoMaximo||pisoActual<pisoMinimo){
        subiendo=true;
        pisoActual++;
        pthread_mutex_lock ( &mutex );
        cout<<CYAN"Piso: "<<pisoActual<<BLACK<<endl;
        pthread_mutex_unlock ( &mutex);
        }else{
          subiendo=false;
        }
      }
      if(!subiendo){
        if(pisoActual>pisoMinimo||pisoActual>pisoMaximo){
          subiendo=false;
          pisoActual--;
          pthread_mutex_lock ( &mutex);
          cout<<CYAN"Piso: "<<pisoActual<<BLACK<<endl;
          pthread_mutex_unlock ( &mutex );
          }else{
            subiendo=true;
          }
        }
        //cout<<cPersonaDentro;
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
      cout<<BLUE"Soy la persona "<<i<<", presione el boton para subir en el piso :"<<meSuboEn<<YELLOW", Y me bajo en el piso: "<<meBajoEn<<BLACK<<endl;
      subidas[meSuboEn]++;
      if(moving==false){
        sem_post(&sAscensor);
      }
      pthread_mutex_unlock ( &mutex );
      sem_wait(&sPersona);
      pthread_mutex_lock ( &mutex );
      bajadas[meBajoEn]++;
      pthread_mutex_unlock ( &mutex );

      pthread_exit ( ( void * ) 0 );	// Finish this thread
    }
