#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#define MAX 505

char entrada[MAX]; 
int vai[MAX][MAX][256], final[MAX]; 
char desempilha[MAX][MAX][256], empilha[MAX][MAX][256], coloca_saida[MAX][MAX][256]; 
int arestas[MAX][2]; 
char arestas_extra[MAX][4]; 
int tamanho_entrada, numero_estados, numero_transicoes; 
int contador = 1; 

char stack[MAX]; 
int tamanho_pilha = 0; 

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
            if (final[i]) fprintf(arquivo, "%d [shape=doublecircle, style=filled, color=red, fillcolor=\"#F1ADA3\", penwidth=2];\n", i); 
            else fprintf(arquivo, "%d [shape=circle, style=filled, color=red, fillcolor=\"#F1ADA3\", penwidth=2];\n", i); 
        }
        else {
            if (final[i]) fprintf(arquivo, "%d [shape=doublecircle];\n", i); 
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

    char label_pilha[MAX]; 
    sprintf(label_pilha, "Pilha: %s\n", stack); 
    //printf("%s\n", label_pilha); 

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

int processa(int u, int i, int deve_estar_vazia) {
    // printf("u: %d | i: %d | entrada[i]: %d\n", u, i, entrada[i]); 
    grafo(u, i); 
    if (i == tamanho_entrada) {
        int ok = final[u] && (deve_estar_vazia ? (tamanho_pilha == 0) : 1); 
        return ok; 
    } 
    int aceita = 0; 
    for (int v = 0; v < numero_estados; v++) {
        // printf("v: %d | vai[u][v][entrada[i]]: %d\n", v, vai[u][v][entrada[i]]); 
        if (vai[u][v][entrada[i]]) {
            if (desempilha[u][v][entrada[i]] != '$') {
                if (tamanho_pilha && stack[tamanho_pilha-1] == desempilha[u][v][entrada[i]]) tamanho_pilha--; 
                else continue; 
            } 
            if (empilha[u][v][entrada[i]] != '$') {
                stack[tamanho_pilha++] = empilha[u][v][entrada[i]]; 
            } 
            // printf("i: %d | entrada[i]: %d | entrada[i] != '$': %d\n", i, entrada[i], entrada[i] != '$'); 
            aceita |= processa(v, i + 1, deve_estar_vazia); 
        } 
    } 
    tamanho_pilha--; 
    return aceita; 
} 

void grafo_svg() {
    for(int i = 1; i < contador; i++) {
        char comando[40];
        sprintf(comando, "dot -Tsvg %d.dot > frame%d.svg", i, i);
        system(comando);
    }
}

int main() {
    for (int i = 0; i < MAX; i++) for (int j = 0; j < MAX; j++) for (int c = 0; c < 256; c++) {
        vai[i][j][c] = 0; 
        desempilha[i][j][c] = empilha[i][j][c] = '$'; 
    } 

    scanf(" %d", &numero_estados);
    scanf(" %d", &numero_transicoes); 
    int deve_estar_vazia; scanf(" %d", &deve_estar_vazia); 

    for (int i = 0; i < numero_transicoes; i++) {
        int u, v; // u and v indexed by 0
        char c, us, pl; 
        scanf(" %d %d %c %c %c", &u, &v, &c, &us, &pl); 

        arestas[i][0] = u; 
        arestas[i][1] = v; 
        arestas_extra[i][0] = c; 
        arestas_extra[i][1] = us; 
        arestas_extra[i][2] = pl; 

        vai[u][v][c] = 1; 
        desempilha[u][v][c] = us; 
        empilha[u][v][c] = pl; 
    } 

    for (int i = 0; i < numero_estados; i++) {
        vai[i][i]['$'] = 1; 
        desempilha[i][i]['$'] = '$'; 
        empilha[i][i]['$'] = '$'; 
    } 


    for (int i = 0; i < numero_estados; i++) {
        scanf(" %d", &final[i]); // binary string with 1 on the i-th position telling that state i is final
    } 

    scanf(" %s", entrada); 
    tamanho_entrada = strlen(entrada); 
    printf("String de entrada: %s\n", entrada); 
    grafo(-1, -1); 
    
    int q0 = 0; 
    printf((processa(q0, 0, deve_estar_vazia) ? "Aceita\n" : "Nao Aceita\n")); 
    
    grafo_svg();    
} 
