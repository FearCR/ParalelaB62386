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

#define NUMTHRDS 100
#define PISOS 16


#define BLACK   "\033[0m"
#define RED     "\033[1;31m"
#define BLUE    "\033[1;34m"

int status;
int n = 256;
pthread_t thds[NUMTHRDS];
int subidas[PISOS];
int bajadas[PISOS];
pthread_mutex_t mutex;

sem_t sAscensor, sPersona;

int cPersonaFuera = 0, cPersonaDentro = 0;

int main ( int argc, char *argv[] );
void *ascensor ( void *arg );
void *persona ( void *arg );


int main ( int argc, char *argv[] ) {
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
   cout<<"Soy el thread "<<i<<endl;
   pthread_mutex_unlock ( &mutex );

   pthread_exit ( ( void * ) 0 );
}


void *persona ( void *arg ) {
   long i;
   i = (long) arg;
   pthread_mutex_lock ( &mutex );
   cout<<"Soy el thread "<<i<<endl;
   pthread_mutex_unlock ( &mutex );

   pthread_exit ( ( void * ) 0 );	// Finish this thread
}
