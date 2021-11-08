#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;

//Przykład: {1 2 5 6 7 9} -> {1, 5, 9}, {2, 6, 7} suma obu tripletów = 15

vector<int> load(string path){
    //wczytywanie liczb z pliku, format: liczby całkowite rozdzielone dowolnym białym znakiem
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
    // funkcja celu, sumujemy różnicę sum poszczególnych tripletów z sumą pierwszego
    // cel jest osiągnięty jeżeli suma wynosi 0
    double sum = 0;
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
    // wypisywanie zestawu dostepnych liczb i możliwość przekierowania outputu do pliku
    out << "{ ";
    for (int x: data) out << x << " ";
    out << "}" << endl;
}

bool isValid(vector<int> data){
    // sprawdzenie czy przyjęto odpowiednią ilość liczb do budowy tripletów
    if(data.size()%3==0 && data.size()>=3) {
        return true;
    }else {
        return false;
    }
}

vector<vector<int>> generate_working_point(vector<int> input){
    // funkcja generuje wektor zawierający triplety z podanego zbioru liczb
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
    // funkcja permutuje listę wszystkich liczb, a następnie na jej podstawie buduje nowe triplety
    next_permutation(numbers.begin(), numbers.end());
    working_point = generate_working_point(numbers);
}

void print_triplets(vector<vector<int>> triplets, ostream &out){
    // 'ładne' wypisywanie wygenerowanych tripletów i możliwość przekierowania outputu do pliku
    out << endl;
    for (vector<int> x: triplets) print(x, out);
}

void brute_force(vector<int> &numbers, vector<vector<int>> &working_point, ostream &out){
    vector<int> starting_point = numbers;
    do{
        generate_next_working_point(numbers,working_point);
        print_triplets(working_point, out);
        if(goal_solution(working_point)==0){
            out<<"^ znaleziono rozwiazanie!"<<endl;
        }
    }while(starting_point!=numbers);
}

vector<int> generate_number_set(int amount){
    vector<int> numbers;
    srand( time(NULL) );
    for(int i=0 ; i<amount; i++){
        numbers.push_back(rand() % 10);
    }
    return numbers;
}


int main(int argc, char** argv) {
    if(argc > 1) {
        vector<int> data = load( argv[1] );
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
            brute_force(data, random, cout);
        }else {
            cout << "Loaded data is not valid. " << endl;
        }
    }else {
        cout << "You did not enter file name."<<endl;
    }
    return 0;
}
