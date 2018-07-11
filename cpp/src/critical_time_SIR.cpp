/**
* \file critical_time_SIS.cpp
* \brief Determine the computational time near the absorbing phase for the SIS 
model on networks.
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
#include <chrono>
#include <cmath>

using namespace std;
using namespace std::chrono;
using namespace net;


int main(int argc, char const *argv[])
{
	// Get the simulation parameters from the command line
	string edge_list_path = argv[1];
	double factor = stod(argv[2]);
	double infected_fraction = stod(argv[3]);
	unsigned int seed = atoi(argv[4]);
	double transmission_rate; //to be determined
	double recovery_rate = 1.;
	double waning_immunity_rate = 0; //SIR

	// Initialize RNG
	RNGType gen(seed);

	//Get the degree sequence and network size
	std::vector<std::pair<NodeLabel, NodeLabel> > edge_list = input_edge_list(
		edge_list_path);
	Network* temp_net_ptr = new Network(edge_list);
	size_t network_size = temp_net_ptr->size();
	vector<double> degree_sequence;
	degree_sequence.reserve(temp_net_ptr->size());
	for (int i = 0; i < temp_net_ptr->size(); i++)
	{
		degree_sequence.push_back(temp_net_ptr->degree(i));
	}
	delete temp_net_ptr;

	//determine the threshold and transmission rate
	double numerator = 0.;
	double denominator = 0.;
	double max_degree = 0.;
	for (int i = 0; i < degree_sequence.size(); i++)
	{
		numerator += degree_sequence[i];
		denominator += degree_sequence[i]*(degree_sequence[i]-2);
	}
	double threshold = numerator/denominator;
	transmission_rate = threshold*factor;

	//determine the number of repetition to reduce variance
	size_t repetition = ceil(1000000./network_size);
	double average_time_span = 0.;

	for (int i = 0; i < repetition; ++i)
	{
		//Initialize network
		StaticNetworkSIR net(edge_list, transmission_rate, recovery_rate, 
			waning_immunity_rate);

		//benchmark time
		steady_clock::time_point t1 = steady_clock::now();
		infect_fraction(net, infected_fraction, gen);
		while (net.get_Inode_number() > 0)
		{
			update_event(net,gen);
		}
		steady_clock::time_point t2 = steady_clock::now();
		duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
		average_time_span += time_span.count()/(2*net.get_Rnode_number());
	}
	cout << 1000000*average_time_span/repetition << endl;

	return 0;
}