//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef ITEM_H
#define ITEM_H

class Item
{
    friend class MultipleKnapsack;

public:
    Item();
    virtual ~Item();

    int size() const;
    void setSize(int size);

    int profit() const;
    void setProfit(int profit);

private:
    int id() const;
    void setId(int id);

    int m_size;
    int m_profit;
    int m_id;
};

#endif // ITEM_H
