#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>

using namespace std;

random_device rd;  //Will be used to obtain a seed for the random number engine
mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

ostream& operator<<(ostream& o, vector<double> v){
    for (auto e : v) {
        o << std::fixed << std::setprecision(5) << " " << e;
    }
    return o;
}

vector<double> simulated_annealing(
        function<double(vector<double>)> f, function<bool(vector<double>)> f_domain,
        vector<double> p0, int iterations, ofstream &out, double shift,
        function<vector<double>(vector<double>)> neighbour, function<double(int, double )> T,
        function<void(int i, double current_goal_val, double goal_val)>
        on_iteration = [](int i, double current_goal_val, double goal_val) {},
        function<void(int c, double dt)> on_statistics = [](int c, double dt){}){
    uniform_real_distribution<> u_k(0.0, 1.0);
    auto start = chrono::steady_clock::now();
    auto p = p0;
    auto p_best = p0;
    int iterator = 0;
    for (int i = 0; i < iterations; i++) {
        auto p2 = neighbour(p);
        if(f_domain(p2)) continue;
        if (f(p2) < f(p)) {
            p = p2;
        }else{
            double u = u_k(gen);
            if (u < exp(-abs(f(p2) - f(p)) / T(i, shift))) {
                p = p2;
            }
        }
        if (f(p) < f(p_best)) {
            p_best = p;
        }
        iterator++;
        on_iteration(iterator, f(p), f(p_best));
    }
    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;

    on_statistics(iterator, duration.count());

    return p_best;
}

auto rastrigin_function_domain = [](vector<double> v){
    vector<bool> domain;
    for(int i =0; i<v.size(); i++){
        domain.push_back((abs(v[i]) <= 5.12) && (abs(v[i]) <= 5.12));
    }
    return count(domain.begin(), domain.end(), false);
};

auto rastrigin_function = [](vector<double> arguments){
    int n = arguments.size();
    double sum;
    for(int i = 0; i < n; i++){
        sum+= pow(arguments[i],2)-(10* cos(M_PI*arguments[i]));
    }
    return 10*n+sum;
};

auto temp_func_1(int k, double shift){
    return 1.0 / (k);
}

auto temp_func_2(int k, double shift){
    return pow(k, shift);
}

auto temp_func_3(int k, double shift){
    return 1/ log10(k);
}

vector<double> generate_neighbour(vector<double> wrk_pnt){
    vector<double> neighbour = wrk_pnt;
    uniform_int_distribution<int> index = uniform_int_distribution<int> (0, neighbour.size()-1);
    uniform_int_distribution<int> choice = uniform_int_distribution<int> (0, 1);
    normal_distribution<> distribution(-1, 1);
    if(choice(gen)) {
        neighbour[index(gen)] -= distribution(gen); //TODO
    }else{
        neighbour[index(gen)] += distribution(gen);;
    }
    return neighbour;
}

int main(int argc, char **argv){
    vector<double> arguments;
    uniform_real_distribution<double> bounds = uniform_real_distribution<double>(-5.12, 5.12);
    string s_size = argv[1];
    int size = stoi(s_size);
    string s_temp_func = argv[2];
    int temp_func = stoi(s_temp_func);
    string s_shift = argv[3];
    double shift = stod(s_shift);
    string s_iterations = argv[4];
    int iterations = stoi(s_iterations);
    string outfile_name = "out.txt";
    for(int i = 0; i<size; i++){
        arguments.push_back(bounds(gen));
    }

    auto step = [&](int i, double p_val, double p_best_val) {
        cout << i << " " << p_val << " " << p_best_val << endl;
    };

    auto finish = [](int iterator, double duration){
        cout << "# count: " << iterator << "; dt:  " << duration << endl;
    };
    vector<double> best;
    ofstream out(outfile_name);
    switch (temp_func) {
        case 1:
            best = simulated_annealing(rastrigin_function, rastrigin_function_domain, arguments,
                                iterations, out, shift, generate_neighbour, temp_func_1, step, finish);
            out.close();
            break;
        case 2:
            best = simulated_annealing(rastrigin_function, rastrigin_function_domain, arguments,
                                iterations, out, shift, generate_neighbour, temp_func_2, step, finish);
            out.close();
            break;
        case 3:
            best = simulated_annealing(rastrigin_function, rastrigin_function_domain, arguments,
                                iterations, out, shift, generate_neighbour, temp_func_3, step, finish);
            out.close();
            break;
    }

    cout << "# best " << temp_func << " result: " <<
         rastrigin_function(best) << endl;
    return 0;
}