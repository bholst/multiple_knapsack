//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef APPROXIMATEDKNAPSACK_H
#define APPROXIMATEDKNAPSACK_H

// Qt
#include <QtCore/QVector>
#include <QtCore/QSet>

class ProfitItem;
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

    QVector<ProfitItem> items() const;
    void setItems(const QVector<ProfitItem> &items);

    int maximumProfit();
    QSet<int> maximumProfitItems();

private:
    void recalculateValues();

    bool m_dirty;
    double m_approximationLevel;
    QVector<ProfitItem> m_items;
    int m_size;
    int m_maximumProfit;
    QSet<int> m_maximumProfitItems;
};

#endif // APPROXIMATEDKNAPSACK_H
