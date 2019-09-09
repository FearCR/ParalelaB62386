

#ifndef Buzon_H
#define Buzon_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdio.h>
#include <cstdlib>
#include <cstdio>
#include <sys/msg.h>
#include <cstring>
#include <string>

#define KEY 0xB62386	// Valor de la llave del recurso

   class Buzon {
      public:
        struct msgbuf{
           long mtype;
           double mtext;
        };
      Buzon();
      ~Buzon();
      int Enviar(double*  mensaje );
      int Recibir( double* mensaje);   // len es el tamaño máximo que soporte la variable mensaje
     private:
      int id;		// Identificador del Buzon
      int st;
      struct msgbuf A;
   };

#endif /* Buzon_H */

Buzon::Buzon() {
    id=msgget(KEY,IPC_CREAT| 0600);
    if(-1==id){
        perror("Buzon::Buzon");
        exit(7);
    }
}


int Buzon::Enviar(double *mensaje ){
    int len;
    len=sizeof(mensaje);
    A.mtype=4;
    A.mtext=*mensaje;
    st=msgsnd(id,&A,len,IPC_NOWAIT);
    if(-1==st){
        perror("Buzon::Enviar");
        exit(9);
    }
    return st;
}

int Buzon::Recibir( double *mensaje){
    st=msgrcv(id,&A,sizeof(A),4,0);
    if(-1==st){
        perror("Buzon::Recibir");
        exit(10);
    }
    *mensaje=A.mtext;
    return st;
}

Buzon::~Buzon() {
    st=msgctl(id,IPC_RMID,0);

    if(-1==st){
        perror("Buzon::~Buzon");
        exit(8);
    }
}
