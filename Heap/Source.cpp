/*
Complexitate top_down O(n*log n)
-mai multe realocari si comparatii 
-folosit deobicei pentru cozi de prioritate 
-se adauga elementele din sir unul cate unul 
-se verifica proprietatea de heap, daca nu se reaseaza elementul


Complexitate bottom_up O(n)
-fiecare element e mutat o singura data 
-utilizat in special sortare 
-se parcurge sirul de la mijloc catre inceput (de la ultimul nod parinte pana la radacina)
-se asigura proprietatea de heap ,daca nu se reaseaza elementul


 Ca nr de atribuiri si comparati pe cazul mediu constructia cu implementare bottom_up sta mai bine avand mai putine
 Iar pe cazul defavavorabil constructia bottom_up utilizeaza mai putine atribuiri si comparari 

 Variata bottom up lucreaza doar cu jumatate de elemente ,deoarce frunzele sunt deja heap uri 
 Lucreaza doar cu n/2 elemente

*/
#include <stdio.h>
#include "Profiler.h"

Profiler p("Laborator_2_heap");
#define Max 10000
#define Steps 100
#define Nr_tests 5

struct Heap {
    int a[Max];
    int heap_length;
    int n;
};


int parent(int i) {
    return (i - 1) / 2;
}

int left(int i) {
    return 2 * i + 1;
}

int right(int i) {
    return 2 * i + 2;
}


void heap_print(Heap H) {

    for (int i = 0; i < H.heap_length; i++)
        printf("%d ", H.a[i]);
   
}


void Heapify1(struct Heap& H, int i) {

    Operation comparari = p.createOperation("Heap_bottom_up_comp", H.n);
    Operation atribuiri = p.createOperation("Heap_bottom_up_atr", H.n);
    
    int largest=i;
    int left_1 = left(i);
    int right_1 = right(i);

    if (left_1 < H.heap_length) {
        comparari.count();
        if (H.a[left_1] > H.a[i]) {
            largest = left_1;
        }
    }
    

    if (right_1 < H.heap_length) {
        comparari.count();
        if (H.a[right_1] > H.a[largest]) {
            largest = right_1;
        }
    }

    if (largest != i) {
        atribuiri.count(3);
        int aux = H.a[i];
        H.a[i] = H.a[largest];
        H.a[largest] = aux;
        Heapify1(H, largest);
    }
}

void build_bottom_up(struct Heap& H) {

    Operation comparari = p.createOperation("Heap_bottom_up_comp", H.n);
    Operation atribuiri = p.createOperation("Heap_bottom_up_atr", H.n);

    atribuiri.count();
    H.heap_length = H.n;

    for (int i = (H.heap_length / 2) - 1; i >= 0; i--) {
        Heapify1(H, i);
    }
}

void HeapSort1(struct Heap& H) {

    build_bottom_up(H);
    printf("Heap bottom_up:");
    heap_print(H);
    printf("\n");

    for (int i = H.n - 1; i > 0; i--) {
        int aux = H.a[0];
        H.a[0] = H.a[i];
        H.a[i] = aux;
        H.heap_length--;
        Heapify1(H, 0);
    }
}



void Heapify2(struct Heap& H, int i) {

   // Operation comparari = p.createOperation("Heap_top_down_comp", H.n);
    //Operation atribuiri = p.createOperation("Heap_top_down_atr", H.n);
    
    int largest=i;
    int left_1 = left(i);
    int right_1 = right(i);

    if (left_1 < H.heap_length) {
      //comparari.count();
        if (H.a[left_1] > H.a[i]) {
            largest = left_1;
        }
    }
    

    if (right_1 < H.heap_length) {
       //comparari.count();
        if (H.a[right_1] > H.a[largest]) {
            largest = right_1;
        }
    }

    if (largest != i) {
       //atribuiri.count(3);
        int aux = H.a[i];
        H.a[i] = H.a[largest];
        H.a[largest] = aux;
        Heapify2(H, largest);
    }
}

void build_top_down(struct Heap& H) {

    Operation comparari = p.createOperation("Heap_top_down_comp", H.n);
    Operation atribuiri = p.createOperation("Heap_top_down_atr", H.n);

    H.heap_length = 0;

    for (int i = 0;  i < H.n; i++) {
        atribuiri.count();
        H.heap_length++;
        int key = i;
        while (key > 0 ) {
            int parent1 = parent(key);
            comparari.count();
            if (H.a[parent1]< H.a[key]) {
                atribuiri.count(3);
                int aux = H.a[key];
                H.a[key] = H.a[parent1];
                H.a[parent1] = aux;
            }
            key = parent1;
        }
    }
}

void HeapSort2(struct Heap& H) {

    build_top_down(H);
    printf("Heap top_down:");
    heap_print(H);
    printf("\n");

    for (int i = H.n - 1; i > 0; i--) {
        int aux = H.a[0];
        H.a[0] = H.a[i];
        H.a[i] = aux;
        H.heap_length--;
        Heapify2(H, 0);
    }
}


void demo1() {
    
    Heap H;

    H.a[0] = 4; H.a[1] = 10; H.a[2] = 10; H.a[3] = 1;
    H.a[4] = 2; H.a[5] = 3;  H.a[6] = 7; H.a[7] = 22;
    H.a[8] = 9; H.a[9] = 5;  H.a[10] = 51; H.a[11] = 90;
    H.n = 12;
    H.heap_length = H.n;
   
    HeapSort1(H);
    printf("Sortat: ");

    for (int i = 0; i < H.n; i++) {
        printf("%d ", H.a[i]);
    }
    printf("\n");
}

void demo2() {

    Heap H;

    H.a[0] = 74; H.a[1] = 10; H.a[2] = 11; H.a[3] = 12;
    H.a[4] = 21; H.a[5] = 2;  H.a[6] = 7; H.a[7] = 20;
    H.a[8] = 19; H.a[9] = 4;  H.a[10] = 5; H.a[11] = 81;
    H.n = 12;
    H.heap_length = H.n;

    
    HeapSort2(H);
    printf("Sortat: ");

    for (int i = 0; i < H.n; i++) {
        printf("%d ", H.a[i]);
    }
    printf("\n");
}



void copy(Heap &H, int a[])
{
    for (int i = 0; i < H.n; i++)
        H.a[i] = a[i];
}



void perf1(int order)
{
    int a[Max];
    for (int n = Steps; n <= Max; n += Steps)
    {
        for (int test = 0; test < Nr_tests; test++)
        {
            Heap H1, H2;
            FillRandomArray(a, n, 10, 50000, false, order);
            H1.n = H2.n = n;
            copy(H1, a); copy(H2, a);
            build_bottom_up(H1);
            build_top_down(H2);
        }
    }
    
    p.divideValues("Heap_bottom_up_atr", Nr_tests);
    p.divideValues("Heap_bottom_up_comp", Nr_tests);
    p.addSeries("Heap_bottom_up_total", "Heap_bottom_up_atr", "Heap_bottom_up_comp");

    p.divideValues("Heap_top_down_atr", Nr_tests);
    p.divideValues("Heap_top_down_comp", Nr_tests);
    p.addSeries("Heap_top_down_total", "Heap_top_down_atr", "Heap_top_down_comp");

    p.createGroup("Atri_total", "Heap_bottom_up_atr", "Heap_top_down_atr");
    p.createGroup("Comp_total", "Heap_bottom_up_comp", "Heap_top_down_comp");
    p.createGroup("Total", "Heap_bottom_up_total", "Heap_top_down_total");
}

void perf_all()
{
    perf1(UNSORTED);
    p.reset("Defavorabil");
    perf1(ASCENDING);
    p.showReport();
}


int main() {
    
    demo1();
    demo2();
    //perf_all();
    return 0;
}