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
* \param[in] transmission_rate double rate of transmission
* \param[in] recovery_rate double rate of recovery for infected nodes
* \param[in] waning_immunity_rate double rate at which recovered nodes
* become susceptible again
*/
StaticNetworkSIR::StaticNetworkSIR(
    vector<pair<NodeLabel, NodeLabel> >& edge_list,
    double transmission_rate, double recovery_rate,
    double waning_immunity_rate, double base) :	Network(edge_list),
    state_vector_(Network::size(),0), Inode_number_(0), Rnode_number_(0),
    transmission_rate_(transmission_rate), recovery_rate_(recovery_rate),
    waning_immunity_rate_(waning_immunity_rate), event_tree_(), hash_(1.,1.),
    max_propensity_vector_(), propensity_group_map_(), mapping_vector_(),
    is_SI_(false), is_SIS_(false), is_SIRS_(false), is_SIR_(true), base_(base),
    inert_node_vector_()
{
    //Identify the model
    if(recovery_rate_ == 0)
    {
        is_SI_ = true;
    }
    else
    {
        if (waning_immunity_rate_ > 0)
        {
            if (isinf(waning_immunity_rate_))
            {
                is_SIS_ = true;
            }
            else
            {
                is_SIRS_ = true;
            }
        }
        else
        {
            is_SIR_ = true;
        }
    }

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
    if(not is_SIRS_)
    {
        //SI, SIR or SIS dynamics
        propensity_max = transmission_rate_*degree_max + recovery_rate_;
        propensity_min = transmission_rate_*degree_min + recovery_rate_;
    }
    else
    {
        //SIRS dynamics
        propensity_max = max(transmission_rate_*degree_max + recovery_rate_,
            waning_immunity_rate_);
        propensity_min = min(transmission_rate_*degree_min + recovery_rate_,
            waning_immunity_rate_);
    }
    size_t number_of_group = max(ceil(log2(propensity_max/propensity_min)/
                log2(base_)),1.);

    //Initialize correct hash object, binary tree
    hash_ = HashPropensity(propensity_min, propensity_max, base_);
    event_tree_ = BinaryTree(number_of_group);

    //reserve size for each propensity group
    for (size_t group_index = 0; group_index < number_of_group;
        ++group_index)
    {
        propensity_group_map_[group_index] = PropensityGroup();
        propensity_group_map_[group_index].reserve(size());
    }

    //Initalize max propensity vector for each group
    max_propensity_vector_.push_back(base_*propensity_min);
    for (size_t group_index = 0; group_index < number_of_group-1;
        ++group_index)
    {
        max_propensity_vector_.push_back(
            base_*max_propensity_vector_[group_index]);
    }
    max_propensity_vector_.pop_back();
    max_propensity_vector_.push_back(propensity_max);

    //Set mapping vector for the groups
    mapping_vector_.resize(1+degree_max);
    if (is_SIRS_)
    {
        mapping_vector_[0] = hash_(waning_immunity_rate_);
    }
    else
    {
        mapping_vector_[0] = 0; //it wont be used anyway
    }
    for (int k = 1; k <= degree_max; k++)
    {
        mapping_vector_[k] = hash_(transmission_rate_*k + recovery_rate_);
    }
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
    empty_configuration.inert_node_vector = inert_node_vector_;
    empty_configuration.event_tree = event_tree_;
    empty_configuration.propensity_group_map = propensity_group_map_;
    empty_configuration.Inode_number = Inode_number_;
    empty_configuration.Rnode_number = Rnode_number_;
}

/*---------------------------
 *      Mutators
 *---------------------------*/

/**
* \brief Reset the network to an absorbing state
*/
void StaticNetworkSIR::reset()
{
    //clean the event tree and propensity groups
    if (event_tree_.get_value() > 0)
    {
        for (auto iter = propensity_group_map_.begin();
            iter != propensity_group_map_.end(); iter ++)
        {
            GroupIndex group_index = iter->first;
            while (not (iter->second).empty()) //assumes S nodes are not in groups
            {
                size_t in_group_index = (iter->second).size()-1;
                if (is_infected(propensity_group_map_[group_index]
                    [in_group_index].first))
                {
                    recovery(group_index, in_group_index);
                }
                else
                {
                    immunity_loss(group_index, in_group_index);
                }
            }
        }
    }

    if(is_SIR_)
    {
        //for SIR model, one needs to set manually recovered nodes to
        //susceptible
        while (not inert_node_vector_.empty())
        {
            Rnode_number_ -= 1;
            state_vector_[inert_node_vector_.back()] = 0;
            inert_node_vector_.pop_back();
        }
    }
}

/**
* \brief Change the state of a node from susceptible to infected
* \param[in] NodeLabel node label
*/
void StaticNetworkSIR::infection(NodeLabel node)
{
    state_vector_[node] = 1;
    double propensity = transmission_rate_*degree(node) + recovery_rate_;
    GroupIndex group_index = mapping_vector_.at(degree(node));
    propensity_group_map_[group_index].push_back(
        pair<NodeLabel,double>(node,propensity));
    event_tree_.update_value(group_index, propensity);
    Inode_number_ += 1;
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
    Inode_number_ -= 1;
    swap(propensity_group[in_group_index], propensity_group.back());
    propensity_group.pop_back();
    event_tree_.update_value(group_index, -propensity);
    if (is_SIS_)
    {
        //node gets susceptible instantly, no new propensity
        state_vector_[node] = 0;
    }
    else
    {
        state_vector_[node] = 2;
        Rnode_number_ += 1;
        if (is_SIRS_)
        {
            //node can become again susceptible
            propensity_group_map_[mapping_vector_[0]].push_back(
                pair<NodeLabel,double>(node, waning_immunity_rate_));
            event_tree_.update_value(mapping_vector_[0], waning_immunity_rate_);
        }
        else
        {
            inert_node_vector_.push_back(node);
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
    state_vector_[node] = 0;
    swap(propensity_group[in_group_index], propensity_group.back());
    propensity_group.pop_back();
    event_tree_.update_value(group_index, -waning_immunity_rate_);
    Rnode_number_ -= 1;
}

/**
* \brief Change the configuration of the system
* \param[in] configuration configuration to replace the current one
*/
void StaticNetworkSIR::set_configuration(Configuration& configuration)
{
    state_vector_ = configuration.state_vector;
    inert_node_vector_ = configuration.inert_node_vector;
    event_tree_ = configuration.event_tree;
    propensity_group_map_ = configuration.propensity_group_map;
    Inode_number_ = configuration.Inode_number;
    Rnode_number_ = configuration.Rnode_number;
}


}//end of namespace net
