/**
* \file time_SIR.cpp
* \brief Calculate the execution time per step for SIR process.
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
	unsigned int seed = atoi(argv[5]);
	double waning_immunity_rate = 0; //SIR

	// we average over 10 realization to reduce the variance observed
	double average_time_span = 0;
	double average_incidence = 0;

	// get edge-list
	std::vector<std::pair<NodeLabel, NodeLabel> > edge_list = input_edge_list(
		edge_list_path);

	// Initialize RNG
	RNGType gen(seed);

	for (int i = 0; i < 10; ++i)
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
		average_time_span += 100000*time_span.count()/(2*net.get_Rnode_number());
		average_incidence += 0.1*net.incidence();

	}
	cout << average_incidence << " " << average_time_span << endl;
	
	// double time_span = double(t2-t1)/CLOCKS_PER_SEC;
	// duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	//output time
	// cout << net.size() << endl;
	

	return 0;
}