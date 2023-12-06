#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <time.h> 

#define CVECTOR_LOGARITHMIC_GROWTH
#include "utils/c-vector/cvector.h"

#define MAX 70 
#define MM 1000000
#define mm 500

char entrada[MM]; 
int final[MAX]; 
int arestas[MAX][2]; 
char arestas_extra[MAX][4]; 
int tamanho_entrada, numero_estados, numero_transicoes; 
int contador = 1; 
int TS = 0; 
char saida[MAX*MAX]; 
cvector_vector_type(char) stack;
cvector_vector_type(char) output; 

typedef struct transicao {
    int v; 
    char caractere, empilha, desempilha, coloca_saida; 
} transicao; 
cvector_vector_type(transicao) tran[MAX]; 

int processa(int u, int pos, int deve_estar_vazia);
void linha();
void menu();
void get_input(char models[8][mm]);
void get_log(cvector_vector_type(char) results, int times_cars[MAX], int times_errors[MAX], char* steps[MAX]);

int main() {
    srand(time(NULL)); 

    FILE* automaton_file = fopen("./files/autofinal", "r"); 
    fscanf(automaton_file, " %d", &numero_estados);
    fscanf(automaton_file, " %d", &numero_transicoes); 
    int deve_estar_vazia; fscanf(automaton_file, " %d", &deve_estar_vazia); 

    if (numero_estados <= 0) {
        printf("O numero de estados nao pode ser inferior a 1\n"); 
        return 0; 
    } 
    else if (numero_estados >= MAX) {
        printf("O numero de estados nao pode ser superior a %d\n", MAX-1); 
        return 0; 
    } 

    if (numero_transicoes < 0) {
        printf("O numero de transicoes nao pode ser negativo\n"); 
        return 0; 
    } 

    for (int i = 0; i < numero_transicoes; i++) {
        int u, v; // u and v indexed by 0
        char c, us, pl, sa; 
        fscanf(automaton_file, " %d %d %c %c %c %c", &u, &v, &c, &us, &pl, &sa); 

        arestas[i][0] = u; 
        arestas[i][1] = v; 
        arestas_extra[i][0] = c; 
        arestas_extra[i][1] = us; 
        arestas_extra[i][2] = pl; 

        if (u < 0 || u >= MAX || v < 0 || v >= MAX) {
            printf("Verifique que as transicoes estao realmente sendo feitas entre estados no intervalo [0, %d)\n", MAX); 
            return 0; 
        } 

        if (c < 0 || us < 0 || pl < 0) {
            printf("Caractere de transicao nao reconhecido\n"); 
            return 0; 
        } 

        transicao new_transition; 
        new_transition.v = v; 
        new_transition.caractere = c; 
        new_transition.desempilha = us; 
        new_transition.empilha = pl; 
        new_transition.coloca_saida = sa; 

        cvector_push_back(tran[u], new_transition); 
    } 

    for (int i = 0; i < numero_estados; i++) {
        fscanf(automaton_file, " %d", &final[i]); 
    } 
    fclose(automaton_file); 

    // taking models' strings from "car_models" (they are in order) 
    FILE* inputs_file = fopen("./files/car_inputs", "r"); 
    char inputs[8][mm]; 
    for (int i = 0; i < 8; i++) fscanf(inputs_file, "%s", inputs[i]); 
    fclose(inputs_file); 

    get_input(inputs); 
    linha();
    printf("\nString de entrada: %s\n", entrada); 

    int q0 = 0; 

    cvector_push_back(stack, '?'); 
    processa(q0, 0, deve_estar_vazia); 

    // pegando tempo que cada um dos carros e erros demoram
    int duration_cars[MAX]; 
    FILE* duration_cars_file = fopen("./files/duration_cars", "r"); 
    for (int i = 0; i < 8; i++) fscanf(duration_cars_file, " %d", &duration_cars[i]); 
    fclose(duration_cars_file); 

    int duration_errors[MAX]; 
    FILE* duration_errors_file = fopen("./files/duration_errors", "r"); 
    for (int i = 0; i < 10; i++) fscanf(duration_errors_file, " %d", &duration_errors[i]); 
    fclose(duration_errors_file); 

    // pegando nomes de cada um dos passos
    char buffer[mm]; 
    char* steps[MAX]; 
    FILE* steps_file = fopen("./files/steps", "r"); 
    for (int i = 0; i < 10; i++) {
        fscanf(steps_file, " %s", buffer); 
        int tam = strlen(buffer); 
        steps[i] = malloc(sizeof(char)*(tam+1)); 
        for (int j = 0; j <= tam; j++) steps[i][j] = buffer[j]; 
    } 

    // printando saida
    printf("Saida: "); 
    for (int i = 0; i < cvector_size(output); i++) printf("%c", output[i]); 
    printf("\n");
    linha();
    
    printf("\nLOGS: \n\n"); 

    // fazendo o log 
    get_log(output, duration_cars, duration_errors, steps);
    linha();
    printf("\n"); 

    return 0; 
} 

char* conv_min_to_time(int min) {
    int hours = min / 60; // Calcula as horas
    int minutes = min % 60; // Calcula os minutos restantes
    char* time_str = malloc(sizeof(char) * 6); // Aloca memória para a string HH:MM\0

    if (time_str == NULL) {
        return NULL; // Verifica se a alocação de memória falhou
    }

    sprintf(time_str, "%02d:%02d", hours, minutes); // Formata a string
    return time_str; // Retorna a string formatada
}

int processa(int u, int pos, int deve_estar_vazia) {
    if (final[u]) return (deve_estar_vazia ? (cvector_size(stack) == 0) : 1); 

    int R = rand()%30000; 
    if (u >= 0 && u < 10 && (double)R/30000 < 0.05) {
        //printf("erro\n"); 
        cvector_push_back(stack, (char)(u + 'A')); 
        return processa(u, pos, deve_estar_vazia); // erro nao avanca no processamento da entrada
    } 

    for (int i = 0; i < cvector_size(tran[u]); i++) {
        int emp = 0; 
        int v = tran[u][i].v; 
        char desempilha = tran[u][i].desempilha; 
        char empilha = tran[u][i].empilha; 
        char caractere = tran[u][i].caractere; 
        char coloca_saida = tran[u][i].coloca_saida; 

        if (caractere == entrada[pos]) {
            if (desempilha != '$') {
                if (cvector_size(stack) > 0 && stack[cvector_size(stack)-1] == desempilha) {
                    cvector_pop_back(stack); 
                }
                else continue; 
            } 
            if (empilha != '$') {
                cvector_push_back(stack, empilha); 
                emp = 1;
            } 
            if (coloca_saida != '$') {
                cvector_push_back(output, coloca_saida); 
            } 
            return processa(v, pos + 1, deve_estar_vazia); 
        } 
        else if (caractere == '$') {
            if (desempilha != '$') {
                if (cvector_size(stack) > 0 && stack[cvector_size(stack)-1] == desempilha) {
                    cvector_pop_back(stack); 
                } 
                else continue; 
            } 
            if (empilha != '$') {
                cvector_push_back(stack, empilha); 
            } 
            if (coloca_saida != '$') {
                cvector_push_back(output, coloca_saida); 
            } 
            return processa(v, pos, deve_estar_vazia); 
        } 
    } 
    return 0; 
} 

void linha() {
    printf("\n============================================================================================\n");
}

void menu() {
    linha();
    printf("=                                        MARZO CARS                                        =");
    linha();
    printf("\n");   
    
    // pegando as especificacoes de cada modelo
    FILE* models_file = fopen("./files/car_models", "r");
    for (int j = 0; j < 8; j++) {
        char buffer[MAX];
        char* models[9];

        for (int i = 0; i < 6; i++) {
            fscanf(models_file, " %s", buffer);
            int tam = strlen(buffer);
            models[i] = malloc(sizeof(char) * (tam + 1));
            for (int k = 0; k <= tam; k++) {
                models[i][k] = buffer[k];
            }
        }

        printf("[Modelo %d] ", j + 1);

        for (int i = 0; i < 6; i++) {
            if(i == 3) printf("%-20s ", models[i]);
            else printf("%-12s ", models[i]);
            free(models[i]); 
        }

        printf("\n");
    }

    fclose(models_file);
    linha();   
    printf("\nEscolha quantos de cada modelo deseja fazer no dia de hoje: \n\n");
}

void get_input(char models[8][mm]) {
    menu();
     
    for (int i = 1; i <= 8; i++) {
        printf("Modelo %d: ", i); 
        int n; scanf(" %d", &n); 
        for (int j = 0; j < n; j++) {
            for (int h = 0; h < strlen(models[i-1]); h++) {
                entrada[tamanho_entrada++] = models[i-1][h] - 1; 
            } 
        } 
    } 

    entrada[tamanho_entrada] = '\0'; 
} 

void get_log(cvector_vector_type(char) results, int times_cars[MAX], int times_errors[MAX], char* steps[MAX]) {
    int cur = 300; 
    int ok = 0;

    int max_time = 400; 

    for (int i = 0; i < cvector_size(results); i++) {
        if (results[i] >= '0' && results[i] <= '7') { // type of car made 
            if(!ok) {
                printf("%s - Fabricacao do [Modelo %d] INICIADA\n", conv_min_to_time(cur), results[i]-'0'+1);
                ok = 1;
            }
            cur += times_cars[results[i]-'0']; 

            if (cur > max_time) {
                printf("Atingiu o tempo maximo!\n"); 
                return; 
            } 

            printf("%s - Fabricacao do [Modelo %d] CONCLUIDA\n", conv_min_to_time(cur), results[i]-'0'+1); 

            char cc = '*'; 
            for (int j = i+1; j < cvector_size(results); j++) {
                if (results[j] >= '0' && results[j] <= '7') {
                    cc = results[j]; 
                    break; 
                } 
            } 

            if (cc != '*') printf("%s - Fabricacao do [Modelo %d] INICIADA\n", conv_min_to_time(cur), cc-'0'+1); 
        } 
        else if (results[i] >= 'A' && results[i] <= 'Z') { // error
            cur += times_errors[results[i]-'A']; 
            if (cur > max_time) {
                printf("Atingiu o tempo maximo!\n"); 
                return; 
            } 
            if(!ok) {
                printf("%s - Fabricacao do [Modelo %d] INICIADA\n", conv_min_to_time(cur), results[i]-'A'+1);
                ok = 1;
            }
            printf("%s - ERRO na etapa %s!\n", conv_min_to_time(cur), steps[results[i]-'A']); 
        } 
    } 
} 
