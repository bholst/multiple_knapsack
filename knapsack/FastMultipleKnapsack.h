/*
//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//
*/

#ifndef FASTMULTIPLEKNAPSACK_H
#define FASTMULTIPLEKNAPSACK_H

// Qt
#include<QtCore/QSet>
#include<QtCore/QVector>

// Project
#include "MultipleKnapsack.h"

class SubsetAssignment;
class ItemWithIndex;

class FastMultipleKnapsack : public MultipleKnapsack
{
public:
    FastMultipleKnapsack();
    virtual ~FastMultipleKnapsack();

private:
    virtual void recalculateValues();
    SubsetAssignment handleSubset(const QSet<int>& subset, const QSet<int>& remainingItems,
                                  int remainingItemsProfit, int minimumProfit);
    bool testAssignment(const QVector<int>& assignment, const QVector<ItemWithIndex>& assignedItems);
    void printAssignment(const QVector<int>& assignment);
};

#endif // FASTMULTIPLEKNAPSACK_H
