/*
//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//
*/

#ifndef APPROXIMATEMULTIPLEKNAPSACK_H
#define APPROXIMATEMULTIPLEKNAPSACK_H

// Qt
#include<QtCore/QSet>
#include<QtCore/QVector>

// Project
#include "MultipleKnapsack.h"

class SubsetAssignment;
class ProfitItemWithIndex;

class ApproximateMultipleKnapsack : public MultipleKnapsack
{
public:
    ApproximateMultipleKnapsack();
    virtual ~ApproximateMultipleKnapsack();

private:
    virtual void recalculateValues();
    SubsetAssignment handleSubset(const QSet<int>& subset, const QSet<int>& remainingItems,
                                  int remainingItemsProfit, int minimumProfit);
    bool testAssignment(const QVector<int>& assignment, const QVector<ProfitItemWithIndex>& assignedItems);
    void printAssignment(const QVector<int>& assignment);
};

#endif // APPROXIMATEMULTIPLEKNAPSACK_H
