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
     * @param subset A bool array which is at least @p count long
     *               for which "highProfitSubset[i] == true" is equal to "item i is in the subset".
     *               It will contain the next subset at return.
     * @param subsetSize The number of items in the subset @p subset.
     * @param profit The sum of all profits for the subset @p subset.
     * @param size The sum of all sizes for the subset @p subset.
     */
    bool nextSubset(bool *subset, int *subsetSize, int *profit, int *size, int *itemProfitSizeOrder, int count, int itemLimit);
    
    /**
     * Creating a printable string from the subset @p subset.
     */
    QString subsetToString(bool* subset, int *itemProfitSizeOrder, int count);
    
    /**
     * Generates the first possible assignment of all items in subset @p highProfitSubset to the bins.
     * 
     * @param subset A bool array which is at least @p count long
     *               for which "subset[i] == true" is equal to "item i is in the subset".
     * @param assignment An int array which is at least items().size() long.
     *                   After returning this contains the assignment of all items in the subset to the bins.
     *                   This assignment is feasible.
     * @param remainingCapacity An int array which is at least sizes().size() long.
     *                          After returning this contains the remaining capacities for each bin.
     * 
     * @return Returns true, if there is a feasible assignment.
     */
    bool firstSubsetAssignment(bool* subset, int* assignment, int* remainingCapacity, int *itemProfitSizeOrder, int count);
    /**
     * Generates the next possible assignment of all items in subset @p highProfitSubset to the bins.
     * 
     * @param subset A bool array which is at least @p count long
     *               for which "subset[i] == true" is equal to "item i is in the subset".
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
    bool nextSubsetAssignment(bool* subset, int* assignment, int* remainingCapacity, int *itemProfitSizeOrder, int count);    
    
    QString subsetAssignmentToString(int* assignment, int *itemProfitSizeOrder, int count);
    
    int m_numberOfBins;
    QList<int> m_sortedSizes;
    int m_largestBinCapacity;
    int m_totalCapacity;
    
    int m_itemNumber;
    int *m_itemProfitSizeOrder;
    double *m_orderedRelativeSizes;
    int m_firstMediumProfitOrderIndex;
    int m_firstLowProfitOrderIndex;
    
    int m_firstMediumProfitMediumSizeOrderIndex;
    int m_firstMediumProfitLowSizeOrderIndex;
    int *m_firstMediumProfitMediumSizeGroupOrderIndex;
    int m_minR;
    int m_maxR;
    
//     QSet<int> m_highProfitItems;
//     QSet<int> m_mediumProfitItems;
//     QSet<int> m_lowProfitItems;
    
//     QSet<int> m_mediumProfitHighSizeItems;
//     QSet<int> m_mediumProfitMediumSizeItems;
//     QSet<int> m_mediumProfitLowSizeItems;
    
    double m_rho;
    double m_K;
    int m_highProfitSubsetSizeLimit;
};

#endif // IMPROVEDAPPROXIMATEMULTIPLEKNAPSACK_H
