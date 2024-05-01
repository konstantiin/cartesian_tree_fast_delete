#include <bits/stdc++.h>
#include "cartesian_tree.h"


std::ofstream fout;
void write_to_file(std::string_view name, std::vector <long long> &v){
    fout << "\"" << name << "\"" << ": [";
    for (int i = 0; i < v.size() - 1; i++) {
        fout << v[i] << ", ";
    }
    fout << v[v.size() - 1] << "]";
}

long long time_it(const auto func){
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>( start- end).count();
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.precision(18);
    fout.open("../data.json");
    fout << "{\n";

    cartesian_tree <int> fast;
    std::set<int> stand;

    std::vector <long long> time_custom;
    std::vector <long long> time_std;

    for (int i = 0; i < 1e6; i++){
        time_custom.push_back(time_it([&fast, &i](){
            fast.insert(i);
        }));
        time_std.push_back(time_it([&stand, &i](){
            stand.insert(i);
        }));

    }
    write_to_file("my_insert", time_custom);
    fout << ",\n";
    write_to_file("std_insert", time_std);
    fout << ",\n";
    time_custom.clear();
    time_std.clear();
    for (int i = 0; i < 1e5; i++){
        time_custom.push_back(time_it([&fast, &i](){
            fast.erase(i);
        }));
        time_std.push_back(time_it([&stand, &i](){
            stand.erase(i);
        }));
    }
    write_to_file("my_erase", time_custom);
    fout << ",\n";
    write_to_file("std_erase", time_std);
    fout << ",\n";
    time_custom.clear();
    time_std.clear();
    for (int i = 0; i < 1e5; i++){
        time_custom.push_back(time_it([&fast](){
            int minimal = fast.find_min();
            fast.erase(minimal);
        }));
        time_std.push_back(time_it([&stand](){
            int minimal= *stand.begin();
            stand.erase(minimal);
        }));
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
            time_custom.push_back(time_it([&fast, &i](){
                fast.insert(i);
            }));
            time_std.push_back(time_it([&stand, &i](){
                stand.insert(i);
            }));
        } else if (cmd == 1){// erase
            time_custom.push_back(time_it([&fast, &i](){
                fast.erase(i);
            }));
            time_std.push_back(time_it([&stand, &i](){
                stand.erase(i);
            }));
        } else{ // get_min
            time_custom.push_back(time_it([&fast](){
                int minimal = fast.find_min();
                fast.erase(minimal);
            }));
            time_std.push_back(time_it([&stand](){
                int minimal= *stand.begin();
                stand.erase(minimal);
            }));
        }
    }
    write_to_file("random_my", time_custom);
    fout << ",\n";
    write_to_file("random_std", time_std);
    fout << "\n}\n";
    return 0;
}
