#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <queue>
#include <vector>
#include <algorithm>

#define ILE_ZESTAWOW_DANYCH 9
#define MAX_ROZM_KOPCA 100

using namespace std;

int ileZadan[ILE_ZESTAWOW_DANYCH]; //ile zadan jest w danym zestawie danych

struct zadanie{
    int nr;
    int r,p,q;

    //przeciazenie operatora przypisania dla latwiejszego przypisywania calych zadan
    zadanie operator=(zadanie prawy){
        nr = prawy.nr;
        r = prawy.r;
        p = prawy.p;
        q = prawy.q;
        return *this;
    }
};

//struktura modelujaca kopiec
//w zaleznosci od atrybutu czyMax kopiec moze byc MAX lub MIN
//atrybuty:
//  tab - tablica przechowujaca zawartosc kopca
//  rozm - rozmiar kopca (po dodaniu 1) oraz indeks ostatniego elementu
//          np. kopiec o rozmiarze 1 elementu bedzie mial rozm == 0
//  czyMax - zmienna okreslajaca czy jest to kopiec roznacy czy malejacy
//          kopiec rosnacy jest sortowany po parametrze r zadania
//          natomiast kopiec malejacy po parametrze q zadania
//          aby ulatwic modelowanie odpowiednio zbioru N i G
struct kopiec{
    zadanie tab[MAX_ROZM_KOPCA];
    int rozm = -1;
    bool czyMax = 1;  // 1 - kopiec sortowany rosnaco, 0 - kopiec sortowany majelaco

        //konstruktor parametryczny uzywany jedynie do testow
    kopiec(zadanie tabArg[MAX_ROZM_KOPCA],int rozmArg,bool czyMaxArg){
        for(int i=0;i<rozmArg;++i){
            tab[i] = tabArg[i];
        }
        rozm = rozmArg-1;
        czyMax = czyMaxArg;
    }

        //metoda zwracajaca indeks rodzica danej galezi
    int rodzicIdx(int i){return (i-1)/2;}

        //metoda zwracajaca indeks lewego syna danego rodzica
    int lewySynIdx(int i){return (2*i + 1);}

        //metoda zwracajaca indeks prawego syna danego rodzica
    int prawySynIdx(int i){return (2*i + 2);}

        //metoda okreslajaca czy kopiec jest pusty
    bool czyPusty(){return (rozm == -1) ? true : false;}

        //metoda zwracajaca wierzcholek kopca
    zadanie top(){return tab[0];}
    
        //metoda wyrownujaca kopiec po dodaniu nowego elementu
    void wyrownajWGore(int i){
        if(czyMax){
            while(i > 0 && tab[rodzicIdx(i)].q < tab[i].q){
                swap(tab[rodzicIdx(i)], tab[i]);
                i = rodzicIdx(i);
            }
        }else{
            while(i > 0 && tab[rodzicIdx(i)].r > tab[i].r){
                swap(tab[rodzicIdx(i)], tab[i]);
                i = rodzicIdx(i);
            }
        }
    }
    
        //metoda wyrownujaca kopiec po usunieciu wierzchniego elementu
    void wyrownajWDol(int i){
        int maxIdx = i;
        int l = lewySynIdx(i);
        if(czyMax){
            if(l <= rozm && tab[l].q > tab[maxIdx].q){
                maxIdx = l;
            }
        }else{
            if(l <= rozm && tab[l].r < tab[maxIdx].r){
                maxIdx = l;
            }
        }

        int r = prawySynIdx(i);
        if(czyMax){
            if(r <= rozm && tab[r].q > tab[maxIdx].q){
                maxIdx = r;
            }
        }else{
            if(r <= rozm && tab[r].r < tab[maxIdx].r){
                maxIdx = r;
            }
        }

        if(i != maxIdx){
            swap(tab[i], tab[maxIdx]);
            wyrownajWDol(maxIdx);
        }
    }
        //metoda dodajaca nowy element
    void push_kopiec(zadanie nowy){
        rozm = rozm + 1;
        tab[rozm] = nowy;
        wyrownajWGore(rozm);
    }
        //metoda usuwajaca wierzchni element
    void pop_kopiec(){
        tab[0] = tab[rozm];
        rozm = rozm - 1;
        wyrownajWDol(0);
    }
};

int szrejderKopiecWlasny(zadanie tabZad[],int ileZad){
    zadanie tabPermutacji[MAX_ROZM_KOPCA];
    zadanie tabtmp[MAX_ROZM_KOPCA];

    for(int i=0;i<MAX_ROZM_KOPCA;++i){
        tabtmp[i].r = 0;
    }

    kopiec zbiorN(tabZad,ileZad,0);
    kopiec zbiorG(tabtmp,0,1);

    //sortowanie tablicy zadan wzgledem r aby w zbiorze N dane byly juz posortowane
    for(int i=0;i<ileZad-1;++i){
        for(int j=0;j<ileZad-i-1;++j){
            if(zbiorN.tab[j].r > zbiorN.tab[j+1].r){
                swap(zbiorN.tab[j],zbiorN.tab[j+1]);
            }
        }
    }

	int tabPermIdx = 0;
	int t = 0, cmax = 0;

	while(!zbiorN.czyPusty() || !zbiorG.czyPusty()){
		while(!zbiorN.czyPusty() && zbiorN.top().r <= t){
            zbiorG.push_kopiec(zbiorN.top());
            zbiorN.pop_kopiec();
		}
		if(!zbiorG.czyPusty()){
			tabPermutacji[tabPermIdx] = zbiorG.top();
            zbiorG.pop_kopiec();
			t += tabPermutacji[tabPermIdx].p;
			cmax = max(cmax, t + tabPermutacji[tabPermIdx].q);
			tabPermIdx++;
			continue;
		}
		if(zbiorG.czyPusty() && zbiorN.top().r > t){
			t = zbiorN.top().r;
		}
	}

    for(int i=0;i<tabPermIdx;++i){
        tabZad[i] = tabPermutacji[i];
    }
	return cmax;
}

int szrajderZPodzialemKopiecWlasny(zadanie tabZad[],int ileZad){
    zadanie tabPermutacji[MAX_ROZM_KOPCA];
    zadanie tabtmp[MAX_ROZM_KOPCA];

    zadanie zadanieL, zadanieE;
    zadanieL.q = 100000;

    kopiec zbiorN(tabZad,ileZad,0);
    kopiec zbiorG(tabtmp,0,1);

    //sortowanie tabliy zadan wzgledem r aby w zbiorze N dane byly juz posortowane
    for(int i=0;i<ileZad-1;++i){
        for(int j=0;j<ileZad-i-1;++j){
            if(zbiorN.tab[j].r > zbiorN.tab[j+1].r){
                swap(zbiorN.tab[j],zbiorN.tab[j+1]);
            }
        }
    }

	//int tabPermIdx = 0;
	int t = 0, cmax = 0;

	while(!zbiorN.czyPusty() || !zbiorG.czyPusty()){
        while(!zbiorN.czyPusty() && zbiorN.top().r <= t){
            zadanieE = zbiorN.top();
            zbiorN.pop_kopiec();
            zbiorG.push_kopiec(zadanieE);
            
            if(zadanieE.q > zadanieL.q){
                zadanieL.p = t - zadanieE.r;
                t = zadanieE.r;
                if(zadanieL.p > 0){
                    zbiorG.push_kopiec(zadanieL);
                }
            }
        }
        if(zbiorG.czyPusty()){
            t = zbiorN.top().r;
        }else{
            zadanieE = zbiorG.top();
            zbiorG.pop_kopiec();

            zadanieL = zadanieE;
            t += zadanieE.p;

			//tabPermutacji[tabPermIdx] = zadanieE;
			cmax = max(cmax, t + zadanieE.q);
			//tabPermIdx++;
        }
	}

    //tabZad = tabPermutacji;
	return cmax;
}

void wyswietlKolejnosc(zadanie tablicaZad[],int ileZad){
    for(int i=0; i<ileZad; ++i){
        cout << tablicaZad[i].nr << " ";
    }
}

void wczytajDane(ifstream &dane,int i,zadanie tablicaZad[]){
    string s = "data.", s1, s2;

    s1 = s + "00" + to_string(i) + ":";

    while (s2 != s1){
        dane >> s2;
    }
    dane >> ileZadan[i];
    for(int j=0;j<ileZadan[i];++j){
        tablicaZad[j].nr = j+1;
        dane >> tablicaZad[j].r;
        dane >> tablicaZad[j].p;
        dane >> tablicaZad[j].q;
    }
}

int main(int argc,char** argv){
    zadanie tablicaZadan[MAX_ROZM_KOPCA];

    if(argc < 2){
        cout << "Brak nazwy pliku z danymi podanej w argumencie wywolania funkcji!" << endl;
        return 0;
    }
    ifstream dane(argv[1]);

    for (int i=0;i<ILE_ZESTAWOW_DANYCH;i++){
        wczytajDane(dane,i,tablicaZadan);
        cout << endl << "Dane nr. 00" << i << ":" << endl;
        cout << "Algorytm Schrage z podzialami:" << endl;
        cout << "cmax: " << szrajderZPodzialemKopiecWlasny(tablicaZadan,ileZadan[i]) << endl;
        cout << "Algorytm Schrage bez podzialow:" << endl;
        cout << "cmax: " << szrejderKopiecWlasny(tablicaZadan,ileZadan[i]) << endl;
        cout << "kolejnosc: ";
        wyswietlKolejnosc(tablicaZadan,ileZadan[i]);
        cout << endl;
    }

    dane.close();
}