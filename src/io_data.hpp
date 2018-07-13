/**
* \file io_data.hpp
* \brief Header file input/output data functions for networks
* \author Guillaume St-Onge
* \version 1.0
* \date 07/02/2018
*/

#ifndef IO_DATA_HPP_
#define IO_DATA_HPP_

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <Network.hpp>



namespace net
{//start of namespace net


void output_edge_list(std::string path, std::vector<std::pair<unsigned int, 
	unsigned int> > edge_list);

std::vector<std::pair<NodeLabel, NodeLabel> > input_edge_list(
	std::string	path);
std::vector<double> input_degree_distribution(std::string path);
std::vector<std::size_t> input_degree_sequence(std::string path);

}//end of namespace net

#endif /* IO_DATA_HPP_ */