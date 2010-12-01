//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// Project
#include "Item.h"
#include "ApproximatedKnapsack.h"

// Self
#include "MultipleKnapsack.h"

using namespace std;

MultipleKnapsack::MultipleKnapsack()
    : m_approximationLevel(0),
      m_dirty(true)
{

}

MultipleKnapsack::~MultipleKnapsack()
{

}

double MultipleKnapsack::approximationLevel() const
{
    return m_approximationLevel;
}

void MultipleKnapsack::setApproximationLevel(double approximationLevel)
{
    m_dirty = true;
    m_approximationLevel = approximationLevel;
}

std::vector<Item> MultipleKnapsack::items() const
{
    return m_items;
}

void MultipleKnapsack::setItems(vector< Item > items)
{
    m_dirty = true;
    m_items = items;
}

std::list<int> MultipleKnapsack::sizes() const
{
    return m_sizes;
}

void MultipleKnapsack::setSizes(std::list< int > sizes)
{
    m_dirty = true;
    m_sizes = sizes;
}

int MultipleKnapsack::maximumProfit()
{
    if(m_dirty) {
        recalculateValues();
    }

    return m_maximumProfit;
}

void MultipleKnapsack::recalculateValues()
{
    // Do the calculation stuff
    vector<Item> remainingItems = m_items;
    m_sizes.sort();
    list<int>::iterator end = m_sizes.end();
    m_maximumProfit = 0;
    for(list<int>::iterator it = m_sizes.begin();
        it != end;
        ++it)
    {
        ApproximatedKnapsack knapsack;
        knapsack.setSize(*it);
        knapsack.setApproximationLevel(m_approximationLevel);
        knapsack.setItems(remainingItems);
        
        m_maximumProfit += knapsack.maximumProfit();
        set<int> maximumProfitItems = knapsack.maximumProfitItems();
        int itemNumber = remainingItems.size();
        vector<Item> newRemainingItems;
        for(int i = 0; i < itemNumber; ++i) {
            if(maximumProfitItems.find(i) == maximumProfitItems.end()) {
                newRemainingItems.push_back(remainingItems[i]);
            }
        }
        
        remainingItems = newRemainingItems;
    }
    m_dirty = false;
}
