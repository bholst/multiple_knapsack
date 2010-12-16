/*
//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//
*/

#ifndef SUBSETASSIGNMENT_H
#define SUBSETASSIGNMENT_H

// Qt
#include <QtCore/QSet>
#include <QtCore/QVector>

class SubsetAssignment
{
public:
    SubsetAssignment();
    
    QSet<int> subset() const;
    void setSubset(const QSet<int>& subset);
    
    QVector<int> assingment() const;
    void setAssignment(const QVector<int>& assingment);
    
    int profit() const;
    void setProfit(int profit);
    
    bool valid() const;
    
    void setNoAssignmentPossible(bool noAssignmentPossible);
    bool noAssignmentPossible() const;
    
    bool operator>(const SubsetAssignment& other);
    
private:
    QSet<int> m_subset;
    bool m_subsetSet;
    QVector<int> m_assingment;
    bool m_assignmentSet;
    int m_profit;
    bool m_profitSet;
    bool m_noAssignmentPossible;
};

#endif // SUBSETASSIGNMENT_H
