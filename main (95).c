#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9

int board[9][9] = {
    {6,2,4,5,3,9,1,8,7},
    {5,1,9,7,2,8,6,3,4},
    {8,3,7,6,1,4,2,9,5},
    {1,4,3,8,6,5,7,2,9},
    {9,5,8,2,4,7,3,6,1},
    {7,6,2,3,9,1,4,5,8},
    {3,7,1,9,5,6,8,4,2},
    {4,9,6,1,8,2,5,7,3},
    {2,8,5,4,7,3,9,1,6}
};

int valid[11]; // 0: rows, 1: cols, 2-10: boxes

void* check_rows(void* arg) {
    for (int i = 0; i < SIZE; i++) {
        int seen[10] = {0};
        for (int j = 0; j < SIZE; j++) {
            int val = board[i][j];
            if (val < 1 || val > 9 || seen[val]) pthread_exit(NULL);
            seen[val] = 1;
        }
    }
    valid[0] = 1;
    pthread_exit(NULL);
}

void* check_cols(void* arg) {
    for (int j = 0; j < SIZE; j++) {
        int seen[10] = {0};
        for (int i = 0; i < SIZE; i++) {
            int val = board[i][j];
            if (val < 1 || val > 9 || seen[val]) pthread_exit(NULL);
            seen[val] = 1;
        }
    }
    valid[1] = 1;
    pthread_exit(NULL);
}

void* check_box(void* arg) {
    int idx = *(int*)arg;
    int row = (idx / 3) * 3;
    int col = (idx % 3) * 3;
    int seen[10] = {0};
    for (int i = row; i < row + 3; i++) {
        for (int j = col; j < col + 3; j++) {
            int val = board[i][j];
            if (val < 1 || val > 9 || seen[val]) pthread_exit(NULL);
            seen[val] = 1;
        }
    }
    valid[2 + idx] = 1;
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[11];
    int box_ids[9];

    pthread_create(&threads[0], NULL, check_rows, NULL);
    pthread_create(&threads[1], NULL, check_cols, NULL);

    for (int i = 0; i < 9; i++) {
        box_ids[i] = i;
        pthread_create(&threads[2 + i], NULL, check_box, &box_ids[i]);
    }

    for (int i = 0; i < 11; i++)
        pthread_join(threads[i], NULL);

    int all_ok = 1;
    for (int i = 0; i < 11; i++) {
        if (!valid[i]) {
            all_ok = 0;
            break;
        }
    }

    printf("%s\n", all_ok ? " OK" : " Not OK");
    return 0;
}
