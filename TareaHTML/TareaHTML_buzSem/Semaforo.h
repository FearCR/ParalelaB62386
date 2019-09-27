//includes
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdio.h>
#include <cstdlib>
#include <cstdio>

#ifndef SEMAFORO_H
#define  SEMAFORO_H
#define KEY 0xB62386	// Valor de la llave del recurso

//header
class Semaforo {
   public:
      Semaforo( int ValorInicial = 0 );
      ~Semaforo();
      int Signal();	// Puede llamarse V
      int Wait();	// Puede llamarse P
      private:
      int id;		// Identificador del semaforo
      int st;
};
#endif /* SEMAFORO_H */


//definicioon del union
union semun {
               int val;                    /* valor para SETVAL */
               struct semid_ds *buf;       /* buffer para IPC_STAT, IPC_SET */
               unsigned short int *array;  /* array para GETALL, SETALL */
               struct seminfo *__buf;      /* buffer para IPC_INFO */
       };


//constructor
Semaforo::Semaforo(int initial)
{
  int status;
  status=semget(KEY,1,IPC_CREAT | 0600);
  if(-1==status){
    perror("Semaforo::Semaforo");
    exit(2);
  }
  union semun x;
  id=status;
  x.val=initial;
  st=semctl(id,0,SETVAL,x);
  if(-1==id){
      perror("Semaforo::Semaforo");
      exit(3);
  }
}

//destructor
Semaforo::~Semaforo()
{
  st=semctl(id,0,IPC_RMID);
  if(-1==id){
        perror("Semaforo::~Semaforo");
        exit(4);
  }
}

//metodo wait o P
int Semaforo::Wait()
{
  struct sembuf x;
  int status;
  x.sem_num=0;
  x.sem_op=-1;
  x.sem_flg=0;
  status=semop(id,&x,1);
  if(-1==id){
    perror("Semaforo::Wait");
    exit(5);
  }
  return status;
}

//metodo signal o V
int Semaforo::Signal()
{
  struct sembuf x;
  int status;
  x.sem_num=0;
  x.sem_op=+1;
  x.sem_flg=0;
  status=semop(id,&x,1);
  if(-1==id){
    perror("Semaforo::Signal");
    exit(6);
  }
  return status;
}
