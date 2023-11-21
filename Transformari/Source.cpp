/*
    Pantea Marius Nicusor

    Structuri de date folosite: - 'Multi' avem cheia si un vector de noduri copil
                                 - 'BNode' avem cheia si legatura catre primul copil si frate

    Operatiile de trasnformare se realizeaza in timp liniar
    R1_R2: Complexitatea totalã: O(dim + numãrul de elemente care nu sunt -1 în p).
    R2_R3: Complexitatea totalã: O(numãr total de noduri în arborele R2)

    prettyR1: Parcurgerea arborelui: Functia parcurge nodurile arborelui R1 o singura data, si pentru fiecare nod,si afiseaza .
              Complexitatea : O(numar total de noduri în arborele R1).
    
    prettyR2: Parcurgerea arborelui: Functia parcurge nodurile arborelui R2 o singura data, si pentru fiecare nod,si afiseaza.
              Complexitatea : O(numar total de noduri în arborele R2).
    
    prettyR3: Parcurgerea arborelui: Functia parcurge nodurile arborelui binar o singurã data, si pentru fiecare nod,si afiseaza .
              Complexitatea : O(numar total de noduri în arborele R3).  
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Multi {
    int key;
    int nrChild;
    Multi** children;

};

struct BNode {
    int key;
    struct BNode* child;
    struct BNode* brother;
};


Multi* R1_R2(int p[], int dim, int R) {
    
    // Alocare si initializare pentru fiercare nod
    Multi** n = (Multi**)malloc(dim * sizeof(Multi));
    for (int i = 0; i < dim; i++) {
        n[i] = (Multi*)malloc(sizeof(Multi));
        n[i]->key = i + 1; //Deoarece avem indexare de 0
        n[i]->nrChild = 0;
        n[i]->children = NULL;
    }

    // Calcularea numarului de copii pentru fiecare nod si alocarea vectorului de copii
    for (int i = 0; i < dim; i++) {
        if (p[i] != -1) {
            int k = p[i] - 1;
            n[k]->nrChild++;  // Incrementarea numarului de copii al nodului p[i] - 1
        }
    }

    // Alocarea vectorului de copii pentru fiecare nod si resetarea numarului de copii la 0
    for (int i = 0; i < dim; i++) {
        if (n[i]->nrChild > 0) {
            n[i]->children = (Multi**)malloc(n[i]->nrChild * sizeof(Multi));
            n[i]->nrChild = 0;  // Resetarea numarului de copii la 0
        }
    }

    // Adaugarea fiecarui nod în vectorul de copii al parintelui sau în ordine
    for (int i = 0; i < dim; i++) {
        if (p[i] != -1) {
            int k = p[i] - 1;
            n[k]->children[n[k]->nrChild++] = n[i];
        }
    }

    // Returnarea radacinii corespunzatoare lui R
    return n[R];
}

BNode* R2_R3(Multi* R) {
    
    //alocarea si initializarea fiecarui nod
    BNode* bN = (BNode*)malloc(sizeof(BNode));
    bN->key = R->key; //R = radacina
    bN->child = NULL;
    bN->brother = NULL;

    // Verificare dacã nodul Multi are copii
    if (R->nrChild > 0) {

        // Recursivitate pentru construirea subarborelui copil
        bN->child = R2_R3(R->children[0]);
        BNode* sN = bN->child;

        // Construirea listei de frati a nodului BNode
        for (int i = 1; i < R->nrChild; ++i) {
            sN->brother = R2_R3(R->children[i]);
            sN = sN->brother;
        }
    }

    return bN;
}


void prettyR1(int p[], int dim, int parent, int l) {

    for (int i = 0; i < dim; i++) {
        if (p[i] == parent) {
            printf("%*c%d\n",l, ' ', i + 1);
            prettyR1(p, dim, i + 1, l+4);
        }
    }
}

void prettyR2(Multi* R, int l) {

    printf("%*c%d\n", l, ' ', R->key);
    for (int i = 0; i < R->nrChild; i++) {
        prettyR2(R->children[i], l + 4);
    }
}

void prettyR3(BNode* R, int l) {

    if (R == NULL) { return; }
    else
    {
        printf("%*c%d\n", l * 4, ' ', R->key);
        prettyR3(R->child, l + 1);
        prettyR3(R->brother, l);

    }
}


int main() {

    int p[] = { 2, 7, 5, 2, 7, 7, -1, 5, 2 };
    int n = sizeof(p) / sizeof(p[0]);

    prettyR1(p, n, -1, 0);
    printf("\n");

    int R = 0;
    for (int i = 0; i < n; ++i) {
        if (p[i] == -1) {
            R = i;
            break;
        }
    }

    Multi* R2 = R1_R2(p, n, R);
    prettyR2(R2, 0);
    printf("\n");

    BNode* R3 = R2_R3(R2);
    prettyR3(R3, 0);
    printf("\n");


    free(R2);
    free(R3);
    return 0;
}