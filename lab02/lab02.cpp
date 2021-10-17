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
    }else {
        int number;
        while (file >> number) data.push_back(number);
        file.close();
        sort(data.begin(), data.end());
        return data;
    }
}

int main(int argc, char** argv) {
    string dir = "C:\\Users\\Admin\\Desktop\\SEM5\\MHE\\lab02\\"; // Z:\MHE\lab02
    vector<int> data = load(dir + argv[1]);
    if(data.empty()){
        return 0;
    }else{
        cout << "Given numbers:" << endl;
        cout << "{ ";
        for (int x: data) cout << x << " ";
        cout << "}" << endl;
    }
    return 0;

}