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

double goal_solution(vector<int> data, vector<bool> generated_solution, int desired_sum){
    int sum = 0;
    double score = 0;
    int element_count = 0;
    for(int i = 0; i<data.size(); i++){
        if(generated_solution[i]){
          sum += data[i];
          element_count++;
        } 
    }
        if(sum != desired_sum) score++;
        if(element_count != 3) score++;
    return score;
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

vector<bool> generate_random_working_point_b(vector<int> input){
    srand(time(NULL));
    vector<bool> random_working_point;
    for (int i = 0; i < input.size(); i++) {
        random_working_point.push_back(rand() % 2);
    }
    return random_working_point;
}
void generate_next_working_point_b(vector<bool> &random_working_point){
    int i = 0;
    do{
       if(!random_working_point[i]){
           random_working_point[i]=true;
           break;
       } else {
           random_working_point[i]=false;
           i++;
           if(i==random_working_point.size()) i=0;
       }
    }while(true);
}

void print_solution(vector<bool> generated_soluton){
            cout<<"{ ";
            for(auto x: generated_soluton) cout<<x<<" ";
            cout<<"}";
}


int main(int argc, char** argv) {
    if(argc == 1) {
        vector<int> data = {1,2,4,5,6,3};
        if(isValid(data)) {
            cout << "Given numbers:" << endl;
            print(data, cout);
            vector<bool> rand_working_p = generate_random_working_point_b(data);
            cout<<"generated working point: "<<endl;
            print_solution(rand_working_p);
            generate_next_working_point_b(rand_working_p);
            cout<<endl<<"next working point: "<<endl;
            print_solution(rand_working_p);
        }else {
            cout << "Loaded data is not valid. " << endl;
        }
    }else {
        cout << "You did not enter file name."<<endl;
    }
    return 0;
}
