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

class FastMultipleKnapsack : public MultipleKnapsack
{
public:
    FastMultipleKnapsack();
    virtual ~FastMultipleKnapsack();

private:
    virtual void recalculateValues();
    SubsetAssignment handleSubset(const QSet<int>& subset);
    bool testAssignment(const QVector<int>& assignment);
    void printAssignment(const QVector<int>& assignment);
};

#endif // FASTMULTIPLEKNAPSACK_H
