/**
* \file PropagationProcess.hpp
* \brief Header file for class PropagationProcess
* \author Guillaume St-Onge
* \version 1.0
* \date 11/07/2018
*/

#ifndef PROPAGATIONPROCESS_HPP_
#define PROPAGATIONPROCESS_HPP_

#include <PropagationProcess.hpp>

namespace net
{//start of namespace net

/**
* \class PropagationProcess PropagationProcess.hpp
* \brief Process class for the simulation of propagation dynamics on networks.
*/
class PropagationProcess : public Network
{
public:
    //Constructor
    PropagationProcess(std::vector<std::pair<NodeLabel, NodeLabel> >& edge_list,
        double transmission_rate, double recovery_rate,
        double waning_immunity_rate);

    //Accessors
    const bool is_susceptible(NodeLabel node)
        {return state_vector_[node] == 0;}
    const bool is_infected(NodeLabel node)
        {return state_vector_[node] == 1;}
    const bool is_recovered(NodeLabel node)
        {return state_vector_[node] == 2;}

    double get_transmission_rate() const
        {return transmission_rate_;}
    double get_recovery_rate() const
        {return recovery_rate_;}
    double get_waning_immunity_rate() const
        {return waning_immunity_rate_;}
    double prevalence() const 
        {return ((1.*Inode_number_)/Network::size());}
    double incidence() const
        {return ((1.*Rnode_number_)/Network::size());}
    std::size_t get_Inode_number() const 
        {return Inode_number_;}
    std::size_t get_Rnode_number() const 
        {return Rnode_number_;}
    BinaryTree& get_event_tree()
        {return event_tree_;}
    GroupIndex get_group_index(double propensity) const
        {return hash_(propensity);}
    const PropensityGroup& get_propensity_group(GroupIndex group_index) const
        {return propensity_group_map_.at(group_index);}
    void get_configuration_copy(Configuration& empty_configuration) const;
    double get_max_propensity(GroupIndex group_index) const
        {return max_propensity_vector_[group_index];}

    //Mutators
    void infection(NodeLabel node);
    void recovery(GroupIndex group_index, size_t in_group_index);
    void immunity_loss(GroupIndex group_index, size_t in_group_index);
    void set_configuration(Configuration& configuration);

private:
    //Unvarying members
    double transmission_rate_;
    double recovery_rate_;
    double waning_immunity_rate_;
    HashPropensity hash_;
    std::vector<double> max_propensity_vector_; //max propensity for each group
    //Varying members
    std::vector<StateLabel> state_vector_;
    BinaryTree event_tree_;
    std::unordered_map<GroupIndex,PropensityGroup> propensity_group_map_;
    std::size_t Inode_number_;
    std::size_t Rnode_number_;

};






}//end of namespace net

#endif /* PROPAGATIONPROCESS_HPP_ */
