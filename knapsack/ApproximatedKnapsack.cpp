//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <cmath>

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

QVector< Item > ApproximatedKnapsack::items() const
{
    return m_items;
}

void ApproximatedKnapsack::setItems(const QVector< Item >& items)
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

QSet< int > ApproximatedKnapsack::maximumProfitItems()
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
    for(QVector<Item>::iterator it = m_items.begin();
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

    QVector<Item> approximatedItems(m_items.size());
    for(int i = 0; i < m_items.size(); ++i) {
        Item approximatedItem = m_items[i];
        approximatedItem.setProfit(approximatedItem.profit() / profitDivisor);
        approximatedItems[i] = approximatedItem;
    }

    knapsack.setItems(approximatedItems);
    knapsack.setSize(m_size);

    m_maximumProfitItems = knapsack.maximumProfitItems();
    QSet<int>::iterator endIterator = m_maximumProfitItems.end();
    m_maximumProfit = 0;
    for(QSet<int>::iterator it = m_maximumProfitItems.begin();
        it != endIterator;
        ++it)
    {
        m_maximumProfit += m_items[*it].profit();
    }

    m_dirty = false;
}




