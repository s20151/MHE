#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

//Przykład: {1 2 3 4 5 6} -> {1, 5, 9}, {2, 6, 7} suma = 15

vector<int> wczytaj_dane(string sciezka){
    vector<int> dane;
    ifstream plik;
    plik.open(sciezka);
    if(!plik) cout<<"Bład. Nie udało się otworzyć pliku.";
    int liczba;
    while(plik >> liczba) dane.push_back(liczba);
    plik.close();
    sort(dane.begin(), dane.end());
    return dane;
}

int main() {
    vector<int> dane = wczytaj_dane("Z:\\MHE\\lab02\\liczby.txt");
    cout<< "Given numbers:" <<endl;
    cout<< "{ "; for(int x : dane) cout<< x<<" "; cout<<"}"<<endl;
    return 0;
}