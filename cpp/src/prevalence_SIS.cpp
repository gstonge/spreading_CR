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
#include <cmath>

using namespace std;
using namespace net;


int main(int argc, char const *argv[])
{
	// Get the simulation parameters from the command line
	string edge_list_path = argv[1];
	double transmission_rate = stod(argv[2]);
	double recovery_rate = stod(argv[3]);
	unsigned int thermalize_step = atoi(argv[4]);
	unsigned int average_step = atoi(argv[5]);
	double infected_fraction = stod(argv[6]);
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

	//thermalize the system
	for (int i = 0; i < thermalize_step and net.prevalence() > 0; ++i)
	{
		update_event(net,gen);
	}

	//average data
	vector<double> t(1,0);
	vector<double> prevalence(1, net.prevalence());
	double dt;
	for (int i = 0; i < average_step and prevalence.back() > 0; ++i)
	{
		dt = get_lifetime(net, gen);
		t.push_back(t.back()+dt);
		update_event(net,gen);
		prevalence.push_back(net.prevalence());
	}

	//get average prevalence
	double avg_prevalence = 0.;
	double second_moment = 0.;
	double total_time = t.back();
	if (prevalence.back() == 0)
	{
		//reached the absorbing state in finite time (prevalence 0)
	}
	else
	{
		for (int i = 0; i < prevalence.size()-1; ++i)
		{
			avg_prevalence += prevalence[i]*(t[i+1]-t[i]);
			second_moment += prevalence[i]*prevalence[i]*(t[i+1]-t[i]);
		}
		avg_prevalence /= total_time;
		second_moment /= total_time;
	}
	double std_prevalence = sqrt(second_moment - avg_prevalence*avg_prevalence);

	cout << avg_prevalence << " " << std_prevalence << endl;

	return 0;
}