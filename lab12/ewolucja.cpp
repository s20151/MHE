#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>

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
        function<vector<double>(vector<double>)> neighbour, function<double(int, double )> T){
    uniform_real_distribution<> u_k(0.0, 1.0);
    auto p = p0;
    auto p_best = p0;
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
        out <<"Xi ;" << p_best << " ; Y ;" << f(p_best) << " ; " << "iterations ;" << iterations << endl;
    }
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
    if(choice(gen)) {
        neighbour[index(gen)] -= 0.5;
    }else{
        neighbour[index(gen)] += 0.5;
    }
    return neighbour;
}

int main(int argc, char **argv){
    vector<double> arguments = {5 , 4 , 3, 1, 3};
    int iterations = stoi(argv[1]);
    int temp_func = stoi(argv[2]);
    double shift = stod(argv[3]);
    ofstream out("out.txt");
    switch (temp_func) {
        case 1:
            simulated_annealing(rastrigin_function, rastrigin_function_domain, arguments,
                                iterations, out, shift, generate_neighbour, temp_func_1);
            out.close();
            break;
        case 2:
            simulated_annealing(rastrigin_function, rastrigin_function_domain, arguments,
                                iterations, out, shift, generate_neighbour, temp_func_2);
            out.close();
            break;
        case 3:
            simulated_annealing(rastrigin_function, rastrigin_function_domain, arguments,
                                iterations, out, shift, generate_neighbour, temp_func_3);
            out.close();
            break;
    }


    return 0;
}