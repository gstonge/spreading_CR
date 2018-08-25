/**
* \file HashPropensity.cpp
* \brief Methods for the class HashPropensity
* \author Guillaume St-Onge
* \version 1.0
* \date 03/02/2018
*/

#include <HashPropensity.hpp>
#include <cmath>
#include <iostream>

using namespace std;

namespace net
{//start of namespace net


/*---------------------------
 *      Constructor
 *---------------------------*/

HashPropensity::HashPropensity(double propensity_min, double propensity_max,
        double base) :
    propensity_min_(propensity_min), propensity_max_(propensity_max),
    power_of_base_(false), base_(base)
{
    if (floor(log2(propensity_max/propensity_min)/log2(base_)) ==
        ceil(log2(propensity_max/propensity_min)/log2(base_)) and
        propensity_max != propensity_min)
    {
        power_of_base_ = true;
    }
}

/*---------------------------
 *      Call operator
 *---------------------------*/

size_t HashPropensity::operator()(double propensity) const
{
    size_t index = floor(log2(propensity/propensity_min_)/log2(base_));
    if (power_of_base_ and propensity == propensity_max_)
    {
        index -=1 ;
    }

    return index;
}




}//end of namespace net
