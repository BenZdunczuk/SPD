#include <iostream>
#include <fstream>
using namespace std;
struct zadanie
{
	int czas;				//czas trwania
	int waga;				//ważność zadania
	int deadline;			//do kiedy ma się wykonać
};

void zamien(zadanie z[], int a, int b){
 zadanie nee;
 nee.czas = z[a].czas;
 nee.waga = z[a].waga;
 nee.deadline = z[a].deadline;
 z[a].czas = z[b].czas;
 z[a].waga = z[b].waga;
 z[a].deadline = z[b].deadline;
 z[b].czas = nee.czas;
 z[b].waga = nee.waga;
 z[b].deadline = nee.deadline;
}
int setup(int size, zadanie ta[]){
		int N = 1 << size;
		int c=0;
		int F[N];
		for (int set = 1; set < N; set++)
		{
			c = 0;
			for (int i = 0, b = 1; i < size; i++, b *= 2)
			{
				if (set & b)
				{
					c += ta[i].czas;
					
				}
			}
			
			F[set] = 999999;
			for (int k = 0, b = 1; k < size; k++, b *= 2)
			{
				if (set & b)
				{
					F[set] = min(F[set], F[set - b] + ta[k].waga * max(c - ta[k].deadline, 0));
					
				}
			}
		}
		return F[N-1];
}
int setup2(int size, zadanie ta[]){
		int witi=0;
		
		for (int i=0; i < size;++i){
			int delay=0;
			int prev=0;
			for (int j=0; j <= i;++j){
				prev += ta[j].czas;
			}
			delay = prev - ta[i].deadline;
			if (delay < 0){delay =0;}
			witi += (ta[i].waga*delay);
		}
		return witi;
}
int main()
{
	
	ifstream wczytaj("witi.data.txt");
	int n, c;
	string s;
	string s1[11] = {"data.10:", "data.11:","data.12:","data.13:","data.14:","data.15:","data.16:","data.17:","data.18:","data.19:","data.20:"};
	for (int iter = 0; iter < 11; iter++)		//pętla przejeżdzająca po wszystkich zbiorach danych
	{
		zadanie tablica[100];
		while (s != s1[iter])			//szukanie odpowiedniego zbioru danych
		{
			wczytaj >> s;
		}
		cout <<"Dane "<<s<<endl;
		wczytaj >> n;					//wczytanie ilości zadań
		for (int i = 0; i < n; i++)		//wczytanie parametrów kolejnych zadań
		{
			wczytaj >> tablica[i].czas >> tablica[i].waga >> tablica[i].deadline;
		}
		
		int N = 1 << n;
		int* F = new int[N];
		F[0] = 0;
		int* Tab = new int[N];
		int sol = setup2(n,tablica);
		for (int h=0;h<10;h++){
			
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					zamien(tablica, i, j);
					int sol2 = setup2(n,tablica);
					if (sol2 >= sol)
					{
						zamien(tablica, i, j);
					}
					else
					{
						sol = sol2;
					}
				}
			}
		}
		sol = setup(n,tablica);
		cout << "Optymalne rozwiazanie: " << sol << endl;
		cout << "Permutacje: ";
		for (int plo=0;plo < n;plo++){cout << tablica[plo].czas << " ";}
		delete[] F;
		delete[] Tab;
		cout<<endl;
	}
	wczytaj.close();
	cout <<endl<< "Koniec dzialania programu." << endl;
	//cin.get();

}