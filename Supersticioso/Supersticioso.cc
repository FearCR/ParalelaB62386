

// Compile with g++ h20.cc -lpthread
//
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <thread>
#include <chrono>
using namespace std;
using namespace std::this_thread;
using namespace std::chrono;
#define NUMTHRDS 100

#define BLACK   "\033[0m"
#define RED     "\033[1;31m"
#define BLUE    "\033[1;34m"

int status;
int n = 256;
pthread_t thds[NUMTHRDS];
pthread_mutex_t mutex;

sem_t sT;

int adentro = 0;
int esperando =0;

int main ( int argc, char *argv[] );
void *T ( void *arg );


int main ( int argc, char *argv[] ) {

  long i;

  sem_init( &sT, 0, 0 );


  pthread_attr_t attr;
  pthread_mutex_init ( &mutex, NULL );
  pthread_attr_init ( &attr );
  pthread_attr_setdetachstate ( &attr, PTHREAD_CREATE_JOINABLE );

  for ( i = 0; i < NUMTHRDS; i++ ) {
       pthread_create ( &thds[i], &attr, T, ( void * ) i );
  }

  pthread_attr_destroy ( &attr );

  for ( i = 0; i < NUMTHRDS; i++ ) {
    pthread_join ( thds[i], ( void ** ) &status );
  }

  pthread_mutex_destroy ( &mutex );
//  pthread_exit ( NULL );
  cout << "\n";
  cout << "  Normal end of execution.\n";

  return 0;
}

void *T ( void *arg ) {
   long i;
   bool enEspera=true;
   int tiempo=rand()%3;
   i = (long) arg;
   while(enEspera){
     if(adentro<12){
       enEspera=false;
       pthread_mutex_lock ( &mutex );
       adentro++;
       cout<<"soy el hilo "<<i<<BLUE" entre"<<BLACK" al cuarto, ahora habemos " <<adentro<<" adentro."<<endl;
       pthread_mutex_unlock(&mutex);
       sleep_for(seconds(tiempo));
       pthread_mutex_lock ( &mutex );
       adentro--;
       cout<<"soy el hilo "<<i<<RED" sali"<<BLACK" del cuarto, ahora habemos " <<adentro<<" adentro."<<endl;
        pthread_mutex_unlock(&mutex);
        sem_post(&sT);
     }else{
       pthread_mutex_lock ( &mutex );
        cout<<RED"soy el hilo " << i<<" hay "<<adentro<<" hilos adentro si entro quedan 13 en el cuarto, espero"<<BLACK<<endl;
       pthread_mutex_unlock(&mutex);
       sem_wait(&sT);
     }
   }

   pthread_exit ( ( void * ) 0 );
}
