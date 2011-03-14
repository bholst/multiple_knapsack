//
// Copyright (C) 2011  Bastian Holst <bastianholst@gmx.de>
//

#ifndef MULTIPLEKNAPSACK_H
#define MULTIPLEKNAPSACK_H

// Qt
#include <QtCore/QVector>
#include <QtCore/QList>

class ProfitItem;

class MultipleKnapsack
{
public:
    MultipleKnapsack();
    virtual ~MultipleKnapsack();
    
    QList<int> sizes() const;
    void setSizes(QList<int> sizes);
    
    QVector<ProfitItem> items() const;
    void setItems(QVector<ProfitItem> items);
    
    double approximationLevel() const;
    void setApproximationLevel(double approximationLevel);
    
    int maximumProfit();
    
    virtual void update();
    bool dirty();
    
    QVector<int> assignment();
    
    int totalSize();
    
protected:
    QVector<int> itemProfitOrder();
    void sortItemSizeOrder(QVector<int> *order, int start, int length);
    
    void setDirty(bool dirty);
    int m_maximumProfit;
    QVector<int> m_assignment;
    
private:
    virtual void recalculateValues() = 0;
    
    /**
     * @param mode 'p' if you want to sort by profit and 's' if you want to sort by size.
     */
    void sortItemOrder(QVector<int> *order, int start, int length, char mode);
    
    QList<int> m_sizes;
    QVector<ProfitItem> m_items;
    double m_approximationLevel;
    bool m_dirty;
    int m_totalSize;
    bool m_totalSizeDirty;
};

#endif // MULTIPLEKNAPSACK_H
