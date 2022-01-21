#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<iomanip>
#include<algorithm>
#include<math.h>
#include<Windows.h>

using namespace std;

int ilosc_miast;
int** macierz;

bool czy_chromosom_istnieje_w_populacji(vector<vector<int>> populacja, vector<int> chromosom)
{
    bool result = false;
    if (find(populacja.begin(), populacja.end(), chromosom) != populacja.end())
    {
        result = true;
    }
    return result;
}
int oblicz_wartosc_chromosomu(vector<int> chromosom)
{
    int wartosc = 0;
    for (int i = 0; i < chromosom.size()-1; i++)
    {
        wartosc += macierz[chromosom[i]][chromosom[i+1]];
    }
    wartosc += macierz[chromosom[chromosom.size()-1]][chromosom[0]];
    return wartosc;
}


vector<vector<int>> selekcja(vector<vector<int>> populacja)
{
    
    return populacja;
}


int main()
{
    srand(unsigned(time(NULL)));

    /*--------------------------------------------------*/

    //Odczyt pliku
    string nazwa_pliku;
    string sciezka_do_pliku = "instancje\\";
    fstream plik;
    string wyraz;
    int wartosc_optymalna;
    double wynik_czasowy;
;
    cout << "Podaj nazwe pliku razem z rozszerzeniem\n";
    cin >> nazwa_pliku;
    sciezka_do_pliku += nazwa_pliku;
    plik.open(sciezka_do_pliku);
    //Sprawdzanie czy plik o podanej nazwie istnieje
    while (!plik)
    {
        cout << "Plik o takiej nazwie nie istnieje!\n\n";
        cout << "Podaj nazwe pliku razem z rozszerzeniem\n";
        sciezka_do_pliku = "instancje\\";
        cin >> nazwa_pliku;
        sciezka_do_pliku += nazwa_pliku;
        plik.open(sciezka_do_pliku);
    }
    //pominiecie pierwszego wiersza
    getline(plik, wyraz);
    //wczytanie ilosci miast
    getline(plik, wyraz);

    ilosc_miast = stoi(wyraz);

    vector<int> miasta;

    //utworzenie macierzy i wpisanie do niej danych
    macierz = new int* [ilosc_miast];
    for (int i = 0; i < ilosc_miast; i++)
    {
        miasta.push_back(i);
        macierz[i] = new int[ilosc_miast];
    }
    int kolumna = 0;
    int rzad = 0;
    for (int i = 0; i < ilosc_miast * ilosc_miast; i++)
    {
        if (kolumna == ilosc_miast)
        {
            kolumna = 0;
            rzad++;
           
        }
        plik >> wyraz;
        macierz[rzad][kolumna] = stoi(wyraz);
        kolumna++;
    }
    plik >> wyraz;
    wartosc_optymalna = stoi(wyraz);

    /*-------------------------------------------------*/

    plik >> wyraz;
    int miasto_startowe = 0;
    int aktualne_miasto = 0;
    int najmniejszy_koszt = INT_MAX;
    int najtansze_miasto;
    int najlepsza_wartosc;
    int wartosc_pierwszej_trasy;
    float wartosc_pierwszej_trasy_prd;
    int aktualny_koszt = 0;
    float prd;
    vector<int> odwiedzone_miasta;
    vector<int> aktualne_rozwiazanie;
    vector<int> najlepsze_rozwiazanie;
    vector<vector<int>> populacja;


    /*--------------------------------------------------*/
    //parametry programu

    int rozmiar_populacji=10;
    int liczba_generacji=100;
    int liczba_krzyzowan_na_generacje=5;
    int liczba_mutacji_na_generacje = 2;
        
   
    
    odwiedzone_miasta.push_back(miasto_startowe);

    //wyznaczenie startowego rozwiazania metoda zachlanna
    while (odwiedzone_miasta.size() != ilosc_miast)
    {
        najmniejszy_koszt = INT_MAX;
        for (int j = 0; j < ilosc_miast; j++)
        {
            if (macierz[aktualne_miasto][j] < najmniejszy_koszt && macierz[aktualne_miasto][j] != -1)
            {

                if (find(odwiedzone_miasta.begin(), odwiedzone_miasta.end(), j) != odwiedzone_miasta.end())
                {
                    continue;
                }
                else
                {
                    najmniejszy_koszt = macierz[aktualne_miasto][j];
                    najtansze_miasto = j;
                }
            }

        }
        aktualny_koszt += najmniejszy_koszt;
        odwiedzone_miasta.push_back(najtansze_miasto);
        aktualne_miasto = najtansze_miasto;
    }
    aktualny_koszt += macierz[aktualne_miasto][0];
    wartosc_pierwszej_trasy = aktualny_koszt;
    aktualne_rozwiazanie = odwiedzone_miasta;
    prd = 100 * (aktualny_koszt - wartosc_optymalna) / (float)wartosc_optymalna;
    wartosc_pierwszej_trasy_prd = prd;
    najlepsza_wartosc = aktualny_koszt;
    cout << "0 " << najlepsza_wartosc << " ";
    cout << fixed << setprecision(2) << prd << "%" << endl;
    najlepsze_rozwiazanie = aktualne_rozwiazanie;

    populacja.push_back(aktualne_rozwiazanie);
    //tworzymy populacje na podstawie losowej zamiany rozwiazania uzyskanego metoda zachlanna
    for (int i = 0; i < rozmiar_populacji-1; i++)
    {
        random_shuffle(aktualne_rozwiazanie.begin(), aktualne_rozwiazanie.end());

        //sprawdzanie czy dany chromosom juz istnieje w populacji
        if (czy_chromosom_istnieje_w_populacji(populacja,aktualne_rozwiazanie))
        {
            i--;
        }
        else
        {
            //sprawdzamy czy nowe rozwiazanie jest najlepsze
            if (oblicz_wartosc_chromosomu(aktualne_rozwiazanie)<najlepsza_wartosc)
            {
                najlepsze_rozwiazanie = aktualne_rozwiazanie;
                najlepsza_wartosc = oblicz_wartosc_chromosomu(aktualne_rozwiazanie);
            }

            //dodajemy rozwiazanie(chromosom) do populacji
            populacja.push_back(aktualne_rozwiazanie);
        }

    }





    vector<int> nowy_chromosom;
    //glowna petla programu
    for (int i = 0; i < liczba_generacji; i++)
    {
        bool czy_poprawa = false;
        int wylosowany_chromosom1;
        int wylosowany_chromosom2;
        int miasto_do_zamiany1;
        int miasto_do_zamiany2;
        for (int j = 0; j < liczba_krzyzowan_na_generacje; j++)
        {
            nowy_chromosom.clear();
            //losowanie chromosomow do krzyzowania
            wylosowany_chromosom1 = rand() % rozmiar_populacji;
 

            do
            {
                wylosowany_chromosom2 = rand() % rozmiar_populacji;
            } while (wylosowany_chromosom2 == wylosowany_chromosom1);
            
            //krzyzowanie
            for (int k = 0; k < ilosc_miast/2; k++)
            {
                nowy_chromosom.push_back(populacja[wylosowany_chromosom1][k]);
            }
            for (int k = 0; k < ilosc_miast; k++)
            {
               
                if (find(nowy_chromosom.begin(), nowy_chromosom.end(), populacja[wylosowany_chromosom2][k]) != nowy_chromosom.end())
                {
                    continue;
                }
                else
                {
                    nowy_chromosom.push_back(populacja[wylosowany_chromosom2][k]);
                }
            }
            if (czy_chromosom_istnieje_w_populacji(populacja, nowy_chromosom))
            {
                j--;
            }
            else
            {
                if (oblicz_wartosc_chromosomu(nowy_chromosom) < najlepsza_wartosc)
                {
                    najlepsze_rozwiazanie = nowy_chromosom;
                    najlepsza_wartosc = oblicz_wartosc_chromosomu(nowy_chromosom);
                    czy_poprawa = true;
                }
                populacja.push_back(nowy_chromosom);
            }
           

            //for (int k = 0; k < nowy_chromosom.size(); k++)
            //{
            //   
            //    cout << nowy_chromosom[k] << " ";
            //}
            //Sleep(100000);
            

        }
        for (int j = 0; j < liczba_mutacji_na_generacje; j++)
        {
            //mutacja swap
            wylosowany_chromosom1 = rand() % populacja.size();
            miasto_do_zamiany1 = rand() % ilosc_miast;
            miasto_do_zamiany2;
            do
            {
                miasto_do_zamiany2 = rand() % ilosc_miast;
            } while (miasto_do_zamiany2 == miasto_do_zamiany1);

            nowy_chromosom = populacja[wylosowany_chromosom1];

            do
            {
                iter_swap(nowy_chromosom.begin() + miasto_do_zamiany1, nowy_chromosom.begin() + miasto_do_zamiany2);

            } while (czy_chromosom_istnieje_w_populacji(populacja, nowy_chromosom));
            if (oblicz_wartosc_chromosomu(nowy_chromosom)<najlepsza_wartosc)
            {
                najlepsze_rozwiazanie = nowy_chromosom;
                najlepsza_wartosc = oblicz_wartosc_chromosomu(nowy_chromosom);
                czy_poprawa = true;
            }
            populacja.push_back(nowy_chromosom);


 /*           cout << endl;
            cout << "Mutacja " << wylosowany_chromosom1 << " " << miasto_do_zamiany1 << " " << miasto_do_zamiany2 << endl;
            Sleep(1000000);*/
        }


        //wypisanie poprawy rozwiazania
        if (czy_poprawa==true)
        {
            cout << i << " " << najlepsza_wartosc << " ";
            prd = 100 * (najlepsza_wartosc - wartosc_optymalna) / (float)wartosc_optymalna;
            cout << fixed << setprecision(2) << prd << "%" << endl;
        }
    }
    //wypisywanie populacji
    //cout << endl << endl << endl;
    //for (int i = 0; i < populacja.size(); i++)
    //{
    //    for (int j = 0; j < populacja[i].size(); j++)
    //    {
    //        cout << populacja[i][j] << " ";
    //    }
    //    cout << endl;
    //}


}