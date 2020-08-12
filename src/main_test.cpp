#include "SpreadingProcess.hpp"
#include "BinaryTree.hpp"
#include <vector>
#include <utility>
#include <iostream>

using namespace std;
using namespace net;

int main (int argc, char *argv[])
{
    vector<pair<NodeLabel,NodeLabel>> edge_list;
    edge_list.push_back(make_pair(0,1));
    edge_list.push_back(make_pair(0,2));
    double transmission_rate = 1.;
    double recovery_rate = 1.;
    double waning_immunity_rate = 0.; //SIR

    SpreadingProcess(edge_list,transmission_rate,recovery_rate,
            waning_immunity_rate);

    cout << "Hello World!" << endl;

    return 0;
}
