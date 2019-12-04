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
  
Con un millon de elementos   
CollatzACC tiempo serial:**4.06288 segundos**  
CollatzACC tiempo Paralelo:**1.12959 segundos**  
Aceleracion: **3.59677**   
   
   
### CollatzACC_pgi.cpp  
Es un programa que implementa la conjetura de Collatz usando el API de OpenACC y el compilador PGI. en lo que cambia con la version sin pgi es que en esta no se usan funciones de la libreria Math. Ya que provoca problemas.  
Compila con:  
```bash
pgc++ -fast -acc CollatzACC_pgi.cpp 
```  
Con un millon de elementos   
CollatzACC tiempo serial:**0.770357 segundos**  
CollatzACC tiempo Paralelo:**0.338243 segundos**  
Aceleracion: **2.27752**   
  
Como se puede observar, aun y si la aceleracion es parecida, los tiempos son mucho mas rapidos usando PGI, ya que se copian los datos automaticamente a la GPU y se trabajan ahi.
