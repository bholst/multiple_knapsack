//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef KNAPSACKTESTINSTANCE_H
#define KNAPSACKTESTINSTANCE_H

// STL
#include <list>
#include <set>

// Project
#include "Knapsack.h"

class KnapsackTestInstance : public Knapsack
{

public:
    KnapsackTestInstance();
    virtual ~KnapsackTestInstance();

    void setExpectedMaximumProfit(int expectedMaximumProfit);
    int expectedMaximumProfit() const;

    void setExpectedMaximumProfitItems(std::list < std::set<int> > list);
    std::list < std::set<int> > expectedMaximumProfitItems();

private:
    int m_expectedMaximumProfit;
    std::list < std::set<int> > m_expectedMaximumProfitItems;
};

#endif // KNAPSACKTESTINSTANCE_H
