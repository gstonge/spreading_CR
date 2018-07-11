/**
* \file StaticNetworkSIR.cpp
* \brief Methods for the class StaticNetworkSIR
* \author Guillaume St-Onge
* \version 1.0
* \date 03/02/2018
*/

#include <StaticNetworkSIR.hpp>
#include <iostream>
#include <algorithm>
#include <utility>

using namespace std;

namespace net
{//start of namespace net


/*---------------------------
 *      Constructor
 *---------------------------*/

/**
* \brief Constructor of the class
* \param[in] edge_list a vector of node index pair representing the edge list
*/
StaticNetworkSIR::StaticNetworkSIR(
    vector<pair<NodeLabel, NodeLabel> >& edge_list, 
    double transmission_rate, double recovery_rate, 
    double waning_immunity_rate) : 	Network(edge_list), 
    state_vector_(Network::size(),0), Inode_number_(0), Rnode_number_(0),
    transmission_rate_(transmission_rate), recovery_rate_(recovery_rate), 
    waning_immunity_rate_(waning_immunity_rate), event_tree_(), hash_(1.,1.),
    max_propensity_vector_(), propensity_group_map_()
{
    //Determine minimal and maximal degree
    size_t degree_min = degree(0);
    size_t degree_max = degree(0);
    for (int i = 0; i < size(); ++i)
    {
        if (degree(i) < degree_min)
        {
            degree_min = degree(i);
        }
        if (degree(i) > degree_max)
        {
            degree_max = degree(i);
        }
    }

    //Get min, max propensity and number of group
    double propensity_max;
    double propensity_min;
    if(waning_immunity_rate == 0. or std::isinf(waning_immunity_rate) 
        or recovery_rate == 0.)
    {
        //SI, SIR or SIS dynamics
        propensity_max = transmission_rate*degree_max + recovery_rate;
        propensity_min = transmission_rate*degree_min + recovery_rate;
    }
    else
    {
        //SIRS dynamics
        propensity_max = max(transmission_rate*degree_max + recovery_rate, 
            waning_immunity_rate);
        propensity_min = min(transmission_rate*degree_min + recovery_rate, 
            waning_immunity_rate);
    }
    size_t number_of_group = max(ceil(log2(propensity_max/propensity_min)),1.);

    //Initialize correct hash object, binary tree
    hash_ = HashPropensity(propensity_min, propensity_max);

    event_tree_ = BinaryTree(number_of_group);

    //reserve size for each propensity group
    for (size_t group_index = 0; group_index < number_of_group-1; ++group_index)
    {
        propensity_group_map_[group_index] = PropensityGroup();
        propensity_group_map_[group_index].reserve(size());
    }

    //Initalize max propensity vector for each group
    max_propensity_vector_.push_back(2*propensity_min);
    for (size_t group_index = 0; group_index < number_of_group-1; ++group_index)
    {
        max_propensity_vector_.push_back(2*max_propensity_vector_[group_index]);      
    }
    max_propensity_vector_.pop_back();
    max_propensity_vector_.push_back(propensity_max);

}

/*---------------------------
 *      Accessors
 *---------------------------*/

/**
* \brief Copy the current configuration
* \param[in] empty_configuration empty structure configuration
*/
void StaticNetworkSIR::get_configuration_copy(
    Configuration& empty_configuration) const
{
    empty_configuration.state_vector = state_vector_;
    empty_configuration.event_tree = event_tree_;
    empty_configuration.propensity_group_map = propensity_group_map_;
    empty_configuration.Inode_number = Inode_number_;
    empty_configuration.Rnode_number = Rnode_number_;
}

/*---------------------------
 *      Mutators
 *---------------------------*/

/**
* \brief Change the state of a node from susceptible to infected
* \param[in] node node label
*/
void StaticNetworkSIR::infection(NodeLabel node)
{
    if (is_susceptible(node))
    {
        state_vector_[node] = 1;
        double propensity = transmission_rate_*degree(node) + recovery_rate_;
        GroupIndex group_index = get_group_index(propensity);
        propensity_group_map_[group_index].push_back(
            pair<NodeLabel,double>(node,propensity));
        event_tree_.update_value(group_index, propensity);
        Inode_number_ += 1;
    }
    else
    {
        cout << "node is not susceptible" << endl;
    }
}

/**
* \brief Change the state of a node from infected to recovered or susceptible
* \param[in] group_index Index for the propensity group
* \param[in] in_group_index Index for the node inside the propensity group
*/
void StaticNetworkSIR::recovery(GroupIndex group_index, 
    size_t in_group_index)
{
    PropensityGroup& propensity_group = propensity_group_map_[group_index];
    NodeLabel node = propensity_group[in_group_index].first;
    double propensity = propensity_group[in_group_index].second;
    if (not is_infected(node))
    {
        cout << "node is not infected" << endl;
    }
    else
    {
        Inode_number_ -= 1;
        swap(propensity_group[in_group_index], propensity_group.back());
        propensity_group.pop_back();
        event_tree_.update_value(group_index, -propensity);
        if (std::isinf(waning_immunity_rate_))
        {
            //node gets susceptible instantly, no new propensity
            state_vector_[node] = 0;
        }
        else
        {
            state_vector_[node] = 2;
            Rnode_number_ += 1;
            if (waning_immunity_rate_ > 0) 
            {
                //node can become again susceptible
                double new_propensity = waning_immunity_rate_;
                GroupIndex new_group_index = get_group_index(new_propensity);
                propensity_group_map_[new_group_index].push_back(
                    pair<NodeLabel,double>(node,new_propensity));
                event_tree_.update_value(new_group_index, new_propensity);
            }
        }
    }
}

/**
* \brief Change the state of a node from recovered to susceptible
* \param[in] group_index Index for the propensity group
* \param[in] in_group_index Index for the node inside the propensity group
*/
void StaticNetworkSIR::immunity_loss(GroupIndex group_index, 
    size_t in_group_index)
{
    PropensityGroup& propensity_group = propensity_group_map_[group_index];
    NodeLabel node = propensity_group[in_group_index].first;
    double propensity = propensity_group[in_group_index].second;
    if (not is_recovered(node))
    {
        cout << "node is not recovered" << endl;
    }
    else
    {
        state_vector_[node] = 0;
        swap(propensity_group[in_group_index], propensity_group.back());
        propensity_group.pop_back();
        event_tree_.update_value(group_index, -1*propensity);
        Rnode_number_ -= 1;
    }
}

/**
* \brief Change the configuration of the system
* \param[in] configuration configuration to replace the current one
*/
void StaticNetworkSIR::set_configuration(Configuration& configuration)
{
    state_vector_ = configuration.state_vector;
    event_tree_ = configuration.event_tree;
    propensity_group_map_ = configuration.propensity_group_map;
    Inode_number_ = configuration.Inode_number;
    Rnode_number_ = configuration.Rnode_number;
}


}//end of namespace net