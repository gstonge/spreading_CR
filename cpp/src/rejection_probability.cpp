/**
* \file critical_time_SIS.cpp
* \brief Script that outputs the rejection probability for different transmission rate
value, for the SIS model.
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
	string model = argv[2];
	double transmission_rate_min = stod(argv[3]);
	double transmission_rate_max = stod(argv[4]);
	double delta_transmission = stod(argv[5]);
	double infected_fraction = stod(argv[6]);
	unsigned int thermalize_steps = atoi(argv[7]);
	unsigned int average_steps = atoi(argv[8]);
	unsigned int seed = atoi(argv[9]);

	// input edge edge_list
	std::vector<std::pair<NodeLabel, NodeLabel> > edge_list = input_edge_list(
		edge_list_path);

	// Initialize transmission rate vector
	vector<double> transmission_rate_vector(1, transmission_rate_min);
	while (transmission_rate_vector.back() < 
		transmission_rate_max - delta_transmission)
	{
		transmission_rate_vector.push_back(
			transmission_rate_vector.back()+delta_transmission);
	}
	double recovery_rate = 1.;
	double waning_immunity_rate;
	if (model == "SIS")
	{
		waning_immunity_rate = std::numeric_limits<double>::infinity(); //SIS
	}
	else
	{
		waning_immunity_rate = 0;
	}

	// Initialize RNG
	RNGType gen(seed);

	// Initialize rejection probability vector
	vector<double> rejection_probability_vector;
	vector<double> average_prevalence_vector;
	vector<double> incidence_vector;


	for (int i = 0; i < transmission_rate_vector.size(); ++i)
	{
		//Initialize the network
		StaticNetworkSIR net(edge_list, transmission_rate_vector[i], recovery_rate, 
			waning_immunity_rate);
		infect_fraction(net, infected_fraction, gen);

		//thermalize
		double productive_event = 0;
		double Inode_number = net.get_Inode_number();
		while (productive_event < thermalize_steps and model == "SIS")
		{
			update_event(net,gen);
			if (net.get_Inode_number() != Inode_number)
			{
				productive_event += 1;
			}
			Inode_number = net.get_Inode_number();
		}

		//Determine the rejection probability
		productive_event = 0.;
		double total_event = 0.;
		double total_time = 0.;
		double dt;
		double average_prevalence = 0.;
		while (productive_event < average_steps and net.get_Inode_number() > 0)
		{
			dt = get_lifetime(net,gen);
			total_time += dt;
			average_prevalence += net.prevalence()*dt;
			update_event(net,gen);
			if (net.get_Inode_number() != Inode_number)
			{
				productive_event += 1;
			}
			total_event += 1;
			Inode_number = net.get_Inode_number();
		}
		rejection_probability_vector.push_back(1 - productive_event/total_event);
		average_prevalence_vector.push_back(average_prevalence/total_time);
		incidence_vector.push_back(net.incidence());
	}

	//output rejection probability and prevalence / incidence
	if (model == "SIS")
	{
		for (int i = 0; i < transmission_rate_vector.size(); ++i)
		{
			cout << average_prevalence_vector[i] << " ";
			cout << rejection_probability_vector[i] << endl;
		}
	}
	else
	{
		for (int i = 0; i < transmission_rate_vector.size(); ++i)
		{
			cout << incidence_vector[i] << " ";
			cout << rejection_probability_vector[i] << endl;
		}
	}

	return 0;
}