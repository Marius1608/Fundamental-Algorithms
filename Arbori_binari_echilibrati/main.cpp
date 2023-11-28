/*
Os_build:
Complexitatea: O(n)
Fiecare nod este creat o singură dată, deci complexitatea este proporțională cu numărul total de noduri.

Os_select:
Complexitatea: O(log n)
Datorită proprietăților arborelui de căutare binar, adâncimea medie este logaritmică în funcție de numărul total de noduri.

Minim:
Complexitatea: O(log n)
Similar cu Os_select, adâncimea medie într-un arbore de căutare binar echilibrat este logaritmică.

Succesor:
Complexitatea: O(log n)
Similar cu Os_select,ca timp

Os_delete:
Complexitatea: O(log n)
Operațiile principale de căutare și găsire a succesorului au o complexitate logaritmică, iar ștergerea unui nod are complexitate logaritmică în funcție de adâncimea arborelui.

print și pretty:
Complexitatea: O(n)


 */

#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Profiler.h"

#define Max 10000
#define Step 100
#define Nr_tests 5
using namespace std;
Profiler p("Lab7");

struct Tree {
    int key;
    int size;
    Tree* left;
    Tree* right;
    Tree* parent;
};

Tree* Os_build(int a, int b, Tree* parent = nullptr)
{
    ///In antet se face parintele NULL,si in functie se atribuie o valoare
    ///Radacina avand legatura parinte NULL
    ///Pus asa pentru a nu mai face alta functie

    if (a > b) {
        return nullptr;
    }

    Tree* node = new Tree();
    int mijloc = (a + b) / 2;

    node->key = mijloc; ///*radcina
    node->size = b - a + 1; ///Dimensiunea subarborelui curent
    node->parent = parent; ///legatura parinte

    ///Subarborele stang si drept
    node->left = Os_build(a, mijloc - 1, node);
    node->right = Os_build(mijloc + 1, b, node);

    return node;
}
Tree* Os_select(Tree* node, int indice, Operation operatii) {

    operatii.count();
    ///Daca nodul este nul ,nu este nimic de selectat
    if (node == nullptr) {
        return nullptr;
    }

    int r;
    ///Se calculeaza r care reprezinta dimensiunea subarborelui stang+1
    operatii.count();
    if (node->left != nullptr) {
        r = node->left->size + 1;
    }
    else {
        r = 1;
    }

    ///Daca indicele se potriveste cu r ,returnam nodul
    if (indice == r) {
        return node;
    }
    else if (indice < r) {
        ///Daca e mai mic decat r cautam in subarborele stang
        return Os_select(node->left, indice, operatii);
    }
    else {
        ///Daca e mai mare decat r in subarborele drept cautam
        return Os_select(node->right, indice - r, operatii);
    }
}


Tree* Minim(Tree* node, Operation operatii) {

    ///se parcurge subarborele stang pana se gaseste cel mai mic nod
    while (node->left != nullptr) {
        operatii.count(2);
        node = node->left;
    }
    return node;
}
Tree* Succesor(Tree* x, Operation operatii) {

    operatii.count();
    ///Daca exista subarbore drept ,succesorul e minimul de acolo
    if (x->right != nullptr) {
        return Minim(x->right, operatii);
    }

    operatii.count();
    ///Se cauta primul parinte carui x este fiu stang
    Tree* y = x->parent;
    while (y != nullptr && x == y->right) {
        operatii.count(2);
        x = y;
        y = y->parent;
    }

    return y;
}
Tree* Os_select_for_delete(Tree* node, int indice, Operation operatii) {

    operatii.count();
    if (node == nullptr) {
        return nullptr;
    }

    int r;
    operatii.count();
    if (node->left != nullptr) {
        r = node->left->size + 1;
    }
    else {
        r = 1;
    }

    if (indice == r) {
        return node;
    }
    else if (indice < r) {
        return Os_select_for_delete(node->left, indice, operatii);
    }
    else {
        return Os_select_for_delete(node->right, indice - r, operatii);
    }
}
void Os_delete(Tree*& R, int indice, Operation operatii) {

    ///R=root,(radacina)
    ///Se cauta nodul care urmeaza sa fie sters
    Tree* D = Os_select_for_delete(R, indice, operatii); ///D-nodul care se sterge
    operatii.count();
    if (D != nullptr) {

        Tree *y,*x;
        ///Nodul y e chiar nodul de intrare D ,dacă D are cel mult un fiu
        ///Sau succesorul lui D ,daca d are 2 fii

        ///Daca y are fiu stang,x este fiul stang a lui y
        ///Daca nu are fiu stang ,si are drept atunci e fiul drept
        operatii.count(2);
        if (D->left == nullptr || D->right == nullptr) {
            y = D;
        }
        else {
            operatii.count();
            y = Succesor(D, operatii);
        }

        ///Se cauta x care este fiul nenul al lui y
        operatii.count();
        if (y->left != nullptr) {
            operatii.count();
            x = y->left;
        }
        else {
            x = y->right;
        }

        ///Se actualizeaza legaturile parinte-copil pentru x
        operatii.count();
        if (x != nullptr) {
            operatii.count();
            x->parent = y->parent;
        }

        ///Actualizam legatura parinte pentru y
        ///Sau pentru radacina

        operatii.count();
        if (y->parent == nullptr) {
            R = x;
        }
        else if (y == y->parent->left) {
            operatii.count();
            y->parent->left = x;
        }
        else {
            operatii.count();
            y->parent->right = x;
        }

        ///Daca y nu e egal cu D ,copiem cheia lui y in D
        operatii.count();
        if (y != D) {
            operatii.count();
            D->key = y->key;
        }

        operatii.count();
        ///Se actualizeaza dimensiunile la size de la nod pana la radacina
        ///Ca radacina nu are parinte
        Tree* Size = y->parent;
        while (Size != nullptr) {
            operatii.count(2);
            Size->size--;
            Size = Size->parent;
        }
        delete y; ///eliberare de memorie in c++( echivalent cu free in c)
    }

}


void print(Tree* R){
    ///R=Radacina
    if (R != nullptr) {
        print(R->left);
        cout<<"Key: "<< R->key <<",Size: "<< R->size << '\n';
        print(R->right);
    }
}
void pretty(Tree* R, int l) {
    ///R=Radacina
    if (R == nullptr) {
        return;
    }
    else {
        cout << string(l * 4, ' ') << R->key<<",Size:"<< R->size <<'\n';
        pretty(R->left,l + 1);
        pretty(R->right,l);
    }
}


void perf() {
    for (int n = Step; n <= Max; n += Step) {
        for (int test = 0; test < Nr_tests; test++) {
            Tree* tree = Os_build(1, n);
            for (int i = 0; i < n; i++) {
                Operation operati1 = p.createOperation("Select_operatii", n);
                Operation operati2 = p.createOperation("Delete_operatii", n);
                int k = rand() % (n - i) + 1;
                Os_select(tree, k, operati1);
                Os_delete(tree, k, operati2);
            }
        }
    }
    p.divideValues("Select_operatii",Nr_tests);
    p.divideValues("Delete_operatii",Nr_tests);
    p.createGroup("Total", "Select_operatii", "Delete_operatii");
    p.showReport();
}
void demo() {

    int n = 11;
    ///Folosit pentru nu a utiliza alte functii in care nu au parametrul operatii
    Operation operatii = p.createOperation("Demo", n);

    Tree* tree;
    tree = Os_build(1, n);

    pretty(tree, 0);
    cout << "\n";

    print(tree);
    cout << "\n";

    int k1 = 5;
    Os_delete(tree, k1, operatii);
    cout << "\nD1:\n";
    pretty(tree, 0);
    cout<<"\n";
    print(tree);

    int k2 = 2;
    Os_delete(tree, k2, operatii);
    cout << "\nD2:\n";
    pretty(tree, 0);
    cout<<"\n";
    print(tree);


    int k3 = 1;
    Os_delete(tree, k3, operatii);
    cout << "\nD3:\n";
    pretty(tree, 0);
    cout<<"\n";
    print(tree);
}


int main() {

    demo();
    //perf();
    return 0;
}
