//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef KNAPSACK_H
#define KNAPSACK_H

// STL
#include <vector>
#include <set>

// Project
#include "Item.h"

class Knapsack
{

public:
    Knapsack();
    virtual ~Knapsack();

    int size() const;
    void setSize(int size);

    std::vector<Item> items() const;
    void setItems(const std::vector<Item> &items);
    void setItems(const std::vector<int> &sizes, const std::vector<int> &profits);

    int maximumProfit();
    std::set<int> maximumProfitItems();

private:
    void recalculateValues();

    // Input
    bool              m_dirty;
    int               m_size;
    std::vector<Item> m_items;

    // Output
    int m_maximumProfit;
    std::set<int> m_maximumProfitItems;
};

#endif // KNAPSACK_H
