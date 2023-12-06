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
            } 
            else fprintf(arquivo, "%d [shape=circle, style=filled, color=red, fillcolor=\"#F1ADA3\", penwidth=2];\n", i); 
        }
        else {
            if (final[i]) {
                fprintf(arquivo, "%d [shape=doublecircle];\n", i); 
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

    iniciar_grafo(arquivo); 
    for (int i = 0; i < numero_transicoes; i++) {
        adicionar_aresta(arquivo, arestas[i][0], arestas[i][1], arestas_extra[i][0], arestas_extra[i][1], arestas_extra[i][2]); 
    } 

    char label_pilha[MAX+100]; 

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

void grafo_svg() {
    for(int i = 1; i < contador; i++) {
        char comando[40];
        sprintf(comando, "dot -Tsvg %d.dot > frame%d.svg", i, i);
        system(comando);
    }
}