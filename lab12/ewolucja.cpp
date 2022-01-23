#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <random>
#include <vector>
#include <algorithm>

using namespace std;

random_device rd;  //Will be used to obtain a seed for the random number engine
mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

vector<double> simulated_annealing(
        function<double(vector<double>)> f, function<bool(vector<double>)> f_domain,
        vector<double> p0, int iterations, uniform_real_distribution<double> shift,
        function<double(int)> T){
    uniform_real_distribution<> u_k(0.0, 1.0);
    auto p = p0;
    auto p_best = p0;
    uniform_int_distribution<> distrib(0, p.size()-1);
    for (int i = 0; i < iterations; i++) {
        vector<double> p2;
        do{ p2 = p;
            p2[distrib(gen)] += shift(gen);
        }while(!f_domain(p2));
        double y2 = f(p2);
        if (y2 < f(p)) {
            p = p2;
        }else{
            double u = u_k(gen);
            if (u < exp(-abs(f(p2) - f(p)) / T(i))) {
                p = p2;
            }
        }
        if (f(p) < f(p_best)) {
            p_best = p;
        }
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

int main(int argc, char **argv){
    uniform_real_distribution<> hump_r(-5.15, 5.12);
    vector<double> arguments;
    for(int i = 0; i < 25; i++){
        arguments.push_back(hump_r(gen));
    }
    auto f = rastrigin_function(arguments);
    cout<< f;
    vector<bool> jajco = {true, true, false};
    if(!count(jajco.begin(), jajco.end(), false)){
        cout<<"dsafsda";
    }
    return 0;
}