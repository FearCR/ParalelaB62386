collatz funciona, falta comunicar el array con los demas procesos y recibir los datos al finalizar(bcast/gather, scatter/send, recieve)
Recordatoio:



    int *matrix;

    if (id == 0) {
        matrix = createMatrix(p, p); // Master process creates matrix
        printf("Initial matrix:\n");
        printArray(matrix, p*p);
    }

    int *procRow = malloc(sizeof(int) * (TAMANO/comm_size)); // received row will contain p integers
    if (procRow == NULL) {
        perror("Error in malloc 3");
        exit(1);
    }

    MPI_Scatter(matrix, p, MPI_INT, // send one row, which contains p integers
                procRow, p, MPI_INT, // receive one row, which contains p integers
                0, MPI_COMM_WORLD)
