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
        sum += abs(first_triplet_sum-z);
    }
    return sum;
}


void print(vector<int> data, ostream &out){
    // wypisywanie zestawu dostepnych liczb i możliwość przekierowania outputu do pliku
    out << "{ ";
    for (int x: data) out << x << " ";
    out << "} ";
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
    for (vector<int> x: triplets) print(x, out);
}

void brute_force(vector<int> numbers, vector<vector<int>> working_point, ostream &out){
    clock_t start = clock();
    vector<int> starting_point = numbers;
    do{
        generate_next_working_point(numbers,working_point);
        print_triplets(working_point, out);
        out << " Goal value:" << goal_solution(working_point);
        if(goal_solution(working_point)==0){
            clock_t end = clock();
            double elapsed = double(end - start) / CLOCKS_PER_SEC;
            out<<" <<< solution"<<endl;
            out<<"Brute force duration: "<<elapsed<<endl;
            return;
        }
        out<<endl;
    }while(starting_point!=numbers);
    clock_t end = clock();
    double elapsed = double(end - start) / CLOCKS_PER_SEC;
    out<<"Unable to find solution in given set."<<endl;
    out<<"Brute force duration: "<<elapsed<<endl;
}

vector<vector<vector<int>>> generate_neighbours(vector<vector<int>> working_point){
    vector<vector<vector<int>>> neighbours;

    for(int i = 1; i<working_point.size(); i++){
        for(int a = 0; a<3; a++){
            for(int b = 0; b<3; b++){
                auto current_neighbour = working_point;
                swap(current_neighbour[0][a],current_neighbour[i][b]);
                neighbours.push_back(current_neighbour);
            }
        }
    }

    return neighbours;
}

vector<vector<int>> hill_climb(int iterations, const vector<vector<int>> working_point, ostream &out){
    clock_t start = clock();
    auto best_point = working_point;
    for (int i = 0; i < iterations; i++) {
        out<<i<<endl;
        auto best_point_copy = best_point;
        auto neighbours = generate_neighbours(best_point_copy);
        for(auto neighbour : neighbours){
            out<<"X";
            if (goal_solution(neighbour) < goal_solution(best_point)) {
                best_point = neighbour;
            }
        }
        if (best_point == best_point_copy) {
            break;
        }
    }
    clock_t end = clock();
    double elapsed = double(end - start) / CLOCKS_PER_SEC;
    print_triplets(best_point, out);
    out<<endl<<"Goal value: "<<goal_solution(best_point);
    out<<endl<<"Hill climb duration: "<<elapsed<<endl;
    return best_point;
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
        vector<int> data = load(argv[1]);
        if(isValid(data)) {
            ofstream outfile (argv[2],ofstream::binary);
            vector<vector<int>> wrk_pnt = generate_working_point(data);
            brute_force(data, wrk_pnt, outfile);
            hill_climb(20,wrk_pnt,outfile);
            outfile.close();
        }else {
            cout << "Loaded data is not valid. " << endl;
        }
    }else {
        cout << "You did not enter file name."<<endl;
        cout << "Enter 1 to generate set."<<endl<<"Enter 2 to enter your numbers."<<endl;
        int choice;
        do {
            cin >> choice;
        }while(choice != 1 && choice != 2);
        if(choice == 1){
            cout<<"Set size = '3 * n' Enter n"<<endl;
            int size;
            do{
                cin >> size;
            }while(size%3!=0);
            vector<int> numbers = generate_number_set(size);
            cout<<"Given numbers: ";
            print(numbers,cout);
            cout<<endl<<"Brute force: "<<endl;
            vector<vector<int>> starting_point = generate_working_point(numbers);
            vector<vector<int>> p_c = starting_point;
            vector<vector<int>> p_c2 = starting_point;
            brute_force(numbers, p_c, cout);
            hill_climb(20,p_c2,cout);




        }else{
            cout<<"Enter numbers amount (a multiple of three): ";
            int amount;
            do{
                cin >> amount;
            }while(amount%3!=0 && amount<3);
            vector<int> numbers;
            cout <<"Enter "<<amount<< " numbers: "<<endl;
            for(int i = 0; i < amount; i++){
                int number;
                cin >> number;
                numbers.push_back(number);
            }
            cout<<"Given numbers: ";
            print(numbers,cout);
            cout<<endl<<"Brute force: "<<endl;
            vector<vector<int>> starting_point = generate_working_point(numbers);
            brute_force(numbers,starting_point,cout);
            hill_climb(20,starting_point,cout);

        }
    }
    return 0;
}
