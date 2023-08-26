#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <strings.h>

/*
----------------------------------
GOAL: Write a C program to generate a square matrix (2D array)
      of size n x n where n is provided by the user. Fill the
      matrix with random integers between 1 to 100.
----------------------------------
*/

int main(int argc, char** argv) {

    // Initializes rand()
    srand(time(NULL));
    // Converts the second argument of the commmand line into an integer
    int num = atoi(argv[1]);

    // Creates an double pointer array array
    int **arr = (int**) malloc(sizeof(int) * num);

    /* FOR; i < num; i++
            ARR2 = (int*) malloc(sizeof(int) * num)
            FOR x < num; x++
                ARR2[x] = rand() % 101
            ARR[i] = ARR2
    */

    for (int i = 0; i < num; i++) {
        // Create space for the columns of integers
        // '(int*) malloc(sizeof(int) * num)' will always represent a dynamic array
        arr[i] = (int*) malloc(sizeof(int) * num);

        for (int x = 0; x < num; x++) {
            // Fill each space in the 2D Array with a random number
            arr[i][x] = rand() % 101;
        }
    }
    printf("-----------------------------------------------------------------\n");
    printf("Element value in the first column and row of the Matrix\n");
    printf("%i\n", arr[0][0]);
    printf("-----------------------------------------------------------------\n");
    printf("Length of diagonals\n");
    printf("%i elements long\n", num);
    printf("-----------------------------------------------------------------\n");
    free(arr);

    return 0;
}