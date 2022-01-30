#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>
#include <set>
#include <list>
#include <functional>

using namespace std;



//Przykład: {1 2 5 6 7 9} -> {1, 5, 9}, {2, 6, 7} suma obu tripletów = 15


vector<int> load(string path) {
    //wczytywanie liczb z pliku, format: liczby całkowite rozdzielone dowolnym białym znakiem
    vector<int> data;
    ifstream file;
    file.open(path);
    if (file) {
        int number;
        while (file >> number) data.push_back(number);
    }
    file.close();
    return data;
}


double goal_solution(vector<vector<int>> triplets) {
    // funkcja celu, sumujemy różnicę sum poszczególnych tripletów z sumą pierwszego
    // cel jest osiągnięty jeżeli suma wynosi 0
    double sum = 0;
    vector<double> triplets_sums;
    for (auto x: triplets) {
        double triplet_sum = accumulate(x.begin(), x.end(), 0.0);
        triplets_sums.push_back(triplet_sum);
    }
    int first_triplet_sum = triplets_sums[0];
    for (int z: triplets_sums) {
        sum += abs(first_triplet_sum - z);
    }
    return sum;
}

auto tabu_search = [](
        auto cost,
        auto generate_first_point,
        auto neighbours_f, /// current work point neighbours
        int N,
        int tabu_size,
        std::function<void(int c, double dt)> on_statistics = [](int c, double dt) {}) {
    auto start = chrono::steady_clock::now();

    auto best_p = generate_first_point();

    set<vector<vector<int>>> taboo_set;
    list<vector<vector<int>>> taboo_list;

    auto is_in_taboo = [&](auto e) {
        //if (taboo_set.count(e)) cerr << "tabu hit" << endl;
        return taboo_set.count(e);
    };
    auto add_to_taboo = [&](auto e) {
        taboo_set.insert(e);
        taboo_list.push_back(e);
    };
    auto shrink_taboo = [&]() {
        if (taboo_set.size() > tabu_size) {
            taboo_set.erase(taboo_list.front());
            taboo_list.pop_front();
        }
    };

    auto p = best_p; // current work point

    for (int i = 0; i < N; i++) {
        auto neighbours = neighbours_f(best_p);
        neighbours.erase(std::remove_if(neighbours.begin(),
                                        neighbours.end(),
                                        [&](auto e) { return is_in_taboo(e); }),
                         neighbours.end());
        if (neighbours.size() == 0) break;
        p = *max_element(neighbours.begin(), neighbours.end(), [&](auto a, auto b) {
            return cost(a) > cost(b);
        });
        add_to_taboo(p);
        const double cost_value = cost(p);
        if (cost(p) < cost(best_p)) {
            best_p = p;
        }
        shrink_taboo();
    }
    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;
    cout << "Taboo search duration: ;" << duration.count() << "; Problem size: ;" << 6 <<
         "; Goal value: ;" << goal_solution(best_p) << endl;
    return best_p;
};


void print(vector<int> data, ostream &out) {
    // wypisywanie zestawu dostepnych liczb i możliwość przekierowania outputu do pliku
    out << "{ ";
    for (int x: data) out << x << " ";
    out << "} ";
}

bool isValid(vector<int> data) {
    // sprawdzenie czy przyjęto odpowiednią ilość liczb do budowy tripletów
    if (data.size() % 3 == 0 && data.size() >= 3) {
        return true;
    } else {
        return false;
    }
}

vector<vector<int>> generate_working_point(vector<int> input) {
    // funkcja generuje wektor zawierający triplety z podanego zbioru liczb
    vector<int> numbers = input;
    std::random_device dev;
    std::mt19937 rng(dev());
    shuffle(begin(numbers), end(numbers), rng);
    int v_size = numbers.size();
    int iterator = 0;
    vector<vector<int>> triplets;
    for (int i = 0; i < v_size / 3; i++) {
        vector<int> triplet;
        for (int j = 0; j < 3; j++) {
            triplet.push_back(numbers[iterator]);
            iterator++;
        }
        triplets.push_back(triplet);
    }
    return triplets;
}

void generate_next_working_point(vector<int> &numbers, vector<vector<int>> &working_point) {
    // funkcja permutuje listę wszystkich liczb, a następnie na jej podstawie buduje nowe triplety
    next_permutation(numbers.begin(), numbers.end());
    working_point = generate_working_point(numbers);
}

void print_triplets(vector<vector<int>> triplets, ostream &out) {
    // 'ładne' wypisywanie wygenerowanych tripletów i możliwość przekierowania outputu do pliku
    for (vector<int> x: triplets) print(x, out);
}

void brute_force(vector<int> numbers, vector<vector<int>> working_point, ostream &out) {
    auto start = chrono::steady_clock::now();
    vector<int> starting_point = numbers;
    do {
        generate_next_working_point(numbers, working_point);
        if (goal_solution(working_point) == 0) {
            auto finish = chrono::steady_clock::now();
            chrono::duration<double> duration = finish - start;
            out << "Brute force duration: ;" << duration.count() << "; Problem size: ;" << numbers.size() <<
                "; Goal value: ;" << goal_solution(working_point) << endl;
            return;
        }
    } while (starting_point != numbers);
    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;
    out << "Brute force duration: ;" << duration.count() << "; Problem size: ;" << numbers.size() <<
        "; Goal value: ;" << goal_solution(working_point) << endl;
}

vector<vector<vector<int>>> generate_neighbours(vector<vector<int>> working_point) {
    vector<vector<vector<int>>> neighbours;

    for (int i = 1; i < working_point.size(); i++) {
        for (int a = 0; a < 3; a++) {
            for (int b = 0; b < 3; b++) {
                auto current_neighbour = working_point;
                swap(current_neighbour[0][a], current_neighbour[i][b]);
                neighbours.push_back(current_neighbour);
            }
        }
    }

    return neighbours;
}

vector<vector<int>> hill_climb(int iterations, const vector<vector<int>> working_point, ostream &out) {
    auto start = chrono::steady_clock::now();
    auto best_point = working_point;
    for (int i = 0; i < iterations; i++) {
        auto best_point_copy = best_point;
        auto neighbours = generate_neighbours(best_point_copy);
        for (auto neighbour: neighbours) {
            if (goal_solution(neighbour) < goal_solution(best_point)) {
                best_point = neighbour;
            }
        }
        if (best_point == best_point_copy) {
            break;
        }
    }
    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;
    out << "Hill climb duration: ;" << duration.count() << "; Problem size: ;" << 24 <<
        "; Goal value: ;" << goal_solution(best_point) << endl;
    return best_point;
}

vector<vector<int>> hill_climb_stochastic(int iterations, const vector<vector<int>> working_point, ostream &out) {
    std::random_device dev;
    std::mt19937 rng(dev());
    auto start = chrono::steady_clock::now();
    auto best_point = working_point;
    for (int i = 0; i < iterations; i++) {
        auto best_point_copy = best_point;
        auto neighbours = generate_neighbours(best_point_copy);
        uniform_int_distribution<> distrib(0, neighbours.size() - 1);
        for (int i = 0; i < neighbours.size(); i++) {
            int random_number = distrib(rng);
            auto neighbour = neighbours[random_number];
            if (goal_solution(neighbour) < goal_solution(best_point)) {
                best_point = neighbour;
            }
        }
        if (best_point == best_point_copy) {
            break;
        }
    }
    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;
    out << "stochastic Hill climb result: ";
    print_triplets(best_point, out);
    out << endl << "Goal value: " << goal_solution(best_point);
    out << endl << "stochastic Hill climb duration: " << duration.count() << endl;
    return best_point;
}


vector<int> generate_number_set(int amount) {
    vector<int> numbers;
    srand(time(NULL));
    for (int i = 0; i < amount; i++) {
        numbers.push_back(rand() % 10);
    }
    return numbers;
}

vector<vector<int>> generate_neighbour_for_SA(vector<vector<int>> wrk_pnt){
    random_device dev;
    vector<vector<int>> neighbour = wrk_pnt;
    uniform_int_distribution<int> triplet_index = uniform_int_distribution<int> (0, wrk_pnt.size()-1);
    uniform_int_distribution<int> number_index = uniform_int_distribution<int> (0, 2);

    int t_i1 = triplet_index(dev);
    int t_i2 = triplet_index(dev);
    int n_i1 = number_index(dev);
    int n_i2 = number_index(dev);
    swap(neighbour[t_i1][n_i1], neighbour[t_i2][n_i2]);

    return neighbour;
}

auto temp_func(int k){
    return 1.0 / (k);
}

vector<vector<int>> simulated_annealing(
        vector<vector<int>> p0,
        int iterations,
        ofstream &out,
        function<double(int)> T){

    uniform_real_distribution<> u_k(0.0, 1.0);
    std::random_device dev;
    std::mt19937 rng(dev());

    auto start = chrono::steady_clock::now();

    auto p = p0;
    auto p_best = p0;

    int iterator = 0;

    for (int i = 0; i < iterations; i++) {
        auto p2 = generate_neighbour_for_SA(p);
        if (goal_solution(p2) < goal_solution(p)) {
            p = p2;
        }else{
            double u = u_k(dev);
            if (u < exp(-abs(goal_solution(p2) - goal_solution(p)) / T(i))) {
                p = p2;
            }
        }
        if (goal_solution(p) < goal_solution(p_best)) {
            p_best = p;
        }
        iterator++;
    }
    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;

    out << "Simulated annealing duration: ;" << duration.count() << "; Problem size: ;" << p_best.size()*3 <<
        "; Goal value: ;" << goal_solution(p_best) << endl;

    return p_best;
}


//
// Genetyczny:
// 1. zaczynamy od losowej populacji losowej
// 2. naturalna selekcja -> funkcja fitness jeżeli nie spełnia warunku fitness=0
// 3. wybieramy 2 rodziców <selection>
// 4. dzielimy ich genom i zamieniamy go <crossover>
// 5. powtarzamy 3 i 4 do powstania nowej generacji
// 6. przenosimy najlepsze rozwiazanie
// 7. mutacje
// 8. powtarzamy do znalezienia rozwiazania
//



int main(int argc, char **argv) {
    if (argc > 1) {
        vector<int> data = load(argv[1]);
        if (isValid(data)) {
            ofstream outfile(argv[2], ios_base::app);
            for (int i = 0; i < 25; i++) {
                vector<vector<int>> wrk_pnt = generate_working_point(data);
                simulated_annealing(wrk_pnt, 100, outfile, temp_func);
            }
            outfile.close();
        } else {
            cout << "Loaded data is not valid. " << endl;
        }
    } else {
        cout << "You did not enter file name." << endl;
        cout << "Enter 1 to generate set." << endl << "Enter 2 to enter your numbers." << endl;
        int choice;
        do {
            cin >> choice;
        } while (choice != 1 && choice != 2);
        if (choice == 1) {
            cout << "Set size = '3 * n' Enter n" << endl;
            int size;
            do {
                cin >> size;
            } while (size % 3 != 0);
            vector<int> numbers = generate_number_set(size);
            cout << "Given numbers: ";
            print(numbers, cout);
            cout << endl << "Brute force: " << endl;
            vector<vector<int>> starting_point = generate_working_point(numbers);
            vector<vector<int>> p_c = starting_point;
            vector<vector<int>> p_c2 = starting_point;
            vector<vector<int>> p_c3 = starting_point;
            brute_force(numbers, p_c, cout);
            hill_climb(20, p_c2, cout);
            hill_climb_stochastic(20, p_c2, cout);
            vector<vector<int>> tabu = tabu_search(
                    goal_solution,
                    [&]() { return p_c3; },
                    generate_neighbours,
                    20,
                    p_c3.size(),
                    [](int c, double dt) {
                        cout << "# count TS: " << c << endl << "Tabu search duration: " << dt << endl;
                    });
            cout << "Tabu result: ";
            print_triplets(tabu, cout);


        } else {
            cout << "Enter numbers amount (a multiple of three): ";
            int amount;
            do {
                cin >> amount;
            } while (amount % 3 != 0 && amount < 3);
            vector<int> numbers;
            cout << "Enter " << amount << " numbers: " << endl;
            for (int i = 0; i < amount; i++) {
                int number;
                cin >> number;
                numbers.push_back(number);
            }
            cout << "Given numbers: ";
            print(numbers, cout);
            cout << endl << "Brute force: " << endl;
            vector<vector<int>> starting_point = generate_working_point(numbers);
            brute_force(numbers, starting_point, cout);
            hill_climb(20, starting_point, cout);
            hill_climb_stochastic(20, starting_point, cout);
            vector<vector<int>> tabu = tabu_search(
                    goal_solution,
                    [&]() { return starting_point; },
                    generate_neighbours,
                    20,
                    starting_point.size(),
                    [](int c, double dt) {
                        cout << "# count TS: " << c << endl << "Tabu search duration: " << dt << endl;
                    });
            cout << "Tabu result: ";
            print_triplets(tabu, cout);
        }
    }
    return 0;
}
