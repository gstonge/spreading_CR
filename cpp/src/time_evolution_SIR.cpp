/**
* \file incidence_SIS.cpp
* \brief output the incidence and time vector.
* \author Guillaume St-Onge
* \version 1.0
* \date 09/02/2018
*/

#include <evolution.hpp>
#include <io_data.hpp>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>

using namespace std;
using namespace net;


int main(int argc, char const *argv[])
{
	// Get the simulation parameters from the command line
	string edge_list_path = argv[1];
	double transmission_rate = stod(argv[2]);
	double recovery_rate = stod(argv[3]);
	double infected_fraction = stod(argv[4]);
	unsigned int seed = atoi(argv[5]);
	double waning_immunity_rate = 0; //SIR

	// Initialize RNG
	RNGType gen(seed);

	//Initialize network and data saving vectors
	std::vector<std::pair<NodeLabel, NodeLabel> > edge_list = input_edge_list(
		edge_list_path);
	StaticNetworkSIR net(edge_list, transmission_rate, recovery_rate, 
		waning_immunity_rate);
	infect_fraction(net, infected_fraction, gen);
	vector<double> t(1,0);
	vector<double> incidence(1, net.incidence());

	double dt;
	while (net.prevalence() > 0)
	{
		dt = get_lifetime(net,gen);
		t.push_back(t[t.size()-1]+dt);
		update_event(net,gen);
		incidence.push_back(net.incidence());
	}

	//output data
	for (int i = 0; i < t.size(); ++i)
	{
		cout << t[i] << " " << incidence[i] << endl;
	}

	return 0;
}