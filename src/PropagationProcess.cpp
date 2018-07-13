/**
* \file PropagationProcess.cpp
* \brief Methods for the class PropagationProcess
* \author Guillaume St-Onge
* \version 1.0
* \date 03/02/2018
*/

#include <PropagationProcess.hpp>
#include <evolution.hpp>

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
PropagationProcess::PropagationProcess(
    vector<pair<NodeLabel, NodeLabel> >& edge_list, 
    double transmission_rate, double recovery_rate, 
    double waning_immunity_rate) : network_(edge_list, transmission_rate, 
    recovery_rate, waning_immunity_rate), time_vector_(),
    Inode_number_vector_(), Rnode_number_vector_(), gen_(42)
{
}

/*---------------------------
 *     Mutators 
 *---------------------------*/

/**
* \brief Initialize the state of the system with infected nodes
* \param[in] Inode_vector vector of NodeLabel for each initially infected node 
*/
void PropagationProcess::initialize(vector<NodeLabel>& Inode_vector, 
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
* \brief Evolution of the process for a single state transition
*/
void PropagationProcess::next_state()
{
    if (network_.get_Inode_number() > 0)
    {
	double dt = 0;
    	bool new_state = false;
    	while (not new_state)
    	{
	    dt += get_lifetime(network_, gen_);
	    update_event(network_, gen_);
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
void PropagationProcess::evolve(double time_variation)
{
    bool reached_absorbing_state = false;
    double current_time_variation = 0;
    while (current_time_variation < time_variation and not 
		    reached_absorbing_state)
    {
	next_state();
	current_time_variation += time_vector_.back()-
		time_vector_[time_vector_.size()-2];
	if (Inode_number_vector_.back() == 0)
	{
	    reached_absorbing_state = true;
	}
    }
}


}//end of namespace net
