#include <stdio.h>
#include <stdlib.h>

void build_mtrx(int mtrx[100][3], char buffer[100], int k) {
    int col = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] >= 48 && buffer[i] <= 57) {
            mtrx[k][col] = buffer[i] - '0';
            col++;
        }
    }
}



int main() {
    
    FILE *file_ptr = fopen("../docs/input.txt", "r");
    if (file_ptr == NULL) {
        printf("\nERRO: Nao foi possivel abrir o arquivo em \"../docs/input.txt\"\n\n");
        exit(1);
    } else {

        int mtrx[100][3];
        char buffer[100];
        int k = 0;

        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 3; j++)
                mtrx[i][j] = 0;
        }

        while (fgets(buffer, 100, file_ptr)) {
            build_mtrx(mtrx, buffer, k);
            k++;
        }

        printf("mtrx:\n");
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < 3; j++)
                printf("%d ", mtrx[i][j]);
            printf("\n");
        }

    }

    return 0;
}