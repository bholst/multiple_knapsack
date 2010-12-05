/*
//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//
*/

#ifndef FASTMULTIPLEKNAPSACK_H
#define FASTMULTIPLEKNAPSACK_H

// STL
#include <set>

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
};

#endif // FASTMULTIPLEKNAPSACK_H
