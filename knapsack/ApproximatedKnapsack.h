//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef APPROXIMATEDKNAPSACK_H
#define APPROXIMATEDKNAPSACK_H

// STL
#include <vector>
#include <set>

class Item;
class Knapsack;

class ApproximatedKnapsack
{

public:
    ApproximatedKnapsack();
    ApproximatedKnapsack(const Knapsack &knapsack);
    virtual ~ApproximatedKnapsack();

    void setSize(int size);
    int size() const;

    void setApproximationLevel(double approximationLevel);
    double approximationLevel() const;

    std::vector<Item> items() const;
    void setItems(const std::vector<Item> &items);

    int maximumProfit();
    std::set<int> maximumProfitItems();

private:
    void recalculateValues();

    bool m_dirty;
    double m_approximationLevel;
    std::vector<Item> m_items;
    int m_size;
    int m_maximumProfit;
    std::set<int> m_maximumProfitItems;
};

#endif // APPROXIMATEDKNAPSACK_H
