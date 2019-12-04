# Tarea Programada 2

### CollatzOMP.cpp
Es un programa que implementa la conjetura de Collatz usando el API de OpenMP.  
Compila con:  
```bash
g++ CollatzOMP.cpp -fopenmp
```
  
### CollatzMPI.cpp  
Es un programa que implementa la conjetura de Collatz usando el API de MPI.  
Compila con:  
```bash
mpic++ CollatzMPI.cpp 
```  
Y corre con:  
```bash
mpiexec -n 4 ./a.out
```  
El numero se puede reemplazar con la cantidad de procesos que soporte la computadora(preferiblemente un numero divisible con la cantidad de numeros con la que se va a trabajar).
  
### CollatzACC.cpp  
Es un programa que implementa la conjetura de Collatz usando el API de OpenACC.  
Compila con:  
```bash
 g++ -fopenacc -Ofast -c CollatzACC.cpp
 g++ -fopenacc CollatzACC.o -o collatz
```  


