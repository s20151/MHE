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
    if(file){
        int number;
        while (file >> number) data.push_back(number);
    }
    file.close();
    return data;
}

void print(vector<int> data){
        cout << "Given numbers:" << endl;
        cout << "{ ";
        for (int x: data) cout << x << " ";
        cout << "}" << endl;
}

bool isValid(vector<int> data){
    if(data.size()%3==0 && data.size()>=3) {
        return true;
    }else if(data.size()%3!=0) {
        return false;
    }else {
        return false;
    }
}

double goal_function(vector<int> solution){}

vector<int> generate_solution(vector<int> input){

}

vector<int> next_solution(vector<int> solution){}






int main(int argc, char** argv) {
    if(argc > 1) {
        vector<int> data = load( argv[1] );
        if(isValid(data)) {
            print(data);
        }else {
            cout << "Loaded data is not valid. " << endl;
        }
    }else {
        cout << "You did not enter file name."<<endl;
    }
    return 0;
}