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


int silnia(int n)
{
	int liczba = 1;
	for (int i = 1; i <= n; i++)
	{
		liczba = liczba * i;
	}
	return liczba;
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
	
	while (1)
	{

		minimalna.koszt = 999999;

		ifstream plik;
		int typ;
		char alg;


		string nazwa;
		cout << endl<< "Podaj nazwe pliku (bez rozszerzenia.txt)";
		cin >> nazwa;
		cout <<endl<< "1 - aby przetestowac metode przegladu zupelnego." << endl;
		cout << "2 - aby pzetestowac metode z programowaniem dynamicznym" << endl;
		cout << "3 - aby symulowac wyzarzanie" << endl;
		cout << "jakikolwiek przycisk aby wyjsc z programu" << endl;


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



		/////////////////////////////////////start

		int poczatek = 0;
		vector<int> perm;

		int pozy = 0;
		int maska = (1 << 0);

		Licznik co; // liczby bez generacji danych losowych
		Licznik co1; //lczy wraz z generacja danych

		alg = _getch();
		double czas;
		double czas1;
		double chwi;

		double chwila = 9992718; //////////////////////////////////czas do ustawienia w ms


		//zmienne do sym wyzazania
		int mask = 0;
		int mask1 = (1 << n) - 1;
		vector<int> sciezka;
		int a;
		int b;
		int koszt;
		int flag = 0;

		double temperatura = 5; //////////////////////////////////////////////////////////////////////////////temperatura
		double delta = 5;//////////////////////////////////////////////////////////////////////////////////////////delta

		double sredni_koszt = 0;
		double sredni_czas = 0;
		double sredni_blad = 0;
		double tempe=10000;

		double wlasciwy = 2707; //gr48
		cout << "Podaj wlasciwy koszt drogi " << endl;
		cin >> wlasciwy;


		double temp;
		int ile;

		int iteracje = 0;

		struct wyniczek
		{
			int koszt;
			vector<int> path;
		};
		wyniczek nowa;
		wyniczek najlepsza;

		int licznik = 0;





		switch (alg)
		{
		case '1':

			poczatek = 0;
			cout << endl;

			perm.push_back(poczatek);
			for (int i = 0; i < n; i++)
			{
				if (i != poczatek)
				{
					perm.push_back(i);
				}
			}

			cout << perm << endl;


			minimalna.koszt = 999999;

			co.StartCounter();

			permu(perm, 1, n - 1);

			czas = co.GetCounter();

			cout << "w czasie " << czas << " sekund" << endl;

			cout << "udalo sie" << endl;

			cout << "koszt: " << minimalna.koszt << " a droga to: " << minimalna.droga << endl;

			break;
		case '2':

			koszty = new int *[pow(2, n)];
			for (int i = 0; i < pow(2, n); i++)
			{
				koszty[i] = new int[n];
			}

			koniec = (1 << n) - 1;

			for (int i = 0; i < (1 << n); i++) {
				for (int j = 0; j < n; j++) {
					koszty[i][j] = -1;
				}
			}

			int wyn;


			miasta = new int*[pow(2, n)];
			for (int i = 0; i < pow(2, n); i++)
			{
				miasta[i] = new int[n];
			}


			co.StartCounter();
			wyn = dynamic(1, 0, n);

			czas = co.GetCounter();
			cout << "teraz dynamicznie, koszt: " << wyn << endl;

			cout << "w czasie: " << czas << endl;

			cout << "to sa miasta: ";

			while (maska != koniec)
			{
				cout << pozy << " ";
				pozy = miasta[maska][pozy];


				maska |= (1 << pozy);
			}
			cout << pozy << " 0" << endl;
			pozy = 0;
			maska = (1 << 0);


			for (int i = 0; i < pow(2, n); i++)
			{
				delete[] miasta[i];
			}
			delete[] miasta;

			for (int i = 0; i < pow(2, n); i++)
			{
				delete[] koszty[i];
			}
			delete[] koszty;

			break;

		case '3':
			srand(time(NULL));

			// wybieranie poczatowego poczatkowego punktu (sciezki)

			int random;
			int random1;

			cout << "Podaj temperature poczatkowa i spadek temperatury wyrazony w % na iteracje i ilosc iteracji" << endl;
			cin >> temperatura;
			temp = temperatura;
			cin >> delta;
			cin >> ile;

			for (int it = 0; it < ile; it++)
			{
				koszt = 9999999; najlepsza.path.clear(); najlepsza.koszt = 9999999; nowa.path.clear(); nowa.koszt = 9999999; iteracje = 0; Sleep(1000);
				temperatura = temp; sciezka.clear(); flag = 0;



				co1.StartCounter();

				if (n < 10)
				{
					while (mask != mask1)
					{
						random = rand() % n;
						//a = (1 << random) | mask; 

						if (((1 << random) | mask) > mask) // a>mask					
						{
							mask |= (1 << random);
							sciezka.push_back(random);
						}

					}
				}
				else //dla instancji duuzych powyzsza metoda generowania losowych nie dziala
				{
					for (int i = 0; i < n; i++)
					{
						sciezka.push_back(i);
					}

					for (int i = 0; i < n*n; i++)
					{
						random = rand() % n;
						random1 = rand() % n;
						swap(sciezka[random], sciezka[random1]);
					}
				}


				cout << "licze..." << endl;

				co.StartCounter();

				//cout << "poczatkowa sciezka: " << sciezka << endl;						
				koszt = licz_koszt(sciezka, odleglosci);
				cout << "oto koszt losowy: " << koszt << endl << endl;

				najlepsza.path = sciezka;
				najlepsza.koszt = koszt;

				//cout <<endl<< "licze..." << endl;

				/*
				sciezka.clear(); sciezka.push_back(1); sciezka.push_back(3); sciezka.push_back(0); sciezka.push_back(2);
				cout << "poczatkowa sciezka: " << sciezka << endl;
				koszt = licz_koszt(sciezka, odleglosci);
				cout << "oto koszt: " << koszt << endl;
				*/

				while(((koszt - wlasciwy) / wlasciwy)>0.3 && co1.GetCounter()<300000)// && temperatura >0.000001)//while(co1.GetCounter() <=60000 )*///for (iteracje; iteracje < 1000; iteracje++)
				{
					/*if (co1.GetCounter() > tempe)
					{
						cout << najlepsza.koszt <<"   "<<temperatura<< endl;
						tempe += 10000;
					}*/
					
					iteracje++;
					for (int i = 0; i < n; i++)
					{
						for (int j = i + 1; j < n; j++)
						{
							chwi = co1.GetCounter();
							if (chwi >= chwila && flag == 0)
							{
								cout << "w czasie z algorytmu DP( " << chwila << " ms) otrzymujemy:" << endl;
								//cout << "trasa: " << najlepsza.path << endl;
								cout << "koszt " << licz_koszt(najlepsza.path, odleglosci) << endl << endl;
								flag = 1;
							}

							koszt = licz_koszt(sciezka, odleglosci);

							swap(sciezka[i], sciezka[j]);
							nowa.path = sciezka;
							swap(sciezka[j], sciezka[i]);

							nowa.koszt = licz_koszt(nowa.path, odleglosci);

							if (nowa.koszt < najlepsza.koszt)
							{
								najlepsza.path = nowa.path;
								najlepsza.koszt = nowa.koszt;
							}

							if (nowa.koszt < koszt)
							{
								koszt = nowa.koszt;
								sciezka = nowa.path;
								
								/*if (co1.GetCounter() > tempe)
								{
									cout << "nowy " << koszt << "    " << temperatura <<"po "<<co1.GetCounter()/1000<<" sekundach"<< endl;
									tempe += 5000;
								}*/

							}
							else
							{
								if ((1 / (1 + pow(E, ((koszt - nowa.koszt) / temperatura)))) < (double)pow(E, ((koszt - nowa.koszt) / temperatura)))
								{
									//cout << "aktualny " << koszt << endl;
									koszt = nowa.koszt;
									sciezka = nowa.path;
									//cout << "gorszy " << koszt << endl;
								}
							}
							if (koszt < najlepsza.koszt)
							{
								najlepsza.path = sciezka;
								najlepsza.koszt = koszt;
							}
						}
					}

					//cout << temperatura << endl;
					temperatura = temp* pow( delta, iteracje);
					//temperatura *= (1 - (double)(delta / 100.0));
					//temperatura -= delta;
				}

				czas1 = co1.GetCounter();
				czas = co.GetCounter();

				//cout << "koncowa sciezka: " << najlepsza.path << endl;
				koszt = licz_koszt(najlepsza.path, odleglosci);
				cout << "oto koszt: " << koszt << endl;
				sredni_koszt += koszt;
				sredni_czas += czas1;
				sredni_blad += (koszt - wlasciwy) / wlasciwy;
				cout << "w czasie " << czas << " ms" << endl;
				cout << "w czasie z generacja " << czas1 << " ms" << endl;

			}

			sredni_koszt /= ile;
			sredni_czas /= ile;
			sredni_blad /= ile;
			cout <<endl<< "sredni koszt " << sredni_koszt << endl;
			cout << "sredni czas " << sredni_czas << " ms" << endl; 
			cout << endl << "sredni blad " << sredni_blad*100<<" %"<<endl;
			















			break;

		default:

			for (int i = 0; i < n; i++)
			{
				delete[] odleglosci[i];
			}
			delete[] odleglosci;

			return(0);

			break;
		}

		


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
