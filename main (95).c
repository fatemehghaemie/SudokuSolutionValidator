#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 9  // size

int sudoku[SIZE][SIZE];  

// checking row
void* check_row(void* arg) {
    int row = *((int*) arg);
    int check[SIZE] = {0};

    for (int i = 0; i < SIZE; i++) {
        int num = sudoku[row][i];
        if (check[num - 1] == 1) {
            pthread_exit((void*) 0);  // invalid
        }
        check[num - 1] = 1;
    }

    pthread_exit((void*) 1);  // valid
}

// checking colum
void* check_column(void* arg) {
    int col = *((int*) arg);
    int check[SIZE] = {0};

    for (int i = 0; i < SIZE; i++) {
        int num = sudoku[i][col];
        if (check[num - 1] == 1) {
            pthread_exit((void*) 0);  // invalid
        }
        check[num - 1] = 1;
    }

    pthread_exit((void*) 1);  // valid
}


int main() {
    
    
    pthread_t threads[SIZE * 2];  
    int row_indices[SIZE], col_indices[SIZE];
    
    
    for (int i = 0; i < SIZE; i++) {
        row_indices[i] = i;
        pthread_create(&threads[i], NULL, check_row, &row_indices[i]);
    }

    
    for (int i = 0; i < SIZE; i++) {
        col_indices[i] = i;
        pthread_create(&threads[SIZE + i], NULL, check_column, &col_indices[i]);
    }

    // checking for results
    for (int i = 0; i < SIZE * 2; i++) {
        void* result;
        pthread_join(threads[i], &result);
        if (result == (void*) 0) {
            printf("it is not okay\n");
            return 0;
        }
    }

    printf("it is okay\n");
    return 0;
}
