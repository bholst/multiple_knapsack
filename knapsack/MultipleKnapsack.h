//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef MULTIPLEKNAPSACK_H
#define MULTIPLEKNAPSACK_H

// std
#include <vector>
#include <list>

class Item;

class MultipleKnapsack
{

public:
    MultipleKnapsack();
    virtual ~MultipleKnapsack();

    std::list<int> sizes() const;
    std::vector<int> sizesVector();
    void setSizes(std::list<int> sizes);

    std::vector<Item> items() const;
    void setItems(std::vector<Item> items);

    double approximationLevel() const;
    void setApproximationLevel(double approximationLevel);

    int maximumProfit();
    
    virtual void update();
    bool dirty();
    
protected:
    void setDirty(bool dirty);
    int m_maximumProfit;

private:
    virtual void recalculateValues();

    std::list<int> m_sizes;
    std::vector<int> m_sizesVector;
    bool m_sizesVectorDirty;
    std::vector<Item> m_items;
    double m_approximationLevel;
    bool m_dirty;
};

#endif // MULTIPLEKNAPSACK_H
