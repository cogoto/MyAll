#include <iostream>

#include <string>

#include <chrono>

#include <algorithm>

#include "MyRandom.hpp"

using namespace std;


int main() {

    cout << "TEST OF [MyRandom]" << endl << endl;

    MyAll::MyRandom rnd_;

    string seed_type;
    //int seed_val = 0;

    int min  = 0;
    int max  = 180-1;
    int size = 54;
    int n    = 5;

    cout << "[Min.] " << min << endl;
    cout << "[Max.] " << max << endl;
    cout << "[Size] " << size << endl;

    vector<int> result;

    //std::chrono::system_clock::time_point  start, end; // 型は auto で可
    auto start = std::chrono::system_clock::now(); // 計測開始時間

    rnd_.Seed("seed", 2020);

    for(int i=0;i<n;i++) {

        //rnd_.Seed();
        //rnd_.Seed("time");

        rnd_.MakeVector_uni(min, max, size, result);

        cout << "[random array]" ;
        for (const auto &it:result) {
            cout << " " << it;
        }
        cout << endl;

        sort(result.begin(), result.end());

        //

        int seed_val;

        rnd_.GetSeed(seed_type, seed_val);
        cout << "[SEED] " << seed_type << " : " << seed_val << endl;

        cout << "[random array]" ;
        for (const auto &it:result) {
            cout << " " << it;
        }
        cout << endl;

    }

    auto end = std::chrono::system_clock::now();  // 計測終了時間
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    cout << "duration : " << elapsed << " msec" << endl;

    cout << endl << "TEST END." << endl;

    return 0;

}
