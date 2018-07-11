/**
* \file Network.cpp
* \brief Methods for the class Network
* \author Guillaume St-Onge
* \version 1.0
* \date 
*/

#include "Network.hpp"

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
Network::Network(vector<pair<NodeLabel, NodeLabel> >& edge_list) :
	adjacency_list_(new vector<vector<NodeLabel> >)
{
	size_t N = 0;
	//Determine the number of nodes (brut force)
	for (int i = 0; i<edge_list.size(); i++)
    {
    	if (edge_list[i].first > N)
    	{
    		N = edge_list[i].first;
    	}
    	if (edge_list[i].second > N)
    	{
    		N = edge_list[i].second;
    	}
    }
    N += 1; //the label starts to 0 by convention
    //Initialize adjacency_list_
    vector<NodeLabel> node_vector;
    for (NodeLabel i = 0; i < N; i++)
    {
    	adjacency_list_->push_back(node_vector);
    }
    for (int i = 0; i < edge_list.size(); i++)
    {
    	NodeLabel n1 = edge_list[i].first;
    	NodeLabel n2 = edge_list[i].second;
    	(*adjacency_list_)[n1].push_back(n2);
    	(*adjacency_list_)[n2].push_back(n1);
    }
}

/**
* \brief Constructor of the class. The adjacency list is an alias to an
*  external adjacency list.
* \param[in] adjacency_list a vector containing the vector of neighbors for 
*  each node
*/
Network::Network(vector<vector<NodeLabel> >* adjacency_list) :
    adjacency_list_(adjacency_list)
{
}


}//end of namespace net