#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

struct zadanie{
    int id;
    int r;
    int p;
    int q;
};

int dlugosc(zadanie zadania[], int rozm){
    int m = 0, c = 0;
    for (int i=0;i<rozm;i++){
        m = max(m, zadania[i].r) + zadania[i].p;
        c = max(c, m + zadania[i].q);
    }
    return c;
}

void zamien(zadanie A, zadanie B){
    zadanie tmp;
    tmp = A;
    A = B;
    B = tmp;
}

void zamien(zadanie tab[], int a, int b){
    zadanie tmp;

    tmp.id = tab[a].id;
    tmp.r = tab[a].r;
    tmp.q = tab[a].q;
    tmp.p = tab[a].p;

    tab[a].id = tab[b].id;
    tab[a].r = tab[b].r;
    tab[a].p = tab[b].p;
    tab[a].q = tab[b].q;

    tab[b].id = tmp.id;
    tab[b].r = tmp.r;
    tab[b].p = tmp.p;
    tab[b].q = tmp.q;
}

void szeregowanie(zadanie zadania[], int rozm){
    int dlugosc1 = dlugosc(zadania, rozm);
    int dlugosc2;

    for (int k=0;k<10;k++){
        for (int i=0;i<rozm;i++){
            for (int j=0;j<rozm;j++){
                //zamien(zadania[i], zadania[j]);
                zamien(zadania,i,j);
                dlugosc2 = dlugosc(zadania, rozm);

                if (dlugosc2 > dlugosc1){
                    zamien(zadania,i,j);
                    //zamien(zadania[i], zadania[j]);
                }else{
                    dlugosc1 = dlugosc2;
                }
            }
        }
    }
}

void wyswietlKolejnosc(zadanie zadania[], int rozm){
    cout << "Kolejnosc wykonywania zadan:" << endl;
    for (int i=0;i<rozm;i++){
        cout << zadania[i].id << " ";
    }
    cout << endl;
}

int main(){
    string odczyt;
    string data[4] = {"data.1", "data.2", "data.3", "data.4"};
    int n;
    int czasTrwania = 0;
    int dlugosciZbioruDanych[4];
    zadanie zadania[100];
    ifstream dane("data.txt");
    for(int i=0;i<4;i++){
        cout << "Zbior danych: " << i + 1 << endl;
        while(odczyt!=data[i]){
            dane >> odczyt;
        }
        dane >> n;

        for(int j=0;j<n;j++){
            zadania[j].id = j + 1;
            dane >> zadania[j].r;
            dane >> zadania[j].p;
            dane >> zadania[j].q;
        }
        szeregowanie(zadania,n);
        dlugosciZbioruDanych[i] = dlugosc(zadania,n);
        czasTrwania += dlugosc(zadania,n);
        wyswietlKolejnosc(zadania,n);
        cout << endl;
    }
    for(int i=0;i<4;i++){
        cout << "Dlugosc " << i+1 << " zbioru danych: " << dlugosciZbioruDanych[i] << endl;
    }
    cout << "Laczna dlugosc czasu trwania: " << czasTrwania << endl;
    dane.close();
    return 0;
}
