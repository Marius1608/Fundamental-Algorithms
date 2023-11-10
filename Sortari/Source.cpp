/*
Toti cei 3 algoritmi au complexitate O(n^2)
Pe cazul mediu selection_sort utilizeaza cele mai putine atrtibuiri, pe cand insertion la nr de comparari
Iar ca nr total de operatii pe cazul mediu ,selection si insertion utilieaza acelasi aproximativ acelasi nr

Pe cazul favorabil
Buble si Selection nu fac atribuiri 
Buble si Insertion fac cele mai putine comparari 
Iar ca nr total de operatii Buble face cele mai putine

Pe cazul nefavorabil
Selection face cele mai putine atribuiri
Ca nr de comparati toate cele 3 fac aproximativ acelasi nr 
Iar ca nr total Selection face cele mai putine operati

Buble-e stabil
Lucreazã prin compararea si interschimbarea repetatã a elementelor alaturate într-un vector pânã când întregul vector este sortat

Insertion-e stabil
Algoritmul începe prin a considera primul element al vectorului ca fiind parte sortatã.
Apoi, el trece prin restul elementelor din partea nesortatã si le insereazã în locul potrivit în partea sortatã.

Selection-nu se stie daca e stabil,depinde de caz
El gãseste cel mai mic element din partea nesortatã si îl plaseazã în partea sortatã.
Procesul se repetã pânã când toate elementele sunt mutate din partea nesortatã în partea sortatã, iar vectorul este complet sortat.
*/

#include<stdio.h>
#include "Profiler.h"
 

Profiler p("Laborator1");
#define MAX 10000
#define STEP 100
#define NR_TESTS 10

void buble_sort(int v[], int n)
{
	Operation comparari = p.createOperation("Buble_comp", n);
	Operation atribuiri = p.createOperation("Buble_atri", n);
	int ok=false;
	for (int i=n-1; i>1; i--)
	{
		ok = false;
		for (int j=0; j<i; j++)
		{
			comparari.count();
			if (v[j] > v[j + 1]) {

				atribuiri.count(3);
				int aux = v[j];
				v[j] = v[j + 1];
				v[j + 1] = aux;
				ok = true;
			}
			
		}
		if (ok != true) break;
	}
}

void insertion_sort(int v[], int n)
{
	Operation comparari = p.createOperation("Insertion_comp", n);
	Operation atribuiri = p.createOperation("Insertion_atri", n);
	int key=0,i=0;
	for (int j=2; j<n; j++)
	{
		atribuiri.count();
		key = v[j];
		i = j - 1;
		comparari.count();
		while (i >= 0 && v[i] > key)
		{
			comparari.count();
			atribuiri.count();
			v[i + 1] = v[i];
			i--;
		}
		atribuiri.count();
		v[i + 1] = key;
	}
}

void selection_sort(int v[], int n)
{
	Operation comparari = p.createOperation("Selection_comp", n);
	Operation atribuiri = p.createOperation("Selection_atri", n);
	
	for (int j = 0; j < n - 1; j++)
	{
		int pmin = j;
		for (int i = j + 1; i< n; i++)
		{
			comparari.count();
			if (v[i] < v[pmin])
				pmin = i;
		}
		if (pmin != j)
		{
			atribuiri.count(3);
			int aux = v[j];
			v[j] = v[pmin];
			v[pmin] = aux;
		}
	}
}

void copy(int a[], int b[], int n)
{
	for (int i = 0; i < n; i++)
		a[i] = b[i];
}

void demo()
{
	int v[] = { 7,2,8,5,9,1,6,5,3,2 };
	int v1[] = { 4,5,6,8,2,11,4,5,6 };
	int v2[] = { 4,6,5,4,5,6,3,2,3 };

	int n = sizeof(v) / sizeof(v[0]);
	int n1 = sizeof(v1) / sizeof(v1[0]);
	int n2 = sizeof(v2) / sizeof(v2[0]);
	
	buble_sort(v, n);
	for (int i = 0; i < n; i++) printf("%d ", v[i]);
	printf("\n");

	insertion_sort(v1, n1);
	for (int i = 0; i < n1; i++) printf("%d ", v1[i]);
	printf("\n");

	selection_sort(v2, n2);
	for (int i = 0; i < n2; i++) printf("%d ", v2[i]);

	
}

void perf1(int order)
{

	int v[MAX],a[MAX];
	for (int n = STEP; n<= MAX; n+= STEP) {
		for (int test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(v, n, 10, 10000, false, order);
			copy(a, v, n);
			buble_sort(a, n);
			copy(a, v, n);
			insertion_sort(a, n);
			copy(a, v, n);
			selection_sort(a, n);
			
		}
	}

	p.divideValues("Buble_atri", NR_TESTS);
	p.divideValues("Buble_comp", NR_TESTS);
	p.addSeries("Buble_total", "Buble_atri", "Buble_comp");

	p.divideValues("Insertion_atri", NR_TESTS);
	p.divideValues("Insertion_comp", NR_TESTS);
	p.addSeries("Insertion_total", "Insertion_atri", "Insertion_comp");


	p.divideValues("Selection_atri", NR_TESTS);
	p.divideValues("Selection_comp", NR_TESTS);
	p.addSeries("Selection_total", "Selection_atri", "Selection_comp");

	p.createGroup("Atri_total_Selection","Selection_atri");
	p.createGroup("Atri_total", "Buble_atri", "Insertion_atri", "Selection_atri");

	p.createGroup("Comp_total", "Buble_comp", "Insertion_comp", "Selection_comp");
	p.createGroup("Comp_total_Buble_Insertion", "Buble_comp", "Insertion_comp");

	p.createGroup("Total", "Buble_total", "Insertion_total", "Selection_total");
	p.createGroup("Total_Buble_Insertion", "Buble_total", "Insertion_total");

}

void perf_all()
{
	perf1(UNSORTED);
	p.reset("Favorabil");
	perf1(ASCENDING);
	p.reset("Defavorabil");
	perf1(DESCENDING);
	p.showReport();
}

int main()
{
	demo();
	///perf_all();
	return 0;

}