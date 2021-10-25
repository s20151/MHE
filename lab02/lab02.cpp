#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

//Przykład: {1 2 5 6 7 9} -> {1, 5, 9}, {2, 6, 7} suma = 15

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

double goal_solution(vector<vector<int>> triplets, int desired_sum){
    double sum = 0;
    for(int i = 0; i<triplets.size(); i++){
        int triplet_sum = 0;
        for(int j = 0; j<triplets[i].size(); j++){
            triplet_sum += triplets[i][j];
        }
        if(triplet_sum != desired_sum) sum++;
        if(triplets[i].size() != 3) sum++;
    }
    return sum;
}


void print(vector<int> data, ostream &out){
        out << "{ ";
        for (int x: data) out << x << " ";
        out << "}" << endl;
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

vector<vector<int>> generate_working_point(vector<int> input){
    vector<int> numbers = input;
    int v_size = numbers.size();
    int iterator = 0;
    vector<vector<int>> triplets;
    for(int i=0; i<v_size/3; i++){
        vector<int> triplet;
        for(int j=0; j<3; j++){
            triplet.push_back(numbers[iterator]);
            iterator++;
        }
        triplets.push_back(triplet);
    }
    return triplets;
}

void generate_next_working_point(vector<int> &numbers){
   next_permutation(numbers.begin(), numbers.end());
}

void print_triplets(vector<vector<int>> triplets, ostream &out){
    for (vector<int> x: triplets) print(x, out);
}




int main(int argc, char** argv) {
    if(argc == 1) {
        vector<int> data = {1,2,3,4,5,6};
        if(isValid(data)) {
            cout << "Given numbers:" << endl;
            print(data, cout);
            vector<vector<int>> random = generate_working_point(data);
            cout << "Random triplets:" << endl;
            print_triplets(random, cout);
            double goal = goal_solution(random, 15);
            cout << "Goal solution: " << goal << endl;
            generate_next_working_point(data);
            cout << "Given numbers:" << endl;
            print(data, cout);

        }else {
            cout << "Loaded data is not valid. " << endl;
        }
    }else {
        cout << "You did not enter file name."<<endl;
    }
    return 0;
}
