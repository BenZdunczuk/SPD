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
    public:
    int nr;
    int r,p,q;
};

struct zbiorNieuszer{
    bool operator()(zadanie pierwsze,zadanie drugie){
        return pierwsze.r > drugie.r;
    }
};

struct zbiorGotowy{
    bool operator()(zadanie pierwsze,zadanie drugie){
        return pierwsze.q < drugie.q;
    }
};

int szrejderKopiecSTL(array<zadanie,MAX_ROZM_KOPCA> &tabZad,int ileZad){
    array<zadanie,MAX_ROZM_KOPCA> tabPermutacji;

    vector<zadanie> zbiorN;
    vector<zadanie> zbiorG;

    for(int i=0;i<ileZad;++i){
        zbiorN.push_back(tabZad[i]);
    }

    make_heap(zbiorN.begin(),zbiorN.end(),zbiorNieuszer());
    make_heap(zbiorG.begin(),zbiorG.end(),zbiorGotowy());

	int tabPermIdx = 0;
	int t = 0, cmax = 0;

	while(!zbiorN.empty() || !zbiorG.empty()){
		while(!zbiorN.empty() && zbiorN.front().r <= t){
            zbiorG.push_back(zbiorN.front());
            push_heap(zbiorG.begin(),zbiorG.end(),zbiorGotowy());
            pop_heap(zbiorN.begin(),zbiorN.end(),zbiorNieuszer());
            zbiorN.pop_back();
		}
		if(!zbiorG.empty()){
			tabPermutacji[tabPermIdx] = zbiorG.front();
            pop_heap(zbiorG.begin(),zbiorG.end(),zbiorGotowy());
            zbiorG.pop_back();
			t += tabPermutacji[tabPermIdx].p;
			cmax = max(cmax, t + tabPermutacji[tabPermIdx].q);
			tabPermIdx++;
			continue;
		}
		if(zbiorG.empty() && zbiorN.front().r > t){
			t = zbiorN.front().r;
		}
	}

    tabZad = tabPermutacji;
	return cmax;
}

int szrajderZPodzialemKopiecSTL(array<zadanie,MAX_ROZM_KOPCA> &tabZad,int ileZad){
    array<zadanie,MAX_ROZM_KOPCA> tabPermutacji;
    zadanie zadanieL, zadanieE;
    zadanieL.q = 100000;

    vector<zadanie> zbiorN;
    vector<zadanie> zbiorG;

	//int tabPermIdx = 0;
	int t = 0, cmax = 0;

    for(int i=0;i<ileZad;++i){
        zbiorN.push_back(tabZad[i]);
    }

    make_heap(zbiorN.begin(),zbiorN.end(),zbiorNieuszer());
    make_heap(zbiorG.begin(),zbiorG.end(),zbiorGotowy());

	while(!zbiorN.empty() || !zbiorG.empty()){
        while(!zbiorN.empty() && zbiorN.front().r <= t){
            zadanieE = zbiorN.front();
            pop_heap(zbiorN.begin(),zbiorN.end(),zbiorNieuszer());
            zbiorN.pop_back();

            zbiorG.push_back(zadanieE);
            push_heap(zbiorG.begin(),zbiorG.end(),zbiorGotowy());
            
            if(zadanieE.q > zadanieL.q){
                zadanieL.p = t - zadanieE.r;
                t = zadanieE.r;
                if(zadanieL.p > 0){
                    zbiorG.push_back(zadanieL);
                    push_heap(zbiorG.begin(),zbiorG.end(),zbiorGotowy());
                }
            }
        }
        if(zbiorG.empty()){
            t = zbiorN.front().r;
        }else{
            zadanieE = zbiorG.front();
            pop_heap(zbiorG.begin(),zbiorG.end(),zbiorGotowy());
            zbiorG.pop_back();

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

void wyswietlKolejnosc(array<zadanie,MAX_ROZM_KOPCA> &tablicaZad,int ileZad){
    for(int i=0; i<ileZad; ++i){
        cout << tablicaZad[i].nr << " ";
    }
}

void wczytajDane(ifstream &dane,int i,array<zadanie,MAX_ROZM_KOPCA> &tablicaZad){
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
    array<zadanie,MAX_ROZM_KOPCA> tablicaZadan;

    if(argc < 2){
        cout << "Brak nazwy pliku z danymi podanej w argumencie wywolania funkcji!" << endl;
        return 0;
    }
    ifstream dane(argv[1]);

    for (int i=0;i<ILE_ZESTAWOW_DANYCH;i++){
        wczytajDane(dane,i,tablicaZadan);
        cout << endl << "Dane nr. 00" << i << ":" << endl;
        cout << "Algorytm Schrage z podzialami:" << endl;
        cout << "cmax: " << szrajderZPodzialemKopiecSTL(tablicaZadan,ileZadan[i]) << endl;
        cout << "Algorytm Schrage bez podzialow:" << endl;
        cout << "cmax: " << szrejderKopiecSTL(tablicaZadan,ileZadan[i]) << endl;
        cout << "kolejnosc: ";
        wyswietlKolejnosc(tablicaZadan,ileZadan[i]);
        cout << endl;
    }

    dane.close();
}