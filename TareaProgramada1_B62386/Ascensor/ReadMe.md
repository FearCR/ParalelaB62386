# Problema de sincronizacion, Asensor en un edificio de 16 pisos  
Update noviembre 8: El programa funciona bien.  
   
Para usar el programa solo se compila con:  
```bash
g++ Ascensor -lpthread
```
  
Y se corre el archivo a.out.   
  
Dentro del programa se especifica con distintos colores las acciones que se realizan para mayor facilidad de revisión.    
  
    
El programa consiste en 2 metodos, uno para el Ascensor que es manejado por un solo hilo y uno para las personas que van a subir al ascensor.  
- Las personas son un thread cada una, estas solo "presionan el boton" del piso al que van a subir, entran al ascensor si no esta lleno, "presionan el boton" del piso en el que van a bajar, esperan y cuando llegan a su destino salen y su ejecucion se termina.  
- El Ascensor tiene el trabajo mas pesado, es el encargado de subir y bajar pisos, avisar cuando una persona sube y cuando baja, y transportar a maximo 8 personas a la vez a sus pisos de destino, tomando en cuenta la regla de que si está subiendo solo puede seguir subiendo y viceversa.  
- El metodo principal crea los hilos y los envia a sus metodos correspondientes.  
  
Nota: el programa cuenta con 17 pisos realmente, el piso 0 se puede contar como el "sotano" que es donde inicia el ascensor. no vuelve a este piso. y las personas nunca piden ir a el. se usa mas que todo para hacer el conteo de 1 a 16 en vez de 0 a 15, no afecta en la ejecución del programa.
  
    
## Fecha de entrega:10 de noviembre al finalizar el día.
