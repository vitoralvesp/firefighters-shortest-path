#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_LINES_INPUT (100)
#define MAX_CORNERS (10)
#define CHARS_TO_READ_IN_LINE (7) // 3 numeros, 2 espacos, 1 '\n'

/* build_mtrx: recebe uma buffer com elementos numéricos do tipo char e os converte para 
   inteiros em uma matriz utilizada para armazenar as esquinas e custos (void)
   'mtrx': matriz do tipo int de tamanho 100 x 3
   'buffer': vetor de elementos numéricos do tipo char com parte do conteúdo lido de um arquivo
   'k': identificador da linha que o conteúdo do buffer será alocado na matriz  */
void build_mtrx(int mtrx[MAX_LINES_INPUT][3], char buffer[MAX_LINES_INPUT], int k) {
    int col = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] > 48 && buffer[i] <= 57) {
            mtrx[k][col] = buffer[i] - '0';
            col++;
        }
    }
}

/* build_table: monta uma tabela apenas com os custos do percurso de uma esquina i até uma esquina j (void)
   'mtrx':  matriz do tipo int de tamanho 100 x 3
   'table': tabela do tipo int que será montada com os custos do percurso das esquinas em mtrx
   'n_rows': total de linhas da matriz */
void build_table(int mtrx[MAX_LINES_INPUT][3], int table[MAX_CORNERS][MAX_CORNERS], int n_rows){
    for(int row = 0; row < n_rows; row++)
        table[mtrx[row][0]-1][mtrx[row][1]-1] = mtrx[row][2]; 
}

/* get_min_idx: retorna o índice do menor elemento de um vetor de inteiros (int)
   'v': vetor de inteiros que será analisado
   'used': vetor que identifica os elementos que estão sendo utilizados no vetor
   'size': tamanho do vetor 
   'min': índice do menor elemento no vetor */
int get_min_idx (int v[], int used[], int size){
    int min = 1;
    for (int i = 2; i < size; i++) {
        if (v[i] < v[min] && !used[i]) {
            min = i;
        }
    }
    return min;
}

/* check: verifica se uma esquina já foi inserida no vetor de caminho das esquinas até a esquina de destino (int)
   'check_number': elemento que será procurado no vetor
   'path': vetor de inteiros que representa as esquinas percorridas
   'path_size': tamanho do vetor de caminhos
   'return 1': elemento encontrado
   'return 0': elemento não encontrado */
int check(int check_number, int path[], int path_size) {
    for (int i = 0; i < path_size; i++)
        if (path[i] == check_number) return 1;
    return 0;
}

/* generate_roadtrip: analisa as possibilidades armazenadas na tabela em relação aos custos dos percursos
   das esquinas e retorna o percurso e tempo ótimos para uma esquina de destino x (void)
   'optimal_time': vetor de tempos que conterá o tempo ótimo para a esquina de destino
   'optimal_path': vetor de caminhos que conterá a trajetória ótima de todas as esquinas até a esquina de destino
   'corner_on_fire': esquina de destino
   'biggest_corner': 
 */
void generate_roadtrip(int optimal_time[], int optimal_path[], int corner_on_fire, int biggest_corner, int used[],int table[MAX_CORNERS][MAX_CORNERS]){
    int count_used = 1,corner = 1, originCost, destCost, routeCost, origin = corner_on_fire, path_idx = 0;

     //Inicializando com infinitos, exceto a esquina inicial
    for(int i = 1; i < biggest_corner; i++) optimal_time[i] = INT_MAX;

    optimal_time[0] = 0;
    used[0] = 1;     

    while(count_used < biggest_corner){ //equivalente: enquanto E nao estiver vazio 
        originCost = optimal_time[corner-1];
        for(int corner_idx = 0; corner_idx<biggest_corner; corner_idx++){
            routeCost = table[corner-1][corner_idx];
            destCost = optimal_time[corner_idx];
            /*Ignorar se:
                -Nao houver caminho (currTime = 0)
                -destino = origem (corner_idx = corner)
                -Ja foi vistada
            */
            if (!check(corner, r, biggest_corner)) {
                optimal_path[path_idx] = corner;
                path_idx++;
            }
            
            if(routeCost == 0 || corner_idx == corner - 1 || used[corner_idx] ) continue;
            
            //Se descobre um tempo melhor, substitui
            if(originCost + routeCost < destCost) {
                optimal_time[corner_idx] = originCost + routeCost;
            }
        }
        //Mudando a esquina de referencia
        // printf("Esquina atual: %d\n", corner);
        // printf("Trocando esquina...\n");
        corner = get_min_idx(optimal_time, used, biggest_corner) + 1;
        count_used++;
        used[corner-1] = 1;
    }
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