/**
* \file PropagationProcess.hpp
* \brief Header file for class PropagationProcess
* \author Guillaume St-Onge
* \version 1.0
* \date 11/07/2018
*/

#ifndef PROPAGATIONPROCESS_HPP_
#define PROPAGATIONPROCESS_HPP_

#include <StaticNetworkSIR.hpp>
#include <evolution.hpp>

namespace net
{//start of namespace net

/**
* \class PropagationProcess PropagationProcess.hpp
* \brief Process class for the simulation of propagation dynamics on networks.
*/
class PropagationProcess 
{
public:
    //Constructor
    PropagationProcess(
        std::vector<std::pair<NodeLabel, NodeLabel> >& edge_list,
        double transmission_rate, 
    	double recovery_rate,
        double waning_immunity_rate);

    //Accessors
    std::vector<double> get_time_vector() const
    	{return time_vector_;}
    std::vector<unsigned int> get_Inode_number_vector() const
    	{return Inode_number_vector_;}
    std::vector<unsigned int> get_Rnode_number_vector() const
    	{return Rnode_number_vector_;}
    bool is_absorbed();

    //Mutators
    void initialize(double fraction, unsigned int seed);
    void initialize(std::vector<NodeLabel>& Inode_vector,
	unsigned int seed);
    void reset();
    void reset(double transmission_rate, double recovery_rate,
        double waning_immunity_rate);
    void next_state();
    void evolve(double time_variation);
    //void evolve_fixed_transition(int transition);
    

private:
    StaticNetworkSIR network_;
    RNGType gen_;
    //simulation results members
    std::vector<double> time_vector_;
    std::vector<unsigned int> Inode_number_vector_;
    std::vector<unsigned int> Rnode_number_vector_;
};


}//end of namespace net

#endif /* PROPAGATIONPROCESS_HPP_ */
