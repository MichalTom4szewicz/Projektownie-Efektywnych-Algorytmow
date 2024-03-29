// PEA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <mmsystem.h>
#include <ctime>
#include "Licznik.h"
#include <conio.h>

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <math.h>
#include <time.h>

using namespace std;

double PI = 3.141592;
double E = 2.71828182846;
double RRR = 6378.388;

int **odleglosci;
int ko;
int n;


struct trasa
{
	vector<int> droga;

	int koszt;

};

bool niema(int a, vector<int> b)
{
	for (int i = 0; i < b.size(); i++)
	{
		if (b[i] == a)
		{
			return false;
		}
	}

	return true;
}

trasa minimalna;

ostream& operator<<(ostream& os, const vector<int>& a)
{
	for (int i = 0; i<a.size(); i++)
	{
		os << a[i] << " ";

	}
	return os;
}

int licz_koszt(vector<int> tab, int **odleglosci)
{
	int koszt = 0;
	koszt += odleglosci[tab[0]][tab[1]];

	for (int i = 1; i<tab.size() - 1; i++)
	{
		koszt += odleglosci[tab[i]][tab[i + 1]];
	}
	koszt += odleglosci[tab[tab.size() - 1]][tab[0]];

	return koszt;
}



void permu(vector<int> a, int b, int c)
{

	if (b == c)
	{

		ko = licz_koszt(a, odleglosci);
		if (minimalna.koszt>ko)
		{
			minimalna.droga = a;
			minimalna.koszt = ko;
		}
		//cout << a << " " << ko << endl;
		
	}
	else
	{
		for (int i = b; i <= c; i++)
		{
			swap(a[b], a[i]);
			permu(a, b + 1, c);
			swap(a[b], a[i]);
		}

	}
}

int **koszty;
int koniec;
int **miasta;

int dynamic(int mask, int poz, int n)
{
	if (mask == koniec)
	{
		return odleglosci[poz][0];

	}

	if (koszty[mask][poz] !=-1)
	{
		return koszty[mask][poz];
	}

	int odp = 999999;
	
	for (int i = 0; i<n; i++)
	{
		if ((mask &(1 << i)) == 0)
		{
			int tmp = odleglosci[poz][i] + dynamic(mask |(1 << i), i, n);
			if (odp > tmp)
			{
				miasta[mask][poz] = i;
				odp = tmp;
			}
		}
	}

	koszty[mask][poz] = odp;

	return odp;
}


struct osobnik
{
	int koszt;
	vector<int> sciezka;

	bool operator < (const osobnik& str) const
	{
		return (koszt < str.koszt);
	}
};

vector<osobnik> populacja;
vector<osobnik> nowa_populacja;

void krzyzowanie1(vector<int> v1, vector<int> v2)
{
	int wielkosc = v1.size();

	int punkt = 0;

	vector <int> new1;
	vector <int> new2;

	while (punkt < 1 || punkt >= wielkosc - 2)
	{
		punkt = rand() % wielkosc;
	}

	//punkt = 2;
	//cout << "oto punkt: " << punkt << endl;

	for (int i = 0; i <= punkt; i++)
	{
		new1.push_back(v1[i]);

	}
	for (int i = punkt + 1; i <wielkosc; i++)
	{
		new2.push_back(v2[i]);
	}

	for (int j = 0; j<wielkosc; j++)
	{
		if (niema(v2[j], new1))
		{
			new1.push_back(v2[j]);
		}

		if (niema(v1[j], new2))
		{
			new2.push_back(v1[j]);
		}

	}

	osobnik o1;
	o1.sciezka = new1;
	o1.koszt = licz_koszt(new1, odleglosci);
	osobnik o2;
	o2.sciezka = new2;
	o2.koszt = licz_koszt(new2, odleglosci);

	nowa_populacja.push_back(o1);
	nowa_populacja.push_back(o2);

}



void krzyzowanie2(vector<int> v1, vector<int> v2)
{
	//cout << endl;
	//cout << v1 << endl;
	//cout << v2 << endl;

	int wielkosc = v1.size();

	int punkt = 0;
	int punkt1 = 0;

	vector <int> new1(wielkosc);
	vector <int> new2(wielkosc);

	for (int i = 0; i < wielkosc; i++)
	{
		new1[i] = -1;
		new2[i] = -1;
	}

	//vector<int> tabu1;
	//vector<int> tabu2;


	while (punkt < 1 || punkt >= wielkosc - 3)
	{
		punkt = rand() % wielkosc;
	}

	while (punkt1 <= punkt || punkt1 >= wielkosc - 2)
	{
		punkt1 = rand() % wielkosc;
	}

	//punkt = 4;
	//punkt1 = 5;
	//cout << "oto punkt: " << punkt << endl;
	//cout << "oto punkt1: " << punkt1 << endl;

	
	for (int i = punkt+1; i <= punkt1; i++)
	{
		new1[i] = v1[i];
		//tabu1.push_back(v1[i]);
	}

	for (int i = punkt+1; i <= punkt1; i++)
	{
		new2[i] = v2[i];
		//tabu2.push_back(v2[i]);
	}
	
	int k = 0;
	int l1 = 0;
	int l2 = 0;

	while (l1<wielkosc)
	{
		if (new1[l1] != -1)
		{
			l1++;
			k--;
		}

		if (niema(v2[k], new1))
		{
			if (new1[l1] == -1)
			{
				new1[l1] = v2[k];
			}
		}
		else
		{
			l1--;
		}

		l1++;
		k++;
	}
	k = 0;
	while (l2<wielkosc)
	{
		if (new2[l2] != -1)
		{
			l2++;
			k--;
		}

		if (niema(v1[k], new2))
		{
			if (new2[l2] == -1)
			{
				new2[l2] = v1[k];
			}
		}
		else
		{
			l2--;
		}

		l2++;
		k++;

	}
	
	osobnik o1;
	o1.sciezka = new1;
	o1.koszt = licz_koszt(new1, odleglosci);
	osobnik o2;
	o2.sciezka = new2;
	o2.koszt = licz_koszt(new2, odleglosci);

	nowa_populacja.push_back(o1);
	nowa_populacja.push_back(o2);
	


	//cout << new1 << endl;
	//cout << new2 << endl;
	


}
void nwox(vector<int> v1, vector<int> v2)
{
	int wielkosc = v1.size();

	int punkt = 0;
	int punkt1 = 0;

	vector <int> new1 = v1;
	vector <int> new2 = v2;
	vector <int> g1;
	vector <int> g2;


	while (punkt < 1 || punkt >= wielkosc - 3)
	{
		punkt = rand() % wielkosc;
	}

	while (punkt1 <= punkt || punkt1 >= wielkosc - 2)
	{
		punkt1 = rand() % wielkosc;
	}

	//punkt = 1;
	//punkt1 = 5;
	//cout << "oto punkt: " << punkt << endl;
	//cout << "oto punkt1: " << punkt1 << endl;

	for (int i = punkt + 1; i<punkt1 + 1; i++)
	{
		g1.push_back(v1[i]);
		g2.push_back(v2[i]);
	}

	for (int i = 0; i<wielkosc; i++)
	{
		for (int j = 0; j<(punkt1 - punkt); j++)
		{
			if (new1[i] == g2[j])
			{
				new1[i] = -1;
			}
			if (new2[i] == g1[j])
			{
				new2[i] = -1;
			}
		}
	}




	int zm1 = wielkosc - 1;
	int zm2 = wielkosc - 1;
	while (new1.size()> wielkosc - g2.size())
	{
		if (new1[new1.size() - 1] == -1)
		{
			//new1.erase(new1.begin() + (new1.size()-1));
			new1.pop_back();
			zm1--;
		}
		else
		{
			zm1--;
			if (new1[zm1] == -1)
			{
				new1.erase(new1.begin() + zm1);
			}
		}

	}
	//cout<<g1.size()<<" "<<zm2<<endl; 


	while (new2.size()> wielkosc - g1.size())
	{
		if (new2[new2.size() - 1] == -1)
		{
			//new2.erase(new2.begin() + (new2.size()-1));
			new2.pop_back();
			zm2--;
		}
		else
		{
			zm2--;
			if (new2[zm2] == -1)
			{
				new2.erase(new2.begin() + zm2);
				//zm2--;
			}

		}
	}




	for (int i = g1.size() - 1; i>-1; i--)
	{
		new1.insert(new1.begin() + punkt + 1, 1, g2[i]);
		new2.insert(new2.begin() + punkt, 1, g1[i]);
	}


	//cout << new1 << endl;
	//cout << new2 << endl;
	//cout << endl;

	osobnik o1;
	o1.sciezka = new1;
	o1.koszt = licz_koszt(new1, odleglosci);
	osobnik o2;
	o2.sciezka = new2;
	o2.koszt = licz_koszt(new2, odleglosci);

	nowa_populacja.push_back(o1);
	nowa_populacja.push_back(o2);

}

void mut_swap(vector<osobnik> v, double pstwo)
{
	//int a = v.size() / 14;
	int random;
	int p1; 
	int p2;

	for (int i = 0; i < v.size(); i++)
	{
		random = ((double)rand() / (RAND_MAX)) ;
		if (random <= pstwo)
		{
			p1 = rand() % n;
			p2 = rand() % n;
			while (p1 == p2)
			{
				p2 = rand() % n;
			}
			swap(v[i].sciezka[p1], v[i].sciezka[p2]);
			
			v[i].koszt = licz_koszt(v[i].sciezka,odleglosci);
		}

	}
}

void mut_insert(vector<osobnik> v, double pstwo)
{
	//int a = v.size() / 14;
	int random;
	int p1;
	int p2;
	//int a;

	for (int i = 0; i < v.size(); i++)
	{
		random = ((double)rand() / (RAND_MAX)) ;
		if (random <= pstwo)
		{
			p1 = rand() % n;
			p2 = rand() % n;
			while (p2 == p1)
			{
				p2 = rand() % n;
			}
			
			//a = v[i].sciezka[p2];
			v[i].sciezka.insert(v[i].sciezka.begin() + min(p1, p2)+1, 1, v[i].sciezka[max(p1, p2)] );
			v[i].sciezka.erase(v[i].sciezka.begin() + max(p1, p2) + 1);
			//swap(v[i].sciezka[p1], v[i].sciezka[p2]);

			v[i].koszt = licz_koszt(v[i].sciezka, odleglosci);
		}

	}
}


osobnik minikoszt(vector<osobnik> v)
{
	osobnik min;
	min.koszt = v[0].koszt;
	for (int i = 1; i < v.size(); i++)
	{
		if (v[i] < min)
		{
			min = v[i];
		}
	}
	return min;
}






struct city
{
	int numer;
	double latitude;	//te geo latitude y
	double longtitude;	// longtitude x
	double x;	//euc
	double y;

};

int main()
{
	srand(time(NULL));
	
	while (1)
	{

		minimalna.koszt = 999999;

		ifstream plik;
		int typ;
		char alg;


		string nazwa;
		cout << endl<< "Podaj nazwe pliku (bez rozszerzenia.txt)";
		cin >> nazwa;
		cout <<endl<< "Podaj paramery dla algorytmu genettycznego" << endl;


		nazwa = nazwa + ".txt";
		plik.open(nazwa.c_str());

		plik >> n;
		cout <<endl<<"Oto ilosc miast: "<<n << endl;
		

		odleglosci = new int *[n]; //macierz kwadratowa przechowujaca odleglosci miedzy miasatami
		for (int i = 0; i < n; i++)
		{
			odleglosci[i] = new int[n];
		}

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				odleglosci[i][j] = 0;
			}
		}

		plik >> typ;

		if (typ == 2)
		{
			city *t = new city[n];

			for (int i = 0; i < n; i++)
			{
				plik >> t[i].numer;
				plik >> t[i].x; //long dlugosc
				plik >> t[i].y; //latitude szerokosc

				int deg = round(t[i].x);	//tsplib documentation
				double min = t[i].x - deg;
				t[i].latitude = PI * (deg + 5.0 * min / 3.0) / 180.0; //latitude
				deg = round(t[i].y);
				min = t[i].y - deg;
				t[i].longtitude = PI * (deg + 5.0 * min / 3.0) / 180.0; //long
			}



			for (int i = 0; i < n; i++) 	//wypelniamy zerami
			{
				for (int j = 0; j < n; j++)
				{
					odleglosci[i][j] = 0;
				}
			}
			for (int i = 0; i < n; i++) //oobliczamy odleglosci(tsplib documentation)
			{
				for (int j = 0; j < n; j++)
				{
					if (i != j)
					{
						double q1 = cos(t[i].longtitude - t[j].longtitude);
						double q2 = cos(t[i].latitude - t[j].latitude);
						double q3 = cos(t[i].latitude + t[j].latitude);
						double dij = (int)(RRR * acos(0.5*((1.0 + q1)*q2 - (1.0 - q1)*q3)) + 1.0);
						odleglosci[i][j] = dij;
					}
				}
			}
			delete[] t;
		}
		if (typ == 1)
		{


			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					plik >> odleglosci[i][j];

				}
			}

		}
		if (typ == 3)
		{
			int ile = 1;

			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < ile; j++)
				{
					plik >> odleglosci[i][j];
				}
				ile++;
			}


			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					if (odleglosci[i][j] == 0 && (i != j))
					{
						odleglosci[i][j] = odleglosci[j][i];
					}
				}
			}


		}

		plik.close();

		/*
		for(int i=0; i<n; i++)//wypisywanie koorydnatów eo
		{
		cout<<t[i].numer<<" ";
		cout<<t[i].x<<" ";
		cout<<t[i].y<<endl;
		}
		*/


		for (int i = 0; i < n; i++) //wypisujemy macierz odleglosci
		{
			for (int j = 0; j < n; j++)
			{
				//cout << odleglosci[i][j] << " ";
			}
			//cout << endl;
		}

		//gr48 500 4 3 2 1

		/////////////////////////////////////start

		Licznik co; 
		Licznik co1;

		double czas;
		double czas1;
		double chwi;

		double chwila = 9992718; //////////////////////////////////czas do ustawienia w ms

		osobnik najlepszy;
		najlepszy.koszt = 999999999;

		double sredni_koszt=0;
		double sredni_blad=0;
		double sredni_czas = 0;
		int wpokoleniu = 0;

		vector<int> turniej;
		vector<int> turniej2;
		vector<osobnik> turni;
		vector<osobnik> turni2;

		int los;
		

		double wlasciwy = 3454;

		//cout << "Podaj wlasciwy koszt drogi " << endl;
		cin >> wlasciwy;

		int ile_razy = 3; // ile razy algorytm ma sie wykonac
		cin >> ile_razy;

		int pokolenia = 100; //ile nowych pokolen bedzie w iteracji algorytmu
		cin >> pokolenia;

		int wielkosc_populacji=2;
		cin >> wielkosc_populacji;// wielkosc populacji n*<wartosc wpisana>
		wielkosc_populacji *= n;

		int krzyz = 1; //krzyzowanie 1 lub 2 punktowe
		cin >> krzyz;
		double p_krzyzowania = 0.7;

		int selekcja=1; //gdy 1 to posortowane rosnaco kosztami - parami obok siebie sie krzyzuja
						//gdy 2  to nal=jlepsi z najgorszymi i tak dazy do srodka ze sredni ze srednimi
		cin >> selekcja;
		
		double laczenie_pokolen = 0.3; //% starej populacji jaki zostanie wlaczony do newj populacji (polowa najlepszych i polowa najgorszych)
		//cin >> laczenie_pokolen;
		//int wielkosc turnieju

		double p_mutacji = 0.1; // 
		int mut;				//1 to swap, 2 to insert
		//cin >> p_mutacji;
		cin >> mut;
		cin >> p_krzyzowania;
		cin >> p_mutacji;

		//cin >> laczenie_pokolen;


		if (wielkosc_populacji % 2 == 1) wielkosc_populacji++;
		cout << "oto wielkosc populacji: " << wielkosc_populacji << endl << endl;

		int random;
		int random1;
		double random3;
		double random4;


		for (int ile = 0; ile < ile_razy; ile++) //ile razy algorytm ma się wykonać
		{
			populacja.clear();
			nowa_populacja.clear();
			najlepszy.koszt = 999999; wpokoleniu = 0;

			co.StartCounter();

			for (int i = 0; i < wielkosc_populacji; i++)// generowanie populacji poczatkowej
			{
				osobnik o;

				for (int i = 0; i < n; i++)
				{
					o.sciezka.push_back(i);
				}
				for (int i = 0; i < n*n; i++)
				{
					random = rand() % n;
					random1 = rand() % n;
					swap(o.sciezka[random], o.sciezka[random1]);
				}

				o.koszt = licz_koszt(o.sciezka, odleglosci);
				populacja.push_back(o);
			}

			/*cout << "oto nasza populacja" << endl;
			sort(populacja.begin(), populacja.end());
			for (int i = 0; i < wielkosc_populacji; i++)
			{
				cout << populacja[i].sciezka <<"   "<<populacja[i].koszt<< endl;
			}

			cout << "min koszt z naszej populacji: " << minikoszt(populacja).koszt << endl;*/

			/*
			vector<int> a;
			a.push_back(7);
			a.push_back(2);
			a.push_back(0);
			a.push_back(5);
			a.push_back(1);
			a.push_back(4);
			a.push_back(6);
			a.push_back(3);

			vector<int> b;
			for (int i = 0; i < 8; i++)
			{
				b.push_back(i);
			}
			*/
			int pocz;
			najlepszy = minikoszt(populacja);
			//for (int liczn = 0; liczn < pokolenia; liczn++) /////////////////////////////////////////////////// tuuu działa algoprytm
			while(co.GetCounter()<=pokolenia && najlepszy.koszt!= wlasciwy)
			{
				pocz = wielkosc_populacji - 1;

				/*co1.StartCounter();*/

				//czas1 = co.GetCounter();//
				//cout << "1 sort " << czas1 << endl;//

				//if (minikoszt(populacja) < najlepszy) najlepszy = minikoszt(populacja);
				sort(populacja.begin(), populacja.end());
				if (populacja[0] < najlepszy) najlepszy = populacja[0];


				if (krzyz == 1)
				{
					if (selekcja == 1)
					{
						for (int i = 0; i < wielkosc_populacji / 2; i++)
						{
							while (turniej.size() < 5)
							{
								los = rand() % n;
								while (niema(los, turniej))
								{
									turniej.push_back(los);
								}
							} 
							while (turniej2.size() < 5)
							{
								los = rand() % n;
								while (niema(los, turniej2))
								{
									turniej2.push_back(los);
								}
							}

							for (int j = 0; j < turniej.size(); j++)
							{
								turni.push_back(populacja[turniej[j]]);
								turni2.push_back(populacja[turniej2[j]]);
							}
							sort(turni.begin(), turni.end());

							sort(turni2.begin(), turni2.end());

							random3 = ((double)rand() / (RAND_MAX));

							//cout << random3 << endl;
							if (random3 <= p_krzyzowania)
							{
								krzyzowanie1(turni[0].sciezka,turni2[0].sciezka);
							}
							
							turniej.clear();
							turni.clear();
							turniej2.clear();
							turni2.clear();
						}
						//for (int i = 0; i < wielkosc_populacji; i+=2) ///////////////////////////////////////krzyzowanie
						//{
						//	krzyzowanie1(populacja[i].sciezka, populacja[i+1].sciezka);
						//	//pocz--;
						//}
					}
					if (selekcja == 2)
					{
						//sort(populacja.begin(), populacja.end());
						for (int i = 0; i < wielkosc_populacji / 2; i++) ///////////////////////////////////////krzyzowanie
						{
							random3 = ((double)rand() / (RAND_MAX)) ;
							//cout << random3 << endl;
							if (random3 <= p_krzyzowania)
							{
								krzyzowanie1(populacja[i].sciezka, populacja[pocz].sciezka);
							}
							
							//krzyzowanie2(populacja[i].sciezka, populacja[pocz].sciezka);
							//nwox(populacja[i].sciezka, populacja[pocz].sciezka);
							pocz--;

							//krzyzowanie1(populacja[i].sciezka, populacja[pocz].sciezka);
							//pocz--;
						}
					}
				}
				if (krzyz == 2) //nwox
				{
					if (selekcja == 1)
					{
						for (int i = 0; i < wielkosc_populacji / 2; i++)
						{
							while (turniej.size() < 5)
							{
								los = rand() % n;
								while (niema(los, turniej))
								{
									turniej.push_back(los);
								}
							}
							while (turniej2.size() < 5)
							{
								los = rand() % n;
								while (niema(los, turniej2))
								{
									turniej2.push_back(los);
								}
							}

							for (int j = 0; j < turniej.size(); j++)
							{
								turni.push_back(populacja[turniej[j]]);
								turni2.push_back(populacja[turniej2[j]]);
							}
							sort(turni.begin(), turni.end());

							sort(turni2.begin(), turni2.end());

							random3 = ((double)rand() / (RAND_MAX));

							//cout << random3 << endl;
							if (random3 <= p_krzyzowania)
							{
								nwox(turni[0].sciezka, turni2[0].sciezka);
							}
							
							turniej.clear();
							turni.clear();
							turniej2.clear();
							turni2.clear();
						}
						//for (int i = 0; i < wielkosc_populacji; i += 2) ///////////////////////////////////////krzyzowanie
						//{
						//	//krzyzowanie2(populacja[i].sciezka, populacja[i + 1].sciezka);
						//	nwox(populacja[i].sciezka, populacja[i + 1].sciezka);
						//	//pocz--;
						//}
					}
					if (selekcja == 2)
					{
						//sort(populacja.begin(), populacja.end());
						
						for (int i = 0; i < wielkosc_populacji / 2; i++) ///////////////////////////////////////krzyzowanie
						{

							random3 = ((double)rand() / (RAND_MAX)) ;
							//cout << random3 << endl;
							if (random3 <= p_krzyzowania)
							{
								nwox(populacja[i].sciezka, populacja[pocz].sciezka);
							}
							else
							{
								//nowa_populacja.push_back(populacja[i]);
								//nowa_populacja.push_back(populacja[pocz]);
							}
							//krzyzowanie2(populacja[i].sciezka, populacja[pocz].sciezka);
							//nwox(populacja[i].sciezka, populacja[pocz].sciezka);
							pocz--;
						}
						
					}
				}

				//if (minikoszt(populacja) < najlepszy) najlepszy = minikoszt(populacja);

				sort(nowa_populacja.begin(), nowa_populacja.end());
				if (nowa_populacja[0] < najlepszy) najlepszy = nowa_populacja[0];
				
				int wp = wielkosc_populacji * laczenie_pokolen;
				if (wp % 2 == 1) wp++;

				/*if (wp != 0) // to gdy chcemy miec staly % udzialu starej populacji w nowej
				{
					for (int i = 0; i < wp; i++)
					while(nowa_populacja.size()>wp)
					{
						nowa_populacja.pop_back();
					}

					for (int i = 0; i < wp / 2; i++)
					int cos = 0;
					while(nowa_populacja.size()<wielkosc_populacji)
					{
						nowa_populacja.push_back(populacja[cos]);
						if (nowa_populacja.size() == wielkosc_populacji)
						{
							nowa_populacja.push_back(populacja.back());
							populacja.pop_back();
						}
						cos++;
						
					}
				}*/

				//////////////////////////////////////////////////////     a tu gdy starą populacją dopełniamy do stałej wielkości
				int cos = 0;
				while (nowa_populacja.size()<wielkosc_populacji)
				{
					nowa_populacja.push_back(populacja[cos]);
					if (nowa_populacja.size() == wielkosc_populacji)
					{
						nowa_populacja.push_back(populacja.back());
						populacja.pop_back();
					}
					cos++;

				}
				///



				populacja.clear();

				for (int i = 0; i < wielkosc_populacji; i++)
				{
					populacja.push_back(nowa_populacja[i]);
				}
				nowa_populacja.clear();

				if (p_mutacji != 0)
				{
					if (mut == 1)
					{
						mut_swap(populacja, p_mutacji);
					}
					if (mut == 2)
					{
						mut_insert(populacja, p_mutacji);
					}
					
				}
				if (minikoszt(populacja) < najlepszy) najlepszy = minikoszt(populacja);
				wpokoleniu++;


				//czas1 = co.GetCounter();//
				//cout << "koniec " << czas1 << endl;//
			}

			czas = co.GetCounter();



			/*cout << endl << "oto nowa populacja:" << endl;
			sort(nowa_populacja.begin(), nowa_populacja.end());

			for (int i = 0; i < nowa_populacja.size(); i++)
			{
				cout << nowa_populacja[i].sciezka << "   " << nowa_populacja[i].koszt << endl;
			}

			cout << "min koszt z nowej populacji: " << minikoszt(nowa_populacja).koszt << endl;*/

			cout << "oto najlepszy" << najlepszy.koszt <<" po "<<wpokoleniu<<" pokoleniach"<< endl;

			sredni_koszt += najlepszy.koszt;
			sredni_blad += (najlepszy.koszt - wlasciwy) / wlasciwy;
			sredni_czas += czas;
			cout << "     w czasie "<< czas<<" ms" << endl;
	
		}

		sredni_blad /= ile_razy;
		sredni_koszt /= ile_razy;
		sredni_czas /= ile_razy;

		cout << endl << "sredni koszt " << sredni_koszt << endl;
		cout << "sredni czas " << sredni_czas << " ms" << endl;
		cout << endl << "sredni blad " << sredni_blad * 100 << " %" << endl;

		cout << endl << "Aby wyjsc z programu nacsnij ESC";
		cout <<endl<< "Aby pozostac w programie nacisnij jakikolwiek inny przycisk" << endl;

		char wyj;
		wyj = _getch();

		if (wyj == 27)
		{

			for (int i = 0; i < n; i++)
			{
				delete[] odleglosci[i];
			}
			delete[] odleglosci;
			return(0);
		}

		
		//cout << "zwolniono pamiec" << endl;


		//PlaySound(TEXT("muzyka.wav"), NULL, SND_SYNC);
	}
	system("pause");
	return 0;
}
