#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_LINES_INPUT (100)
#define MAX_CORNERS (10)
#define MAX_LINE_CHARS_COUNT (17) // 7(3 numeros, 2 espacos, 1 '\n') padrao + margem de 10 espacos por linha
#define MAX_FILE_PATH_SIZE (255)

//------INTEGRANTES------
//Lucas Pires de Camargo Sarai - 10418013
//Vitor Alves Pereira - 10410862
//CC
//04D
//Proj. Analise II
//Referencias: 
//          -> Semana 09/10 - 11/10 -> Algoritmos Gulosos
//          -> Semana 14/10 - 18/10 -> Algoritmos Gulosos
//----------------------

/* strlen_no_spaces: versao adaptada do strlen que nao conta os espacos
    'str': string a ser lida
*/
int strlen_no_spaces(char str[]){
    int count = 0;
    for(int str_idx = 0; str[str_idx] != '\0'; str_idx++){
        if(isalnum(str[str_idx]) && !isalpha(str[str_idx])) count++;
    }
    return count;
}

/* build_matrix_line: recebe uma buffer com elementos numericos do tipo char e os converte para 
   inteiros em uma matriz utilizada para armazenar as esquinas e custos (int)
   'mtrx': matriz do tipo int de tamanho 100 x 3
   'buffer': vetor de elementos numericos do tipo char com parte do conteudo lido de um arquivo
   'line': identificador da linha que o conteudo do buffer será alocado na matriz  
   'return 1': valor nulo ou negativo encontrado (retorno invalido)
   'return 0': retorno valido
   'return -1': Quantia incorreta de valores de entrada (retorno invalido)
*/
int build_matrix_line(int mtrx[MAX_LINES_INPUT][3], char buffer[MAX_LINES_INPUT], int line) {
    int col = 0;
    int read_values = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] >= 48 && buffer[i] <= 57) {
            mtrx[line][col] = buffer[i] - '0';
            read_values++;
            if(mtrx[line][col] == 0) return 1; 
            col++;
        }
    }
    if(read_values != 3) {
        return -1;
    }
    return 0; 
}

/* build_table: monta uma tabela apenas com os custos do percurso de uma esquina i ate uma esquina j (int)
   'mtrx':  matriz do tipo int de tamanho 100 x 3
   'table': tabela do tipo int que será montada com os custos do percurso das esquinas em mtrx
   'n_rows': total de linhas da matriz
   'return 1': mao dupla encontrada (retorno invalido)
   'return 0': retorno valido
*/
int build_table(int mtrx[MAX_LINES_INPUT][3], int table[MAX_CORNERS][MAX_CORNERS], int n_rows){
    for(int row = 0; row < n_rows; row++){
        table[mtrx[row][0]-1][mtrx[row][1]-1] = mtrx[row][2]; 
        if(table[mtrx[row][1]-1][mtrx[row][0]-1] != 0) return 1; 
    }
    return 0;
        
}

/* get_min_idx: retorna o indice do menor elemento de um vetor de inteiros (int)
   'v': vetor de inteiros que sera analisado
   'used': vetor que identifica os elementos que estao sendo utilizados no vetor
   'size': tamanho do vetor 
   'min': indice do menor elemento no vetor */
int get_min_idx (int v[], int used[], int size){
    int min = 1;
    for (int i = 2; i < size; i++) {
        if (v[i] < v[min] && !used[i]) {
            min = i;
        }
    }
    return min;
}

/* generate_roadtrip(ROTA-MAIS-RAPIDA): analisa as possibilidades armazenadas na tabela em relação aos custos dos percursos
   das esquinas e retorna o percurso e tempo otimos para uma esquina de destino x (void)
   'optimal_time': vetor de tempos que conterá o tempo otimo para a esquina de destino
   'optimal_path': vetor de caminhos que conterá a trajetoria otima de todas as esquinas até a esquina de destino
   'corner_on_fire': esquina de destino
   'biggest_corner': maior numero identificador de esquina (esquinas vao de 1 a biggest_corner)
 */
void generate_roadtrip(int optimal_time[], int optimal_path[], int corner_on_fire, int biggest_corner, int used[],int table[MAX_CORNERS][MAX_CORNERS]){
    int count_used = 1,corner = 1, originCost, destCost, routeCost, origin = corner_on_fire, path_idx = 0;
    int predecessor_corner[biggest_corner];
    predecessor_corner[0] = 1;

     //Inicializando com infinitos, exceto a esquina inicial
    for(int i = 1; i < biggest_corner; i++) optimal_time[i] = INT_MAX;

    optimal_time[0] = 0;
    used[0] = 1;     

    //Definindo os menores custos de tempo para chegar a cada esquina
    while(count_used < biggest_corner){ 
        originCost = optimal_time[corner-1];
        for(int corner_idx = 0; corner_idx<biggest_corner; corner_idx++){
            routeCost = table[corner-1][corner_idx];
            destCost = optimal_time[corner_idx];
            
            /*Ignorar se:
                -Nao houver caminho (currTime = 0)
                -destino = origem (corner_idx = corner)
                -Ja foi vistada
            */
            if(routeCost == 0 || corner_idx == corner - 1 || used[corner_idx] ) continue;
            
            //Se descobre um tempo melhor, substitui
            if(originCost + routeCost < destCost) {
                optimal_time[corner_idx] = originCost + routeCost;
                predecessor_corner[corner_idx] = corner;
            }
        }
        //Mudando a esquina de referencia
        corner = get_min_idx(optimal_time, used, biggest_corner) + 1;
        count_used++;
        used[corner-1] = 1;
    }

    optimal_path[path_idx++] = corner_on_fire;
    while(path_idx < biggest_corner){
        optimal_path[path_idx] = predecessor_corner[optimal_path[path_idx-1]-1];
        if(optimal_path[path_idx] == 1) break;
        path_idx++;
    }
}

int main() {
    char file_path[MAX_FILE_PATH_SIZE];
    FILE* file_ptr;
    printf("Insira o caminho do arquivo de entrada: ");
    scanf("%s", file_path);
    file_ptr = fopen(file_path, "r");
    while(file_ptr == NULL){ 
        printf("\nERRO: Nao foi possivel abrir o arquivo em %s\n\n",file_path);
        printf("Insira o caminho do arquivo de entrada: ");
        scanf("%s", file_path);
        file_ptr = fopen(file_path, "r");
    } 

    int mtrx[MAX_LINES_INPUT][3];
    int table[MAX_CORNERS][MAX_CORNERS];
    int optimal_time[MAX_CORNERS];
    int optimal_path[MAX_CORNERS];
    char buffer[MAX_LINES_INPUT];
    int biggest_corner = 1;
    int corner_on_fire = 1;
    int file_line = 0;

    for (int i = 0; i < MAX_LINES_INPUT; i++) {
        for (int j = 0; j < 3; j++)
            mtrx[i][j] = 0;
    }

    //file_line = 0 -> esquina pegando fogo
    //file_line = 1 -> Numero de Esquinas
    //file_line > 1 -> Passa a ler a matriz de rotas
    while (fgets(buffer, MAX_LINE_CHARS_COUNT, file_ptr)) {
        if(file_line > 1 && strlen_no_spaces(buffer)==1 && buffer[0] == '0') break;
        if(file_line == 0) {
            if(strlen_no_spaces(buffer) != 1){
                printf("ERRO! Entrada insuficiente ou excessiva na linha 1\n");
                return EXIT_FAILURE;
            }
            corner_on_fire = buffer[0] - '0';
            if(corner_on_fire < 1){
                printf("ERRO! Esquina do incendio invalida! (deve ser maior que 0).\n");
                return EXIT_FAILURE;
            }
            file_line++;
        }
        else if(file_line == 1) {
            if(strlen_no_spaces(buffer) != 1){
                printf("ERRO! Entrada insuficiente ou excessiva na linha 2\n");
                return EXIT_FAILURE;
            }
            biggest_corner = buffer[0] - '0';
            if(corner_on_fire > biggest_corner){
                printf("ERRO! Esquina do incendio(linha 1) nao deve ultrapassar o numero de esquinas(linha 2)!\n");
                return EXIT_FAILURE;
            }
            file_line++;
        }
        else {
            int check_buffer = build_matrix_line(mtrx, buffer, file_line-2);
            if(check_buffer == 1) {
                printf("ERRO! Esquinas e custos devem ser maiores que 0!\n");
                return EXIT_FAILURE;
            }
            if(check_buffer == -1){
                printf("ERRO! Entrada insuficiente ou excessiva na linha %d.\n",file_line);
                return EXIT_FAILURE;
            }
            file_line++;
        }
    }

    //Fechando o arquivo
    fclose(file_ptr);

    printf("Esquinas e Custos:\n");
    for (int i = 0; i < file_line-2; i++) {
        for (int j = 0; j < 3; j++)
            printf("%d ", mtrx[i][j]);
        printf("\n");
    }

    //Inicializando tabela com 0s (onde nao existe rota)
    for(int i = 0; i < biggest_corner; i++){
        for(int j = 0; j < biggest_corner; j++){
            table[i][j] = 0;
        }
    }

    for (int i = 0; i < biggest_corner; i++)
        optimal_path[i] = 0;

    int check_table = build_table(mtrx,table,file_line);
    if(check_table){
        printf("ERRO! Nao podem existir vias de mao dupla!\n");
        return EXIT_FAILURE;
    }
    printf("\nTabela:\n");
    for (int i = 0; i < biggest_corner; i++) {
        for (int j = 0; j < biggest_corner; j++)
            printf("%d ", table[i][j]);
        printf("\n");
    }

    int *used = calloc(biggest_corner, sizeof(int)); // Inicializa com zeros
    generate_roadtrip(optimal_time,optimal_path,corner_on_fire,biggest_corner,used,table);
    printf("\nRota ate a esquina #%d: ", corner_on_fire);

    //Como a rota eh armazenada invertida, precisa procurar o inicio e exibir na ordem correta
    int start_route_idx;
    for(int search_idx = 0; search_idx < biggest_corner; search_idx++){
        if(optimal_path[search_idx] == 1) start_route_idx = search_idx; //esquina 1 eh onde comeca
    }
    for(int i = start_route_idx; i >= 0; i--) {
        printf("%d ", optimal_path[i]);
    }
    printf("\nTempo calculado para a rota: %d min.", optimal_time[corner_on_fire-1]);

    return 0;
}