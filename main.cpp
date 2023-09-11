//
// Created by renat on 9/5/2023.
//

#include "src/ISAM.cpp"
using namespace std;



int main(int argc, char* argv[]) {
    ISAM<int> isam;
    IndexPage<int> ip;

    for (int i=0; i<ip.getCapacity(); ++i) {
        ip.setKey(i ,i);
    }
    for (int i=0; i<ip.getCapacity()+1; ++i) {
        ip.setChild(i, i);
    }
    ip.setIsLeaf(true);

    ofstream file("out.dat", ios::binary);
    ip.write(file);

    ifstream f("out.dat", ios::binary);
    IndexPage<int> ip2;
    ip2.read(f);

    int* keys = ip.getKeys();
    POS_TYPE * child = ip.getChildren();

    int* keys2 = ip2.getKeys();
    POS_TYPE * child2 = ip2.getChildren();


    for (int i=0; i<ip.getCapacity(); ++i) {
        cout << keys[i] << " ";
        cout << keys2[i] << " ";
    }
    cout << endl;

    for (int i=0; i<ip.getCapacity()+1; ++i) {
        cout << child[i] << " ";
        cout << child2[i] << " ";
    }
    cout << endl;
    cout << ip2.getIsLeaf() << endl;



    return 0;
}