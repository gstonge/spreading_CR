/**
* \file incidence_SIS.cpp
* \brief output the final size of the outbreak
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

	while (net.prevalence() > 0)
	{
		update_event(net,gen);
	}

	//output data
	cout << net.incidence() << endl;

	return 0;
}