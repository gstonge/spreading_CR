/**
* \file SpreadingProcess.hpp
* \brief Header file for class SpreadingProcess
* \author Guillaume St-Onge
* \version 1.0
* \date 11/07/2018
*/

#ifndef SPREADINGPROCESS_HPP_
#define SPREADINGPROCESS_HPP_

#include <StaticNetworkSIR.hpp>
#include <evolution.hpp>

namespace net
{//start of namespace net

/**
* \class SpreadingProcess SpreadingProcess.hpp
* \brief Process class for the simulation of spreading dynamics on networks.
*/
class SpreadingProcess
{
public:
    //Constructor
    SpreadingProcess(
        std::vector<std::pair<NodeLabel, NodeLabel> >& edge_list,
        double transmission_rate,
    	double recovery_rate,
        double waning_immunity_rate,
        double base = 2);

    //Accessors
    std::vector<double> get_time_vector() const
    	{return time_vector_;}
    std::vector<unsigned int> get_Inode_number_vector() const
    	{return Inode_number_vector_;}
    std::vector<unsigned int> get_Rnode_number_vector() const
    	{return Rnode_number_vector_;}
    size_t get_Rnode_number() const
        {return network_.get_Rnode_number();}
    size_t get_size() const
        {return network_.size();}
    bool is_absorbed();

    const std::unordered_set<NodeLabel>& get_Snode_set() const
        {return network_.get_Snode_set();}
    const std::unordered_set<NodeLabel>& get_Inode_set() const
        {return network_.get_Inode_set();}
    const std::unordered_set<NodeLabel>& get_Rnode_set() const
        {return network_.get_Rnode_set();}

    //Mutators
    void set_tracing(bool tracing)
        {tracing_ = tracing;}
    void initialize_random(double fraction, unsigned int seed);
    void initialize(std::vector<NodeLabel>& Inode_vector,
            unsigned int seed);
    void initialize(std::vector<NodeLabel>& Inode_vector,
            std::vector<NodeLabel>& Rnode_vector,
            unsigned int seed);
    void reset();
    void next_state();
    void evolve(double time_variation);
    std::pair<double,double> estimate_R0(unsigned int sample,
            unsigned int seed = 42);

private:
    StaticNetworkSIR network_;
    RNGType gen_;
    std::uniform_real_distribution<double> random_01_;
    double log_table_[TABLE_SIZE];
    //simulation results members
    std::vector<double> time_vector_;
    std::vector<unsigned int> Inode_number_vector_;
    std::vector<unsigned int> Rnode_number_vector_;
    std::vector<std::pair<NodeLabel,NodeLabel>> transmission_vector_;
    bool tracing_;
};


}//end of namespace net

#endif /* SPREADINGPROCESS_HPP_ */
