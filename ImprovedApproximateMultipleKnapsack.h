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
    
    /**
     * Create the groups of items.
     * @param approximateMaximum is the approximated maximum.
     */
    void groupItems(int approximateMaximum);
    
    void groupMediumItems(int remainingArea);
    
    /**
     * Generates the next possible subset of all large items.
     * 
     * @param highProfitSubset A bool array which is at least m_firstMediumProfitOrderIndex long
     *                         for which "highProfitSubset[i] == true" is equal to "item i is in the subset".
     *                         It will contain the next subset at return.
     * @param subsetSize The number of items in the subset highProfitSubset.
     * @param profit The profit for the subset highProfitSubset.
     * @param size The size for the subset highProfitSubset.
     */
    bool nextHighProfitSubset(bool *highProfitSubset, int *subsetSize, int *profit, int *size);
    
    /**
     * Creating a printable string from the subset @p highProfitSubset.
     */
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
    /**
     * Generates the next possible assignment of all items in subset @p highProfitSubset to the bins.
     * 
     * @param highProfitSubset A bool array which is at least m_firstMediumProfitOrderIndex long
     *                         for which "highProfitSubset[i] == true" is equal to "item i is in the subset".
     * @param assignment An int array which is at least items().size() long.
     *                   It has to be an assignment of all items in the subset that does not have to be
     *                   feasible.
     *                   After returning this contains the assignment of all items in the subset to the bins.
     *                   This assignment is feasible.
     * @param remainingCapacity An int array which is at least sizes().size() long.
     *                          It has to contain the remaining capacities of all bins for the assignment
     *                          assignment.
     *                          After returning this contains the remaining capacities for each bin.
     */
    bool nextHighProfitSubsetAssignment(bool *highProfitSubset, int *assignment, int *remainingCapacity);    
    
    QString highProfitSubsetAssignmentToString(int *assignment);
    
    int m_numberOfBins;
    QList<int> m_sortedSizes;
    int m_largestBinCapacity;
    int m_totalCapacity;
    
    QVector<int> m_itemProfitOrder;
    int m_firstMediumProfitOrderIndex;
    int m_firstLowProfitOrderIndex;
    
    QSet<int> m_highProfitItems;
    QSet<int> m_mediumProfitItems;
    QSet<int> m_lowProfitItems;
    
    QSet<int> m_mediumProfitHighSizeItems;
    QSet<int> m_mediumProfitMediumSizeItems;
    QSet<int> m_mediumProfitLowSizeItems;
    
    double m_rho;
    double m_K;
    int m_highProfitSubsetSizeLimit;
};

#endif // IMPROVEDAPPROXIMATEMULTIPLEKNAPSACK_H
