//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <cmath>
#include <iostream>

// Project
#include "Knapsack.h"
#include "Item.h"

// Self
#include "ApproximatedKnapsack.h"

using namespace std;

ApproximatedKnapsack::ApproximatedKnapsack()
    : m_dirty(true),
      m_approximationLevel(0.0),
      m_items(),
      m_size(0)
{
}

ApproximatedKnapsack::ApproximatedKnapsack(const Knapsack& knapsack)
    : m_dirty(true),
      m_approximationLevel(0.0),
      m_items(knapsack.items()),
      m_size(knapsack.size())
{

}

ApproximatedKnapsack::~ApproximatedKnapsack()
{
}

void ApproximatedKnapsack::setSize(int size)
{
    m_dirty = true;
    m_size = size;
}

int ApproximatedKnapsack::size() const
{
    return m_size;
}

void ApproximatedKnapsack::setApproximationLevel(double approximationLevel)
{
    m_dirty = true;
    m_approximationLevel = approximationLevel;
}

double ApproximatedKnapsack::approximationLevel() const
{
    return m_approximationLevel;
}

std::vector< Item > ApproximatedKnapsack::items() const
{
    return m_items;
}

void ApproximatedKnapsack::setItems(const std::vector< Item >& items)
{
    m_dirty = true;
    m_items = items;
}

int ApproximatedKnapsack::maximumProfit()
{
    if(m_dirty) {
        recalculateValues();
    }
    return m_maximumProfit;
}

std::set< int > ApproximatedKnapsack::maximumProfitItems()
{
    if(m_dirty) {
        recalculateValues();
    }
    return m_maximumProfitItems;
}

void ApproximatedKnapsack::recalculateValues()
{
    Knapsack knapsack;

    int maximumItemProfit = 0;
    for(vector<Item>::iterator it = m_items.begin();
        it != m_items.end();
        ++it)
    {
        int profit = it->profit();

        if(profit > maximumItemProfit) {
            maximumItemProfit = profit;
        }
    }

    int profitDivisor = floor(m_approximationLevel * (double) maximumItemProfit
                              / (double) m_items.size());
    if(profitDivisor < 1) {
        profitDivisor = 1;
    }

    vector<Item> approximatedItems(m_items.size());
    for(int i = 0; i < m_items.size(); ++i) {
        Item approximatedItem = m_items[i];
        approximatedItem.setProfit(approximatedItem.profit() / profitDivisor);
        approximatedItems[i] = approximatedItem;
    }

    knapsack.setItems(approximatedItems);
    knapsack.setSize(m_size);

    cerr << knapsack.toString();

    m_maximumProfitItems = knapsack.maximumProfitItems();
    set<int>::iterator endIterator = m_maximumProfitItems.end();
    m_maximumProfit = 0;
    for(set<int>::iterator it = m_maximumProfitItems.begin();
        it != endIterator;
        ++it)
    {
        m_maximumProfit += m_items[*it].profit();
    }

    m_dirty = false;
}




