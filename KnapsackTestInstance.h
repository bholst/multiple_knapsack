//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef KNAPSACKTESTINSTANCE_H
#define KNAPSACKTESTINSTANCE_H

// Qt
#include <QtCore/QList>
#include <QtCore/QSet>

// Project
#include "Knapsack.h"

class KnapsackTestInstance : public Knapsack
{

public:
    KnapsackTestInstance();
    virtual ~KnapsackTestInstance();

    void setExpectedMaximumProfit(int expectedMaximumProfit);
    int expectedMaximumProfit() const;

    void setExpectedMaximumProfitItems(QList < QSet<int> > list);
    QList < QSet<int> > expectedMaximumProfitItems();

private:
    int m_expectedMaximumProfit;
    QList < QSet<int> > m_expectedMaximumProfitItems;
};

#endif // KNAPSACKTESTINSTANCE_H
