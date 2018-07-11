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
	unsigned int thermalize_steps = atoi(argv[4]);
	unsigned int average_steps = atoi(argv[5]);
	unsigned int seed = atoi(argv[6]);
	double transmission_rate; //to be determined
	double recovery_rate = 1.;
	double waning_immunity_rate = std::numeric_limits<double>::infinity(); //SIS

	// Initialize RNG
	RNGType gen(seed);

	//Get the degree sequence
	std::vector<std::pair<NodeLabel, NodeLabel> > edge_list = input_edge_list(
		edge_list_path);
	Network* temp_net_ptr = new Network(edge_list);
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
		denominator += degree_sequence[i]*(degree_sequence[i]-1);
		if (degree_sequence[i] > max_degree)
		{
			max_degree = degree_sequence[i];
		}
	}
	double threshold1 = numerator/denominator;
	double threshold2 = sqrt(2/max_degree);
	double threshold = min(threshold1,threshold2);
	transmission_rate = threshold*factor;

	//Initialize the network
	StaticNetworkSIR net(edge_list, transmission_rate, recovery_rate, 
		waning_immunity_rate);
	infect_fraction(net, infected_fraction, gen);

	//thermalize
	int productive_event = 0;
	bool absorbing_state_reached = false;
	double Inode_number = net.get_Inode_number();
	while (productive_event < thermalize_steps and not absorbing_state_reached)
	{
		update_event(net,gen);
		if (net.get_Inode_number() == 0)
		{
			absorbing_state_reached = true;
		}
		if (net.get_Inode_number() != Inode_number)
		{
			productive_event += 1;
		}
		Inode_number = net.get_Inode_number();
	}

	//benchmark time
	productive_event = 0;
	steady_clock::time_point t1 = steady_clock::now();
	while (productive_event < average_steps and not absorbing_state_reached)
	{
		update_event(net,gen);
		if (net.get_Inode_number() == 0)
		{
			absorbing_state_reached = true;
		}
		if (net.get_Inode_number() != Inode_number)
		{
			productive_event += 1;
		}
		Inode_number = net.get_Inode_number();
	}
	steady_clock::time_point t2 = steady_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	//output time
	if (not absorbing_state_reached)
	{
		cout << 1000000*time_span.count()/average_steps << endl; //output in μs
	}

	return 0;
}