/**
* \file evolution.cpp
* \brief Function module propagation dynamics evolution
* \author Guillaume St-Onge
* \version 1.0
* \date 06/02/2018
*/
#include <iostream>
#include <evolution.hpp>
#include <utility>

using namespace std;

namespace net
{


/**
 * \brief Infect a fraction of the nodes
 * \param[in] net a reference to an object StaticNetworkSIR
 * \param[in] fraction a double representing the fraction to infect
 */
void infect_fraction(StaticNetworkSIR& net, double fraction, RNGType& gen)
{
	unsigned int number_of_infection = floor(net.size()*fraction);
	uniform_real_distribution<double> random_01(0.,1.);
	NodeLabel i;
	while (net.get_Inode_number() < number_of_infection)
	{
		i = floor(random_01(gen)*net.size());
		if (net.is_susceptible(i))
		{
			net.infection(i);
		}
	}
}

/**
 * \brief Choose between infection, recovery or immunity loss event
 * \param[in] net a reference to an object StaticNetworkSIR
 * \param[in] gen a reference to a random number generator
 */
void update_event(StaticNetworkSIR& net, RNGType& gen)
{
	uniform_real_distribution<double> random_01(0.,1.);
	GroupIndex group_index = (net.get_event_tree()).get_leaf_index(
		random_01(gen));

	//alias to propensity group
	const PropensityGroup& propensity_group = net.get_propensity_group(
		group_index); 
	double max_propensity = net.get_max_propensity(group_index);

	//Determine the in_group_index
	// uniform_int_distribution<int> random_index(0,propensity_group.size()-1);
	size_t in_group_index;
	double propensity;
	bool chosen = false;
	while (not chosen)
	{
		double r1 = random_01(gen);
		in_group_index = floor(r1*propensity_group.size());
		//recycling r1
		double r2 = r1*propensity_group.size()-in_group_index;
		propensity = propensity_group[in_group_index].second;
		if (r2 < propensity/max_propensity)
		{
			chosen = true;
		}
	}

	//Determine which type of event
	NodeLabel node = propensity_group[in_group_index].first;
	if (net.is_recovered(node) and net.get_waning_immunity_rate() > 0)
	{
		//immunity loss event
		net.immunity_loss(group_index, in_group_index);
	}
	else
	{
		//the node is infected - infection or recovery
		double r1 = random_01(gen);
		if (r1 < net.get_recovery_rate()/propensity)
		{
			//recovery
			net.recovery(group_index, in_group_index);
		}
		else
		{
			//recycling r1
			double r2 = (propensity*r1-net.get_recovery_rate())/(propensity
				-net.get_recovery_rate());
			//infection attempt of a neighbor
			const vector<NodeLabel>& neighbor_vector = net.get_neighbor_vector(node);
			NodeLabel neighbor_node = neighbor_vector[floor(r2*
				neighbor_vector.size())];
			if (net.is_susceptible(neighbor_node))
			{
				net.infection(neighbor_node);
			}
		}
	}
}

/**
 * \brief return the average life time for the configuration
 * \param[in] net a reference to an object StaticNetworkSIR
 * \param[in] gen a reference to a RNG
 */
double get_lifetime(StaticNetworkSIR& net, RNGType& gen)
{
	exponential_distribution<double> random_lifetime(
		(net.get_event_tree()).get_value());
	return random_lifetime(gen);
}

/**
 * \brief update the history vector of Inode
 * \param[in] net a reference to an object StaticNetworkSIR
 * \param[in] max_configuration the maximal history vector size
 * \param[in] history_vector vector of network state
 * \param[in] gen a reference to a random number generator
 */
void update_history(StaticNetworkSIR& net, unsigned int max_configuration, 
	vector<Configuration>& history_vector, RNGType& gen)
{
	size_t M = history_vector.size();
	if (M >= max_configuration)
	{
		uniform_int_distribution<int> random_index(0, M-1);
		size_t index = random_index(gen);
		swap(history_vector[index],history_vector[M-1]);
		history_vector.pop_back();	
	}
	history_vector.push_back(Configuration());
	net.get_configuration_copy(history_vector[M-1]);
}

/**
 * \brief Get a random state from the history
 * \param[in] net a reference to an object StaticNetworkSIR
 * \param[in] history_vector vector of network state
 * \param[in] gen a reference to a random number generator
 */
void get_from_history(StaticNetworkSIR& net,
	vector<Configuration>& history_vector, RNGType& gen)
{
	size_t M = history_vector.size();
	uniform_int_distribution<int> random_index(0, M-1);
	size_t index = random_index(gen);
	net.set_configuration(history_vector[index]);
}

} //end of namespace
