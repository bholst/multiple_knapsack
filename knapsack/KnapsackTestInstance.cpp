//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#include "KnapsackTestInstance.h"

KnapsackTestInstance::KnapsackTestInstance()
    : Knapsack(),
      m_expectedMaximumProfit(0)
{
}

KnapsackTestInstance::~KnapsackTestInstance()
{
}

void KnapsackTestInstance::setExpectedMaximumProfit(int expectedMaximumProfit)
{
    m_expectedMaximumProfit = expectedMaximumProfit;
}

int KnapsackTestInstance::expectedMaximumProfit() const
{
    return m_expectedMaximumProfit;
}

void KnapsackTestInstance::setExpectedMaximumProfitItems(std::list< std::set< int > > list)
{
    m_expectedMaximumProfitItems = list;
}

std::list< std::set< int > > KnapsackTestInstance::expectedMaximumProfitItems()
{
    return m_expectedMaximumProfitItems;
}


