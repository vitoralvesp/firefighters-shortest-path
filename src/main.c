#include <stdio.h>
#include <stdlib.h>

void remove_in_str(char str[6], char character) {
    int k = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != character) {
            str[k] = str[i];
            k++;
        }
    }
    str[k] = '\0';
}


void build_mtrx(int mtrx[100][3], char buffer[6], int k) {
    int value;
    remove_in_str(buffer, ' ');
    remove_in_str(buffer, '\n');
    for (int i = 0; buffer[i] != '\0'; i++) {
        value = buffer[i] - '0';
        if (value < 0) mtrx[k][i] = 0;
        else mtrx[k][i] = value;
    }
    // printf("\n");
}



int main() {
    
    FILE *file_ptr = fopen("../../docs/input.txt", "r");
    if (file_ptr == NULL) {
        printf("\nERRO: Nao foi possivel abrir o arquivo em \"../../docs/input.txt\"\n\n");
        exit(1);
    } else {

        int mtrx[100][3];
        char buffer[6];
        int k;

        while (fgets(buffer, 6, file_ptr)) {
            build_mtrx(mtrx, buffer, k);
            k++;
        }

        printf("\nmtrx:\n");
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 3; j++)
                printf("%d ", mtrx[i][j]);
            printf("\n");
        }

    }

    return 0;
}