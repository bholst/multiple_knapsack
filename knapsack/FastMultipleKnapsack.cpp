/*
//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//
*/

// STD
#include <cmath>
#include <iostream>

// Qt
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QtAlgorithms>

// Project
#include "Item.h"
#include "FillRemainingMultipleKnapsack.h"
#include "SubsetAssignment.h"

// Self
#include "FastMultipleKnapsack.h"

using namespace std;

FastMultipleKnapsack::FastMultipleKnapsack()
{

}

FastMultipleKnapsack::~FastMultipleKnapsack()
{

}

void FastMultipleKnapsack::recalculateValues()
{
    QList<int> sortedSizes = sizes();
    qSort(sortedSizes);

    double rho = approximationLevel() / 5.0;
    MultipleKnapsack greedy;
    greedy.setApproximationLevel(rho/2.0);
    greedy.setItems(items());
    greedy.setSizes(sortedSizes);
    
    int approximatedMaximum = greedy.maximumProfit();
    cout << "The approximated Maximum is :" << approximatedMaximum << endl;
    
    // Split the set of items into 2 groups:
    // Much profit:
    QSet<int> muchProfitItems;
    // Rest:
    QSet<int> littleProfitItems;
    int profitForLittleProfitItems = 0;
    
    QVector<Item> allItems = items();
    int numberOfItems = allItems.size();
    int muchProfit = ceil(rho/sizes().size() * 2.0 * (1 + rho) * approximatedMaximum);
    qDebug() << "muchProfit is" << muchProfit;
    //int small = rho * (*sortedSizes.begin());
    for(int i = 0; i < numberOfItems; ++i) {
        qDebug() << "Profit of item" << i << "is" << allItems[i].profit();
        if(allItems[i].profit() >= muchProfit) {
            muchProfitItems.insert(i);
            qDebug() << "The item has much profit.";
        }
        else {
            littleProfitItems.insert(i);
            profitForLittleProfitItems += allItems[i].profit();
            qDebug() << "The item has little profit.";
        }
    }
    
    int numberOfTestedSubsets = 0;
    // The list of the sets we tested until now
    QList< QSet<int> > subsets;
    QSet<int> emptySet;
    subsets.push_back(emptySet);
    // Storing the subset with the largest profit until now
    SubsetAssignment largestSubsetAssignment = handleSubset(emptySet, littleProfitItems,
                                                            profitForLittleProfitItems,
                                                            0);
    numberOfTestedSubsets++;

    // Only include itemLimit items per set
    int itemLimit = sizes().size() / rho;
    
    // Run though all subsets of items with much profit
    QSet<int>::iterator endIterator = muchProfitItems.end();
    for(QSet<int>::iterator it = muchProfitItems.begin();
        it != endIterator;
        ++it)
    {
        // Run throuh all subsets generated before and add the current item if possible
        // to create a new subset.
        
        QList< QSet<int> >::iterator endSetIterator = subsets.end();
        int numberOfSubsets = subsets.size();
        for(int i = 0;
            i < numberOfSubsets;
            ++i)
        {               
            QSet<int> expandedSet = subsets[i];
            expandedSet.insert(*it);

            // Test if we find a valid assignment for this subset.
            SubsetAssignment otherSubsetAssignment = handleSubset(expandedSet, littleProfitItems,
                                                                  profitForLittleProfitItems,
                                                                  largestSubsetAssignment.profit() /*approximatedMaximum*/);
            numberOfTestedSubsets++;
            
            if(!otherSubsetAssignment.noAssignmentPossible()) {
                if(expandedSet.size() < itemLimit) {
                    subsets.push_back(expandedSet);
                }
            }
            
            if(otherSubsetAssignment.valid()) {
                if(otherSubsetAssignment > largestSubsetAssignment) {
                    largestSubsetAssignment = otherSubsetAssignment;
                }
            }
        }
    }
    
    qDebug() << "Tested" << numberOfTestedSubsets << "subsets";
    
    if(largestSubsetAssignment.valid()) {
        m_maximumProfit = largestSubsetAssignment.profit();
    }
    else {
        // We did not find any valid assignment for any subset.
        m_maximumProfit = 0;
    }
}

SubsetAssignment FastMultipleKnapsack::handleSubset(const QSet< int >& subset,
                                                    const QSet<int>& remainingItems,
                                                    int remainingItemsProfit,
                                                    int minimumProfit)
{
    int totalItemSize = 0;
    int totalItemProfit = 0;
    QVector<Item> allItems = items();
    // Create a start assignment and calculate the total size of our subsets.
    qDebug() << "Items selected";
    QVector<int> assignment(items().size(), -1);
    QSet<int>::const_iterator endIterator = subset.end();
    for(QSet<int>::const_iterator it = subset.begin();
        it != endIterator;
        ++it)
    {
        assignment[*it] = 0;
        totalItemSize += allItems[*it].size();
        totalItemProfit += allItems[*it].profit();
    }
    
    // There is no assignment for the items of our subset.
    if(totalItemSize > totalSize()) {
        qDebug() << "The items do not fit";
        SubsetAssignment subsetAssignment;
        subsetAssignment.setNoAssignmentPossible(true);
        return subsetAssignment;
    }
    
    if((totalItemProfit + remainingItemsProfit) < minimumProfit) {
        qDebug() << "Total profit is" << totalItemProfit + remainingItemsProfit << "but we need a profit of" << minimumProfit;
        return SubsetAssignment();
    }
    
    static int numberOfTestedSubsets = 0;
    numberOfTestedSubsets++;
    
    bool validAssignmentFound = testAssignment(assignment);
    bool runThroughAllAssignments = false;
    while(!validAssignmentFound && !runThroughAllAssignments) {
        bool assignmentChanged = false;
        endIterator = subset.end();
        QSet<int>::const_iterator it = subset.begin();
        if(it == endIterator) {
            runThroughAllAssignments = true;
        }
        while (!assignmentChanged && !runThroughAllAssignments) {
            if(assignment[*it] < sizes().size() - 1) {
                assignment[*it] = assignment[*it] + 1;
                assignmentChanged = true;
            }
            else {
                assignment[*it] = 0;
                ++it;
                
                if(it == endIterator) {
                    runThroughAllAssignments = true;
                }
            }
        }
        
        if(!runThroughAllAssignments) {
            validAssignmentFound = testAssignment(assignment);
        }
    }
    
    qDebug() << "Really tested" << numberOfTestedSubsets << "subsets";
    
    if(validAssignmentFound) {
        // Fill in the small items.
        FillRemainingMultipleKnapsack fillRemaining;
        fillRemaining.setStartAssignment(assignment);
        fillRemaining.setItems(items());
        fillRemaining.setSizes(sizes());
        fillRemaining.setItemsToUse(remainingItems);
        assignment = fillRemaining.assignment();
        int profitForAssignment = fillRemaining.maximumProfit();
        qDebug() << "Our profit is:" << fillRemaining.maximumProfit();
        
        // Create the subset assignment.
        SubsetAssignment subsetAssignment;
        subsetAssignment.setSubset(subset);
        subsetAssignment.setProfit(profitForAssignment);
        subsetAssignment.setAssignment(assignment);
        subsetAssignment.setNoAssignmentPossible(false);
        return subsetAssignment;
    }
    
    SubsetAssignment subsetAssignment;
    subsetAssignment.setNoAssignmentPossible(true);
    return subsetAssignment;
}

bool FastMultipleKnapsack::testAssignment(const QVector< int >& assignment)
{
    QVector<int> remainingSizes = sizesVector();
    QVector<Item> allItems = items();
    int numberOfItems = assignment.size();
    for(int i = 0; i < numberOfItems; ++i)
    {
        if(assignment[i] < 0) {
            continue;
        }
        remainingSizes[assignment[i]] = remainingSizes[assignment[i]] - allItems[i].size();
        if(remainingSizes[assignment[i]] < 0) {
            return false;
        }
    }
    
    cout << "Valid assignment!" << endl;
    printAssignment(assignment);
    return true;
}

void FastMultipleKnapsack::printAssignment(const QVector< int >& assignment)
{
    int numberOfItems = assignment.size();
    for(int i = 0; i < numberOfItems; ++i)
    {
        cout << "Item " << i << " in bin " << assignment[i] << endl;
    }
    cout << endl;
}
