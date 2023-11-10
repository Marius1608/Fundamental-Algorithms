/*
Pantea Marius Nicusor

1.Recursiv vs Iterativ 
-Insertion sort ,varianta recursiva si cea iterativa au aproximativ acelasi numar de operatii
-Iar ca timp de executie ,insertion recursiv este mai rapid
-Complexitatea tot O(n^2)

2.Quick_sort vs Heap_sort
   -In cazul best si mediu quick_sort ,ca si numar de atribuiri si comparati este mai bun decat heap_sort
   -In cazul worst heap_sort este mult mai bun decat quick_sort atat ca numar de atribuiri cat si ca numar de comparatii
   -In cazul worst quick_sort are complexitate O(n^2) iar heap_sort O(nlogn)
   -Iar in cazul best si mediu quick are complexitate O(nlogn) si heap_sort tot O(nlogn)
*/

#include<stdio.h>
#include "Profiler.h"

Profiler p("Laborator3");
#define Max 10000
#define Step 100
#define Nr_tests 10


void insertion_sort_iterativ(int v[], int n)
{
	Operation operatii= p.createOperation("Insertion_sort_iterativ",n);
    
	int key = 0, i = 0;
	for (int j = 2; j < n; j++)
	{
        operatii.count();
		key = v[j];
		i = j - 1;
        operatii.count();
		while (i >= 0 && v[i] > key)
		{
            operatii.count(2);
			v[i + 1] = v[i];
			i--;
		}
        operatii.count();
		v[i + 1] = key;
	}
}

void insertion_sort_recursiv_op(int v[], int n,Operation operatii) {
    
    if (n <= 1) {
        return;
    }
    insertion_sort_recursiv_op(v, n - 1,operatii);
    operatii.count();
    int key = v[n - 1];
    int i = n - 2;

    operatii.count();
    while (i >= 0 && v[i] > key) {

        operatii.count(2);
        v[i + 1] = v[i];
        i--;
    }
    operatii.count();
    v[i + 1] = key;
}

void insertion_sort_recursiv_time(int v[], int n) {
    if (n <= 1) {
        return;
    }
    insertion_sort_recursiv_time(v, n - 1);
    
    int key = v[n - 1];
    int i = n - 2;
    while (i >= 0 && v[i] > key) {

       
        v[i + 1] = v[i];
        i--;
    }
    v[i + 1] = key;
}

void copy(int a[], int b[], int n)
{
    for (int i = 0; i < n; i++)
        a[i] = b[i];
}

void perf_insertion_op() {

    int v[Max], a[Max];
    for (int n = Step; n <= Max; n += Step) {
        Operation operatii = p.createOperation("Insertion_sort_recursiv", n);
        for (int test = 0; test < Nr_tests; test++) {
            FillRandomArray(v, n, 10, 50000, false, UNSORTED);
            copy(a, v, n);
            insertion_sort_recursiv_op(a, n,operatii);
            copy(a, v, n);
            insertion_sort_iterativ(a, n);
        }
    }
    p.divideValues("Insertion_sort_iterativ",Nr_tests);
    p.divideValues("Insertion_sort_recursiv", Nr_tests);
    p.createGroup("Operatii_total", "Insertion_sort_iterativ", "Insertion_sort_recursiv");
    p.showReport();

}

void perf_insertion_time() {

    int a[Max], v[Max];
    for (int n = Step; n <= Max; n += Step) {

        FillRandomArray(v, n, 10, 50000, false, UNSORTED);
        p.startTimer("Insertion_sort_iterativ_timp",n);
        for (int test = 0; test < 200; test++) {
            copy(a, v, n);
            insertion_sort_iterativ(a, n);
        }
        p.stopTimer("Insertion_sort_iterativ_timp", n);

        p.startTimer("Insertion_sort_recursiv_timp", n);
        for (int test = 0; test < 200; test++) {
            copy(a, v, n);
            insertion_sort_recursiv_time(a, n);
        }
        p.stopTimer("Insertion_sort_recursiv_timp", n);
    }

    //p.divideValues("Insertion_sort_iterativ",Nr_tests);
    //p.divideValues("Insertion_sort_recursiv",Nr_tests);
    //p.addSeries("Total", "Insertion_sort_iterativ", "Insertion_sort_recursiv");
    
    //p.createGroup("Operatii_total", "Insertion_sort_iterativ", "Insertion_sort_recursiv");
    p.createGroup("Timp_total","Insertion_sort_iterativ_timp", "Insertion_sort_recursiv_timp");
    p.showReport();

}



struct Heap {
    int a[Max];
    int heap_length;
    int n;
};

int left(int i) {
    return 2 * i + 1;
}

int right(int i) {
    return 2 * i + 2;
}

void Heapify1(struct Heap& H, int i) {

    Operation comparari = p.createOperation("Heap_comparari", H.n);
    Operation atribuiri = p.createOperation("Heap_atribuiri", H.n);

    int largest = i;
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

    Operation comparari = p.createOperation("Heap_comparari", H.n);
    Operation atribuiri = p.createOperation("Heap_atribuiri", H.n);

    atribuiri.count();
    H.heap_length = H.n;

    for (int i = (H.heap_length / 2) - 1; i >= 0; i--) {
        Heapify1(H, i);
    }
}

void HeapSort(struct Heap& H) {

    Operation atr = p.createOperation("Heap_atribuiri", H.n);
    Operation comp = p.createOperation("Heap_comparari", H.n);

    build_bottom_up(H);
    for (int i = H.n - 1; i > 0; i--) {

        atr.count(4);
        int aux = H.a[0];
        H.a[0] = H.a[i];
        H.a[i] = aux;
        H.heap_length--;
        Heapify1(H, 0);
    }
}

void copy_heap(Heap& H, int a[])
{
    for (int i = 0; i < H.n; i++)
        H.a[i] = a[i];
}




int partition(int v[], int s, int r,CONST int R)
{
    Operation comparari = p.createOperation("Quick_mediu_comp", R);
    Operation atribuiri = p.createOperation("Quick_mediu_atr", R);
    
    atribuiri.count();
    int x = v[r];
    int i = s - 1;
    for (int j = s ; j <= r ; j++)
    {
        comparari.count();
        if (v[j] < x) {
            
            atribuiri.count(3);
            i++;
            int aux = v[i];
            v[i] = v[j];
            v[j] = aux;
        }
    
    }
    atribuiri.count();

    int rev = v[i + 1];
    v[i + 1] = v[r];
    v[r] = rev;
    return i+1;
}

void quick_sort(int v[], int p, int r,CONST int R) {

    if (p < r)
    {
        int q = partition(v, p, r,R);
        quick_sort(v, p, q - 1,R);
        quick_sort(v, q + 1, r,R);
    }
}

void demo_quick() {

    int v[] = { 2,3,4,5,4,63,32,2,2,4,21,10,2,31,1,31 };
    int n = sizeof(v) / sizeof(v[0]);
    quick_sort(v, 0, n - 1, n - 1);

    for (int i = 0; i < n; i++)
    {
        printf("%d ", v[i]);
    }
    printf("\n");
}

void perf_mediu_quick() {

    int v[Max], a[Max];
    for (int n = Step; n <= Max; n += Step) {
        for (int test = 0; test < Nr_tests; test++)
        {
            FillRandomArray(v, n, 10, 50000, false, UNSORTED);
            quick_sort(v, 0, n, n);

        }
    }
    p.divideValues("Quick_mediu_comp", Nr_tests);
    p.divideValues("Quick_mediu_atr", Nr_tests);
    p.addSeries("Total", "Quick_mediu_comp", "Quick_mediu_atr");
    p.showReport();

}



int partition_best_case(int v[], int s, int r, CONST int R) {

    Operation comparari = p.createOperation("Quick_best_comp", R);
    Operation atribuiri = p.createOperation("Quick_best_atr", R);

    atribuiri.count();
    int x = v[(r+s)/2];
    //best case piv mij x= [p+r/2]
    int i = s - 1;
    for (int j = s; j <= r; j++)
    {
        comparari.count();
        if (v[j] < x) {

            atribuiri.count(3);
            i++;
            int aux = v[i];
            v[i] = v[j];
            v[j] = aux;
        }

    }
    atribuiri.count();

    int rev = v[i + 1];
    v[i + 1] = v[r];
    v[r] = rev;
    return i + 1;
}

void quick_sort_best(int v[], int p, int r, CONST int R) {

    if (p < r)
    {
        int q = partition_best_case(v, p, r, R);
        quick_sort_best(v, p, q - 1, R);
        quick_sort_best(v, q + 1, r, R);
    }
}


int partition_mediu(int v[], int s, int r, CONST int R) {
    Operation comparari = p.createOperation("Quick_mediu_comp", R);
    Operation atribuiri = p.createOperation("Quick_mediu_atr", R);

    atribuiri.count();
    int x = v[r];
    int i = s - 1;
    for (int j = s; j <= r; j++)
    {
        comparari.count();
        if (v[j] < x) {

            atribuiri.count(3);
            i++;
            int aux = v[i];
            v[i] = v[j];
            v[j] = aux;
        }

    }
    atribuiri.count();

    int rev = v[i + 1];
    v[i + 1] = v[r];
    v[r] = rev;
    return i + 1;
}

void quick_sort_mediu(int v[], int p, int r, CONST int R) {

    if (p < r)
    {
        int q = partition_mediu(v, p, r, R);
        quick_sort_mediu(v, p, q - 1, R);
        quick_sort_mediu(v, q + 1, r, R);
    }
}


int partition_worst(int v[], int s, int r, CONST int R) {

    Operation comparari = p.createOperation("Quick_worst_comp", R);
    Operation atribuiri = p.createOperation("Quick_worst_atr", R);

    atribuiri.count();
    int x = v[r];
    //best case piv mij x= [p+r/2]
    int i = s - 1;
    for (int j = s; j <= r; j++)
    {
        comparari.count();
        if (v[j] < x) {

            atribuiri.count(3);
            i++;
            int aux = v[i];
            v[i] = v[j];
            v[j] = aux;
        }

    }
    atribuiri.count();

    int rev = v[i + 1];
    v[i + 1] = v[r];
    v[r] = rev;
    return i + 1;
}

void quick_sort_worst(int v[], int p, int r, CONST int R) {

    if (p < r)
    {
        int q = partition_worst(v, p, r, R);
        quick_sort_worst(v, p, q - 1, R);
        quick_sort_worst(v, q + 1, r, R);
    }
}



void inversare(int v[], int n) {

    int s = 0;
    int d = n - 1;

    while (s < d) {
        
        int aux = v[s];
        v[s] = v[d];
        v[d] = aux;
        s++;
        d--;
    }
}

void perf_heap_vs_quick_best()
{
    int a[Max], b[Max];
    for (int n = Step; n <= Max; n += Step) {
        for (int test = 0; test < Nr_tests; test++)
        {
            FillRandomArray(a, n, 10, 50000, false, ASCENDING);
            Heap H1;
            H1.n = n;
            copy(b, a, n);
            inversare(b, n);
            copy_heap(H1, b);
            HeapSort(H1);
            quick_sort_best(a, 0, n, n);

        }
    }

    p.divideValues("Quick_best_comp", Nr_tests);
    p.divideValues("Quick_best_atr", Nr_tests);
    p.addSeries("Total_quick_best", "Quick_best_comp", "Quick_best_atr");

    p.divideValues("Heap_comparari", Nr_tests);
    p.divideValues("Heap_atribuiri", Nr_tests);
    p.addSeries("Total_heap", "Heap_comparari", "Heap_atribuiri");

    p.createGroup("Comp_total_best", "Quick_best_comp", "Heap_comparari");
    p.createGroup("Atr_total_best", "Quick_best_atr", "Heap_atribuiri");
    p.createGroup("Total_best", "Total_quick_best", "Total_heap");
    p.showReport();
}

void perf_heap_vs_quick_mediu()
{
    int a[Max];
    for (int n = Step; n <= Max; n += Step) {
        for (int test = 0; test < Nr_tests; test++)
        {
            Heap H1;
            FillRandomArray(a, n, 10, 50000, false, UNSORTED);
            H1.n = n;
            copy_heap(H1, a);
            HeapSort(H1);
            quick_sort_mediu(a, 0, n, n);

        }
    }

    p.divideValues("Quick_mediu_comp", Nr_tests);
    p.divideValues("Quick_mediu_atr", Nr_tests);
    p.addSeries("Total_quick_mediu", "Quick_mediu_comp", "Quick_mediu_atr");

    p.divideValues("Heap_comparari", Nr_tests);
    p.divideValues("Heap_atribuiri", Nr_tests);
    p.addSeries("Total_heap", "Heap_comparari", "Heap_atribuiri");

    p.createGroup("Comp_total_mediu", "Quick_mediu_comp", "Heap_comparari");
    p.createGroup("Atr_total_mediu", "Quick_mediu_atr", "Heap_atribuiri");
    p.createGroup("Total_mediu", "Total_quick_mediu", "Total_heap");
    p.showReport();
}

void perf_heap_vs_quick_worst()
{
    int a[Max],b[Max];
    for (int n = Step; n <= Max; n += Step) {
        for (int test = 0; test < Nr_tests; test++)
        {
            Heap H1;
            FillRandomArray(a, n, 10, 50000, false,DESCENDING);
            copy(b, a, n);
            inversare(b, n);
            H1.n = n;
            copy_heap(H1, b);
            HeapSort(H1);
            quick_sort_worst(a, 0, n, n);

        }
    }

    p.divideValues("Quick_worst_comp", Nr_tests);
    p.divideValues("Quick_worst_atr", Nr_tests);
    p.addSeries("Total_quick_worst", "Quick_worst_comp", "Quick_worst_atr");

    p.divideValues("Heap_comparari", Nr_tests);
    p.divideValues("Heap_atribuiri", Nr_tests);
    p.addSeries("Total_heap", "Heap_comparari", "Heap_atribuiri");

    p.createGroup("Comp_total_worst", "Quick_worst_comp", "Heap_comparari");
    p.createGroup("Atr_total_worst", "Quick_worst_atr", "Heap_atribuiri");
    p.createGroup("Total_worst", "Total_quick_worst", "Total_heap");
    
    p.createGroup("Heap_comparari", "Heap_comparari");
    p.createGroup("Heap_atribuiri", "Heap_atribuiri");
    p.createGroup("Total_heap", "Total_heap");

    p.showReport();
}



int main()
{
      demo_quick();
    //perf_insertion_time();
    //perf_insertion_op();
    //perf_mediu_quick();
    //perf_heap_vs_quick_best();
    //perf_heap_vs_quick_mediu();
    //perf_heap_vs_quick_worst();

}