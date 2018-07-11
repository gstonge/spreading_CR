/**
* \file Network.hpp
* \brief Header file for class Network
* \author Guillaume St-Onge
* \version 1.0
* \date 27/01/2018
*/

#ifndef NETWORK_HPP_
#define NETWORK_HPP_

#include <utility>
#include <vector>
#include <memory>

namespace net
{//start of namespace net

typedef unsigned int NodeLabel;


/**
* \class Network Network.hpp
* \brief Framework for the establishement of propagation dynamics on an 
*  undirected networks.
*/

class Network
{
public:
    //Constructor
    Network(std::vector<std::pair<NodeLabel, NodeLabel> >& edge_list);
    Network(std::vector<std::vector<NodeLabel> >* adjacency_list);

    //Accessors
    std::size_t degree(NodeLabel node) const 
    	{return (*adjacency_list_)[node].size();}
    std::size_t size() const 
        {return adjacency_list_->size();}
    const std::vector<NodeLabel>& get_neighbor_vector(NodeLabel node) const
    	{return (*adjacency_list_)[node];}
    const std::vector<std::vector<NodeLabel> >& get_adjacency_list() const 
        {return *adjacency_list_;}

private:
    //Members
    std::shared_ptr<std::vector<std::vector<NodeLabel> > > adjacency_list_;
};

}//end of namespace net

#endif /* NETWORK_HPP_ */