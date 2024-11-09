#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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
int get_min_idx (int v[],int used[], int size){
    int min = 1;
    for(int i = 2; i < size; i++){
        if(v[i] < v[min] && !used[i]) {
            min = i;
        }
    }
   
    return min;
}

int check(int check_number, int path[], int path_size) {
    for (int i = 0; i < path_size; i++)
        if (path[i] == check_number) return 1;
    return 0;
}


//TODO: Montar vetor de tempo
//TODO: Adicionar parametro de vetor que mostre onde ocorre qual a esquina de origem
//TODO: Criar rota
void generate_roadtrip(int t[], int or[],int r[], int corner_on_fire, int biggest_corner, int used[],int table[MAX_CORNERS][MAX_CORNERS]){
    int count_used = 1,corner = 1, originCost, destCost, routeCost, origin = corner_on_fire, path_idx = 0;

     //Inicializando com infinitos, exceto a esquina inicial
    for(int i = 1; i < biggest_corner; i++) t[i] = INT_MAX;

    t[0] = 0;
    used[0] = 1;     

    while(count_used < biggest_corner){ //equivalente: enquanto E nao estiver vazio 
        originCost = t[corner-1];
        for(int corner_idx = 0; corner_idx<biggest_corner; corner_idx++){
            routeCost = table[corner-1][corner_idx];
            destCost = t[corner_idx];
            /*Ignorar se:
                -Nao houver caminho (currTime = 0)
                -destino = origem (corner_idx = corner)
                -Ja foi vistada
            */
            if (!check(corner, r, biggest_corner)) {
                r[path_idx] = corner;
                path_idx++;
            }
            
            if(routeCost == 0 || corner_idx == corner - 1 || used[corner_idx] ) continue;
            
            //Se descobre um tempo melhor, substitui
            if(originCost + routeCost < destCost) {
                t[corner_idx] = originCost + routeCost;
            }
        }
        //Mudando a esquina de referencia
        // printf("Esquina atual: %d\n", corner);
        // printf("Trocando esquina...\n");
        corner = get_min_idx(t,used,biggest_corner) + 1;
        count_used++;
        used[corner-1] = 1;
    }

    // while(origin != 1){
    //     int corner_idx = 0, r_idx = 0;
    //     while(corner_idx < origin-1) corner_idx++; //Procurar esquina
    //     r[biggest_corner - r_idx++] = or[corner_idx]; //adicionar a rota a sua origem
    //     origin = or[corner_idx]+1; //trocar a esquina a procurar
    // }


}



int main() {
    
    FILE *file_ptr = fopen("../../docs/input.txt", "r");
    if (file_ptr == NULL) {
        printf("\nERRO: Nao foi possivel abrir o arquivo em \"../docs/input.txt\"\n\n");
        exit(1);
    } else {

        int mtrx[MAX_LINES_INPUT][3];
        int table[MAX_CORNERS][MAX_CORNERS];
        int t[MAX_CORNERS];
        int or[MAX_CORNERS];
        int r[MAX_CORNERS];
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

        printf("Esquinas e Custos:\n");
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

        for (int i = 0; i < biggest_corner; i++)
            r[i] = 0;

        build_table(mtrx,table,lines_idx);
        printf("\nTabela:\n");
        for (int i = 0; i < biggest_corner; i++) {
            for (int j = 0; j < biggest_corner; j++)
                printf("%d ", table[i][j]);
            printf("\n");
        }

        int *used = calloc(biggest_corner, sizeof(int)); // Inicializa com zeros
        generate_roadtrip(t,or,r,corner_on_fire,biggest_corner,used,table);
        r[biggest_corner-1] = corner_on_fire;
        printf("\nrota ate a esquina #%d: ", corner_on_fire);
        for(int i = 0; i < biggest_corner-1; i++) {
            printf("%d ", r[i]);
        }
        printf("%d\n", r[biggest_corner-1]);
        printf("tempo calculado para a rota: %d min.", t[corner_on_fire-1]);


    }

    return 0;
}