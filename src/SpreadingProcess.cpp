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
#include <cmath>
#include <exception>
#include <stdexcept>

using namespace std;

namespace net
{//start of namespace net

struct MyException : public exception {
   const char * what () const throw () {
      return "Transmission rate must be greater than 0";
   }
};


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
        gen_(42), random_01_(0.,1.), transmission_vector_(), tracing_(false)
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
void SpreadingProcess::initialize_random(double fraction, unsigned int seed)
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
void SpreadingProcess::initialize(const vector<NodeLabel>& Inode_vector,
        unsigned int seed)
{
    for (int i=0; i<Inode_vector.size(); i++)
    {
    	network_.infection(Inode_vector.at(i));
    }
    gen_.seed(seed);
    time_vector_.push_back(0);
    Inode_number_vector_.push_back(network_.get_Inode_number());
    Rnode_number_vector_.push_back(network_.get_Rnode_number());
}

/**
* \brief Initialize the state of the system with specified infected nodes
* \param[in] Inode_vector vector of NodeLabel for each initially infected node
*/
void SpreadingProcess::initialize(const vector<NodeLabel>& Inode_vector)
{
    for (int i=0; i<Inode_vector.size(); i++)
    {
    	network_.infection(Inode_vector.at(i));
    }
    time_vector_.push_back(0);
    Inode_number_vector_.push_back(network_.get_Inode_number());
    Rnode_number_vector_.push_back(network_.get_Rnode_number());
}


/**
* \brief Initialize the state of the system with specified infected and
* recovered nodes
* \param[in] Inode_vector vector of NodeLabel for each initially infected node
* \param[in] Rnode_vector vector of NodeLabel for each initially recovered node
*/
void SpreadingProcess::initialize(const vector<NodeLabel>& Inode_vector,
        const std::vector<NodeLabel>& Rnode_vector, unsigned int seed)
{
    for (int i=0; i<Inode_vector.size(); i++)
    {
    	network_.infection(Inode_vector.at(i));
    }
    for (int i=0; i<Rnode_vector.size(); i++)
    {
    	network_.set_recovered(Rnode_vector.at(i));
    }
    gen_.seed(seed);
    time_vector_.push_back(0);
    Inode_number_vector_.push_back(network_.get_Inode_number());
    Rnode_number_vector_.push_back(network_.get_Rnode_number());
}

/**
* \brief Initialize the state of the system with specified infected and
* recovered nodes
* \param[in] Inode_vector vector of NodeLabel for each initially infected node
* \param[in] Rnode_vector vector of NodeLabel for each initially recovered node
*/
void SpreadingProcess::initialize(const vector<NodeLabel>& Inode_vector,
        const std::vector<NodeLabel>& Rnode_vector)
{
    for (int i=0; i<Inode_vector.size(); i++)
    {
    	network_.infection(Inode_vector.at(i));
    }
    for (int i=0; i<Rnode_vector.size(); i++)
    {
    	network_.set_recovered(Rnode_vector.at(i));
    }
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
    transmission_vector_.clear();
    time_vector_.clear();
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
    	    update_event(network_, gen_, random_01_, transmission_vector_,
                    tracing_);
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

pair<double,double> SpreadingProcess::estimate_R0(unsigned int sample,
        unsigned int seed, const vector<NodeLabel>& Rnode_vector)
{
    if (network_.get_transmission_rate() <= 0)
    {
        throw MyException();
    }
    bool initial_tracing = tracing_;
    set_tracing(true);
    gen_.seed(seed);
    vector<unsigned int> secondary_case_vector(sample,0);
    double R0_mean = 0.;
    double R0_std = 0.;
    NodeLabel source_node, secondary_node;
    unsigned int i = 0;
    while (i < sample)
    {
        reset();
        //initialize network with recovered people
        if (Rnode_vector.size() > 0)
        {
            //initialize manually
            for (int i=0; i<Rnode_vector.size(); i++)
            {
                network_.set_recovered(Rnode_vector.at(i));
            }
        }
        //infect a random node (not recovered)
        bool source_found = false;
        while (not source_found)
        {
		    source_node = floor(random_01_(gen_)*network_.size());
            if (network_.is_susceptible(source_node))
            {
                source_found = true;
                network_.infection(source_node);
            }
        }
        time_vector_.push_back(0);
        Inode_number_vector_.push_back(network_.get_Inode_number());
        Rnode_number_vector_.push_back(network_.get_Rnode_number());

	    next_state();
        //check if node did transmit or died
        if (transmission_vector_.size() > 0)
        {
            secondary_node = transmission_vector_.back().second;
            while (network_.is_infected(secondary_node))
            {
                next_state();
            }
            //count the number of appearance of secondary node
            for (int j = 0; j < transmission_vector_.size(); j++)
            {
                if (transmission_vector_[j].first == secondary_node)
                {
                    secondary_case_vector[i] += 1;
                    R0_mean += 1;
                }
            }
            i += 1;
        }
    }
    reset();
    set_tracing(initial_tracing);
    R0_mean /= sample;

    //calculate std on R0
    for (int j = 0; j < sample; j++)
    {
        R0_std += ((secondary_case_vector[j] - R0_mean)
                *(secondary_case_vector[j] - R0_mean));
    }
    R0_std /= sample;
    R0_std = sqrt(R0_std);

    return make_pair(R0_mean,R0_std);
}

vector<double> SpreadingProcess::final_size_sample(unsigned int sample,
        unsigned int seed, double threshold)
{
    if (not network_.is_SIR())
    {
        throw runtime_error("Must be SIR process");
    }
    gen_.seed(seed);
    vector<NodeLabel> Inode_vector;
    vector<double> final_size_vector;
    for (unsigned int i = 0; i < sample; i++)
    {
        reset();
        //get initial infected node
        NodeLabel source_node = floor(random_01_(gen_)*network_.size());
        Inode_vector.push_back(source_node);
        initialize(Inode_vector);
        evolve(numeric_limits<double>::infinity());
        if (network_.final_size() > threshold)
        {
            final_size_vector.push_back(network_.final_size());
        }
        Inode_vector.clear();
    }
    reset();

    return final_size_vector;
}

}//end of namespace net
