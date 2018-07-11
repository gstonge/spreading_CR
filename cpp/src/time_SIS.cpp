/**
* \file prevalence_SIS.cpp
* \brief output the prevalence and time vector.
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

using namespace std;
using namespace std::chrono;
using namespace net;


int main(int argc, char const *argv[])
{
	// Get the simulation parameters from the command line
	string edge_list_path = argv[1];
	double transmission_rate = stod(argv[2]);
	double recovery_rate = stod(argv[3]);
	double infected_fraction = stod(argv[4]);
	unsigned int thermalize_steps = atoi(argv[5]);
	unsigned int average_steps = atoi(argv[6]);
	unsigned int seed = atoi(argv[7]);
	double waning_immunity_rate = std::numeric_limits<double>::infinity(); //SIS

	// Initialize RNG
	RNGType gen(seed);

	//Initialize network
	std::vector<std::pair<NodeLabel, NodeLabel> > edge_list = input_edge_list(
		edge_list_path);
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
	double dt;
	vector<double> t(1,0);
	vector<double> prevalence(1, net.prevalence());
	productive_event = 0;
	steady_clock::time_point t1 = steady_clock::now();
	while (productive_event < average_steps and not absorbing_state_reached)
	{
		dt = get_lifetime(net, gen);
		t.push_back(t.back()+dt);
		update_event(net,gen);
		prevalence.push_back(net.prevalence());
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

	//get average prevalence
	double average_prevalence = 0;
	for (int i = 0; i < prevalence.size()-1; ++i)
	{
		average_prevalence += prevalence[i]*(t[i+1]-t[i]);
	}
	average_prevalence /= t.back();

	//output time
	if (not absorbing_state_reached)
	{
		cout << average_prevalence << " " ;
		cout << 1000000*time_span.count()/average_steps << endl; //output in μs
	}

	return 0;
}