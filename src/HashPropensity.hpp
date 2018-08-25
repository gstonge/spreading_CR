/**
* \file HashPropensity.hpp
* \brief Header file for class HashPropensity
* \author Guillaume St-Onge
* \version 1.0
* \date 27/01/2018
*/

#ifndef HASHPROPENSITY_HPP_
#define HASHPROPENSITY_HPP_

#include <cstdlib>

namespace net
{//start of namespace net

/**
* \class HashPropensity HashPropensity.hpp
* \brief Unary function object to hash the propensity of events
*/


class HashPropensity
{
public:
    //Constructor
    HashPropensity(double propensity_min, double propensity_max, double base_ = 2);

    //Call operator definition
    std::size_t operator()(double propensity) const;

private:
    //Members
    double propensity_min_;
    double propensity_max_;
    double base_;
    bool power_of_base_;
};

}//end of namespace net

#endif /* HASHPROPENSITY_HPP_ */
