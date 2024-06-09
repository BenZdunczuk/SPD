#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <climits>

using namespace std;

//funkcja oblicza cmax
int cmax(int N, int M, int *P, int *X){
    int T[100];
    for(int m = 0;m <= M;++m){
        T[m] = 0;
    }
    for(int n = 0; n < N;++n){
        for(int m = 1; m <= M;++m){
            T[m] = max(T[m], T[m - 1]) + P[(m - 1) + X[n] * M];
        }
    }
    return T[M];
}

//sortowanie quicksort
void quickSort(int *tab, int *tab2, int lewy, int prawy){
	if(prawy <= lewy) return;
	
	int i = lewy - 1, j = prawy + 1, 
	pivot = tab[(lewy+prawy)/2];
	
	while(1){
		while(pivot>tab[++i]);
		while(pivot<tab[--j]);
		if(i <= j){
			swap(tab[i],tab[j]);
            swap(tab2[i],tab2[j]);
        }else{break;}
	}

	if(j > lewy)
	quickSort(tab,tab2, lewy, j);
	if(i < prawy)
	quickSort(tab,tab2, i, prawy);
}

// funkcja przydziela wagi procesom oraz sortuje je wg wag
void wazenie(int N, int M, int *P, int *X){
    int *W = new int[N];
    for (int c = 0; c < N;++c){
        for (int d = 0; d < M;++d){
            W[c] += P[c * M + d];
        }
    }
    quickSort(W,X,0,N-1);
    // for (int b = 0; b < N - 1; b++){
    //     for (int a = 0; a < N - 1; a++)
    //     {
    //         if (W[a] < W[a + 1]){
    //             swap(W[a], W[a + 1]);
    //             swap(X[a], X[a + 1]);
    //         }
    //     }
    // }
    delete[] W;
}

//funkcja realizuje algorytm neh
int NEH(int N, int M, int *P, int *X){
    wazenie(N, M, P, X); //zwraca posortowane procesy wg wag
    for(int n = 0;n < N;++n){
        int bestP = -1, bestCmax = INT_MAX;
        for(int p = n;p >= 0;--p){
            int tmp = cmax(n + 1, M, P, X);
            if(bestCmax >= tmp){
                bestCmax = tmp;
                bestP = p;
            }
            if(p){
                swap(X[p], X[p - 1]);
            }
        }
        for(int i = 0;i < bestP;++i){
            swap(X[i], X[i + 1]);
        }
    }
    return cmax(N, M, P, X);
}

int main(int argc, char** argv){
    int N;          //liczba zadan do odczytania z pliku z danymi
    int M;          //liczba maszyn do odczytania z pliku z danymi
    int P[10000];   //parametry zadan
    int X[1000];    //numer zadania
    int x;          //zmienna pomocnicza, tymczasowa
    string data = "data.", data1, data2; //zmienne tekstowe do wyswietlania i rozrozniania zestawow danych

    if(argc < 1){
        cout << "Brak nazwy pliku z danymi podanej w argumencie wywolania funkcji!" << endl;
        return 0;
    }
    ifstream f(argv[1]); // otworzenie strumienia wczytywania danych z pliku

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0;i < 121;++i){
        if (i < 10){
            data1 = data + "00" + to_string(i) + ":";
        }
        else if (i < 100){
            data1 = data + "0" + to_string(i) + ":";
        }
        else{
            data1 = data + to_string(i) + ":";
        }
        while (data2 != data1){
            f >> data2;
        }
        f >> N >> M;
        
        for (int j = 0;j < N * M;++j){
            f >> P[j];
            //cout << "P[j]: " << P[j] << endl;
        }
        for (int j = 0;j < N;++j){
            X[j] = j;
            //cout << "X[j]: " << X[j] << endl;
        }
        auto start1 = chrono::high_resolution_clock::now();
        x = NEH(N, M, P, X);
        auto end1 = chrono::high_resolution_clock::now();
        auto duration1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1);
        cout << data1 << " cmax: " << x << " czas: " << duration1.count() * 1e-9 << "s" << endl;
        //cout << data1 << " cmax: " << x << endl;
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Czas działania: " << duration.count()*1e-3 << "s" << endl;
    f.close();
}