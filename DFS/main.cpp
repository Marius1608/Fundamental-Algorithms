/*
 *
 Pantea Marius Nicusor
 -Dfs:
    Timp: O(V^2)-Pentru fiecare nod,se apeleaza Dfs_visit
    Spatiu: O(V)-Stiva pentru recursivitate
 -Dfs_visit:
    Timp: O(V)-Toate nodurile (V) ale grafului sunt parcurse
    Spatiu: O(V)-Stiva pentru recursivitate
 -Sortare topologica:
    Sunt la fel ca la dfs si dfs_vizit,aduagand doar cateva chestii in plus
 -Tarjan(Graph *graph):
    Timp: O(V+E)-Parcurgem toate nodurile și muchiile ale grafului.
    Spatiu: O(V)-Stiva pentru recursivitate

 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Profiler.h"
Profiler p("dfs");

#define WHITE 0
#define GRAY 1
#define BLACK 2

struct Node{
    int key;
    Node *next;
};  ///structura pentru lista

struct V {
    int color,d,f,parent,lowlink;
    Node* adj;

}; ///structura pentru un varf din graf

struct Graph {
    V* varfuri; ///vector de varfuri
    int Nr; ///nr varfurilor din graf
};

void Adaugare(Graph* graph, int u, int v) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = v;
    node->next = graph->varfuri[u].adj;
    graph->varfuri[u].adj = node;
}
void DfsVisit(Graph* graph, int u, int* time,Operation operatii) {

    operatii.count();
    graph->varfuri[u].color = GRAY; ///varful doar ce a fost descoperit
    *time = *time + 1;
    graph->varfuri[u].d = *time; /// se seteaza timpul de incepul

    Node* n = graph->varfuri[u].adj;///pointer la primul vecin al varfului u in l.a
    while (n != NULL) {
        operatii.count(1);
        int v = n->key;
        if (graph->varfuri[v].color == WHITE) {
            operatii.count();
            graph->varfuri[v].parent = u;///se seteaza parintele varfului v la u,ca a fost descoperit din u
            DfsVisit(graph, v, time, operatii);
        }
        n = n->next; ///urmatorul vecin din lista de adiacenta
    }

    operatii.count();
    graph->varfuri[u].color = BLACK; ///varful este gata si nu se mai revine la el
    *time = *time + 1;
    graph->varfuri[u].f = *time; ///se seteaza timpul de finalizare
}
void Dfs(Graph* graph,Operation operatii) {
    int time = 0;
    for (int u = 0; u < graph->Nr; ++u) {
        operatii.count();
        ///se steaza toate varfurile ca fiind nedescoperite
        graph->varfuri[u].color = WHITE;
        ///se initializeaza parintele fiecarui varf cu -1
        graph->varfuri[u].parent = -1;
    }
    for (int u = 0; u < graph->Nr; ++u) {
        operatii.count();
        if (graph->varfuri[u].color == WHITE) {
            DfsVisit(graph, u, &time,operatii);
        }
    }
}

void AfisareCuLista(Graph* graph) {
    printf("Graful prin lista de adiacenta :\n");
    for (int i = 0; i < graph->Nr; ++i) {
        printf("%d: ", i + 1);
        Node* node = graph->varfuri[i].adj;
        while (node != NULL) {
            printf("%d ", node->key + 1);
            node = node->next;
        }
        printf("\n");
    }
}
void Pretty(int p[], int dim, int parent, int l) {

    for (int i = 0; i < dim; i++) {
        if (p[i] == parent) {
            printf("%*c%d\n",l, ' ', i + 1);
            Pretty(p, dim, i + 1, l+4);
        }
    }
}
void AfisareDupaDfs(Graph* graph) {

    printf("\nArborele rez\n");
    int* vectorDeTati = (int*)malloc(graph->Nr * sizeof(int));
    for (int i = 0; i < graph->Nr; ++i) {
        vectorDeTati[i] = graph->varfuri[i].parent + 1;
    }
    Pretty(vectorDeTati, graph->Nr, 0, 0);
    free(vectorDeTati);

}
void EliberareMemorie(Graph* graph) {
    for (int i = 0; i < graph->Nr; ++i) {
        Node* node = graph->varfuri[i].adj;
        while (node != NULL) {
            Node* next = node->next;
            free(node);
            node = next;
        }
    }
    free(graph->varfuri);
}

void DfsTopologic(Graph* graph, int u, int* time, int* ord, int* k) {
    ///la fel ca la DfsVisit
    ///un vector unde se tin in ordine topologica varfurile
    ///si un contor pentru pozitie
    graph->varfuri[u].color = GRAY;
    *time = *time + 1;
    graph->varfuri[u].d = *time;

    Node* n = graph->varfuri[u].adj;
    while (n != NULL) {
        int v = n->key;
        if (graph->varfuri[v].color == WHITE) {
            graph->varfuri[v].parent = u;
            DfsTopologic(graph, v, time, ord, k);
        }
        n = n->next;
    }

    graph->varfuri[u].color = BLACK;
    *time = *time + 1;
    graph->varfuri[u].f = *time;
    ord[*k] = u; ///adaugam varful in ordine topologica
    (*k)++;
}
void DfsT(Graph* graph) {

    ///initializare vector unde vor fi tinute varfurile pentru sortarea topologica
    int* ord = (int*)malloc(graph->Nr * sizeof(int));
    int k = 0;
    int time = 0;

    for (int u = 0; u < graph->Nr; ++u) {
        graph->varfuri[u].color = WHITE;
        graph->varfuri[u].parent = -1;
    }

    for (int u = 0; u < graph->Nr; ++u) {
        if (graph->varfuri[u].color == WHITE) {
            DfsTopologic(graph, u, &time, ord, &k);
        }
    }

    ///afisare sortari topologice
    printf("\nDupa sortarea topologica:\n");
    for (int i = 0; i < graph->Nr; ++i) {
        printf("%d ", ord[i] + 1);
    }
    free(ord);
}

void TarjanDfs(Graph* graph, int u, int* time, int* stiva, int* k) {

    graph->varfuri[u].color = GRAY;
    *time = *time + 1;
    graph->varfuri[u].d = *time;
    graph->varfuri[u].lowlink = *time;

    stiva[*k] = u; ///se adauga varful in stiva
    (*k)++;

    Node* n = graph->varfuri[u].adj;
    while (n != NULL) {
        int v = n->key;
        if (graph->varfuri[v].color == WHITE) {
            graph->varfuri[v].parent = u;
            TarjanDfs(graph, v, time, stiva, k);

            ///se actualizeaza lowlink-ul varfului u cu minimul dintre lowlink-ul sau și al vecinului v
            if (graph->varfuri[v].lowlink < graph->varfuri[u].lowlink) {
                graph->varfuri[u].lowlink = graph->varfuri[v].lowlink;
            }
        } else if (graph->varfuri[v].d < graph->varfuri[u].lowlink && graph->varfuri[v].color == GRAY) {
            /// dacă vecinul v este încă în stiva și are un timp de descoperire mai mic decat lowlink-ul varfului u
            /// se actualizează lowlink-ul varfului u cu timpul de descoperire al vecinului v
            graph->varfuri[u].lowlink = graph->varfuri[v].d;
        }
        n = n->next;
    }

    /// verifică daca varful u este radacina unei componente puternic conexe și afișează acea componentă.
    if (graph->varfuri[u].lowlink == graph->varfuri[u].d) {
        int x;
        do {
            (*k)--;
            x = stiva[*k];
            printf("%d ", x + 1); ///afisarea nodurile componentei conex
        } while (x != u);
        printf("\n");
    }

    graph->varfuri[u].color = BLACK;
    *time = *time + 1;
    graph->varfuri[u].f = *time;
}
void Tarjan(Graph *graph) {
    int time = 0;
    int *stiva = (int *)malloc(graph->Nr * sizeof(int));
    int k = 0;
    for (int u = 0; u < graph->Nr; ++u) {
        graph->varfuri[u].color = WHITE;
        graph->varfuri[u].parent = -1;
    }
    for (int u = 0; u < graph->Nr; ++u) {
        if (graph->varfuri[u].color == WHITE) {
            TarjanDfs(graph, u, &time, stiva, &k);
        }
    }
    free(stiva);
}

void Demo() {

    Operation operatii = p.createOperation("", 0);
    Graph graph;
    graph.Nr = 7;
    graph.varfuri = (V*)malloc(graph.Nr * sizeof(V));
    for (int i = 0; i < graph.Nr; ++i) {
        graph.varfuri[i].adj = NULL;
    }

    Adaugare(&graph, 0, 1);
    Adaugare(&graph, 0, 2);
    Adaugare(&graph, 1, 3);
    Adaugare(&graph, 2, 4);
    Adaugare(&graph, 3, 5);
    Adaugare(&graph, 4, 6);

    AfisareCuLista(&graph);
    printf("\n");
    Dfs(&graph, operatii);
    AfisareDupaDfs(&graph);
    printf("\n");
    printf("Dupa Tarjan\n");
    Tarjan(&graph);
    DfsT(&graph);
    EliberareMemorie(&graph);
}

void Generare(Graph *graph, int V, int E) {

    srand(time(NULL));
    int **Matrice = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; ++i) {
        Matrice[i] = (int *)calloc(V, sizeof(int));
    }

    while (E > 0) {
        int u = rand() % V;
        int v = rand() % V;
        if (u != v && Matrice[u][v] == 0) {
            Adaugare(graph, u, v);
            Matrice[u][v] = 1;
            E--;
        }
    }

    for (int i = 0; i < V; ++i) {
        free(Matrice[i]);
    }
    free(Matrice);
}
void Perf() {

    for (int n = 1000; n <= 4500; n += 100) {
        Operation operatii = p.createOperation("dfs-edges", n);
        Graph graph;
        graph.Nr = 100;
        graph.varfuri = (V*)malloc(graph.Nr * sizeof(V));
        for (int i = 0; i < graph.Nr; ++i) {
            graph.varfuri[i].adj = NULL;
        }

        Generare(&graph, 100, n);
        Dfs(&graph, operatii);
        EliberareMemorie(&graph);
    }

    for (int n = 100; n <= 200; n += 10) {
        Operation operatii = p.createOperation("dfs-vertices", n);
        Graph graph;
        graph.Nr = n;
        graph.varfuri = (V*)malloc(graph.Nr * sizeof(V));
        for (int i = 0; i < graph.Nr; ++i) {
            graph.varfuri[i].adj = NULL;
        }
        Generare(&graph, n, 4500);
        Dfs(&graph, operatii);
        EliberareMemorie(&graph);
    }
    p.showReport();
}

int main() {
    Demo();
    //Perf();
    return 0;
}