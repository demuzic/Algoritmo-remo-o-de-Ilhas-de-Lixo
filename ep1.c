#include <stdio.h>
#include <stdlib.h>
#include <math.h> // para a funcao pow()

// VARIAVEIS GLOBAIS
int profundidade_container;
int linhas_matriz;
int colunas_matriz;

int *matriz_sub_ilhas;      // armazena os valores de lixo de cada celula
int *matriz_ilha_sub_ilhas; // armazena o id da ilha para cada celula
int *visitados;             // matriz para controlar celulas já visitadas pela busca 
int numero_de_ilhas = 0;
int *profundidades;

// FUNCOES
void import_dos_parametros(int argc, char *argv[]);
void import_matriz(char *nome_matriz);
void busca_ilha(int index, int id_ilha_atual);
void encontrar_ilhas();
void profundidade_da_ilha();
int *geracao_base_binaria(int tamanho, int numero);
void combinacao_de_ilhas_em_conteiner();
void liberar_memoria();
void demostrar_teste();

// CODIGO
int main(int argc, char *argv[]) {

    import_dos_parametros(argc, argv);

    // tarefa 1: identificar o número de ilhas
    encontrar_ilhas();
    printf("%d\n", numero_de_ilhas);

    if (numero_de_ilhas > 0) {
        // Tarefa 2: calcular e imprimir as profundidades
        profundidade_da_ilha();
        for (int i = 0; i < numero_de_ilhas; i++) {
            // imprime com espaço, exceto para o último elemento
            printf("%d%s", profundidades[i], (i == numero_de_ilhas - 1) ? "" : " ");
        }
        printf("\n");

        // tarefa 3: encontrar uma combinação válida
        combinacao_de_ilhas_em_conteiner();
    } else {
        printf("\n");
        printf("nao ha resposta valida!\n");
    }

    liberar_memoria();
    return 0;
}

void import_dos_parametros(int argc, char *argv[]) {
    profundidade_container = atoi(argv[1]);
    linhas_matriz = atoi(argv[2]);
    colunas_matriz = atoi(argv[3]);
    import_matriz(argv[4]);
}

void import_matriz(char *nome_matriz) {
    int tamanho_total = linhas_matriz * colunas_matriz;

    // aloca memoria para todas as matrizes que usaremos
    matriz_sub_ilhas = malloc(tamanho_total * sizeof(int));
    matriz_ilha_sub_ilhas = malloc(tamanho_total * sizeof(int));
    visitados = malloc(tamanho_total * sizeof(int));

    if (matriz_sub_ilhas == NULL || matriz_ilha_sub_ilhas == NULL || visitados == NULL) {
        printf("Erro de alocacao de memoria para as matrizes\n");
        exit(1);
    }

    FILE *pont_arq = fopen(nome_matriz, "r");
    if (pont_arq == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nome_matriz);
        exit(1);
    }

    for (int i = 0; i < tamanho_total; i++) {
        fscanf(pont_arq, "%d", &matriz_sub_ilhas[i]);
        // inicia as matrizes auxiliares com 0
        matriz_ilha_sub_ilhas[i] = 0;
        visitados[i] = 0;
    }
    fclose(pont_arq);
}


void busca_ilha(int index, int id_ilha_atual) {
    // condicoes de parada da recursao:
    if (index < 0 || index >= linhas_matriz * colunas_matriz || visitados[index] || matriz_sub_ilhas[index] == 0) {
        return;
    }

    // marca a celula atual como visitada e atribui o id da ilha
    visitados[index] = 1;
    matriz_ilha_sub_ilhas[index] = id_ilha_atual;

    int linha = index / colunas_matriz;
    int coluna = index % colunas_matriz;

    // chama a funcao para os 4 vizinhos (cima, baixo, esquerda, direita)
    if (linha > 0) busca_ilha(index - colunas_matriz, id_ilha_atual); // c
    if (linha < linhas_matriz - 1) busca_ilha(index + colunas_matriz, id_ilha_atual); // b
    if (coluna > 0) busca_ilha(index - 1, id_ilha_atual); // e
    if (coluna < colunas_matriz - 1) busca_ilha(index + 1, id_ilha_atual); // d
}


void encontrar_ilhas() { //Varredura de ilhas visitadas
    for (int i = 0; i < linhas_matriz * colunas_matriz; i++) {
        
        if (matriz_sub_ilhas[i] > 0 && !visitados[i]) {
            
            numero_de_ilhas++;
            // Inicia a busca para mapear todas as suas partes
            busca_ilha(i, numero_de_ilhas);
        }
    }
}

void profundidade_da_ilha() {
    if (numero_de_ilhas == 0) return;

    profundidades = malloc(numero_de_ilhas * sizeof(int));

    for (int i = 0; i < numero_de_ilhas; i++) {
        profundidades[i] = 0;
    }
    for (int i = 0; i < colunas_matriz * linhas_matriz; i++) { 
        int index_ilha = matriz_ilha_sub_ilhas[i];
        if (index_ilha != 0) {
           
            profundidades[index_ilha - 1] += matriz_sub_ilhas[i];
        }
    }

    // calculo da profundidade com o arrendomanento
    for (int i = 0; i < numero_de_ilhas; i++) {
        float decimal = (float)profundidades[i] / 6.0;
        
        if (decimal > 0 && decimal < 1) {
            profundidades[i] = 1; 
        } else {
            profundidades[i] = (int)decimal; 
        }
    }
}

int *geracao_base_binaria(int tamanho, int numero) {
    int *vetor = malloc(tamanho * sizeof(int));

    for (int i = 0; i < tamanho; i++) {
        vetor[i] = 0;
    }

    int index = tamanho - 1;
    while (numero > 0) {
        vetor[index] = numero % 2;
        numero = numero / 2;
        index--;
    }
    return vetor;
}

void combinacao_de_ilhas_em_conteiner() {
    int num_combinacoes = pow(2, numero_de_ilhas);
    int solucao_encontrada = 0; // flag para parar na primeira solução

    for (int a = 1; a < num_combinacoes; a++) {
        int soma = 0;
        int *base_binaria = geracao_base_binaria(numero_de_ilhas, a);

        for (int b = 0; b < numero_de_ilhas; b++) {
            if (base_binaria[b] == 1) {
                soma += profundidades[b];
            }
        }

        if (soma == profundidade_container) {
            for (int c = 0; c < numero_de_ilhas; c++) {
                if (base_binaria[c] == 1) {
                    printf("%d ", profundidades[c]);
                }
            }
            printf("\n");
            solucao_encontrada = 1;
            free(base_binaria); 
            return; 
        }
        
        free(base_binaria); 
    }

    if (!solucao_encontrada) {
        printf("nao ha resposta valida!\n");
    }
}

void liberar_memoria() {
    free(matriz_sub_ilhas);
    free(matriz_ilha_sub_ilhas);
    free(visitados);
    if (numero_de_ilhas > 0) {
        free(profundidades);
    }
}

