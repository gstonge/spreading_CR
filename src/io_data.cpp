/**
* \file io_data.cpp
* \brief functions definition for input/output data for networks
* \author Guillaume St-Onge
* \version 1.0
* \date 07/02/2018
*/

#include <io_data.hpp>

using namespace std;

namespace net
{//start of namespace net

/**
 * \brief Output edge list to a file
 * \param[in] path path name to the file
 * \param[in] edge_list vector of pair representing the edge list
 */
void output_edge_list(string path, vector<pair<NodeLabel, NodeLabel> > 
	edge_list)
{
	ofstream outStream;
	outStream.open(path, ios::out);

	for (int i = 0; i < edge_list.size(); i++)
	{
		outStream << edge_list[i].first << " " << edge_list[i].second << endl;
	}

	outStream.close();
}

/**
 * \brief Input  the edge list from file
 * \param[in] path path name to the file
 */
vector<pair<NodeLabel, NodeLabel> > input_edge_list(string path)
{
	ifstream in_stream;
	in_stream.open(path, ios::in);
	string line;

	vector<pair<NodeLabel, NodeLabel> > edge_list;

	while(getline(in_stream,line))
	{
		pair<NodeLabel, NodeLabel> edge;
		stringstream line_stream(line);
		line_stream >> edge.first >> edge.second;
		edge_list.push_back(edge);
	}
	in_stream.close();

	return edge_list;
}

/**
 * \brief Input the degree distribution
 *
 * \param[in] path path name to the file
 */
vector<double> input_degree_distribution(string path)
{
	ifstream in_stream;
	in_stream.open(path, ios::in);
	string line;

	vector<double> degree_distribution;

	while(getline(in_stream,line))
	{
		double pk;
		stringstream line_stream(line);
		line_stream >> pk;
		degree_distribution.push_back(pk);
	}
	in_stream.close();

	return degree_distribution;
}

/**
 * \fn vector<unsigned int> input_degree_sequence(string path)
 * \brief Input the degree sequence
 *
 * \param[in] path path name to the file
 */
vector<size_t> input_degree_sequence(string path)
{
	ifstream in_stream;
	in_stream.open(path, ios::in);
	string line;

	vector<size_t> degree_sequence;

	while(getline(in_stream,line))
	{
		size_t k;
		stringstream line_stream(line);
		line_stream >> k;
		degree_sequence.push_back(k);
	}
	in_stream.close();

	return degree_sequence;
}


}//end of namespace net