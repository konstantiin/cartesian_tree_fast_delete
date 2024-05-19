#include <string_view>
#include <iostream>
#include <cassert>
#include <set>
#include <fstream>
#include <chrono>
#include "cartesian_tree.h"



void write_to_file(std::string_view name, std::vector <long long> &v, std::ofstream &fout){
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
void test(cartesian::cartesian_tree <int> &cartesianTree){
    for (int i = 0; i < 10; i++){
        cartesianTree.insert(i);
    }
    for (int i = 0; i < 5; i++){
        cartesianTree.insert(i);
    }
    assert(*cartesianTree.find_min() == 0);
    for (int i = 0; i < 5; i++){
        cartesianTree.erase(i);
    }
    assert(*cartesianTree.find_min() == 0);
    for (int i = 0; i < 5; i++){
        cartesianTree.erase(i);
    }
    assert(*cartesianTree.find_min() == 5);
    cartesianTree.insert(4);
    assert(*cartesianTree.find_min() == 4);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.precision(18);
    std::ofstream fout;
    fout.open("../data.json");
    fout << "{\n";

    cartesian::cartesian_tree <int> fast;
    test(fast);
    std::multiset<int> stand;

    std::vector <long long> time_custom;
    std::vector <long long> time_std;

    for (int i = 0; i < 1e2; i++){
        time_custom.push_back(time_it([&fast, &i](){
            fast.insert(i);
        }));
        time_std.push_back(time_it([&stand, &i](){
            stand.insert(i);
        }));

    }
    write_to_file("my_insert", time_custom, fout);
    fout << ",\n";
    write_to_file("std_insert", time_std, fout);
    fout << ",\n";
    time_custom.clear();
    time_std.clear();
    for (int i = 0; i < 1e2; i++){
        time_custom.push_back(time_it([&fast, &i](){
            fast.erase(i);
        }));
        time_std.push_back(time_it([&stand, &i](){
            stand.erase(i);
        }));
    }
    write_to_file("my_erase", time_custom, fout);
    fout << ",\n";
    write_to_file("std_erase", time_std, fout);
    fout << ",\n";
    time_custom.clear();
    time_std.clear();
    for (int i = 0; i < 1e2; i++){
        time_custom.push_back(time_it([&fast](){
            int* ptr = fast.find_min();
            if (ptr){
                fast.erase(*ptr);
            }
        }));
        time_std.push_back(time_it([&stand](){
            if (!stand.empty()){
                stand.erase(*stand.begin());
            }
        }));
    }
    write_to_file("my_min_erase", time_custom, fout);
    fout << ",\n";
    write_to_file("std_min_erase", time_std, fout);
    fout << ",\n";
    time_custom.clear();
    time_std.clear();
    for(int i = 0; i < 1e2; i++){
        int cmd = cartesian::uniform(cartesian::rng) % 3;
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
                int* ptr = fast.find_min();
                if (ptr){
                    fast.erase(*ptr);
                }
            }));
            time_std.push_back(time_it([&stand](){
                if (!stand.empty()){
                    stand.erase(*stand.begin());
                }
            }));
        }
    }
    write_to_file("random_my", time_custom, fout);
    fout << ",\n";
    write_to_file("random_std", time_std, fout);
    fout << "\n}\n";
    fout.close();
    return 0;
}
