/*
//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//
*/

#include "SubsetAssignment.h"

using namespace std;

SubsetAssignment::SubsetAssignment()
    : m_subsetSet(false),
      m_assignmentSet(false),
      m_profitSet(false)
{

}

std::set< int > SubsetAssignment::subset() const
{
    return m_subset;
}

void SubsetAssignment::setSubset(const std::set< int >& subset)
{
    m_subset = subset;
    m_subsetSet = true;
}

std::vector< int, std::allocator< int > > SubsetAssignment::assingment() const
{
    return m_assingment;
}

void SubsetAssignment::setAssignment(const std::vector< int >& assingment)
{
    m_assingment = assingment;
    m_assignmentSet = true;
}

int SubsetAssignment::profit() const
{
    return m_profit;
}

void SubsetAssignment::setProfit(int profit)
{
    m_profit = profit;
    m_profitSet = true;
}

bool SubsetAssignment::operator>(const SubsetAssignment& other)
{
    return m_profit > other.profit();
}

bool SubsetAssignment::valid() const
{
    return m_assignmentSet && m_profitSet && m_subsetSet;
}

