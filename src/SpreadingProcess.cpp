/**
* \file SpreadingProcess.cpp
* \brief Methods for the class SpreadingProcess
* \author Guillaume St-Onge
* \version 1.0
* \date 03/02/2018
*/

#include <SpreadingProcess.hpp>
#include <evolution.hpp>
#include <iostream>

using namespace std;

namespace net
{//start of namespace net


/*---------------------------
 *      Constructor
 *---------------------------*/

/**
* \brief Constructor of the class
* \param[in] edge_list a vector of node index pair representing the edge list
* \param[in] transmission_rate a double for the rate of transmission
* \param[in] recovery_rate a double for the rate of recovery
* \param[in] waning_immunity_rate a double for the rate a node gets immuned
*/
SpreadingProcess::SpreadingProcess(
    vector<pair<NodeLabel, NodeLabel> >& edge_list,
    double transmission_rate, double recovery_rate,
    double waning_immunity_rate, double base) : network_(edge_list,
        transmission_rate, recovery_rate, waning_immunity_rate, base),
        time_vector_(), Inode_number_vector_(), Rnode_number_vector_(),
        gen_(42), random_01_(0.,1.)
{
    // construct the log table
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        log_table_[i] = -log((i+0.5)/TABLE_SIZE);
    }
}

/*---------------------------
 *    accessors
 *---------------------------*/
/**
 * \brief Verify if the network is in an absorbing state
 */
bool SpreadingProcess::is_absorbed()
{
    bool is_absorbed = false;
    if (network_.get_waning_immunity_rate() > 0)
    {
        //absorbing state is all node susceptible
        is_absorbed = (network_.get_Inode_number() == 0 and network_.get_Rnode_number() == 0);
    }
    else
    {
        //absorbing state is all node susceptible or recovered
        is_absorbed = network_.get_Inode_number() == 0;
    }
    return is_absorbed;
}


/*---------------------------
 *     Mutators
 *---------------------------*/
/**
* \brief Initialize the state of the system with randomly selected nodes
* \param[in] Inode_vector vector of NodeLabel for each initially infected node
*/
void SpreadingProcess::initialize(double fraction, unsigned int seed)
{
    gen_.seed(seed);
    infect_fraction(network_, fraction, gen_, random_01_);
    time_vector_.push_back(0);
    Inode_number_vector_.push_back(network_.get_Inode_number());
    Rnode_number_vector_.push_back(network_.get_Rnode_number());
}

/**
* \brief Initialize the state of the system with specified infected nodes
* \param[in] Inode_vector vector of NodeLabel for each initially infected node
*/
void SpreadingProcess::initialize(vector<NodeLabel>& Inode_vector,
    unsigned int seed)
{
    for (int i=0; i<Inode_vector.size(); i++)
    {
    	network_.infection(Inode_vector[i]);
    }
    gen_.seed(seed);
    time_vector_.push_back(0);
    Inode_number_vector_.push_back(network_.get_Inode_number());
    Rnode_number_vector_.push_back(network_.get_Rnode_number());
}

/**
* \brief Reset the process
*/
void SpreadingProcess::reset()
{
    Inode_number_vector_.clear();
    Rnode_number_vector_.clear();
    time_vector_.clear();
    gen_.seed(42);
    network_.reset();
}

/**
* \brief Evolution of the process for a single state transition
*/
void SpreadingProcess::next_state()
{
    if (not is_absorbed())
    {
	    double dt = 0;
    	bool new_state = false;
    	while (not new_state)
    	{
    	    dt += get_lifetime(network_, gen_, log_table_);
    	    update_event(network_, gen_, random_01_);
            if (network_.get_Inode_number() != Inode_number_vector_.back())
    	    {
    	        new_state = true;
    	    }
    	}
        time_vector_.push_back(time_vector_.back()+dt);
        Inode_number_vector_.push_back(network_.get_Inode_number());
        Rnode_number_vector_.push_back(network_.get_Rnode_number());
    }
}

/**
* \brief Evolution of the process for an inclusive time variation
* \param[in] time_variation double representing the time variation
*/
void SpreadingProcess::evolve(double time_variation)
{
    double current_time_variation = 0;
    while (current_time_variation < time_variation and not
	   is_absorbed())
    {
	    next_state();
    	current_time_variation += time_vector_.back()-
		time_vector_[time_vector_.size()-2];
    }
}


}//end of namespace net
