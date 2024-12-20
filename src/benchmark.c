//Nick Belsten for MIT STAR Lab APRA project


#if __STDC_VERSION__ >= 199901L // to use POSIX standards with corresponding C version
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include <linux/time.h>
// #include <sys/time.h>

#include "nr.h"
#include "nrutil.h"
#include "my_math_utils.h"

//Here we requires one based indexing
void print_row_major_complex(double arr[], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("(%e+%ej)", arr[(i*cols + j)*2 + 1], arr[(i*cols + j)*2 + 2]);
            if (j < (cols - 1)) {
                printf(", ");
            }
        }
        printf("\n");
    }
}


/**
 * Prints a 2D array of doubles stored in a pointer-to-pointer based structure.
 *
 * @param arr Pointer to the first element of an array of pointers to doubles.
 * @param rows The number of rows in the array.
 * @param cols The number of columns in each row of the array.
 */
void print_double_array(double** arr, int rows, int cols) {
    if (arr == NULL) {
        printf("The array pointer is NULL.\n");
        return;
    }
    for (int i = 1; i <= rows; i++) {
        if (arr[i] == NULL) {
            printf("Row %d is NULL.\n", i);
            continue;
        }
        for (int j = 1; j <= cols; j++) {
            printf("%f ", arr[i][j]);
        }
        printf("\n");
    }
}


double time_print(const struct timespec* start, const struct timespec* end,long repeats) {
    char* units = " microSeconds";

    double elapsed = (end->tv_sec - start->tv_sec) * 1e6 +
                    (end->tv_nsec - start->tv_nsec) / 1e3;

    elapsed = elapsed / repeats;

    double elapsed_s = elapsed /1e6;

    if (elapsed > 1e3)
    {
        // Convert to milliseconds
        units = " milliSeconds";
        elapsed /= 1e3;
    }
    if (elapsed > 1e3)
    {
        // Convert to seconds
        units = " Seconds";
        elapsed /= 1e3;
    }

    printf("Elapsed: %f%s\n", elapsed, units);

    return elapsed_s;
}

void initializedMatrix(double **matrix, long nrl, long nrh, long ncl, long nch) {
    long i, j;

    // Seed the random number generator
    srand((unsigned int)time(NULL));

    int k=0;
    for (i = nrl; i <= nrh; i++) {
        for (j = ncl; j <= nch; j++) {
            // Generate a random float between 0 and 1
            //matrix[i][j] = (double)rand() / RAND_MAX;
            matrix[i][j] = (double)(k); k++;
        }
    }
}

void initializeiMatrix(int **matrix, long nrl, long nrh, long ncl, long nch) {
    long i, j;

    // Seed the random number generator
    srand((unsigned int)time(NULL));

    for (i = nrl; i <= nrh; i++) {
        for (j = ncl; j < nch; j++) {
            // Generate a random int
            matrix[i][j] = (int)rand();
        }
    }
}

void initializeMatrix(float **matrix, long nrl, long nrh, long ncl, long nch) {
    long i, j;

    // Seed the random number generator
    srand((unsigned int)time(NULL));

    for (i = nrl; i <= nrh; i++) {
        for (j = ncl; j < nch; j++) {
            // Generate a random float between 0 and 1
            matrix[i][j] = (float)rand() / RAND_MAX;
        }
    }
}

/**************************************************************
 * ************************************************************
 * ********************  MVMLULT CODE  ************************
 * ************************************************************
 * ************************************************************
*/

double compare_dmvmult(long cols){
    long rows = cols*20;
    /************************* Double ****************************/
    printf("Representation double\n");

    // Allocate memory for the matrix
    double **myMatrix = dmatrix(1, rows, 1, cols);
    double *myVector = dvector(1, cols);
    double *result = dvector(1,rows);


    int reps = 5;
    int i;
    double elapsed_seconds;
    struct timespec start, stop;

    // Initialize the matrix with random values
    initializedMatrix(myMatrix, 1, rows, 1, cols);

    initializedMatrix(&myVector,0,0,1,cols);
    
    //Sequential
    printf("Sequential implementation\n");
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (i = 0; i < reps; i++) {
        dmvmul(result,myMatrix,myVector,rows,cols);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_dmatrix(myMatrix,1,rows,1,cols);
    free_dvector(myVector,1,cols);
    free_dvector(result,1,rows);

    return elapsed_seconds;
}

double time_mvmult(long cols){
    long rows = cols*20;

    /************************* FLOAT ****************************/
    printf("Representation float\n");

    // Allocate memory for the matrix
    float **myMatrix = matrix(1, rows, 1, cols);
    float *myVector = vector(1, cols);
    float *result = vector(1, rows);

    // Initialize the matrix with random values
    initializeMatrix(myMatrix, 1, rows, 1, cols);

    initializeMatrix(&myVector,0,0,1,cols);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 5;
    int i;
    for (i = 0; i < reps; i++) {
        mvmul(result,myMatrix,myVector,rows,cols);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_matrix(myMatrix,1,rows,1,cols);
    free_vector(myVector,1,cols);
    free_vector(result,1,rows);

    return elapsed_seconds;
}

double time_dmvmult(long cols){
    long rows = cols*20;
    /************************* Double ****************************/
    printf("Representation double\n");

    // Allocate memory for the matrix
    double **myMatrix = dmatrix(1, rows, 1, cols);
    double *myVector = dvector(1, cols);
    double *result = dvector(1, rows);

    // Initialize the matrix with random values
    initializedMatrix(myMatrix, 1, rows, 1, cols);

    initializedMatrix(&myVector,0,0,1,cols);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 5;
    int i;
    for (i = 0; i < reps; i++) {
        dmvmul(result,myMatrix,myVector,rows,cols);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_dmatrix(myMatrix,1,rows,1,cols);
    free_dvector(myVector,1,cols);
    free_dvector(result,1,rows);

    return elapsed_seconds;
}

double time_imvmult(long cols){
    long rows = cols*20;
        /************************* INT ****************************/
    printf("Representation INT\n");

    // Allocate memory for the matrix
    int **myMatrix = imatrix(1, rows, 1, cols);
    int *myVector = ivector(1, cols);
    int *result = ivector(1, rows);

    // Initialize the matrix with random values
    initializeiMatrix(myMatrix, 1, rows, 1, cols);

    initializeiMatrix(&myVector,0,0,1,cols);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 30;
    int i;
    for (i = 0; i < reps; i++) {
        imvmul(result,myMatrix,myVector,rows,cols);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_imatrix(myMatrix,1,rows,1,cols);
    free_ivector(myVector,1,cols);
    free_ivector(result,1,rows);
    
    return elapsed_seconds;
}

/**************************************************************
 * ************************************************************
 * ********************  FFT TIMING CODE  *********************
 * ************************************************************
 * ************************************************************
*/

double time_fft(long data_length){
    /************************* FLOAT ****************************/
    printf("Representation float\n");

    // Allocate memory for the matrix
    float *myVector = vector(1, data_length*2);

    // Initialize the matrix with random values
    initializeMatrix(&myVector,0,1,1,data_length*2);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 30;
    int i;
    for (i = 0; i < reps; i++) {
        four1(myVector,data_length,1);
        //Brandon's FFT thing
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_vector(myVector,1,data_length*2);

    return elapsed_seconds;
}


double time_dfft(long data_length){
    /************************* DOUBLE ****************************/
    printf("Representation double\n");

    // Allocate memory for the matrix
    double *myVector = dvector(1, data_length*2);

    // Initialize the matrix with random values
    initializedMatrix(&myVector,0,1,1,data_length*2);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 30;
    int i;
    for (i = 0; i < reps; i++) {
        dfour1(myVector,data_length,1);
        //Brandon's FFT thing
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_dvector(myVector,1,data_length*2);

    return elapsed_seconds;
}


double time_ifft(long data_length){
    /************************* INT ****************************/
    printf("Representation int\n");

    // Allocate memory for the matrix
    int *myVector = ivector(1, data_length*2);

    // Initialize the matrix with random values
    initializeiMatrix(&myVector,0,1,1,data_length*2);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 30;
    int i;
    for (i = 0; i < reps; i++) {
        ifour1(myVector,data_length,1);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_ivector(myVector,1,data_length*2);

    return elapsed_seconds;
}


/**************************************************************
 * ************************************************************
 * ********************  2D FFT TIMING CODE  *********************
 * ************************************************************
 * ************************************************************
*/

double time_fft2D(long N) //N is the number of complex values along one dimension
{
    /************************* FLOAT ****************************/
    printf("Representation float\n");

    int isign;
    unsigned long *nn; //vector dimension
    nn=lvector(1,2);
    int NDIM;
    int NDAT2;
	isign = 1;
    NDIM = 2;
    NDAT2 = N*N*NDIM;

	nn[1]=N;
	nn[2]=N;

    // Allocate memory for the matrix
    float *data1 = vector(1, NDAT2);

    // Initialize the matrix with random values
    //void initializeMatrix(float **matrix, long nrl, long nrh, long ncl, long nch) {
    initializeMatrix(&data1,0,0,1,NDAT2);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 2;
    int i;
    for (i = 0; i < reps; i++) {
        fourn(data1,nn,NDIM,isign); 
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_vector(data1,1,NDAT2);
	free_lvector(nn,1,NDIM);
    return elapsed_seconds;
}

//havent filled in the below yet
double time_dfft2D(long N){
    /************************* DOUBLE ****************************/
    printf("Representation double\n");

    int isign;
    unsigned long *nn; //vector dimension
    nn=lvector(1,2);
    int NDIM;
    int NDAT2;
	isign = 1;
    NDIM = 2;
    NDAT2 = N*N*NDIM;

	nn[1]=N;
	nn[2]=N;

    // Allocate memory for the matrix
    double *data1 = dvector(1, NDAT2);

    // Initialize the matrix with random values
    initializedMatrix(&data1,0,0,1,N*2);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 2;
    int i;
    for (i = 0; i < reps; i++) {
        dfourn(data1,nn,NDIM,isign); 
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_dvector(data1,1,N*2);

    return elapsed_seconds;
}


// double time_ifft2D(long data_length){
//     /************************* DOUBLE ****************************/
//     printf("Representation int\n");

//     // Allocate memory for the matrix
//     int *myVector = ivector(1, data_length*2);

//     // Initialize the matrix with random values
//     initializeiMatrix(&myVector,0,0,1,data_length*2);

//     struct timespec start, stop;
//     clock_gettime(CLOCK_MONOTONIC, &start);

//     int reps = 10;
//     int i;
//     for (i = 0; i < reps; i++) {
//         ifourn(data1,nn,NDIM,isign); 
//     }
//     clock_gettime(CLOCK_MONOTONIC, &stop);
//     double elapsed_seconds = time_print(&start,&stop,reps);

//     // Remember to free the allocated memory when done
//     free_ivector(myVector,1,data_length*2);

//     return elapsed_seconds;
// }

/**************************************************************
 * ************************************************************
 * ********************  GEMM TIMING CODE  ********************
 * ************************************************************
 * ************************************************************
*/

double time_dmatmul(long size) {
    printf("Representation double\n");

    // Set matrix dimensions
    long rows = size;
    long cols = size;

    // Allocate memory for matrices
    double **A = dmatrix(1, rows, 1, cols);
    double **B = dmatrix(1, cols, 1, cols);
    double **C = dmatrix(1, rows, 1, cols);
    //double **C1 = dmatrix(1, rows, 1, cols);

    // Initialize matrices with random values
    initializedMatrix(A, 1, rows, 1, cols);
    initializedMatrix(B, 1, cols, 1, cols);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 1;
    for (int i = 0; i < reps; i++) {
        dmatmul(C, A, B, rows, cols, cols);
    }

    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start, &stop, reps);

    //CORRECTNESS CHECK
    // clock_gettime(CLOCK_MONOTONIC, &start);

    // for (int i = 0; i < reps; i++) {
    //     sequential_dmatmul(C1, A, B, rows, cols, cols);
    // }

    // clock_gettime(CLOCK_MONOTONIC, &stop);
    // printf("Time for iterative method\n");
    // time_print(&start, &stop, reps);

    // if(dmat_approx_equal(C,C1,rows,cols,1e-3)){
    //     printf("Matrices equal\n");
    // }else{printf("Matrices not equal\n");}

    // Free allocated memory
    free_dmatrix(A, 1, rows, 1, cols);
    free_dmatrix(B, 1, cols, 1, cols);
    free_dmatrix(C, 1, rows, 1, cols);
    //free_dmatrix(C1, 1, rows, 1, cols);

    return elapsed_seconds;
}

double time_matmul(long size) {
    printf("Representation float\n");

    // Set matrix dimensions
    long rows = size;
    long cols = size;

    // Allocate memory for matrices
    float **A = matrix(1, rows, 1, cols);
    float **B = matrix(1, cols, 1, cols);
    float **C = matrix(1, rows, 1, cols);

    // Initialize matrices with random values
    initializeMatrix(A, 1, rows, 1, cols);
    initializeMatrix(B, 1, cols, 1, cols);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 1;
    for (int i = 0; i < reps; i++) {
        matmul(C, A, B, rows, cols, cols);
    }

    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start, &stop, reps);


    // Free allocated memory
    free_matrix(A, 1, rows, 1, cols);
    free_matrix(B, 1, cols, 1, cols);
    free_matrix(C, 1, rows, 1, cols);

    return elapsed_seconds;
}

double time_imatmul(long size) {
    printf("Representation int\n");

    // Set matrix dimensions
    long rows = size;
    long cols = size;

    // Allocate memory for matrices
    int **A = imatrix(1, rows, 1, cols);
    int **B = imatrix(1, cols, 1, cols);
    int **C = imatrix(1, rows, 1, cols);

    // Initialize matrices with random values
    initializeiMatrix(A, 1, rows, 1, cols);
    initializeiMatrix(B, 1, cols, 1, cols);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 1;
    for (int i = 0; i < reps; i++) {
        imatmul(C, A, B, rows, cols, cols);
    }

    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start, &stop, reps);


    // Free allocated memory
    free_imatrix(A, 1, rows, 1, cols);
    free_imatrix(B, 1, cols, 1, cols);
    free_imatrix(C, 1, rows, 1, cols);

    return elapsed_seconds;
}


/**************************************************************
 * ************************************************************
 * ********************  CGEMM TIMING CODE  *******************
 * ************************************************************
 * ************************************************************
*/

double time_complex_dmatmul(long size) {
    printf("Representation double\n");

    // Set matrix dimensions
    long rows = size;
    long cols = size;

    // Allocate memory for matrices
    double *A = dvector(1, 2*rows*cols);
    double *B = dvector(1, 2*rows*cols);
    double *C = dvector(1, 2*rows*cols);
    double *C1 = dvector(1, 2*rows*cols);

    // Initialize matrices with random values
    initializedMatrix(&A, 0, 0, 1, 2*rows*cols);
    initializedMatrix(&B, 0, 0, 1, 2*rows*cols);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 1;
    for (int i = 0; i < reps; i++) {
        complex_dmatmul(C, A, B, rows, cols, cols);
    }

    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start, &stop, reps);

    //CORRECTNESS CHECK
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < reps; i++) {
        sequential_complex_dmatmul(C1, A, B, rows, cols, cols);
    }

    clock_gettime(CLOCK_MONOTONIC, &stop);
    printf("Time for iterative method\n");
    time_print(&start, &stop, reps);

    // printf("A\n");
    // print_row_major_complex(A,rows,cols);
    // printf("B\n");
    // print_row_major_complex(B,rows,cols);
    // printf("C\n");
    // print_row_major_complex(C,rows,cols);
    // printf("C1\n");
    // print_row_major_complex(C1,rows,cols);


    if(complex_dmat_approx_equal(C,C1,rows,cols,1e-3)){
        printf("Matrices equal\n");
    }else{printf("Matrices not equal\n");}

    // Free allocated memory
    free_dvector(A, 1, 2*rows*cols);
    free_dvector(B, 1, 2*rows*cols);
    free_dvector(C, 1, 2*rows*cols);
    free_dvector(C1,1, 2*rows*cols);

    return elapsed_seconds;
}


/**************************************************************
 * ************************************************************
 * ********************  ATA TIMING CODE  *********************
 * ************************************************************
 * ************************************************************
*/
double time_dATA(long cols){
    /************************* double ****************************/
    printf("Representation double\n");
    long rows = cols*20;

    // Allocate memory for the matrix
    double **A = dmatrix(1, rows, 1, cols);
    double **result = dmatrix(1, cols, 1, cols);

    // Initialize the matrix with random values
    initializedMatrix(A, 1, rows, 1, cols);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 1;
    int i;
    for (i = 0; i < reps; i++) {
        dcompute_ata(result,A,rows,cols);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_dmatrix(A,1,rows,1,cols);
    free_dmatrix(result,1,cols,1,cols);
    return elapsed_seconds;
}


double time_ATA(long cols){
    /************************* float ****************************/
    printf("Representation float\n");
    long rows = cols*20;

    // Allocate memory for the matrix
    float **A = matrix(1, rows, 1, cols);
    float **result = matrix(1, cols, 1, cols);

    // Initialize the matrix with random values
    initializeMatrix(A, 1, rows, 1, cols);


    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 1;
    int i;
    for (i = 0; i < reps; i++) {
        compute_ata(result,A,rows,cols);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);
    printf("finished compute_ata\n");

    // Remember to free the allocated memory when done
    free_matrix(A,1,rows,1,cols);
    free_matrix(result,1,cols,1,cols);
    return elapsed_seconds;
}


double time_iATA(long cols){
    /************************* int ****************************/
    printf("Representation int\n");
    long rows = cols*20;

    // Allocate memory for the matrix
    int **A = imatrix(1, rows, 1, cols);
    int **result = imatrix(1, cols, 1, cols);

    // Initialize the matrix with random values
    initializeiMatrix(A, 1, rows, 1, cols);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 1;
    int i;
    for (i = 0; i < reps; i++) {
        icompute_ata(result,A,rows,cols);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_imatrix(A,1,rows,1,cols);
    free_imatrix(result,1,cols,1,cols);
    return elapsed_seconds;
}


/**************************************************************
 * ************************************************************
 * ********************  QR DCMP  ************************
 * ************************************************************
 * ************************************************************
*/
//void qrdcmp(float **a, int n, float *c, float *d, int *sing)
double time_qrdcmp(long cols){
    long n = cols;
    int sing; // Flag for if singularity encountered

    /************************* FLOAT ****************************/
    printf("Representation float\n");

    // Allocate memory for the matrix
    float **a = matrix(1, n, 1, n);
    float *c = vector(1, n);
    float *d = vector(1, n);

    // Initialize the matrix with random values
    initializeMatrix(a, 1, n, 1, n);

    //initializeMatrix(&c,0,1,1,n);
    //initializeMatrix(&d,0,1,1,n);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 1;
    int i;
    for (i = 0; i < reps; i++) {
        qrdcmp(a, n, c, d, &sing);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_matrix(a,1,n,1,n);
    free_vector(c,1,n);
    free_vector(d,1,n);

    return elapsed_seconds;
}

double time_dqrdcmp(long cols){
    long n = cols;
    int sing; // Flag for if singularity encountered

    /************************* Double ****************************/
    printf("Representation double\n");

    // Allocate memory for the matrix
    double **a = dmatrix(1, n, 1, n);
    double *c = dvector(1, n);
    double *d = dvector(1, n);

    // Initialize the matrix with random values
    initializedMatrix(a, 1, n, 1, n);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 1;
    int i;
    for (i = 0; i < reps; i++) {
        dqrdcmp(a, n, c, d, &sing);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_dmatrix(a,1,n,1,n);
    free_dvector(c,1,n);
    free_dvector(d,1,n);

    return elapsed_seconds;
}

double time_iqrdcmp(long cols){
    long n = cols;
    int sing; // Flag for if singularity encountered

    /************************* Int ****************************/
    printf("Representation int\n");

    // Allocate memory for the matrix
    int **a = imatrix(1, n, 1, n);
    int *c = ivector(1, n);
    int *d = ivector(1, n);

    // Initialize the matrix with random values
    initializeiMatrix(a, 1, n, 1, n);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 1;
    int i;
    for (i = 0; i < reps; i++) {
        iqrdcmp(a, n, c, d, &sing);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_imatrix(a,1,n,1,n);
    free_ivector(c,1,n);
    free_ivector(d,1,n);

    return elapsed_seconds;
}

/**************************************************************
 * ************************************************************
 * ********************  QR Solv  ************************
 * ************************************************************
 * ************************************************************
*/
//solves if precomputed
double time_qrsolv(long cols){
    long n = cols;
    int sing; // Flag for if singularity encountered

    /************************* FLOAT ****************************/
    printf("Representation float\n");

    // Allocate memory for the matrix
    float **a = matrix(1, n, 1, n);
    float *c = vector(1, n);
    float *d = vector(1, n);
    float *x = vector(1, n);

    // Initialize the matrix with random values
    initializeMatrix(a, 1, n, 1, n);

    //initializeMatrix(&c,0,1,1,n);
    //initializeMatrix(&d,0,1,1,n);
    qrdcmp(a, n, c, d, &sing);
    
    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 30;
    int i;
    for (i = 0; i < reps; i++) {
        qrsolv(a,n,c,d,x);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_matrix(a,1,n,1,n);
    free_vector(c,1,n);
    free_vector(d,1,n);
    free_vector(x,1,n);

    return elapsed_seconds;
}


double time_dqrsolv(long cols){
    long n = cols;
    int sing; // Flag for if singularity encountered

    /************************* DOUBLE ****************************/
    printf("Representation double\n");

    // Allocate memory for the matrix
    double **a = dmatrix(1, n, 1, n);
    double *c = dvector(1, n);
    double *d = dvector(1, n);
    double *x = dvector(1, n);

    // Initialize the matrix with random values
    initializedMatrix(a, 1, n, 1, n);

    //initializeMatrix(&c,0,1,1,n);
    //initializeMatrix(&d,0,1,1,n);
    dqrdcmp(a, n, c, d, &sing);
    
    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 30;
    int i;
    for (i = 0; i < reps; i++) {
        dqrsolv(a,n,c,d,x);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_dmatrix(a,1,n,1,n);
    free_dvector(c,1,n);
    free_dvector(d,1,n);
    free_dvector(x,1,n);

    return elapsed_seconds;
}

double time_iqrsolv(long cols){
    long n = cols;
    int sing; // Flag for if singularity encountered

    /************************* INT ****************************/
    printf("Representation int\n");


    // Allocate memory for the matrix
    int **a = imatrix(1, n, 1, n);

        fflush(stdout);
    int *c = ivector(1, n);
    int *d = ivector(1, n);
    int *x = ivector(1, n);

            fflush(stdout);


    // Initialize the matrix with random values
    initializeiMatrix(a, 1, n, 1, n);
    initializeiMatrix(&c,0,1,1,n);
    initializeiMatrix(&d,0,1,1,n);
    initializeiMatrix(&x,0,1,1,n);

    fflush(stdout);
    iqrdcmp(a, n, c, d, &sing);

    fflush(stdout);
    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 30;
    int i;
    for (i = 0; i < reps; i++) {
        iqrsolv(a,n,c,d,x);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_imatrix(a,1,n,1,n);
    free_ivector(c,1,n);
    free_ivector(d,1,n);
    free_ivector(x,1,n);

    return elapsed_seconds;
}

/**************************************************************
 * ************************************************************
 * ********************  QR Inv  ************************
 * ************************************************************
 * ************************************************************
*/
double time_qrinv(long cols){
    long n = cols;
    int sing; // Flag for if singularity encountered

    /************************* FLOAT ****************************/
    printf("Representation float\n");

    // Allocate memory for the matrix
    float **a = matrix(1, n, 1, n);
    float **ainv = matrix(1, n, 1, n);
    float *c = vector(1, n);
    float *d = vector(1, n);

    // Initialize the matrix with random values
    initializeMatrix(a, 1, n, 1, n);

    qrdcmp(a, n, c, d, &sing);
    
    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 30;
    int i;
    for (i = 0; i < reps; i++) {
        qrinv(ainv,a,n,c,d);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_matrix(a,1,n,1,n);
    free_matrix(ainv,1,n,1,n);
    free_vector(c,1,n);
    free_vector(d,1,n);

    return elapsed_seconds;
}

double time_dqrinv(long cols){
    long n = cols;
    int sing; // Flag for if singularity encountered

    /************************* DOUBLE ****************************/
    printf("Representation float\n");

    // Allocate memory for the matrix
    double **a = dmatrix(1, n, 1, n);
    double **ainv = dmatrix(1, n, 1, n);
    double *c = dvector(1, n);
    double *d = dvector(1, n);

    // Initialize the matrix with random values
    initializedMatrix(a, 1, n, 1, n);

    dqrdcmp(a, n, c, d, &sing);
    
    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 30;
    int i;
    for (i = 0; i < reps; i++) {
        dqrinv(ainv,a,n,c,d);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_dmatrix(a,1,n,1,n);
    free_dmatrix(ainv,1,n,1,n);
    free_dvector(c,1,n);
    free_dvector(d,1,n);

    return elapsed_seconds;
}

double time_iqrinv(long cols){
    long n = cols;
    int sing; // Flag for if singularity encountered

    /************************* INT ****************************/
    printf("Representation int\n");

    // Allocate memory for the matrix
    int **a = imatrix(1, n, 1, n);
    int **ainv = imatrix(1, n, 1, n);
    int *c = ivector(1, n);
    int *d = ivector(1, n);

    // Initialize the matrix with random values
    initializeiMatrix(a, 1, n, 1, n);

    iqrdcmp(a, n, c, d, &sing);
    
    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int reps = 30;
    int i;
    for (i = 0; i < reps; i++) {
        iqrinv(ainv,a,n,c,d);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_seconds = time_print(&start,&stop,reps);

    // Remember to free the allocated memory when done
    free_imatrix(a,1,n,1,n);
    free_imatrix(ainv,1,n,1,n);
    free_ivector(c,1,n);
    free_ivector(d,1,n);

    return elapsed_seconds;
}


/**************************************************************
 * ************************************************************
 * ********************  MAIN  ********************************
 * ************************************************************
 * ************************************************************
*/

int main() {


    FILE *csv_file = fopen("benchmark_results.csv", "w");
    if (csv_file == NULL) {
        perror("Failed to open CSV file");
        return 1;
    }

    // Write the CSV header
    fprintf(csv_file, "Size,Float,Double,Int\n");

    int i;

#ifdef ENABLE_MATRIX_VECTOR_BENCHMARK
    printf("Starting Matrix Vector Benchmark\n");
    fprintf(csv_file,"Matrix Vector Benchmark\n");

    for (i = 128; i <= 8192; i = i * 2) {
        printf("Number of Columns = %d\n", i);
        double float_rep_time, double_rep_time, int_rep_time;
        float_rep_time = time_mvmult(i);
        double_rep_time = time_dmvmult(i);
        int_rep_time = time_imvmult(i);

        // Write the results to the CSV file
        fprintf(csv_file, "%d,%.6f,%.6f,%.6f\n", i, float_rep_time, double_rep_time, int_rep_time);
        fflush(csv_file);
    }

#endif

#ifdef ENABLE_FFT_BENCHMARK
    printf("Starting 1D FFT Benchmark\n");
    fprintf(csv_file,"1D FFT Benchmark\n");

    for (i = 1024; i <= 1024*16; i = i * 2) {
        printf("Number of Elements = %d\n", i);
        double float_rep_time, double_rep_time, int_rep_time;
        float_rep_time = time_fft(i);
        double_rep_time = time_dfft(i);
        int_rep_time = time_ifft(i);

        // Write the results to the CSV file
        fprintf(csv_file, "%d,%.6f,%.6f,%.6f\n", i, float_rep_time, double_rep_time, int_rep_time);
        fflush(csv_file);
    }

#endif

#ifdef ENABLE_FFT2D_BENCHMARK
    printf("Starting 2D FFT Benchmark\n");
    fprintf(csv_file,"2D FFT Benchmark\n");

    for (i = 128; i <= 8192; i = i * 2) {
        printf("Number of Elements = %d\n", i);
        double float_rep_time, double_rep_time, int_rep_time;
        float_rep_time = time_fft2D(i);
        double_rep_time = time_dfft2D(i);
        //int_rep_time = time_ifft2D(i);

        // Write the results to the CSV file
        //fprintf(csv_file, "%d,%.6f,%.6f,%.6f\n", i, float_rep_time, double_rep_time, int_rep_time);
        fprintf(csv_file, "%d,%.6f,%.6f\n", i, float_rep_time, double_rep_time);
        //fprintf(csv_file, "%d,%.6f\n", i, float_rep_time);
        fflush(csv_file);
    }

#endif

#ifdef ENABLE_ATA_BENCHMARK
    printf("Starting Matrix Matrix (ATA) Benchmark\n");
    fprintf(csv_file,"Matrix Matrix (ATA) Benchmark\n");

    for(i=128;i<=4096;i=i*2){
        printf("Number of Columns = %d\n", i);
        double float_rep_time, double_rep_time, int_rep_time;
        float_rep_time = time_ATA(i);
        double_rep_time = time_dATA(i);
        int_rep_time = time_iATA(i);
        // Write the results to the CSV file
        //fprintf(csv_file, "%d,%.6f\n", i,  double_rep_time);
        fprintf(csv_file, "%d,%.6f,%.6f,%.6f\n", i, float_rep_time, double_rep_time, int_rep_time);
        fflush(csv_file);
    }

#endif

#ifdef ENABLE_GEMM_BENCHMARK
    printf("Starting GEMM Benchmark\n");
    fprintf(csv_file,"GEMM Benchmark\n");

    for(i=128;i<=4096;i=i*2){
        printf("n = %d\n", i);
        double float_rep_time, double_rep_time, int_rep_time;
        float_rep_time = time_matmul(i);
        double_rep_time = time_dmatmul(i);
        int_rep_time = time_imatmul(i);
        // Write the results to the CSV file
        fprintf(csv_file, "%d,%.6f,%.6f,%.6f\n", i, float_rep_time, double_rep_time, int_rep_time);
        //fprintf(csv_file, "%d,%.6f\n", i, double_rep_time);
        fflush(csv_file);
    }

#endif

#ifdef ENABLE_CGEMM_BENCHMARK
    printf("Starting CGEMM Benchmark\n");
    fprintf(csv_file,"CGEMM Benchmark\n");

    for(i=128;i<=128*16;i=i*2){
        printf("n = %d\n", i);
        double float_rep_time, double_rep_time, int_rep_time;
        //float_rep_time = time_qrinv(i);
        double_rep_time = time_complex_dmatmul(i);
        //int_rep_time = time_iqrinv(i);
        // Write the results to the CSV file
        //fprintf(csv_file, "%d,%.6f,%.6f,%.6f\n", i, float_rep_time, double_rep_time, int_rep_time);
        fprintf(csv_file, "%d,%.6f\n", i, double_rep_time);
        fflush(csv_file);
    }

#endif


#ifdef ENABLE_QR_DCMP_BENCHMARK
    printf("Starting QR DCMP Benchmark\n");
    fprintf(csv_file,"QR DCMP Benchmark\n");

    for(i=128;i<=128*16;i=i*2){
        printf("n = %d\n", i);
        double float_rep_time, double_rep_time, int_rep_time;
        float_rep_time = time_qrdcmp(i);
        double_rep_time = time_dqrdcmp(i);
        int_rep_time = time_iqrdcmp(i);
        // Write the results to the CSV file
        fprintf(csv_file, "%d,%.6f,%.6f,%.6f\n", i, float_rep_time, double_rep_time, int_rep_time);
        fflush(csv_file);
    }

#endif

#ifdef ENABLE_QR_SOLVE_BENCHMARK
    printf("Starting QR Solve Benchmark\n");
    fprintf(csv_file,"QR Solve Benchmark\n");

    for(i=128;i<=128*16;i=i*2){
        printf("n = %d\n", i);
        double float_rep_time, double_rep_time, int_rep_time;
        float_rep_time = time_qrsolv(i);
        double_rep_time = time_dqrsolv(i);
        int_rep_time = time_iqrsolv(i);
        // Write the results to the CSV file
        fprintf(csv_file, "%d,%.6f,%.6f,%.6f\n", i, float_rep_time, double_rep_time, int_rep_time);
        fflush(csv_file);
    }

#endif

#ifdef ENABLE_QR_INV_BENCHMARK
    printf("Starting QR Inversion Benchmark\n");
    fprintf(csv_file,"QR Inversion Benchmark\n");

    for(i=128;i<=128*16;i=i*2){
        printf("n = %d\n", i);
        double float_rep_time, double_rep_time, int_rep_time;
        float_rep_time = time_qrinv(i);
        double_rep_time = time_dqrinv(i);
        int_rep_time = time_iqrinv(i);
        // Write the results to the CSV file
        fprintf(csv_file, "%d,%.6f,%.6f,%.6f\n", i, float_rep_time, double_rep_time, int_rep_time);
        fflush(csv_file);
    }

#endif

    fclose(csv_file);
    printf("Benchmark results have been written to 'benchmark_results.csv'\n");

    return 0;
}