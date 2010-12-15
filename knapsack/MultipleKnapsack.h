//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef MULTIPLEKNAPSACK_H
#define MULTIPLEKNAPSACK_H

// Qt
#include <QtCore/QVector>
#include <QtCore/QList>

class Item;

class MultipleKnapsack
{

public:
    MultipleKnapsack();
    virtual ~MultipleKnapsack();

    QList<int> sizes() const;
    void setSizes(QList<int> sizes);
    
    int totalSize();

    QVector<Item> items() const;
    void setItems(QVector<Item> items);

    double approximationLevel() const;
    void setApproximationLevel(double approximationLevel);

    int maximumProfit();
    
    virtual void update();
    bool dirty();
    
    QVector<int> assignment();
    
protected:
    void setDirty(bool dirty);
    int m_maximumProfit;
    QVector<int> m_assignment;

private:
    virtual void recalculateValues();

    QList<int> m_sizes;
    int m_totalSize;
    bool m_totalSizeDirty;
    QVector<Item> m_items;
    double m_approximationLevel;
    bool m_dirty;
};

#endif // MULTIPLEKNAPSACK_H
