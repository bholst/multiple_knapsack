//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef ITEM_H
#define ITEM_H

class Item
{

public:
    Item();
    virtual ~Item();

    int size() const;
    void setSize(int size);

    int profit() const;
    void setProfit(int profit);

private:
    int m_size;
    int m_profit;
};

#endif // ITEM_H
