//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// STL
#include <iostream>
#include <limits>

// Project
#include "Item.h"

// Self
#include "Knapsack.h"

using namespace std;

Knapsack::Knapsack()
    : m_dirty(false),
      m_size(0),
      m_maximumProfit(0)
{

}

Knapsack::~Knapsack()
{

}

int Knapsack::size() const
{
    return m_size;
}

void Knapsack::setSize ( int size )
{
    m_dirty = true;
    m_size = size;
}

std::vector< Item > Knapsack::items() const
{
    return m_items;
}

void Knapsack::setItems ( const vector< Item >& items )
{
    m_dirty = true;
    m_items = items;
}

void Knapsack::setItems(const std::vector<int>& sizes, const std::vector<int>& profits)
{
    if(sizes.size() == profits.size()) {
        std::vector<Item> items;

        int length = sizes.size();
        for(int i = 0; i < length; ++i) {
            Item item;
            item.setSize(sizes[i]);
            item.setProfit(profits[i]);
            items.insert(items.end(), item);
        }
        setItems(items);
    }
}

int Knapsack::maximumProfit()
{
    if(m_dirty) {
        recalculateValues();
    }

    return m_maximumProfit;
}

void Knapsack::recalculateValues()
{
    int profitSum = 0;
    for(int i=0; i < m_items.size(); ++i) {
        profitSum += m_items[i].profit();
    }

    int minimumSize[m_items.size()][profitSum];
    int maximumProfit = 0;

    int profitFirst = m_items[0].profit();
    int sizeFirst = m_items[0].size();

    if( sizeFirst <= m_size ) {
        maximumProfit = profitFirst;
    }

    for(int i = 1; i <= profitFirst; ++i) {
        minimumSize[0][i-1] = sizeFirst;
    }
    for(int i = profitFirst + 1; i <= profitSum; ++i) {
        minimumSize[0][i-1] = -1;
    }

    for(int j = 1; j < m_items.size(); ++j) {
        for(int i = 1; i <= profitSum; ++i) {
            int a, b = -1;
            a = minimumSize[j-1][i-1];

            if(i <= m_items[j].profit()) {
                b = m_items[j].size();
            }
            else {
                if(minimumSize[j-1][i-1-m_items[j].profit()] >= 0) {
                    b = m_items[j].size() + minimumSize[j-1][i-1-m_items[j].profit()];
                }
            }

            minimumSize[j][i-1] = ((a <= b && a >= 0) || b < 0) ? a : b;
            if(minimumSize[j][i-1] >= 0
               && minimumSize[j][i-1] <= m_size
               && maximumProfit < i)
            {
                maximumProfit = i;
            }
        }
    }

    m_maximumProfit = maximumProfit;
    m_dirty = false;
}


