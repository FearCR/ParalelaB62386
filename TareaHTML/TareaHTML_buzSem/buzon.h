/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   buzon.h
 * Author: osboxes
 *
 * Created on September 12, 2018, 2:47 PM
 */

#ifndef BUZON_H
#define BUZON_H

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

   class buzon {
      public:
      buzon();
      ~buzon();
      int Enviar(char * mensaje );
      int Recibir( char* mensaje);   // len es el tamaño máximo que soporte la variable mensaje
     private:
      int id;		// Identificador del buzon
      int st;
      struct msgbuf A;
   };

#endif /* BUZON_H */

buzon::buzon() {
    id=msgget(KEY,IPC_CREAT| 0600);
    if(-1==id){
        perror("Buzon::Buzon");
        exit(7);
    }
}


int buzon::Enviar(char * mensaje ){
    int len;
    len=strlen(mensaje);
    A.mtype=4;
    strncpy(A.mtext,mensaje, len);
    st=msgsnd(id,&A,len,IPC_NOWAIT);
    if(-1==st){
        perror("Buzon::Enviar");
        exit(9);
    }
    return st;
}

int buzon::Recibir( char* mensaje ){
  int len;
    st=msgrcv(id,&A,sizeof(A),4,0);
    if(-1==st){
        perror("Buzon::Recibir");
        exit(10);
    }
    len=strlen(A.mtext);
    strncpy(mensaje, A.mtext, len);
    return st;
}

buzon::~buzon() {
    st=msgctl(id,IPC_RMID,0);

    if(-1==st){
        perror("buzon::~buzon");
        exit(8);
    }
}
