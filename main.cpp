//
// Created by renat on 9/5/2023.
//

#include "src/ISAM.cpp"
#include "src/Tester.cpp"
using namespace std;



int main(int argc, char* argv[]) {
    Tester<int> t1;
//    Tester<float> t2;

    t1.test_all();
//    t2.test_all();
    return 0;
}