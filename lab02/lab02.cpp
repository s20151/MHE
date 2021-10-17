#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

//Przykład: {1 2 3 4 5 6} -> {1, 5, 9}, {2, 6, 7} suma = 15

vector<int> load(string path){
    vector<int> data;
    ifstream file;
    file.open(path);
    if(!file) {
        cout<<"Error opening selected file.";
        file.close();
    }else {
        int number;
        while (file >> number) data.push_back(number);
        file.close();
        sort(data.begin(), data.end());
    }
    return data;
}

void print(vector<int> data){
    if(data.size()>0){
        cout << "Given numbers:" << endl;
        cout << "{ ";
        for (int x: data) cout << x << " ";
        cout << "}" << endl;
    }else {
        cout<< "Could not find any data to print.";
    }
}


int main(int argc, char** argv) {
    string dir = "C:\\Users\\Admin\\Desktop\\SEM5\\MHE\\lab02\\"; // Z:\MHE\lab02
    if(argc == 2) {
        vector<int> data = load(dir + argv[1]);
        print(data);
    }else {
        cout << "You did not enter file name."<<endl;
    }
    return 0;
}