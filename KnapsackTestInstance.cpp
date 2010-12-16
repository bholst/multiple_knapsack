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

void KnapsackTestInstance::setExpectedMaximumProfitItems(QList< QSet< int > > list)
{
    m_expectedMaximumProfitItems = list;
}

QList< QSet< int > > KnapsackTestInstance::expectedMaximumProfitItems()
{
    return m_expectedMaximumProfitItems;
}


