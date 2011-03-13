//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef GREEDYMULTIPLEKNAPSACK_H
#define GREEDYMULTIPLEKNAPSACK_H

// Project
#include "MultipleKnapsack.h"

class GreedyMultipleKnapsack : public MultipleKnapsack
{

public:
    GreedyMultipleKnapsack();
    virtual ~GreedyMultipleKnapsack();

private:
    virtual void recalculateValues();
};

#endif // GREEDYMULTIPLEKNAPSACK_H
