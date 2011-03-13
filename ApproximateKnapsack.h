//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef APPROXIMATEKNAPSACK_H
#define APPROXIMATEKNAPSACK_H

// Qt
#include <QtCore/QVector>
#include <QtCore/QSet>

class ProfitItem;
class Knapsack;

class ApproximateKnapsack
{

public:
    ApproximateKnapsack();
    ApproximateKnapsack(const Knapsack &knapsack);
    virtual ~ApproximateKnapsack();

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

#endif // APPROXIMATEKNAPSACK_H
