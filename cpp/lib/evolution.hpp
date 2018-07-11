/**
* \file evolution.hpp
* \brief Header file for propagation dynamics evolution
* \author Guillaume St-Onge
* \version 1.0
* \date 06/02/2018
*/

#ifndef EVOLUTION_HPP_
#define EVOLUTION_HPP_

#include <StaticNetworkSIR.hpp>
#include <random>

namespace net
{//start of namespace net

//define type 
typedef std::mt19937 RNGType;

//Static networks evolution
void infect_fraction(StaticNetworkSIR& net, double fraction, RNGType& gen);

void update_event(StaticNetworkSIR& net, RNGType& gen);

double get_lifetime(StaticNetworkSIR& net, RNGType& gen);

void update_history(StaticNetworkSIR& net, unsigned int max_configuration, 
	std::vector<Configuration>& history_vector, RNGType& gen);

void get_from_history(StaticNetworkSIR& net,
	std::vector<Configuration>& history_vector, RNGType& gen);

}//end of namespace net

#endif /* EVOLUTION_HPP_ */