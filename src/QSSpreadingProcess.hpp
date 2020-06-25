/**
* \file QSSpreadingProcess.hpp
* \brief Header file for class QSSpreadingProcess
* \author Guillaume St-Onge
* \version 1.0
* \date 23/06/2020
*/

#ifndef QSSPREADINGPROCESS_HPP_
#define QSSPREADINGPROCESS_HPP_

#include <SpreadingProcess.hpp>
#include <stdexcept>
#include <random>

namespace net
{//start of namespace net

/**
* \class QSSpreadingProcess QSSpreadingProcess.hpp
* \brief Quasi-stationary process class for the simulation of spreading
*        dynamics on networks.
*/
class QSSpreadingProcess : public SpreadingProcess
{
public:
    //Constructor
    QSSpreadingProcess(
        std::vector<std::pair<NodeLabel, NodeLabel> >& edge_list,
        double transmission_rate,
    	double recovery_rate,
        double waning_immunity_rate,
        double base = 2,
        double update_history_rate = 1,
        unsigned int history_vector_size = 100);

    //Mutators
    void initialize_random(double fraction, unsigned int seed);
    void initialize(const std::vector<NodeLabel>& Inode_vector,
            unsigned int seed);
    void initialize(const std::vector<NodeLabel>& Inode_vector);
    void initialize(const std::vector<NodeLabel>& Inode_vector,
        const std::vector<NodeLabel>& Rnode_vector, unsigned int seed);
    void initialize(const std::vector<NodeLabel>& Inode_vector,
        const std::vector<NodeLabel>& Rnode_vector);
    void set_update_history_rate(double update_history_rate)
        {update_history_rate_ = update_history_rate;}

    void reset();
    void evolve(double time_variation);

    //ensure the following methods are not implemented
    void set_tracing(bool tracing)
        {throw std::runtime_error("Not implemented for QSSpreadingProcess");}
    void next_state()
        {throw std::runtime_error("Not implemented for QSSpreadingProcess");}

    std::vector<double> get_time_vector() const
    	{throw std::runtime_error("Not implemented for QSSpreadingProcess");
         return time_vector_;}
    std::vector<unsigned int> get_Inode_number_vector() const
    	{throw std::runtime_error("Not implemented for QSSpreadingProcess");
         return Inode_number_vector_;}
    std::vector<unsigned int> get_Rnode_number_vector() const
        {throw std::runtime_error("Not implemented for QSSpreadingProcess");
    	 return Rnode_number_vector_;}

    std::pair<double,double> estimate_R0(unsigned int sample,
            unsigned int seed = 42,
            const std::vector<NodeLabel>& Rnode_vector = std::vector<NodeLabel>())
        {throw std::runtime_error("Not implemented for QSSpreadingProcess");
         return std::make_pair(0.,0.); }
    std::vector<double> final_size_sample(unsigned int sample, unsigned int seed,
            double threshold = 1e-4)
        {throw std::runtime_error("Not implemented for QSSpreadingProcess");
         return std::vector<double>(); }

private:
    std::vector<Configuration> history_vector_;
    double update_history_rate_;
    unsigned int history_vector_size_;
    std::uniform_int_distribution<int> random_index_;

    //private methods
    void initialize_history_vector();
    void update_history();
    void get_configuration_from_history();
};


}//end of namespace net

#endif /* QSSPREADINGPROCESS_HPP_ */
