//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef KNAPSACKTESTINSTANCE_H
#define KNAPSACKTESTINSTANCE_H

// Project
#include "Knapsack.h"

class KnapsackTestInstance : public Knapsack
{

public:
    KnapsackTestInstance();
    virtual ~KnapsackTestInstance();

    void setExpectedMaximumProfit(int expectedMaximumProfit);
    int expectedMaximumProfit() const;

private:
    int m_expectedMaximumProfit;
    // List of sets of items
};

#endif // KNAPSACKTESTINSTANCE_H
