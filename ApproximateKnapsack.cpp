//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <cmath>

// Project
#include "Knapsack.h"
#include "ProfitItem.h"

// Self
#include "ApproximateKnapsack.h"

using namespace std;

ApproximateKnapsack::ApproximateKnapsack()
    : m_dirty(true),
      m_approximationLevel(0.0),
      m_items(),
      m_size(0)
{
}

ApproximateKnapsack::ApproximateKnapsack(const Knapsack& knapsack)
    : m_dirty(true),
      m_approximationLevel(0.0),
      m_items(knapsack.items()),
      m_size(knapsack.size())
{

}

ApproximateKnapsack::~ApproximateKnapsack()
{
}

void ApproximateKnapsack::setSize(int size)
{
    m_dirty = true;
    m_size = size;
}

int ApproximateKnapsack::size() const
{
    return m_size;
}

void ApproximateKnapsack::setApproximationLevel(double approximationLevel)
{
    m_dirty = true;
    m_approximationLevel = approximationLevel;
}

double ApproximateKnapsack::approximationLevel() const
{
    return m_approximationLevel;
}

QVector< ProfitItem > ApproximateKnapsack::items() const
{
    return m_items;
}

void ApproximateKnapsack::setItems(const QVector< ProfitItem >& items)
{
    m_dirty = true;
    m_items = items;
}

int ApproximateKnapsack::maximumProfit()
{
    if(m_dirty) {
        recalculateValues();
    }
    return m_maximumProfit;
}

QSet< int > ApproximateKnapsack::maximumProfitItems()
{
    if(m_dirty) {
        recalculateValues();
    }
    return m_maximumProfitItems;
}

void ApproximateKnapsack::recalculateValues()
{
    Knapsack knapsack;

    int maximumItemProfit = 0;
    for(QVector<ProfitItem>::const_iterator it = m_items.constBegin();
        it != m_items.constEnd();
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

    QVector<ProfitItem> approximatedItems(m_items.size());
    for(int i = 0; i < m_items.size(); ++i) {
        ProfitItem approximatedItem = m_items[i];
        approximatedItem.setProfit(approximatedItem.profit() / profitDivisor);
        approximatedItems[i] = approximatedItem;
    }

    knapsack.setItems(approximatedItems);
    knapsack.setSize(m_size);

    m_maximumProfitItems = knapsack.maximumProfitItems();
    QSet<int>::const_iterator endIterator = m_maximumProfitItems.constEnd();
    m_maximumProfit = 0;
    for(QSet<int>::const_iterator it = m_maximumProfitItems.constBegin();
        it != endIterator;
        ++it)
    {
        m_maximumProfit += m_items[*it].profit();
    }

    m_dirty = false;
}




