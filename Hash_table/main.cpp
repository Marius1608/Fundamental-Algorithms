/*

 Pantea Marius Nicusor
 -Se foloseste verificare patratica
 -Inserarea se faca pana cand se gaseste un loc gol
  Se incearca sa se plaseze un elemnete de n ori ,adica de dimensiunea tabelei
  Pana cand poate gaseste un loc neucupat

 -Cautarea-Cautam pana gasim cheia respectiva

 -"EfortMediuGasite" creste proportional cu factorul de umplere
 -Mai multe locuri ocupate inseamna ,mai greu de gasit un element

 */

#include <stdio.h>
#include "Profiler.h"
#define Nr 11
#define N 10007

Profiler p("hash");

struct Entry {
    int id;
    char name[30];
    int isDeleted;
};

int Hash_demo(int i, int key) {
    return (7 * i * i + 13 * i + key) % Nr;
}
void inserare_demo(Entry table[], int key , const char name[])
{
    int k;
    for (int  i = 0; i < Nr; i++)
    {
        k = Hash_demo(i, key);
        if (table[k].id == -1)
        {
            table[k].id = key;
            strcpy(table[k].name, name);
            return;
        }
    }
}
int cautare_demo( Entry hTable[], int key) {
    int k,verif=0;
    for (int i = 0; i < Nr; i++)
    {
        verif++;
        k= Hash_demo(i, key);
        if (hTable[k].id == -1){
            printf("Negasit"); return 0;
        }

        else
        {
            if (hTable[k].id == key){
                printf("Gasit"); return 1;
            }
        }
    }
    if (verif == Nr) {
        printf("Negasit"); return 0;
    }
}
void stergere_demo(Entry hTable[], int key) {

    int k, verif;
    for (int i = 0; i < Nr; i++) {
        verif++;
        k = Hash_demo(i, key);
        if (hTable[k].id == -1) {
            printf("Negasit %d\n", key);
            return;
        } else {
            if (hTable[k].id == key) {
                hTable[k].isDeleted = 1; // Mark the entry as deleted
                printf("Element cu cheia %d sters \n", key);
                return;
            }
        }
    }
    if (verif == Nr) {
        printf("Negasit,%d\n", key);
    }
}


void afisare( Entry hTable[],int n) {
    printf("Tabelul de dispersie:\n");
    for (int i = 0; i < n; i++) {
        printf("%d: ", i);
        if (hTable[i].id != 0) {
            printf("Id: %d, Nume: %s", hTable[i].id, hTable[i].name);
        }
        else {
            printf("Nici un elment");
        }
        printf("\n");
    }
}
void initializare_str(Entry table[], int n) {

    int i;
    for (i = 0; i < n; i++)
    {
        table[i].id = -1;
        strcpy(table[i].name,"Fara Nume");
        table[i].isDeleted = 0;
    }
}

void demo() {

    Entry hTable[Nr];
    initializare_str(hTable, Nr);

    inserare_demo(hTable, 4, "Audi");
    inserare_demo(hTable, 5, "Bmw");
    inserare_demo(hTable, 2, "Toyota");
    inserare_demo(hTable, 7, "Ford");
    inserare_demo(hTable, 4, "Mazda");
    inserare_demo(hTable, 3, "Skoda");
    inserare_demo(hTable, 1, "Porche");
    afisare(hTable,Nr);
    cautare_demo(hTable,2);
    printf("\n");

    stergere_demo(hTable,2);
    afisare(hTable,Nr);

}

double factorUmplere[] = {0.8, 0.85 , 0.9 , 0.95 , 0.99};
int cautari=0;
int Hash(int i,int key){
    return (7 * i * i + 13 * i + key) % N;
}
void inserare(Entry table[], int key){
    int k;
    for (int  i = 0; i < Nr; i++)
    {
        k = Hash(i, key);
        if (table[k].id == -1)
        {
            table[k].id = key;
            return;
        }
    }

}
int cautare( Entry hTable[], int key){
    int k;
    for (int i = 0; i < Nr; i++)
    {
        cautari=cautari+1;
        k= Hash(i, key);
        if (hTable[k].id == -1)
             return 0;
        else
        {
            if (hTable[k].id == key) return 1;
        }
    }

}
void perf(Entry hTable[]){
    FILE *f;
    f = fopen("tabel.csv", "w");

    fprintf(f,"FactorUmplere;EfortMediuGasite;EfortMaximGasite;EfortMediuNegasite;EfortMaximNegasite\n");
    for (int i = 0; i < 5; i++) ///pentru fiecare factor de umplere fac 5 teste
    {

        double gasitMediu = 0,gasitNMediu=0;
        int maxGasit=0,maxNegasit=0;
        for (int j = 0; j < 5; j++)
        {
            int a[N];
            int nr = (int)(factorUmplere[i] * N);
            //printf("%d\n",nr);
            FillRandomArray(a, nr, 1, 25000,true);
            (hTable, N);
            for (int k=0; k < nr; k++)
            {
                inserare(hTable, a[k]);
            }


            int K=0,Mf=0;
            double cautariMediu=0;
            for (int k1 = 0; k1 < 1500; k1++)
            {
                cautari=0;
                int found = cautare(hTable, a[K]); /// Caută valoarea în tabela de dispersie
                K = K + (nr / 1500); /// Actualizez indicele pentru a evita suprapunerea valorilor
                if (cautari > Mf) Mf= cautari;/// Actualizez efortul maxim și efortul mediu dacă este necesar
                cautariMediu = cautariMediu + cautari;
            }
            cautariMediu /= 1500.0; /// Calculez efortul mediu pentru valorile găsite
            gasitMediu+= cautariMediu;
            maxGasit += Mf;


            int K1= 26000,Mnf = 0;
            double CautariNotMediu = 0;
            for (int k2 = 0; k2 < 1500; k2++)
            {
                cautari=0;
                int found2 = cautare(hTable, K1); /// Caut o valoare care nu se află în tabela de dispersie
                K1= K1 + 500;   ///Actualizez indicele pentru a simula o căutare într-un spațiu liber

                if (cautari> Mnf) Mnf = cautari; /// Actualizez efortul maxim și efortul mediu dacă este necesar
                CautariNotMediu = CautariNotMediu + cautari;
            }
            CautariNotMediu /= 1500.0;
            gasitNMediu += CautariNotMediu;
            maxNegasit +=Mnf;
        }
        fprintf(f,"%.2lf;%.3lf;%d;%.3lf;%d\n",factorUmplere[i],gasitMediu/5,maxGasit/5,gasitNMediu/5,maxNegasit/5);

    }
    fclose(f);
}
void apelare(){
    cautari=0;
    Entry hTable[N];
    initializare_str(hTable,N);
    perf(hTable);
}

int main() {

    //demo();
    apelare();
    return 0;
}
