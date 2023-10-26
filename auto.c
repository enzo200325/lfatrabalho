#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#define MAX 505

// cuidar com string vazia depois

char entrada[MAX]; 
int vai[MAX][MAX][256], N, final[MAX]; 
char desempilha[MAX][MAX][256], empilha[MAX][MAX][256]; 

char stack[MAX]; 
int tamanho_pilha = 0; // current size of stack

int processa(int u, int i, int deve_estar_vazia) {
    printf("u: %d, i: %d, entrada[i]: %c\n" , u, i, entrada[i]); 
    if (entrada[i] == '?') {
        printf("u: %d\n", u); 
        return final[u] && (deve_estar_vazia ? (tamanho_pilha == 0) : 1); 
    } 
    int aceita = 0; 
    for (int v = 0; v < N; v++) {
        if (vai[u][v][entrada[i]]) {
            if (desempilha[u][v][entrada[i]] != '$') {
                if (tamanho_pilha && stack[tamanho_pilha-1] == desempilha[u][v][entrada[i]]) tamanho_pilha--; 
                else continue; 
            } 
            if (empilha[u][v][entrada[i]] != '$') stack[tamanho_pilha++] = empilha[u][v][entrada[i]]; 
            aceita |= processa(v, i+1, deve_estar_vazia); 
        } 
    } 
    return aceita; 
} 

int main() {
    for (int i = 0; i < MAX; i++) for (int j = 0; j < MAX; j++) for (int c = 0; c < 256; c++) {
        vai[i][j][c] = 0; 
        desempilha[i][j][c] = empilha[i][j][c] = '$'; 
    } 

    int m; scanf("%d", &m); // number of transitions
    scanf(" %d", &N); // number of states
    printf("m: %d | N: %d\n", N, m); 
    for (int i = 0; i < m; i++) {
        int u, v; // u and v indexed by 0
        char c, us, pl; 
        scanf(" %d %d %c %c %c", &u, &v, &c, &us, &pl); 
        printf(" %d %d %c %c %c\n", u, v, c, us, pl); 
        vai[u][v][c] = 1; 
        desempilha[u][v][c] = us; 
        empilha[u][v][c] = pl; 
    } 

    for (int i = 0; i < N; i++) {
        scanf(" %d", &final[i]); // binary string with 1 on the i-th position telling that state i is final
    } 

    scanf("%s", entrada); 
    int n = strlen(entrada); 
    printf("%s\n", entrada); 
    entrada[n] = '?'; 
    int q0 = 0; 
    printf((processa(q0, 0, 0) ? "Aceita\n" : "Nao Aceita\n")); 
} 
     
