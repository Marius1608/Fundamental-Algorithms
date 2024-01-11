/*

 Pantea Marius Nicusor
 get_neighbors-complexitate timp O(1),4 directii posibile
              -complexitate spatiala O(1)

 bfs-complexitate timp O(V+E),toate nodurile si muchiile pot fi explorate
    -complexitate spatiala O(V),coada poate contine toate nodurile

 shortest_path-complexitate timp O(V+E),se bazeaza pe bfs
              -complexitate spatiala O(V),un vector pentru a stoca drumul

 generateRandomEdges-complexitate temporala O(E)
                    -complexitate spatiala O(v^2)

 grafic-cand variem nodurile,grafic liniar
       -cand variem muchiile,grafic liniar
 */


#include <stdlib.h>
#include <string.h>
#include "bfs.h"

int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    int count = 0;
    int linie[] = {-1, 1, 0, 0};
    int coloana[] = {0, 0, -1, 1};
    for (int i = 0; i < 4; ++i)
    {
        int linie_verif= p.row + linie[i];
        int coloana_verif = p.col + coloana[i];
        if (linie_verif >= 0 && linie_verif < grid->rows && coloana_verif >= 0 && coloana_verif < grid->cols)
            if(grid->mat[linie_verif][coloana_verif] == 0) {
                neighb[count].row = linie_verif;
                neighb[count].col = coloana_verif;
                count++;
            }
    }
    return count;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{

    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node));
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}
void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

struct QNode {
    Node* key; //structura pentru nod care are o cheie si un pointer la next
    QNode* next;
};
struct Queue{
    QNode* first; //o coada care are 2 pointeri de tip nod
    QNode* last; //unul de first si last
};
void enqueue(Queue* q, Node* key, Operation *op) {
    QNode* curent = (QNode*)malloc(sizeof(QNode));
    curent->key = key;
    curent->next = NULL;
    if (q->last == NULL) {
        q->first = curent;
        q->last = curent;
        if (op != NULL) op->count(2);
    } else {
        q->last->next = curent;
        q->last = curent;
        if (op != NULL) op->count(2);
    }
}
Node* dequeue(Queue* q, Operation *op) {
    if (q->first == NULL) {
        return nullptr;
    }
    Node* curent_key = q->first->key;
    QNode* curent = q->first;
    q->first = q->first->next;
    free(curent);
    if (q->first == NULL) {
        q->last = NULL;
        if (op != NULL) op->count();
    } else {
        if (op != NULL) op->count();
    }
    return curent_key;
}
void bfs(Graph *graph, Node *s, Operation *op) {
    for (int i = 0; i < graph->nrNodes; i++) {
        if (op != NULL) op->count(3);
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = -1;
        graph->v[i]->parent = NULL;
    }
    if (op != NULL) op->count(2);
    s->color = COLOR_GRAY;
    s->dist = 0;
    Queue queue;
    queue.first = NULL;
    queue.last = NULL;
    enqueue(&queue, s, op);
    while (queue.first != NULL) {
        Node *u = dequeue(&queue, op);
        for (int i = 0; i < u->adjSize; ++i) {
            Node *v = u->adj[i];
            if (op != NULL) op->count();
            if (v->color == COLOR_WHITE) {
                if (op != NULL) op->count(3);
                v->color = COLOR_GRAY;
                v->dist = u->dist + 1;
                v->parent = u;
                enqueue(&queue, v, op);
            }
        }
        u->color = COLOR_BLACK;
    }
}

void pretty(int p[], int n, int parent, int l, Point repr[]) {
    for (int i = 0; i < n; i++) {
        if (p[i] == parent) {
            printf("%*c(%d, %d)\n", l, ' ', repr[i].row, repr[i].col);
            pretty(p, n, i , l + 5, repr);
        }
    }
}
void print_bfs_tree(Graph *graph) {
    int n = 0;
    int *p = NULL;
    Point *repr = NULL;
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i = 0; i < graph->nrNodes; ++i) {
        if(graph->v[i]->color == COLOR_BLACK) {
            transf[i] = n;
            ++n;
        } else {
            transf[i] = -1;
        }
    }

    if(n == 0) {
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Point));
    for(int i = 0; i < graph->nrNodes && !err; ++i) {
        if(graph->v[i]->color == COLOR_BLACK) {
            if(transf[i] < 0 || transf[i] >= n) {
                err = 1;
            } else {
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL) {
                    p[transf[i]] = -1;
                } else {
                    err = 1;
                    for(int j = 0; j < graph->nrNodes; ++j) {
                        if(graph->v[i]->parent == graph->v[j]) {
                            if(transf[j] >= 0 && transf[j] < n) {
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

    free(transf);
    transf = NULL;

    if(!err) {
        pretty(p, n, -1, 0, repr); ///completare
    }

    if(p != NULL) {
        free(p);
        p = NULL;
    }
    if(repr != NULL) {
        free(repr);
        repr = NULL;
    }
}


void generateRandomEdges(Graph *g, int V, int E) {
    srand(time(NULL));

    ///V-nr de noduri,E-nr de muchii
    ///matrice de adiacenta pentru a nu avea muchii care se repeta
    int **Matrice= (int**)malloc(V*sizeof(int*));
    for (int i = 0; i < V; ++i) {
        Matrice[i] = (int*)calloc(V, sizeof(int));
    }
    ///am legat toate nodurile la nodul 0 ,ca sa fie sigur conex
    for (int i = 1; i < V; ++i) {
        if (Matrice[0][i]==0)
        {
            g->v[i]->adj = (Node**)realloc(g->v[i]->adj,(g->v[i]->adjSize + 1) * sizeof(Node*));
            g->v[i]->adj[g->v[i]->adjSize++] = g->v[0];
            g->v[0]->adj = (Node**)realloc(g->v[0]->adj,(g->v[0]->adjSize + 1) * sizeof(Node*));
            g->v[0]->adj[g->v[0]->adjSize++] = g->v[i];
            Matrice[0][i] = Matrice[i][0] = 1;
        }
    }
    ///am adaugat restul muchiilor random
    while (E-V+1 > 0) ///verificam daca mai sunt muchii de aduagat dupa ce am adaugat deja nr de noduri-1
    {
        int i = rand() % V;
        int j = rand() % V;
        if (i != j && Matrice[i][j]==0)  ///vedem daca muchia exista sau nu
        {
            g->v[i]->adj = (Node**)realloc(g->v[i]->adj, (g->v[i]->adjSize + 1) * sizeof(Node*));
            g->v[j]->adj = (Node**)realloc(g->v[j]->adj, (g->v[j]->adjSize + 1) * sizeof(Node*));
            g->v[i]->adj[g->v[i]->adjSize++] = g->v[j];
            g->v[j]->adj[g->v[j]->adjSize++] = g->v[i];
            Matrice[i][j] = 1;
            Matrice[j][i] = 1;
            E--; ///scadem nr de muchi
        }
    }

    for (int i = 0; i < V; ++i) {
        free(Matrice[i]);
    }
    free(Matrice);
}
void performance() {
    int n, i;
    Profiler p("bfs");

    // Test for different numbers of edges
    for (n = 1000; n <= 4500; n += 100) {
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        generateRandomEdges(&graph, 100,n);
        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // Test for different numbers of vertices
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }

        generateRandomEdges(&graph, n,4500);
        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }
    p.showReport();
}

int shortest_path(Graph* graph, Node* start, Node* end, Node* path[]) {

    int nr=0; ///numarul de noduri din drum
    bfs(graph, start, NULL);
    /// daca nodul final end a fost vizitat in BFS.Daca nu,nu exista un drum de la start la end
    if (end->color != COLOR_BLACK) {
        return -1;
    }

    ///construieste un drum invers de la sfarsit pe legatura de parinte
    while (end != NULL) {
        path[nr] = end;
        nr++;
        end = end->parent;
    }

    ///inverseaza pentru a obtine ordinea corecta
    for (int i = 0; i < nr / 2; ++i) {
        Node *temp = path[i];
        path[i] = path[nr - i - 1];
        path[nr - i - 1] = temp;
    }
    return nr;
}
