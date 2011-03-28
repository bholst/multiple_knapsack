//
// Copyright (C) 2011  Bastian Holst <bastianholst@gmx.de>
//

#ifndef IMPROVEDAPPROXIMATEMULTIPLEKNAPSACK_H
#define IMPROVEDAPPROXIMATEMULTIPLEKNAPSACK_H

// Qt
#include <QtCore/QVector>

// Project
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
     * Generates the next possible subset of the given groups.
     * 
     * @param subset An int array which is at least @p count long
     *               for which "subset[i] = k" is equal to "k items of group i are in the subset.
     *               It will contain the next subset at return.
     * @param subsetSize The number of items in the subset @p subset.
     * @param size The sum of all sizes for the subset @p subset.
     * @param groupSizes An int array which is at least @p count long
     *                   "groupSizes[i] = size" means "the items in group i are size large".
     * @param groupCounts An int array which is at least @p count long
     *                   "groupCount[i] = k" means "group i contains k items".
     * @param count The number of groups.
     */
    bool nextGroupSubset(int *subset, int *subsetSize, int *size, int *groupSizes, int *groupCounts, int count);
    
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
    
    bool packGroupItems(int *groupCount, int *groupSizes, int *remainingCapacities, int *assignment, int count, int *profit);
    
    int removeWorstBinPart(int *assignment, int* profit);
    
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
    
    double m_relativeRemainingArea;
    int m_firstMediumProfitMediumSizeOrderIndex;
    int m_firstMediumProfitLowSizeOrderIndex;
    int m_minR;
    int m_maxR;
    int m_normalGroupNumber;
    int *m_normalGroupSize;
    int *m_normalGroupCount;
    int *m_normalGroupStart;
    int m_extraGroupNumber;
    int *m_extraGroupSize;
    int *m_extraGroupCount;
    int *m_extraGroupStart;
    int m_mediumProfitMediumSizeSubsetSizeLimit;
    int m_mediumProfitLowSizeSize;
    
    double m_rho;
    double m_K;
    int m_highProfitSubsetSizeLimit;
};

#endif // IMPROVEDAPPROXIMATEMULTIPLEKNAPSACK_H
