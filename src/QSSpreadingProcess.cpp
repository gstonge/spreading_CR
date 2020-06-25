/**
* \file QSSpreadingProcess.cpp
* \brief Methods for the class QSSpreadingProcess
* \author Guillaume St-Onge
* \version 1.0
* \date 03/02/2018
*/

#include <QSSpreadingProcess.hpp>
#include <evolution.hpp>
#include <iostream>
#include <cmath>
#include <exception>
#include <stdexcept>
#include <utility>

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
* \param[in] base a double for the ratio of min max propensity within a group
* \param[in] update_history_rate a double to push new states in the history
* \param[in] history_vector_size a double for the maximum size of history
*/
QSSpreadingProcess::QSSpreadingProcess(
    vector<pair<NodeLabel, NodeLabel> >& edge_list,
    double transmission_rate, double recovery_rate,
    double waning_immunity_rate, double base, double update_history_rate,
    unsigned int history_vector_size) :
    SpreadingProcess(edge_list,transmission_rate,recovery_rate,
            waning_immunity_rate,base), history_vector_(),
    update_history_rate_(update_history_rate),
    history_vector_size_(history_vector_size),
    random_index_(0,history_vector_size-1)
{
}


/*---------------------------
 *     Mutators
 *---------------------------*/

void QSSpreadingProcess::initialize_history_vector()
{
    //initialize the history with initial state
    history_vector_ = vector<Configuration>(history_vector_size_,Configuration());
    for (int i = 0; i < history_vector_size_ ; i++)
    {
        network_.get_configuration_copy(history_vector_[i]);
    }
}


/**
* \brief Initialize the state of the system with randomly selected nodes
* \param[in] Inode_vector vector of NodeLabel for each initially infected node
*/
void QSSpreadingProcess::initialize_random(double fraction, unsigned int seed)
{
    gen_.seed(seed);
    infect_fraction(network_, fraction, gen_, random_01_);
    initialize_history_vector();
}

/**
* \brief Initialize the state of the system with specified infected nodes
* \param[in] Inode_vector vector of NodeLabel for each initially infected node
*/
void QSSpreadingProcess::initialize(const vector<NodeLabel>& Inode_vector,
        unsigned int seed)
{
    for (int i=0; i<Inode_vector.size(); i++)
    {
    	network_.infection(Inode_vector.at(i));
    }
    gen_.seed(seed);
    initialize_history_vector();
}

/**
* \brief Initialize the state of the system with specified infected nodes
* \param[in] Inode_vector vector of NodeLabel for each initially infected node
*/
void QSSpreadingProcess::initialize(const vector<NodeLabel>& Inode_vector)
{
    for (int i=0; i<Inode_vector.size(); i++)
    {
    	network_.infection(Inode_vector.at(i));
    }
    initialize_history_vector();
}


/**
* \brief Initialize the state of the system with specified infected and
* recovered nodes
* \param[in] Inode_vector vector of NodeLabel for each initially infected node
* \param[in] Rnode_vector vector of NodeLabel for each initially recovered node
*/
void QSSpreadingProcess::initialize(const vector<NodeLabel>& Inode_vector,
        const vector<NodeLabel>& Rnode_vector, unsigned int seed)
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
    initialize_history_vector();
}

/**
* \brief Initialize the state of the system with specified infected and
* recovered nodes
* \param[in] Inode_vector vector of NodeLabel for each initially infected node
* \param[in] Rnode_vector vector of NodeLabel for each initially recovered node
*/
void QSSpreadingProcess::initialize(const vector<NodeLabel>& Inode_vector,
        const vector<NodeLabel>& Rnode_vector)
{
    for (int i=0; i<Inode_vector.size(); i++)
    {
    	network_.infection(Inode_vector.at(i));
    }
    for (int i=0; i<Rnode_vector.size(); i++)
    {
    	network_.set_recovered(Rnode_vector.at(i));
    }
    initialize_history_vector();
}


/**
* \brief Reset the process
*/
void QSSpreadingProcess::reset()
{
    network_.reset();
    history_vector_.clear();
}

/**
* \brief Evolution of the process for an inclusive time variation
* \param[in] time_variation double representing the time variation
*/
void QSSpreadingProcess::evolve(double time_variation)
{
    double current_time_variation = 0;
    double next_update_time = log_table_[INDEX_MAP(gen_())]/update_history_rate_;
    double dt = get_lifetime(network_, gen_, log_table_);
    while (current_time_variation + dt < time_variation)
    {
        current_time_variation += dt;
        if (current_time_variation >= next_update_time)
        {
            //save current state
            update_history();
            next_update_time += log_table_[INDEX_MAP(gen_())]
                /update_history_rate_;
        }
        update_event(network_, gen_, random_01_, transmission_vector_,
                tracing_);
        if (is_absorbed())
        {
            get_configuration_from_history();
        }
        dt = get_lifetime(network_, gen_, log_table_);
    }
}

/**
* \brief Update the history with current state
*/
void QSSpreadingProcess::update_history()
{
    size_t index = random_index_(gen_);
    swap(history_vector_[index],history_vector_.back());
    history_vector_.pop_back();
    history_vector_.push_back(Configuration());
    network_.get_configuration_copy(history_vector_.back());
}

/**
* \brief Update the current state with one from the history
*/
void QSSpreadingProcess::get_configuration_from_history()
{
    size_t index = random_index_(gen_);
    network_.set_configuration(history_vector_[index]);
}


}//end of namespace net
