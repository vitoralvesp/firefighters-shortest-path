#include <stdio.h>
#include <stdlib.h>

#define MAX_LINES_INPUT (100)
#define MAX_CORNERS (10)
#define CHARS_TO_READ_IN_LINE (7) // 3 numeros, 2 espacos, 1 '\n'

void build_mtrx(int mtrx[MAX_LINES_INPUT][3], char buffer[MAX_LINES_INPUT], int k) {
    int col = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] > 48 && buffer[i] <= 57) {
            mtrx[k][col] = buffer[i] - '0';
            col++;
        }
    }
    
}

//build_table(matriz,tabela,quantidade de linhas) -> monta uma tabela, 
//onde a distancia entre uma esquina 'x' e outra 'y' esta em 'tabela[x-1][y-1]'
void build_table(int mtrx[MAX_LINES_INPUT][3], int table[MAX_CORNERS][MAX_CORNERS], int n_rows){
    for(int row = 0; row < n_rows; row++){
        //A cada linha da matriz, armazena a distancia das esquinas na tabela
        //printf("(%d,%d) = %d\n", mtrx[row][0], mtrx[row][1], mtrx[row][2]);
        table[mtrx[row][0]-1][mtrx[row][1]-1] = mtrx[row][2]; 
        //printf("m[%d][%d] = %d\n\n",mtrx[row][0]-1,mtrx[row][1]-1,table[mtrx[row][0]-1][mtrx[row][1]-1]);
    }
   
}



int main() {
    
    FILE *file_ptr = fopen("../docs/input.txt", "r");
    if (file_ptr == NULL) {
        printf("\nERRO: Nao foi possivel abrir o arquivo em \"../docs/input.txt\"\n\n");
        exit(1);
    } else {

        int mtrx[MAX_LINES_INPUT][3];
        int table[MAX_CORNERS][MAX_CORNERS];
        char buffer[MAX_LINES_INPUT];
        int biggest_corner = 1;
        int corner_on_fire = 1;
        int k = 0;
        int lines_idx = 0;

        for (int i = 0; i < MAX_LINES_INPUT; i++) {
            for (int j = 0; j < 3; j++)
                mtrx[i][j] = 0;
        }

        //k = 0 -> esquina pegando fogo
        //k = 1 -> Numero de Esquinas
        //k > 1 -> Passa a ler a matriz
        while (fgets(buffer, CHARS_TO_READ_IN_LINE, file_ptr) && buffer[0] != '0' ) {
            if(k == 0) {
                corner_on_fire = buffer[0] - '0';
                k++;
            }
            else if(k == 1) {
                biggest_corner = buffer[0] - '0';
                k++;
            }
            else {
                build_mtrx(mtrx, buffer, lines_idx);
                lines_idx++;
            }
        }

        printf("mtrx:\n");
        for (int i = 0; i < lines_idx; i++) {
            for (int j = 0; j < 3; j++)
                printf("%d ", mtrx[i][j]);
            printf("\n");
        }

        //Inicializando tabela com 0s
        for(int i = 0; i < biggest_corner; i++){
            for(int j = 0; j < biggest_corner; j++){
                table[i][j] = 0;
            }
        }

        build_table(mtrx,table,lines_idx);
        printf("\ntable: \n");
        for (int i = 0; i < biggest_corner; i++) {
            for (int j = 0; j < biggest_corner; j++)
                printf("%d ", table[i][j]);
            printf("\n");
        }


    }

    return 0;
}