#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <queue>
#include <vector>
#include <climits>

using namespace std;

#define ILE_ZESTAWOW_DANYCH 9

int ileZadantTab[ILE_ZESTAWOW_DANYCH]; //ile zadan jest w danym zestawie danych

struct zadanie{
    public:
    int nr;         //numer porzadkowy zadania
    int r,p,q;      //parametry zadan
    int t;          //chwila czasu w ktorym dane zadanie sie wykonuje

        //konstruktor parametryczny
    zadanie(int NR,int R,int P,int Q){
        nr = NR;
        r = R;
        p = P;
        q = Q;
    }
        //konstruktor bezparametryczny (domyslny)
    zadanie(){
        nr = -1;
        r = 0;
        p = 0;
        q = 0;
    }
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

int Schrage(array<zadanie,100> &tabZad,int ileZad){
    array<zadanie,100> tabPermutacji;

    priority_queue<zadanie,vector<zadanie>,zbiorNieuszer> zbiorN;
    priority_queue<zadanie,vector<zadanie>,zbiorGotowy> zbiorG;

	int tabPermIdx = 0;
	int t = 0, cmax = 0;

    for(int i=0;i<ileZad;++i){
        zbiorN.push(tabZad[i]);
    }

	while(tabPermIdx != ileZad){
		while(!zbiorN.empty() && zbiorN.top().r <= t){
            zbiorG.push(zbiorN.top());
            zbiorN.pop();
		}
		if(!zbiorG.empty()){
			tabPermutacji[tabPermIdx] = zbiorG.top();
            zbiorG.pop();
			t += tabPermutacji[tabPermIdx].p;
			cmax = max(cmax, t + tabPermutacji[tabPermIdx].q);
            tabPermutacji[tabPermIdx].t = t;
			tabPermIdx++;
			continue;
		}
		if(zbiorG.empty() && zbiorN.top().r > t){
			t = zbiorN.top().r;
		}
	}

    tabZad = tabPermutacji;
	return cmax;
}

int SchrageZPodzialem(array<zadanie,100> &tabZad,int ileZad){
    array<zadanie,100> tabPermutacji;
    zadanie zadanieL, zadanieE;
    zadanieL.q = 10000;

    priority_queue<zadanie,vector<zadanie>,zbiorNieuszer> zbiorN;
    priority_queue<zadanie,vector<zadanie>,zbiorGotowy> zbiorG;

	int t = 0, cmax = 0;

    for(int i=0;i<ileZad;++i){
        zbiorN.push(tabZad[i]);
    }

	while(!zbiorN.empty() || !zbiorG.empty()){
        while(!zbiorN.empty() && zbiorN.top().r <= t){
            zadanieE = zbiorN.top();
            zbiorN.pop();
            zbiorG.push(zadanieE);
            if(zadanieE.q > zadanieL.q){
                zadanieL.p = t - zadanieE.r;
                t = zadanieE.r;
                if(zadanieL.p > 0){
                    zbiorG.push(zadanieL);
                }
            }
        }
        if(zbiorG.empty()){
            t = zbiorN.top().r;
        }else{
            zadanieE = zbiorG.top();
            zbiorG.pop();
            zadanieL = zadanieE;
            t += zadanieE.p;
			cmax = max(cmax, t + zadanieE.q);
        }
	}
	return cmax;

}

void wyswietlKolejnosc(array<zadanie,100> &tablicaZad,int ileZad){
    for(int i=0; i<ileZad; ++i){
        cout << tablicaZad[i].nr << " ";
    }
    cout << endl;
}

void wczytajDane(ifstream &dane,int i,array<zadanie,100> &tablicaZad){
    string s = "data.", s1, s2;

    s1 = s + "00" + to_string(i) + ":";

    while (s2 != s1){
        dane >> s2;
    }
    dane >> ileZadantTab[i];
    for(int j=0;j<ileZadantTab[i];++j){
        tablicaZad[j].nr = j+1;
        dane >> tablicaZad[j].r;
        dane >> tablicaZad[j].p;
        dane >> tablicaZad[j].q;
    }
}

int Carlier(int ileZad, array<zadanie,100> &tablicaZad, int &UB){
    int a=0, b=0, c=-1;     //wartosci poczatkowe
	int pSuma=0;
	int noweR=INT_MAX;   //duze wartosci r i q zadania c do minimalizowania parametrow
	int noweQ=INT_MAX;
    zadanie C;              //zmienna do tymczasowego przechowywania param. zadania c
    
    int LB=0;
    int U = Schrage(tablicaZad,ileZad);

    if(U < UB){UB = U;}

    //szukanie drogi krytycznej (a,b)
    //szukanie b
    b=ileZad-1;
	for(int i=ileZad-1; i>0; i--){
	    if(U == (tablicaZad[i].t + tablicaZad[i].q)){
	        b=i;
	        break;
	    }
    }

    //szukanie a
    int sum_a;
	for(a=0;a<=b;++a){
	    sum_a = 0;
	    for(int i=a;i<=b;++i){
            sum_a += tablicaZad[i].p;
        }
	    if(U == (tablicaZad[a].r + sum_a + tablicaZad[b].q)){break;}
	}

    //szukanie zadania interferencyjnego c
	for(int i=b;i>=a;--i){	
	    if(tablicaZad[i].q < tablicaZad[b].q){
		    c=i;
		    break;
	    }
	}
    
    //jezeli nie znaleziono zadania C
    if(c==-1){return UB;}

    C.nr = tablicaZad[c].nr;
    C.r = tablicaZad[c].r;
	C.q = tablicaZad[c].q;

    for(int i=c+1; i<=b; i++){
		noweR = min(noweR, tablicaZad[i].r);
		noweQ = min(noweQ, tablicaZad[i].q);
		pSuma+= tablicaZad[i].p;
	}
	
        //modyfikacja parametru r zadania c
	tablicaZad[c].r = max(tablicaZad[c].r, noweR + pSuma);

	LB = SchrageZPodzialem(tablicaZad,ileZad);
    if(LB < UB){Carlier(ileZad,tablicaZad,UB);}

	for(int i=0;i<ileZad;++i){
		if(C.nr == tablicaZad[i].nr){
			tablicaZad[i].r = C.r;
		}
	}
        //modyfikacja parametru q zadania c
	tablicaZad[c].q = max(tablicaZad[c].q, noweQ + pSuma);

	LB = SchrageZPodzialem(tablicaZad,ileZad);
	if(LB < UB){Carlier(ileZad,tablicaZad,UB);}

	for(int i=0;i<ileZad;++i){
		if(C.nr == tablicaZad[i].nr){
			tablicaZad[i].q = C.q;
		}
	}
	return UB;
}

int main(int argc,char** argv){
    array<zadanie,100> tablicaZadan;

	ifstream dane(argv[1]);
	for (int i = 0; i < ILE_ZESTAWOW_DANYCH; i++){
        int UB = INT_MAX;
        wczytajDane(dane,i,tablicaZadan);
        cout << endl << "Dane nr. 00" << i << ":" << endl;
		cout << "Schrage: " << Schrage(tablicaZadan,ileZadantTab[i]) << endl;
		cout << "Schrage z podzialem: " << SchrageZPodzialem(tablicaZadan,ileZadantTab[i]) << endl;
		cout << "Carlier: " << Carlier(ileZadantTab[i], tablicaZadan, UB) << endl;
	}
}