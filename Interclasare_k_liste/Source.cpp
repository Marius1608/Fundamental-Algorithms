/*
	Pantea Marius Nicusor 
	Complexitatea algoritmul care nu utilieaza min_heap este O(n*k)
	Se interclaseaza cate primele doua dupa rezultatul cu cea de a treia ,...,pana se interclaseaza toate
	Algoritmul implementat cu ajutorul min_heap-ului este de O(n*logk)
	Min-heap-ul este folosit pentru a gãsi eficient elementul minim din k liste
	Ceea ce îmbunãtãteste semnificativ eficienta algoritmului de interclasare.

*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include"Profiler.h"

#define Max 10000

struct Node {
	int val;
	Node* next;
};

struct MinHeap{
	int key;
	int list_index;
};

Node* create(int val) {
	Node* node = (Node*) malloc(sizeof(Node));
	node->val = val;
	node->next = NULL;
	return node;
}

MinHeap createHeap(int key, int list_index) {
	MinHeap node;
	node.key = key;
	node.list_index = list_index;
	return node;
}

void insert(Node** in, int val) {

	Node* node = create(val); //creaza un nod (nou) cu valoarea data
	if (*in == NULL || val < (*in)->val) {
		//daca lista de intrare e goala sau val data e < valoarea primului nod din lista
		node->next = *in; //noul nod va deveni primul nod din lista
		*in = node;		//Se actualizeaza pointerul de la inceputul liste
	}
	else {
		Node* now = *in;
		while (now->next != NULL) {
			if (val <= now->next->val)break;
			//se cauta in lista pana gaseste locul bun de inserat
			now = now->next;// Se parcurge lista in continuare
		}
		//se leaga noul nod
		node->next = now->next;
		now->next = node;
	}
}



int left(int i) {
	return 2 * i + 1;
}

int right(int i) {
	return 2 * i + 2;
}

void Heapify(MinHeap* v, int n, int i) {

	int s = i;
	int left1 = left(i);
	int right1 = right(i);
	if (left1 < n && v[left1].key < v[s].key) {
		s = left1;
	}
	if (right1 < n && v[right1].key < v[s].key) {
		s = right1;
	}

	if (s != i) {
		MinHeap aux = v[i];
		v[i] = v[s];
		v[s] = aux;
		Heapify(v, n, s);
	}
}

void buildMinHeap(MinHeap* v, int n) {
	for (int i = n / 2 - 1; i >= 0; i--) {
		Heapify(v, n, i);
	}
}



void printL(Node* n) {
	Node* node = n;
	while (node != NULL) {
		printf("%d ", node->val);
		node = node->next;
	}
	printf("\n");
}


Node** random(int n, int k) {
		Node** l = (Node**)calloc(k, sizeof(Node*));
		srand(time(NULL));
		int nr = n / k;
		int nr1 = n % k;

		int c = 0;

		for (int i = 0; i < k; i++) {
			int nr2 = nr;
			if (i < nr1) {
				nr2++;
			}

			for (int j = 0; j < nr2; j++) {
				insert(&l[c], rand() % 1000);
			}

			c++; 
		}

		return l;
	}
	
Node** random2(int n, int k) {
	
	Node** l = (Node**)malloc(k * sizeof(Node*));
	int v[Max];
	int nr = n / k;
	int nr1 = n % k;
	int c = 0;
	for (int i = 0; i < k; i++)
	{	
		int nr2 = nr;
		if (i < nr1)nr2++;
		FillRandomArray(v, nr, 0, 10000, 0, ASCENDING);
		for (int j = 0; j < nr; j++)
		{
			insert(&l[c],v[j]);
		}
		c++;
			
	}
	return l;
}

Node* merge(Node* l[], int k) {

	MinHeap* m = (MinHeap*)malloc(k * sizeof(MinHeap));// Alocare memorie pentru un "vector" de pointeri cu primele elemente din lista
	Node* r = NULL;// Lista cu rezultatul final

	// Ini?ializãm min-heap-ul cu primele elemente din listele de intrare
	for (int i = 0; i < k; i++) {
		m[i] = createHeap(l[i]->val, i);
	}

	//min-heap-ul ini?ial
	buildMinHeap(m, k);

	while (1) {

		// Extragem elementul minim din min-heap
		MinHeap min = m[0];
		int min_index = min.list_index;

		// Inserãm elementul minim în lista rezultat
		insert(&r, l[min_index]->val);

		// Trecem la urmãtorul element din lista de la care am extras minimul
		l[min_index] = l[min_index]->next;

		if (l[min_index] == NULL) {
			// Dacã lista este goalã, înlocuim elementul din min-heap cu INT_MAX
			m[0].key = INT_MAX;
			Heapify(m, k, 0); //  men?ine proprietatea de min-heap
		}
		else {
			// Elementul din min-heap cu urmãtorul element din lista
			m[0] = createHeap(l[min_index]->val, min_index);
			Heapify(m, k, 0); //refacem propritatea de heap
		}

		//verificam daca mai sunt liste de inerclasat
		int ok = 1;
		for (int i = 0; i < k; i++) {
			if (l[i] != NULL) {
				ok = 0;
				break;
			}
		}

		if (ok) {
			break;
		}
	}

	free(m);
	return r;
}




void freeL(Node* n) {
	while (n != NULL) {
		Node* aux = n;
		n = n->next;
		free(aux);
	}
}

void freeM(MinHeap* v) {
	free(v);
}

void demo_2_liste() {

	int k = 2, n = 10;
	Node** l = random(n, k);
	Node** l1 = random(n, k);

	for (int i = 0; i < k; i++) {
		printf("Lista cu nr %d :", i);
		printL(l[i]);
	}

	Node* m = merge(l, k);
	printf("Interclasarea listelor: ");
	printL(m);

	for (int i = 0; i < k; i++) {
		freeL(l[i]);
	}
	free(m);
}

void demo() {
	int k = 3, n = 20;
	Node** l = random(n, k);
	Node** l1 = random2(n, k);

	for (int i = 0; i < k; i++) {
		printf("Lista cu nr %d :", i);
		printL(l[i]);
	}

	Node* m = merge(l, k);
	printf("Interclasarea listelor: ");
	printL(m);

	for (int i = 0; i < k; i++) {
		freeL(l[i]);
	}
	free(m);
}


int main() {

	demo();
	printf("\n");
	demo_2_liste();

}