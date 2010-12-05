/*
//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//
*/

#ifndef FASTMULTIPLEKNAPSACK_H
#define FASTMULTIPLEKNAPSACK_H

// STL
#include <set>
#include <vector>

// Project
#include "MultipleKnapsack.h"

class FastMultipleKnapsack : public MultipleKnapsack
{
public:
    FastMultipleKnapsack();
    virtual ~FastMultipleKnapsack();

private:
    virtual void recalculateValues();
    void handleSubset(const std::set<int>& subset);
    bool testAssignment(const std::vector<int>& assignment);
    void printAssignment(const std::vector<int>& assignment);

    std::vector<int> m_sizesVector;
};

#endif // FASTMULTIPLEKNAPSACK_H
