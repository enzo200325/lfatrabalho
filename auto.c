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

typedef struct transicao {
    int v; 
    char vai, empilha, desempilha, coloca_saida; 
} transicao; 
cvector_vector_type(transicao) tran[MAX]; 

void iniciar_grafo(FILE* arquivo) {
    fprintf(arquivo, "digraph {\n"); 
    fprintf(arquivo, "rankdir=LR;\n"); 
    fprintf(arquivo, "start [shape=point, width=0];\n"); 
    fprintf(arquivo, "start -> 0;\n"); 
} 
void adicionar_aresta(FILE* arquivo, int u, int v, char a, char b, char c) {
    fprintf(arquivo, "%d -> %d [label=\"%c, %c, %c\", fontsize=10];\n", u, v, a, b, c); 
} 
void terminar_grafo(FILE* arquivo) {
    fprintf(arquivo, "}\n") ;
} 
void adicionar_nodos(FILE* arquivo, int dest) {
    for (int i = 0; i < numero_estados; i++) {
        if (i == dest) {
            if (final[i]) {
                fprintf(arquivo, "%d [shape=doublecircle, style=filled, color=red, fillcolor=\"#F1ADA3\", penwidth=2];\n", i); 
                printf("i: %d\n", i); 
            } 
            else fprintf(arquivo, "%d [shape=circle, style=filled, color=red, fillcolor=\"#F1ADA3\", penwidth=2];\n", i); 
        }
        else {
            if (final[i]) {
                fprintf(arquivo, "%d [shape=doublecircle];\n", i); 
                printf("i: %d\n", i); 
            } 
            else fprintf(arquivo, "%d [shape=circle];\n", i); 
        } 
    } 
} 

void iniciar_label(FILE* arquivo) {
    fprintf(arquivo, "label = <<TABLE BORDER=\"0\" CELLBORDER=\"0\">"); 
} 
void terminar_label(FILE* arquivo) {
    fprintf(arquivo, "</TABLE>>;"); 
} 
void adicionar_espaco_label(FILE* arquivo) {
    fprintf(arquivo, "<TR><TD></TD></TR>");
} 

void adicionar_label(FILE* arquivo, char* s) {
    fprintf(arquivo, "<TR><TD>%s</TD></TR>;", s);
} 
void adicionar_label_palavra_destaque(FILE* arquivo, int posicao_destacada) {
       fprintf(arquivo, "<TR><TD>"); 
       for (int i = 0; i < tamanho_entrada; i++) {
           char c = entrada[i]; 
           if (i == posicao_destacada) fprintf(arquivo, "<SUB><FONT COLOR=\"red\">%c</FONT></SUB>", c); 
           else fprintf(arquivo, "<SUB>%c</SUB>", c); 
       } 
       fprintf(arquivo, "</TD></TR>"); 
} 

void ajustar_tables(FILE* arquivo) {
    fprintf(arquivo, "labeljust = \"r\"; labelloc = \"r\";"); 
} 

void grafo(int destaca, int destaca_letra) {
    char nome_arquivo[20]; 
    sprintf(nome_arquivo, "%d.dot", contador++); 
    FILE* arquivo = fopen(nome_arquivo, "w"); 
    // printf("numero_transicoes: %d\n", numero_transicoes); 
    iniciar_grafo(arquivo); 
    for (int i = 0; i < numero_transicoes; i++) {
        // printf("i: %d\n", i); 
        adicionar_aresta(arquivo, arestas[i][0], arestas[i][1], arestas_extra[i][0], arestas_extra[i][1], arestas_extra[i][2]); 
    } 

    char label_pilha[MAX+100]; 
    //sprintf(label_pilha, "Pilha: %s\n", stack); 

    iniciar_label(arquivo); 
    adicionar_label_palavra_destaque(arquivo, destaca_letra); 
    adicionar_espaco_label(arquivo); 
    adicionar_label(arquivo, label_pilha); 
    terminar_label(arquivo); 

    ajustar_tables(arquivo); 

    adicionar_nodos(arquivo, destaca); 
    terminar_grafo(arquivo); 
    fclose(arquivo); 
} 

int TS = 0; 
char saida[MAX*MAX]; 

cvector_vector_type(char) stack;
cvector_vector_type(char) output; 

int processa(int u, int pos, int deve_estar_vazia) {
    //grafo(u, pos); 

    printf("u: %d | pos: %d\n", u, pos); 
    if (final[u]) return (deve_estar_vazia ? (cvector_size(stack) == 0) : 1); 

    // possivel erro
    int R = rand()%30000; 
    if (u >= 0 && u < 10 && (double)R/30000 < 0.05) {
        printf("erro\n"); 
        cvector_push_back(stack, (char)(u + 'A')); 
        return processa(u, pos, deve_estar_vazia); // erro nao avanca no processamento da entrada
    } 

    for (int i = 0; i < cvector_size(tran[u]); i++) {
        int emp = 0; 
        int v = tran[u][i].v; 
        char desempilha = tran[u][i].desempilha; 
        char empilha = tran[u][i].empilha; 
        char vai = tran[u][i].vai; 
        char coloca_saida = tran[u][i].coloca_saida; 

        if (vai == entrada[pos]) {
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
        else if (vai == '$') {
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

void grafo_svg() {
    for(int i = 1; i < contador; i++) {
        char comando[40];
        sprintf(comando, "dot -Tsvg %d.dot > frame%d.svg", i, i);
        system(comando);
    }
}

void get_input(char models[8][mm]) {
    for (int i = 1; i <= 8; i++) {
        printf("Modelo %d - ()\n", i); 
    } 
    printf("Escolha quantos de cada modelo deseja fazer no dia de hoje: \n"); 
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
    int cur = 800; 
    for (int i = 0; i < cvector_size(results); i++) {
        if (results[i] >= '0' && results[i] <= '7') { // type of car made 
            cur += times_cars[results[i]-'0']; 
            printf("%d: Fabricacao de Carro Modelo %d concluida.\n", cur, results[i]-'0'+1); 

            char cc = '*'; 
            for (int j = i+1; j < cvector_size(results); j++) {
                if (results[j] >= '0' && results[j] <= '7') {
                    cc = results[j]; 
                    break; 
                } 
            } 

            if (cc != '*') printf("%d: Fabricacao de Carro Modelo %d iniciada.\n", cur, cc-'0'+1); 
        } 
        else if (results[i] >= 'A' && results[i] <= 'Z') { // error
            cur += times_errors[results[i]-'A']; 
            printf("%d: Erro na etapa %s!\n", cur, steps[results[i]-'A']); 
        } 
    } 
} 

int main() {
    srand(time(NULL)); 

    FILE* FA = fopen("autofinal", "r"); 
    fscanf(FA, " %d", &numero_estados);
    fscanf(FA, " %d", &numero_transicoes); 
    int deve_estar_vazia; fscanf(FA, " %d", &deve_estar_vazia); 

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
        fscanf(FA, " %d %d %c %c %c %c", &u, &v, &c, &us, &pl, &sa); 

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

        transicao nt; 
        nt.v = v; 
        nt.vai = c; 
        nt.desempilha = us; 
        nt.empilha = pl; 
        nt.coloca_saida = sa; 

        cvector_push_back(tran[u], nt); 
    } 

    for (int i = 0; i < numero_estados; i++) {
        fscanf(FA, " %d", &final[i]); 
        printf("%d ", final[i]); 
    } 
    printf("\n"); 
    fclose(FA); 

    // taking models' strings from "car_models" (they are in order) 
    FILE* fp = fopen("./car_models", "r"); 
    char models[8][mm]; 
    for (int i = 0; i < 8; i++) fscanf(fp, "%s", models[i]); 
    fclose(fp); 

    get_input(models); 
    printf("String de entrada: %s\n", entrada); 
    grafo(-1, -1); 

    int q0 = 0; 

    cvector_push_back(stack, '?'); 
    printf((processa(q0, 0, deve_estar_vazia) ? "Aceita\n" : "Nao Aceita\n")); 
    //grafo_svg();    

    // pegando tempo que cada um dos carros e erros demoram
    int duration_cars[MAX]; 
    FILE* FTS = fopen("./duration_cars", "r"); 
    for (int i = 0; i < 8; i++) fscanf(FTS, " %d", &duration_cars[i]); 
    fclose(FTS); 
    int duration_errors[MAX]; 
    FILE* FTE = fopen("./duration_errors", "r"); 
    for (int i = 0; i < 10; i++) fscanf(FTE, " %d", &duration_errors[i]); 
    fclose(FTE); 


    // pegando nomes de cada um dos passos
    char buffer[mm]; 
    char* steps[MAX]; 
    FILE* FS = fopen("./steps", "r"); 
    for (int i = 0; i < 10; i++) {
        fscanf(FS, " %s", buffer); 
        int tam = strlen(buffer); 
        steps[i] = malloc(sizeof(char)*(tam+1)); 
        for (int j = 0; j <= tam; j++) steps[i][j] = buffer[j]; 
    } 

    // printando saida
    printf("saida: \n"); 
    for (int i = 0; i < cvector_size(output); i++) printf("%c", output[i]); 
    printf("\n"); 

    // fazendo o log 
    get_log(output, duration_cars, duration_errors, steps); 



    return 0; 
} 
