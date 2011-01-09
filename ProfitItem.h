//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef PROFITITEM_H
#define PROFITITEM_H

#include <../../home/bastian/important/Uni/prakeffal/knapsack/Item.h>


class ProfitItem : public Item
{
public:
    ProfitItem();
    
    int profit() const;
    void setProfit(int profit);
    
private:
    int m_profit;
};

#endif // PROFITITEM_H
