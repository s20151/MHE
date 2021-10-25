#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>

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

double goal_solution(vector<vector<int>> triplets){
    double sum = 0;
    
    int triplets_amount = triplets.size();
    vector<double> triplets_sums;
    for(auto x: triplets){
        double triplet_sum = accumulate(x.begin(), x.end(), 0.0);
        triplets_sums.push_back(triplet_sum);
    }
    int first_triplet_sum = triplets_sums[0];
    for(int z: triplets_sums){
        sum += first_triplet_sum-z;
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

void generate_next_working_point(vector<int> &numbers, vector<vector<int>> &working_point){
   next_permutation(numbers.begin(), numbers.end());
   working_point = generate_working_point(numbers);
}

void print_triplets(vector<vector<int>> triplets, ostream &out){
    for (vector<int> x: triplets) print(x, out);
}




int main(int argc, char** argv) {
    if(argc == 1) {
        vector<int> data = {1,2,3,4,5,6,1,2,3,4,56,7};
        if(isValid(data)) {
            cout << "Given numbers:" << endl;
            print(data, cout);
            vector<vector<int>> random = generate_working_point(data);
            cout << "Generated triplets:" << endl;
            print_triplets(random, cout);
            double goal = goal_solution(random);
            cout << "Goal solution: " << goal << endl;
            generate_next_working_point(data, random);
            cout << "Next working point:" << endl;
            print(data, cout);
            print_triplets(random, cout);
            cout << "Goal solution: " << goal << endl;
        }else {
            cout << "Loaded data is not valid. " << endl;
        }
    }else {
        cout << "You did not enter file name."<<endl;
    }
    return 0;
}
