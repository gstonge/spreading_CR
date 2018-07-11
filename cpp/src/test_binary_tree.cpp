/**
* \file main.cpp
* \brief main file for c++ project test
* \author Guillaume St-Onge
* \version 1.0
* \date 
*/

#include <iostream>
#include <BinaryTree.hpp>
#include <StaticNetworkSIR.hpp>
#include <evolution.hpp>
#include <io_data.hpp>

using namespace std;
using namespace net;


int main(int argc, char const *argv[])
{
	std::vector<std::pair<NodeLabel, NodeLabel> > edge_list = input_edge_list(
		"./bin/edgeList_ER1000_k10.txt");
	double transmission_rate = 0.10;
	double recovery_rate = 1.;
	double waning_immunity_rate = std::numeric_limits<double>::infinity();
	RNGType gen(42);

	StaticNetworkSIR net(edge_list, transmission_rate, recovery_rate, 
		waning_immunity_rate);
	infect_fraction(net, 0.01, gen);

	vector<double> t(1,0);
	vector<double> prevalence(1, net.prevalence());
	double dt;

	for (int i = 0; i < 50000 and prevalence.back() > 0; ++i)
	{
		dt = get_lifetime(net,gen);
		t.push_back(t[t.size()-1]+dt);
		update_event(net,gen);
		prevalence.push_back(net.prevalence());
		if (prevalence.back() == 0.)
		{
			break;
		}
	}

	//output data
	for (int i = 0; i < t.size(); ++i)
	{
		cout << t[i] << " " << prevalence[i] << endl;
	}

	return 0;
}