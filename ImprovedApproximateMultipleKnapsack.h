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
    
    bool nextHighProfitSubset(bool *highProfitSubset, int *subsetSize, int *profit);
    QString highProfitSubsetToString(bool *highProfitSubset);
    
    /**
     * Generates the first possible assignment of all items in subset @p highProfitSubset to the bins.
     * 
     * @param highProfitSubset A bool array which is at least m_firstMediumProfitOrderIndex long
     *                         for which "highProfitSubset[i] == true" is equal to "item i is in the subset".
     * @param assignment An int array which is at least items().size() long.
     *                   After returning this contains the assignment of all items in the subset to the bins.
     *                   This assignment is feasible.
     * @param remainingCapacity An int array which is at least sizes().size() long.
     *                          After returning this contains the remaining capacities for each bin.
     * 
     * @return Returns true, if there is a feasible assignment.
     */
    bool firstHighProfitSubsetAssignment(bool *highProfitSubset, int *assignment, int *remainingCapacity);
    bool nextHighProfitSubsetAssignment(bool *highProfitSubset, int *assignment, int *remainingCapacity);
    
    int m_numberOfBins;
    QList<int> m_sortedSizes;
    int m_largestBinCapacity;
    
    QVector<int> m_itemProfitOrder;
    int m_firstMediumProfitOrderIndex;
    int m_firstLowProfitOrderIndex;
    
    QSet<int> m_highProfitItems;
    QSet<int> m_mediumProfitItems;
    QSet<int> m_lowProfitItems;
    
    double m_rho;
    int m_highProfitSubsetSizeLimit;
};

#endif // IMPROVEDAPPROXIMATEMULTIPLEKNAPSACK_H
