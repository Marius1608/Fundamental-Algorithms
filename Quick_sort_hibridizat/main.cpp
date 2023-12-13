/*
 Pantea Marius Nicusor

-Parcurgerea unui arbore binar in inordine
 Varianta recursiva si cea iterativa au acelasi numar de operatii de afisare a cheii
 Complexitate O(n),se atinge fiecare nod o singura data


-Quick_sort hibridizat vs Quick_sort_normal
 Pivotul luat in mijloc la partitionare si treshhold=30
 Ca operatii ambele versiuni sunt aproximativ la fel
 Ca timp varianta hibridizata este mai buna decat cea normala dupa grafic

-Prag optim
 Un prag optim in hibridizare ce reiese din graficul de timp si operatii
 S-ar afla intre 10 si 30

 -Complexitate quick_sort_hibrid ,O(n^2),pe caz general

*/
#include <iostream>
#include "Profiler.h";
using namespace std;
Profiler p("Lab8");
#define Max 10000
#define Step 100
#define Nr_tests 10

struct Arb {
    int key;
    Arb* left;
    Arb* right;
};


void creare(Arb*& R, int key) {

    ///Arbore binar de cautare
    if (key != 0) {
        if (R == nullptr) {
            R = new Arb;
            R->key = key;
            R->left = nullptr;
            R->right = nullptr;
        }
        else {
            if (key < R->key) {
                creare(R->left, key);
            }
            else {
                creare(R->right, key);
            }
        }
    }
}
void inordine_rec(Arb* R, Operation operatii, int vers) {
    if (R != nullptr) {
        if (vers == 0) {
            inordine_rec(R->left, operatii, vers);
            operatii.count();
            cout << R->key << " ";
            inordine_rec(R->right, operatii, vers);
        } else if (vers != 0) {
            inordine_rec(R->left, operatii, vers);
            operatii.count();
            // cout << R->key << " ";
            inordine_rec(R->right, operatii, vers);
        }
    }
}
void inordine_itr(Arb* R, Operation operatii, int vers) {

    if (vers == 0) {
        Arb* nodCurent = R;   /// Nodului curent la radacina arborelui
        Arb* predecesor = nullptr;   /// Predecesor=nullptr pentru legături temporare
        /// Parcurgere arborele pana la sfarsit
        while (nodCurent != nullptr) {
            if (nodCurent->left == nullptr) {   /// Verificare dacă nodul curent nu are un subarbore stâng
                //operatii.count();
                cout << nodCurent->key << " ";
                nodCurent = nodCurent->right;   /// Trecerea la nodul din dreapta
            }
            else {
                predecesor = nodCurent->left;   /// Gasirea predecesorului în subarborele stâng

                /// Parcurgerea subarborelui drept al predecesorului până la sfarsit sau pana cand gasim o legatura catre nodul curent
                while (predecesor->right != nullptr && predecesor->right != nodCurent)
                    predecesor = predecesor->right;

                if (predecesor->right == nullptr) {
                    predecesor->right = nodCurent;   ///Crearea legaturii temporare catre nodul curent
                    nodCurent = nodCurent->left;   ///Trecerea la nodul din stânga pentru continuarea parcurgerii în ordine
                }
                else {
                    ///Daca legatura temporară exista ==finalul subarborelui stang
                    predecesor->right = nullptr;   ///Restabilirea legaturii temporare
                    //operatii.count();
                    cout << nodCurent->key << " ";
                    nodCurent = nodCurent->right;   ///Trecerea la nodul din dreapta pentru continuarea parcurgerii în ordine
                }
            }
        }
    }
    else if (vers != 0)
    {
        Arb* nodCurent = R;
        Arb* predecesor = nullptr;
        while (nodCurent != nullptr) {
            if (nodCurent->left == nullptr) {
                operatii.count();
                //cout << nodCurent->key << " ";
                nodCurent = nodCurent->right;
            }
            else {
                predecesor = nodCurent->left;
                while (predecesor->right != nullptr && predecesor->right != nodCurent)
                    predecesor = predecesor->right;

                if (predecesor->right == nullptr) {
                    predecesor->right = nodCurent;
                    nodCurent = nodCurent->left;
                }
                else {

                    predecesor->right = nullptr;
                    operatii.count();
                    ///cout << nodCurent->key << " ";
                    nodCurent = nodCurent->right;
                }
            }
        }
    }
}

void preordine_rec(Arb* R, Operation operatii, int vers) {
    if (R != nullptr) {
        if (vers == 0) {
            operatii.count();
            cout << R->key << " ";
            preordine_rec(R->left, operatii, vers);
            preordine_rec(R->right, operatii, vers);
        } else if (vers != 0) {
            operatii.count();
            // cout << R->key << " ";
            preordine_rec(R->left, operatii, vers);
            preordine_rec(R->right, operatii, vers);
        }
    }
}
void postordine_rec(Arb* R, Operation operatii, int vers) {
    if (R != nullptr) {
        if (vers == 0) {
            postordine_rec(R->left, operatii, vers);
            postordine_rec(R->right, operatii, vers);
            operatii.count();
            cout << R->key << " ";
        } else if (vers != 0) {
            postordine_rec(R->left, operatii, vers);
            postordine_rec(R->right, operatii, vers);
            operatii.count();
            // cout << R->key << " ";
        }
    }
}

void demo_ab() {
    Operation operatii = p.createOperation("", 0);
    Arb* R = nullptr;
    int n = 10;
    int elements[] = {5, 3, 86, 10, 42, 7, 9, 10, 6, 18};
    for (int i = 0; i < n; ++i) {
        creare(R, elements[i]);
    }
    cout << "Inorder Recursiv ";
    inordine_rec(R, operatii, 0);
    cout << "\n";

    cout << "Inorder Iterativ ";
    inordine_itr(R, operatii, 0);
    cout << "\n";

    cout << "Preorder Recursiv ";
    preordine_rec(R, operatii, 0);
    cout << "\n";

    cout << "Postorder Recursiv ";
    postordine_rec(R, operatii, 0);
    cout << "\n";
}
void perf_ab() {
    int a[Max];
    for (int n = Step; n <= Max; n += Step) {
        for (int test = 0; test < Nr_tests; test++) {
            Arb* R = nullptr;
            Arb* R1 = nullptr;
            Operation operati1 = p.createOperation("Inordine_rec", n);
            Operation operati2 = p.createOperation("Inordine_iter", n);
            FillRandomArray(a, n, 10, 50000, false, UNSORTED);
            for (int i = 0; i < n; ++i) {
                creare(R, a[i]);
                creare(R1, a[i]);
            }
            inordine_rec(R, operati1, 1);
            inordine_itr(R1, operati2, 1);
        }
    }
    p.divideValues("Inordine_rec", Nr_tests);
    p.divideValues("Inordine_iter", Nr_tests);
    p.createGroup("Inordine_Total", "Inordine_rec", "Inordine_iter");
    p.showReport();
}

int partition(int v[], int s, int r, Operation operatii)
{
    operatii.count();
    int x = v[(s + r) / 2];
    int i = s - 1;
    for (int j = s; j <= r; j++)
    {
        operatii.count();
        if (v[j] < x) {

            operatii.count(3);
            i++;
            int aux = v[i];
            v[i] = v[j];
            v[j] = aux;
        }

    }
    operatii.count();
    int rev = v[i + 1];
    v[i + 1] = v[r];
    v[r] = rev;
    return i + 1;
}
void quick_sort(int v[], int p, int r, Operation operatii) {

    if (p < r)
    {
        int q = partition(v, p, r, operatii);
        quick_sort(v, p, q - 1, operatii);
        quick_sort(v, q + 1, r, operatii);
    }
}


void insertion_sort_h(int v[], int n, Operation operatii) {
    int key = 0, i = 0;
    for (int j = 1; j < n; j++) {
        operatii.count();
        key = v[j];
        i = j - 1;
        operatii.count();
        while (i >= 0 && v[i] > key) {
            operatii.count(2);
            v[i + 1] = v[i];
            i--;
        }
        operatii.count();
        v[i + 1] = key;
    }
}
int partition_h(int v[], int s, int r, Operation operatii) {
    operatii.count();
    int x = v[(s + r) / 2];
    int i = s - 1;
    for (int j = s; j <= r - 1; j++) {
        operatii.count();
        if (v[j] < x) {
            operatii.count(3);
            i++;
            int aux = v[i];
            v[i] = v[j];
            v[j] = aux;
        }
    }
    operatii.count();
    int rev = v[i + 1];
    v[i + 1] = v[r];
    v[r] = rev;
    return i + 1;
}
void quick_sort_h(int v[], int p, int r, const int T, Operation operatii) {
    if (p < r) {
        if (r - p + 1 <= T) {
            insertion_sort_h(v + p, r - p + 1, operatii);
        }
        else {
            int q = partition_h(v, p, r, operatii);
            quick_sort_h(v, p, q - 1, T, operatii);
            quick_sort_h(v, q + 1, r, T, operatii);
        }
    }
}


void copy(int a[], int b[], int n)
{
    for (int i = 0; i < n; i++)
        a[i] = b[i];
}
void demo_quick_h() {
    Operation operatii = p.createOperation("", 0);
    int v[] = { 2, 3, 4, 5, 4, 63, 32, 2, 2, 4, 21, 10, 2, 31, 1, 31 };
    int n = sizeof(v) / sizeof(v[0]);
    quick_sort_h(v, 0, n - 1, 30, operatii);
    cout << "Sortare quick: ";
    for (int i = 0; i < n; i++) {
        cout << v[i] << " ";
    }
    cout << "\n";
}
void perf_operatii_quick() {
    int a[Max], b[Max];
    for (int n = Step; n <= Max; n += Step) {
        Operation operati1 = p.createOperation("Quick_h", n);
        Operation operati2 = p.createOperation("Quick", n);
        for (int test = 0; test < Nr_tests; test++)
        {
            FillRandomArray(a, n, 10, 50000, false, UNSORTED);
            copy(b, a, n);
            quick_sort_h(a, 0, n, 30, operati1);
            quick_sort(b, 0, n, operati2);
        }
    }
    p.divideValues("Quick_h", Nr_tests);
    p.divideValues("Quick", Nr_tests);
    p.createGroup("Total", "Quick_h", "Quick");
    p.showReport();
}
void perf_timp_quick() {

    int b[Max], a[Max];
    Operation operatii = p.createOperation("", 0);
    for (int n = Step; n <= Max; n += Step) {


        FillRandomArray(b, n, 10, 50000, false, UNSORTED);
        p.startTimer("Quick_h_timp", n);
        for (int test = 0; test < 200; test++) {
            copy(a, b, n);
            quick_sort_h(a, 0, n, 30, operatii);
        }
        p.stopTimer("Quick_h_timp", n);

        p.startTimer("Quick_timp", n);
        for (int test = 0; test < 200; test++) {
            copy(a, b, n);
            quick_sort(a, 0, n, operatii);
        }
        p.stopTimer("Quick_timp", n);
    }

    p.createGroup("Timp_total", "Quick_h_timp", "Quick_timp");
    p.showReport();
}
void hib() {
    int a[Max], b[Max];
    int n = 10000;
    Operation o = p.createOperation("Prag_optim", 0);

    for (int i = 1; i <= 100; i++) {

        FillRandomArray(a, n, 1, 50000, false, UNSORTED);
        Operation operatii = p.createOperation("Prag_optim", i);
        for (int test = 0; test < 25; test++) {

            copy(b, a, n);
            quick_sort_h(b, 0, n - 1, i, operatii);
        }

        p.startTimer("Quick_h_timp", i);
        for (int test = 0; test < 200; test++) {

            copy(b, a, n);
            quick_sort_h(b, 0, n - 1, i, o);
        }
        p.stopTimer("Quick_h_timp", i);


    }
    p.divideValues("Prag_optim", 25);
    p.showReport();
}


int main() {
    demo_ab();
    demo_quick_h();
    //perf_operatii_quick();
    //perf_timp_quick();
    //perf_ab();
    //perf_ab2();
    //hib();
    return 0;
}
