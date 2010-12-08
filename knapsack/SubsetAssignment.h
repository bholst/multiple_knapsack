/*
//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//
*/

#ifndef SUBSETASSIGNMENT_H
#define SUBSETASSIGNMENT_H

#include <set>
#include <vector>

class SubsetAssignment
{
public:
    SubsetAssignment();
    
    std::set<int> subset() const;
    void setSubset(const std::set<int>& subset);
    
    std::vector<int> assingment() const;
    void setAssignment(const std::vector<int>& assingment);
    
    int profit() const;
    void setProfit(int profit);
    
    bool valid() const;
    
    bool operator>(const SubsetAssignment& other);
    
private:
    std::set<int> m_subset;
    bool m_subsetSet;
    std::vector<int> m_assingment;
    bool m_assignmentSet;
    int m_profit;
    bool m_profitSet;
};

#endif // SUBSETASSIGNMENT_H
