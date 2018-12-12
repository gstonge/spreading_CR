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
#include <cmath>

using namespace std;

namespace net
{

uint32_t INDEX_MAP(uint32_t NUM)
{
    return ((NUM == 4294967296) ? (TABLE_SIZE-1) :
            static_cast<uint32_t>(TABLE_SIZE*double(NUM)/4294967296));
}

/**
 * \brief Infect a fraction of the nodes
 * \param[in] net a reference to an object StaticNetworkSIR
 * \param[in] fraction a double representing the fraction to infect
 */
void infect_fraction(StaticNetworkSIR& net, double fraction, RNGType& gen,
        uniform_real_distribution<double>& random_01)
{
	unsigned int number_of_infection = floor(net.size()*fraction);
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
void update_event(StaticNetworkSIR& net, RNGType& gen,
        uniform_real_distribution<double>& random_01)
{
	GroupIndex group_index = (net.get_event_tree()).get_leaf_index(
		random_01(gen));

	//alias to propensity group
	const PropensityGroup& propensity_group = net.get_propensity_group(
		group_index);
	double max_propensity = net.get_max_propensity(group_index);

	//Determine the in_group_index
	size_t in_group_index;
	double propensity;
	bool chosen = false;

    double r;
	while (not chosen)
	{
		r = random_01(gen);
		in_group_index = floor(r*propensity_group.size());
        r = random_01(gen);
		propensity = propensity_group[in_group_index].second;
		if (r < propensity/max_propensity)
		{
			chosen = true;
		}
	}


	r = random_01(gen);
	//Determine which type of event
	NodeLabel node = propensity_group[in_group_index].first;
    if (net.is_infected(node))
	{
		//the node is infected - infection or recovery
		if (r < net.get_recovery_rate()/propensity)
		{
			//recovery
			net.recovery(group_index, in_group_index);
		}
		else
		{
            r = random_01(gen);
			//infection attempt of a neighbor
			const vector<NodeLabel>& neighbor_vector = net.get_neighbor_vector(node);
			NodeLabel neighbor_node = neighbor_vector[floor(r*
				neighbor_vector.size())];
			if (net.is_susceptible(neighbor_node))
			{
				net.infection(neighbor_node);
			}
		}
	}
    else
	{
		//immunity loss event
		net.immunity_loss(group_index, in_group_index);
	}
}

/**
 * \brief return the average life time for the configuration
 * \param[in] net a reference to an object StaticNetworkSIR
 * \param[in] gen a reference to a RNG
 */
double get_lifetime(StaticNetworkSIR& net, RNGType& gen,
        uniform_real_distribution<double>& random_01)
{
    return (-log(random_01(gen))/(net.get_event_tree()).get_value());
}

/**
 * \brief return the average life time for the configuration
 * \param[in] net a reference to an object StaticNetworkSIR
 * \param[in] gen a reference to a RNG
 */
double get_lifetime(StaticNetworkSIR& net, RNGType& gen,
       double (&log_table)[TABLE_SIZE])
{
    return (log_table[INDEX_MAP(gen())]/(net.get_event_tree()).get_value());
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
