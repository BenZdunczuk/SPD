#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <queue>
#include <vector>

#define ILE_ZESTAWOW_DANYCH 9
#define MAX_ROZM_KOPCA 100

using namespace std;

int ileZadan[ILE_ZESTAWOW_DANYCH]; //ile zadan jest w danym zestawie danych

//struktura modelujaca pojedyncze zadanie do wykonania na maszynie
struct zadanie{
    int nr;         //numer porzadkowy
    int r,p,q;      //parametry r,p,q danego zadania
};

//struktura modelujaca sposob porownan w zbiorze N celem jego poprawnego sortowania
//zbior N ma byc posortowany rosnaco wzgledem parametru r zadan
struct zbiorNieuszer{
    bool operator()(zadanie pierwsze,zadanie drugie){
        return pierwsze.r > drugie.r;
    }
};

//struktura modelujaca sposob porownan w zbiorze G celem jego poprawnego sortowania
//zbior G ma byc posortowany malejaco wzgledem parametru q zadan
struct zbiorGotowy{
    bool operator()(zadanie pierwsze,zadanie drugie){
        return pierwsze.q < drugie.q;
    }
};

//funkcja realizujaca algorytm schrage bez podzialow
//oparta na pseudokodzie dostarczonym przez dr. inz. Makuchowskiego
//parametry:
//  tabZad - tablica zawierajaca zadania z jednego zestawu danych
//  ileZad - zmienna okreslajaca ile zadan jest w zestawie danych
//
//funkcja zwraca obliczona wartosc cmax oraz modyfikuje parametr tabZad
//
int szrejder(array<zadanie,MAX_ROZM_KOPCA> &tabZad,int const ileZad){
    array<zadanie,MAX_ROZM_KOPCA> tabPermutacji;    //tabela permutacji PI

    priority_queue<zadanie,vector<zadanie>,zbiorNieuszer> zbiorN;
    priority_queue<zadanie,vector<zadanie>,zbiorGotowy> zbiorG;

	int tabPermIdx = 0;     //pozycja w permutacji k,oraz liczba wykonanych zadan
	int t = 0, cmax = 0;    //zmienna czasowa t, oraz wartosc cmax

    //przypisywanie zbiorowi N calego zbioru danych
    for(int i=0;i<ileZad;++i){
        zbiorN.push(tabZad[i]);
    }

	while(!zbiorN.empty() || !zbiorG.empty()){          //krok 2.
		while(!zbiorN.empty() && zbiorN.top().r <= t){  //krok 3.
            zbiorG.push(zbiorN.top());
            zbiorN.pop();                               //krok 4.
		}
		if(!zbiorG.empty()){                            //krok 7.
			tabPermutacji[tabPermIdx] = zbiorG.top();   
            zbiorG.pop();
			t += tabPermutacji[tabPermIdx].p;           //krok 8.
			cmax = max(cmax, t + tabPermutacji[tabPermIdx].q);
			tabPermIdx++;
			continue;
		}
		if(zbiorG.empty() && zbiorN.top().r > t){       //krok 5. oraz pominiecie mozliwych przerw pomiedzy zadaniami
			t = zbiorN.top().r;                         //krok 6.
		}
	}

    tabZad = tabPermutacji;
	return cmax;
}

//funkcja realizujaca algorytm schrage z podzialami
//parametry:
//  tabZad - tablica zawierajaca zadania z jednego zestawu danych
//  ileZad - zmienna okreslajaca ile zadan jest w zestawie danych
//
//funkcja zwraca obliczona wartosc cmax
//
int szrajderZPodzialem(array<zadanie,MAX_ROZM_KOPCA> const &tabZad,int const ileZad){
    //array<zadanie,MAX_ROZM_KOPCA> tabPermutacji;    //tabela permutacji PI
    //tabela permutacji nie jest uzywana gdyz algorytm schrage z podzialami 
    //nie okresla optymalnej permutacji zadan mozliwej do porownania z danymi
    zadanie zadanieL, zadanieE;
    zadanieL.q = 1000000;          //przypisanie q0 duzej wartosci aby zadanie
                                // nie zostalo pominiete w kroku 0

    priority_queue<zadanie,vector<zadanie>,zbiorNieuszer> zbiorN;
    priority_queue<zadanie,vector<zadanie>,zbiorGotowy> zbiorG;

	//int tabPermIdx = 0;
	int t = 0, cmax = 0;        //zmienna czasowa t, oraz wartosc cmax

    //przypisywanie zbiorowi N calego zbioru danych
    for(int i=0;i<ileZad;++i){zbiorN.push(tabZad[i]);}

	while(!zbiorN.empty() || !zbiorG.empty()){          //krok 2.
        while(!zbiorN.empty() && zbiorN.top().r <= t){  //krok 3.
            zadanieE = zbiorN.top();                    //krok 4.
            zbiorN.pop();
            zbiorG.push(zadanieE);
            if(zadanieE.q > zadanieL.q){
                zadanieL.p = t - zadanieE.r;            //krok 5.
                t = zadanieE.r;
                if(zadanieL.p > 0){
                    zbiorG.push(zadanieL);
                }
            }
        }
        if(zbiorG.empty()){                             //krok 6.
            t = zbiorN.top().r;                         //krok 7.
        }else{
            zadanieE = zbiorG.top();                    //krok 8.
            zbiorG.pop();

            zadanieL = zadanieE;                        //krok 9.
            t += zadanieE.p;

			//tabPermutacji[tabPermIdx] = zadanieE;
			cmax = max(cmax, t + zadanieE.q);
			//tabPermIdx++;
        }
	}

    //tabZad = tabPermutacji;
	return cmax;

}

//funkcja uzywana do wyswietlenia (uporzadkowanego) zbioru danych
void wyswietlKolejnosc(array<zadanie,MAX_ROZM_KOPCA> &tablicaZad,int ileZad){
    for(int i=0; i<ileZad; ++i){
        cout << tablicaZad[i].nr << " ";
    }
}

//funckja wczytajaca dane oddzielnie dla kazdego zestawu danych
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
    array<zadanie,MAX_ROZM_KOPCA> tablicaZadan; //zmienna przechowujaca dane z
                                                //pojedynczego zestawu danych
    if(argc < 2){
        cout << "Brak nazwy pliku z danymi podanej w argumencie wywolania funkcji!" << endl;
        return 0;
    }
    ifstream dane(argv[1]);         //wczytanie danych z pliku podanego w argumencie
                                    //wywolania funkcji

    for (int i=0;i<ILE_ZESTAWOW_DANYCH;i++){
        wczytajDane(dane,i,tablicaZadan);
        cout << endl << "Dane nr. 00" << i << ":" << endl;
        cout << "Algorytm Schrage z podzialami:" << endl;
        cout << "cmax: " << szrajderZPodzialem(tablicaZadan,ileZadan[i]) << endl;
        cout << "Algorytm Schrage:" << endl;
        cout << "cmax: " << szrejder(tablicaZadan,ileZadan[i]) << endl;
        cout << "kolejnosc: ";
        wyswietlKolejnosc(tablicaZadan,ileZadan[i]);
        cout << endl;
    }

    dane.close();
}