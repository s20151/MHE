//
// Created by s20151 on 11.10.2021.
//
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

int main() {
    vector<int> dane;
    string sciezka = "Z:\\MHE\\lab02\\liczby.txt";
    ifstream plik;
    plik.open(sciezka);
    if(!plik){
        cout<<"Bład. Nie udało się otworzyć pliku.";
    }
    int liczba;
    while(plik >> liczba){
        dane.push_back(liczba);
    }
    plik.close();
    cout<< "Given numbers:" <<endl;
    cout<< "{ "; for(int x : dane) cout<< x<<" "; cout<<"}"<<endl;
//    for(int i=0;i<dane.size();i++){
//        cout<<dane[i]<<" ";
//        if((i+1)%3==0) cout<<endl;
//    }
    return 0;
}