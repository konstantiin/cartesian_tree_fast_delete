#include <bits/stdc++.h>
#include "myset.h"

using namespace std;
ofstream fout;
void write_to_file(const string& name, vector <long long> &v){
    fout << "\"" << name << "\"" << ": [";
    for (int i = 0; i < v.size() - 1; i++) fout << v[i] << ", ";
    fout << v[v.size() - 1] << "]";
}




int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.precision(18);
    fout.open("../data.json");
    fout << "{\n";

    Myset <int> fast;
    set<int> stand;

    vector <long long> time_custom;
    vector <long long> time_std;

    for (int i = 0; i < 1e6; i++){
        auto start = chrono::high_resolution_clock::now();
        fast.insert(i);
        auto mid = chrono::high_resolution_clock::now();
        stand.insert(i);
        auto end = chrono::high_resolution_clock::now();

        auto delta1 = chrono::duration_cast<chrono::nanoseconds>(mid - start);
        auto delta2 = chrono::duration_cast<chrono::nanoseconds>( end- mid);
        time_custom.push_back(delta1.count());
        time_std.push_back(delta2.count());
    }
    write_to_file("my_insert", time_custom);
    fout << ",\n";
    write_to_file("std_insert", time_std);
    fout << ",\n";
    time_custom.clear();
    time_std.clear();
    for (int i = 0; i < 1e5; i++){
        auto start = chrono::high_resolution_clock::now();
        fast.erase(i);
        auto mid = chrono::high_resolution_clock::now();
        stand.erase(i);
        auto end = chrono::high_resolution_clock::now();

        auto delta1 = chrono::duration_cast<chrono::nanoseconds>(mid - start);
        auto delta2 = chrono::duration_cast<chrono::nanoseconds>( end- mid);
        time_custom.push_back(delta1.count());
        time_std.push_back(delta2.count());
    }
    write_to_file("my_erase", time_custom);
    fout << ",\n";
    write_to_file("std_erase", time_std);
    fout << ",\n";
    time_custom.clear();
    time_std.clear();
    for (int i = 0; i < 1e5; i++){
        auto start = chrono::high_resolution_clock::now();
        int m1 = fast.find_min();
        fast.erase(m1);
        auto mid = chrono::high_resolution_clock::now();
        int m2 = *stand.begin();
        stand.erase(m2);
        auto end = chrono::high_resolution_clock::now();

        auto delta1 = chrono::duration_cast<chrono::nanoseconds>(mid - start);
        auto delta2 = chrono::duration_cast<chrono::nanoseconds>( end- mid);
        time_custom.push_back(delta1.count());
        time_std.push_back(delta2.count());
        assert(m1 == m2);
    }

    write_to_file("my_min_erase", time_custom);
    fout << ",\n";
    write_to_file("std_min_erase", time_std);
    fout << ",\n";
    time_custom.clear();
    time_std.clear();
    for(int i = 0; i < 1e5; i++){
        int cmd = uniform(rng) % 3;
        if (cmd == 0){ // insert
            auto start = chrono::high_resolution_clock::now();
            fast.erase(i);
            auto mid = chrono::high_resolution_clock::now();
            stand.erase(i);
            auto end = chrono::high_resolution_clock::now();

            auto delta1 = chrono::duration_cast<chrono::nanoseconds>(mid - start);
            auto delta2 = chrono::duration_cast<chrono::nanoseconds>( end- mid);
            time_custom.push_back(delta1.count());
            time_std.push_back(delta2.count());
        } else if (cmd == 1){// erase
            auto start = chrono::high_resolution_clock::now();
            fast.erase(i);
            auto mid = chrono::high_resolution_clock::now();
            stand.erase(i);
            auto end = chrono::high_resolution_clock::now();

            auto delta1 = chrono::duration_cast<chrono::nanoseconds>(mid - start);
            auto delta2 = chrono::duration_cast<chrono::nanoseconds>( end- mid);
            time_custom.push_back(delta1.count());
            time_std.push_back(delta2.count());
        } else{ // get_min
            auto start = chrono::high_resolution_clock::now();
            int m1 = fast.find_min();
            fast.erase(m1);
            auto mid = chrono::high_resolution_clock::now();
            int m2 = *stand.begin();
            stand.erase(m2);
            auto end = chrono::high_resolution_clock::now();

            auto delta1 = chrono::duration_cast<chrono::nanoseconds>(mid - start);
            auto delta2 = chrono::duration_cast<chrono::nanoseconds>( end- mid);
            time_custom.push_back(delta1.count());
            time_std.push_back(delta2.count());
            assert(m1 == m2);
        }
    }

    uniform(rng);
    write_to_file("random_my", time_custom);
    fout << ",\n";
    write_to_file("random_std", time_std);
    fout << "\n}\n";
    return 0;
}

