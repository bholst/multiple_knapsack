//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef KNAPSACK_H
#define KNAPSACK_H

// Qt
#include <QtCore/QVector>
#include <QtCore/QSet>

// Project
#include "Item.h"

class QString;

class Knapsack
{

public:
    Knapsack();
    virtual ~Knapsack();

    int size() const;
    void setSize(int size);

    QVector<Item> items() const;
    void setItems(const QVector<Item> &items);
    void setItems(const QVector<int> &sizes, const QVector<int> &profits);

    int maximumProfit();
    QSet<int> maximumProfitItems();

    QString toString() const;

private:
    void recalculateValues();

    // Input
    bool              m_dirty;
    int               m_size;
    QVector<Item> m_items;

    // Output
    int m_maximumProfit;
    QSet<int> m_maximumProfitItems;
};

#endif // KNAPSACK_H
