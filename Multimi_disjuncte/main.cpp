/*

 Pantea Marius Nicusor

 Implementare
 -Operatii pe multimi disjuncte si algotimul lui Kruskal(gasirea arborelui de acoperire minim)
 -Structuri de date folosite
    .MDisjuncte
    .Mch
 -Complexitatea la algotimul lui Kruskal este O(E*log(E) unde E==nr de muchii (O(nlogn))
 -Complexitate memorie O(n)
 -Complexitate operatii -FindSet(O(logn)
                        -BuildSet(O(n)) ///pentru n noduri
                        -Union(O(logn))


 */

#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
Profiler p("Lab_9");
#define Step 100
#define Max 10000

struct MDisjuncte {
    int *parinte;
    int *rang; ///vector de inaltimi(inaltimea fiecarui element)
    int dim; ///dimensiunea multimii
};
struct Mch {
    int x; /// de unde incepe muchia
    int y; /// unde se termina muchia
    int cost; ///costul muchiei
};

void BuildSet(MDisjuncte *S, int x,Operation operatii) {
    operatii.count(2);
    S->parinte[x] = x;
    S->rang[x] = 0;
}
int FindSet( MDisjuncte *S, int x,Operation operatii) {
    /// gasirea radacini multimi la care apare elementul
    if (x != S->parinte[x]) {
        operatii.count(2);
        S->parinte[x] = FindSet(S, S->parinte[x],operatii);
    }
    return S->parinte[x];
}
void Union( MDisjuncte *S, int x, int y,Operation operatii) {
    int X = FindSet(S,x,operatii); /// parintele multimii unde se afla x
    int Y = FindSet(S,y,operatii); /// parintele multimi unde se afla x
    operatii.count();
    if (X != Y) {
        operatii.count();
        if (S->rang[X] > S->rang[Y]) {
            operatii.count();
            S->parinte[Y] = X; ///uneste multimile ,stabilind parintele lui y ca fiind x
        } else {
            operatii.count();
            S->parinte[X] = Y; ///uneste multimile ,stabilind parintele lui x ca fiind y
            operatii.count();
            if (S->rang[X] == S->rang[Y]) {
                operatii.count();
                S->rang[Y]++; ///creste inaltimea arborelui pentru Y
            }
        }
    }
}
MDisjuncte *create(int dim) {
    Operation operatii=p.createOperation("",0);
    MDisjuncte *S = (MDisjuncte *)malloc(sizeof(MDisjuncte));
    S->dim = dim;
    S->parinte = (int *)malloc(dim * sizeof(int));
    S->rang = (int *)malloc(dim * sizeof(int));
    for (int i = 0; i < dim; i++) {
        BuildSet(S, i,operatii);
    }
    return S;
}
void PrintSet(MDisjuncte *S) {
    Operation operatii=p.createOperation("",0);
    printf("M: ");
    for (int i = 0; i < S->dim; i++) {
        printf("%d ", FindSet(S, i,operatii));
    }
    printf("\n");
}
void Demo_operatii_multimi() {
    Operation operatii=p.createOperation("",0);
    int n = 10;
    MDisjuncte *S = create(n);
    printf("Multimea: ");
    PrintSet(S);
    printf("\n");
    for (int x = 0; x < 5; x++) {
        int y = rand() % 5;
        printf("Union ");
        printf("%d %d\n",x,y);
        Union(S, x, y,operatii);
        PrintSet(S);
        int z = rand() % 5;
        printf("FindSet:%d",z);
        printf( "=%d\n", z, FindSet(S, z,operatii));
        printf("\n");
    }
    free(S->parinte);
    free(S->rang);
    free(S);
}

int partition(Mch v[], int s, int r) {
    int x = v[r].cost;
    int i = s - 1;
    for (int j = s; j <= r; j++) {
        if (v[j].cost < x) {
            i++;
            int aux = v[i].cost;
            v[i].cost = v[j].cost;
            v[j].cost = aux;
        }
    }
    int rev = v[i + 1].cost;
    v[i + 1].cost = v[r].cost;
    v[r].cost = rev;
    return i + 1;
}
void Quick_sort(Mch v[], int p, int r) {
    if (p < r) {
        int q = partition(v, p, r);
        Quick_sort(v, p, q - 1);
        Quick_sort(v, q + 1, r);
    }
}
Mch *AlgoritmKruskal(Mch muchii[], int V, int E,Operation operatii){
    ///parintele <=> reprezetantul
    MDisjuncte M;
    M.dim = V;
    M.parinte = (int *)malloc(V * sizeof(int));
    M.rang = (int *)malloc(V * sizeof(int));
    Mch *Arb = (Mch *)malloc((V - 1) * sizeof(Mch)); ///multimea de muchii in arborele dr acoperire minima
    int nr=0; ///nr de muchii ale arborelui de acoperire minim
    for (int i = 0; i < V; i++) {
        BuildSet(&M, i,operatii);
    }
    Quick_sort(muchii, 0, E - 1); ///Sortarea muchiile dupa cost
    for (int i = 0; i < E; i++) {
        int rX = FindSet(&M, muchii[i].x,operatii); ///parintele la care apartine x
        int rY = FindSet(&M, muchii[i].y,operatii); ///parintele la care apartine y
        if (rX != rY) {
            Arb[nr++] = muchii[i]; ///adaugare muchie in arborele de acoperire minima
            Union(&M, muchii[i].x, muchii[i].y,operatii); ///uneste multimile la care apartine x si y
            if (nr == V - 1) {
                break;
            }
        }
    }

    free(M.parinte);
    free(M.rang);
    return Arb;
}
Mch* GrafRandom(int n) {
    Operation operatii = p.createOperation("", 0);
    Mch *muchii = (Mch *)malloc(n * 4 * sizeof(Mch));
    MDisjuncte *S = create(n);

    for (int i = 0; i < n - 1; ++i) {
        int x = rand() % n;
        int y = rand() % n;
        while (x == y) {
            y = rand() % n;
        }
        Union(S, x, y, operatii);
        muchii[i].x = x; muchii[i].y = y; muchii[i].cost = rand() % 25;
        muchii[n + i].x = y; muchii[n + i].y = x; muchii[n + i].cost = muchii[i].cost;
    }

    for (int i = n - 1; i < n * 4; ++i) {
        int x = rand() % n;
        int y = rand() % n;
        while (x == y) {
            y = rand() % n;
        }
        muchii[i].x = x; muchii[i].y = y; muchii[i].cost = rand() % 25;
    }

    free(S->parinte);
    free(S->rang);
    free(S);
    return muchii;
}
void Demo_Kruskal(int n){
    Operation operatii=p.createOperation("",0);
    Mch *muchii = GrafRandom(n);

    printf("Muchiile grafului:\n");
    for (int i = 0; i < 4*n; ++i) {
        printf("%d,%d : %d\n", muchii[i].x, muchii[i].y, muchii[i].cost);
    }
    Mch *Arb = AlgoritmKruskal(muchii, n, 4*n,operatii);
    printf("\nMuchiile arborelui de acoperire minima:\n");
    for (int i = 0; i < n-1 ; ++i) {
        printf("%d,%d : %d\n", Arb[i].x, Arb[i].y, Arb[i].cost);
    }
    free(muchii);
    free(Arb);
}
void Perf() {
    for (int n = 100; n <= Max; n += Step) {
        Operation operatii=p.createOperation("Operatii multimi disjuncte",n);
        Mch *muchii = GrafRandom(n);
        Mch *Arb = AlgoritmKruskal(muchii, n, n * 4,operatii);
        free(muchii);
        free(Arb);
    }
    p.showReport();
}

int main() {
    //Demo_operatii_multim0i();
    //Demo_Kruskal(4);
    //Perf();
    return 0;
}