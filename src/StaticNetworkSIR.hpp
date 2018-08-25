/**
* \file StaticNetworkSIR.hpp
* \brief Header file for class StaticNetworkSIR
* \author Guillaume St-Onge
* \version 1.0
* \date 27/01/2018
*/

#ifndef STATICNETWORKSIR_HPP_
#define STATICNETWORKSIR_HPP_

#include <Network.hpp>
#include <BinaryTree.hpp>
#include <HashPropensity.hpp>
#include <unordered_map>
#include <cmath>
#include <memory>

namespace net
{//start of namespace net

//Define new types
typedef unsigned int StateLabel;
typedef std::vector<std::pair<NodeLabel,double> > PropensityGroup;
typedef size_t GroupIndex;

//Define structure for configuration (dynamical state)
struct Configuration
{
    std::vector<StateLabel> state_vector;
    std::vector<NodeLabel> inert_node_vector;
    BinaryTree event_tree;
    std::unordered_map<GroupIndex,PropensityGroup> propensity_group_map;
    std::size_t Inode_number;
    std::size_t Rnode_number;
};

/**
* \class StaticNetworkSIR StaticNetworkSIR.hpp
* \brief Framework for the establishement of propagation dynamics on static
*  networks.
*/
class StaticNetworkSIR : public Network
{
public:
    //Constructor
    StaticNetworkSIR(std::vector<std::pair<NodeLabel, NodeLabel> >& edge_list,
        double transmission_rate, double recovery_rate,
        double waning_immunity_rate, double base = 2);

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
    const PropensityGroup& get_propensity_group(GroupIndex group_index) const
        {return propensity_group_map_.at(group_index);}
    void get_configuration_copy(Configuration& empty_configuration) const;
    double get_max_propensity(GroupIndex group_index) const
        {return max_propensity_vector_[group_index];}

    //Mutators
    void reset();
    void infection(NodeLabel node);
    void recovery(GroupIndex group_index, size_t in_group_index);
    void immunity_loss(GroupIndex group_index, size_t in_group_index);
    void set_configuration(Configuration& configuration);

private:
    //Unvarying members
    double transmission_rate_;
    double recovery_rate_;
    double waning_immunity_rate_;
    double base_;
    bool is_SI_;
    bool is_SIS_;
    bool is_SIRS_;
    bool is_SIR_;
    HashPropensity hash_;
    std::vector<double> max_propensity_vector_; //max propensity for each group
    std::vector<GroupIndex> mapping_vector_;
    //Varying members
    std::vector<StateLabel> state_vector_;
    std::vector<NodeLabel> inert_node_vector_;
    BinaryTree event_tree_;
    std::unordered_map<GroupIndex,PropensityGroup> propensity_group_map_;
    std::size_t Inode_number_;
    std::size_t Rnode_number_;

};

}//end of namespace net

#endif /* STATICNETWORKSIR_HPP_ */
