//
// Copyright (C) 2011  Bastian Holst <bastianholst@gmx.de>
//

#ifndef IMPROVEDAPPROXIMATEMULTIPLEKNAPSACK_H
#define IMPROVEDAPPROXIMATEMULTIPLEKNAPSACK_H

#include "MultipleKnapsack.h"


class ImprovedApproximateMultipleKnapsack : public MultipleKnapsack
{

public:
    ImprovedApproximateMultipleKnapsack();
    virtual ~ImprovedApproximateMultipleKnapsack();
    
private:
    virtual void recalculateValues();
    
    void groupItems(int approximateMaximum);
    
    int m_largestBinCapacity;
    
    QVector<int> m_itemProfitOrder;
    int m_firstMediumProfitOrderIndex;
    int m_firstLowProfitOrderIndex;
    
    QSet<int> m_highProfitItems;
    QSet<int> m_mediumProfitItems;
    QSet<int> m_lowProfitItems;
    
    double m_rho;
};

#endif // IMPROVEDAPPROXIMATEMULTIPLEKNAPSACK_H
