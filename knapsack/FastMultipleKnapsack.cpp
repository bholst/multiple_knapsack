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
#include "ItemWithIndex.h"

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
    QVector<int> approximatedAssignment = greedy.assignment();
    SubsetAssignment approximatedSubsetAssignment;
    approximatedSubsetAssignment.setAssignment(approximatedAssignment);
    approximatedSubsetAssignment.setProfit(approximatedMaximum);
    approximatedSubsetAssignment.setSubset(QSet<int>());
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
    if(approximatedSubsetAssignment > largestSubsetAssignment) {
        largestSubsetAssignment = approximatedSubsetAssignment;
    }
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
                                                                  largestSubsetAssignment.profit());
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
//     int totalItemProfit = 0;
    QVector<Item> allItems = items();
    QVector<ItemWithIndex> subsetItems;
    // Create a start assignment and calculate the total size of our subsets.
//     qDebug() << "Items selected";
    QVector<int> assignment(subset.size(), 0);
    QSet<int>::const_iterator endIterator = subset.end();
    for(QSet<int>::const_iterator it = subset.begin();
        it != endIterator;
        ++it)
    {
        subsetItems.push_back(ItemWithIndex(*it, allItems[*it]));
        totalItemSize += allItems[*it].size();
//         totalItemProfit += allItems[*it].profit();
    }
    
    // There is no assignment for the items of our subset.
    if(totalItemSize > totalSize()) {
//         qDebug() << "The items do not fit";
        SubsetAssignment subsetAssignment;
        subsetAssignment.setNoAssignmentPossible(true);
        return subsetAssignment;
    }
    
    // Trying to find out if our set would give a profit large enough makes no sense.
    // It would result into more subsets being in the list of subsets so it later takes more
    // time to do this with expandedSets.
//     if((totalItemProfit + remainingItemsProfit) < minimumProfit) {
// //         qDebug() << "Total profit is" << totalItemProfit + remainingItemsProfit << "but we need a profit of" << minimumProfit;
//         return SubsetAssignment();
//     }
    
    static int numberOfTestedSubsets = 0;
    numberOfTestedSubsets++;
    
    bool validAssignmentFound = testAssignment(assignment, subsetItems);
    bool runThroughAllAssignments = false;
    while(!validAssignmentFound && !runThroughAllAssignments) {
        bool assignmentChanged = false;
        endIterator = subset.end();
        int i = 0;
        if(i == subsetItems.size()) {
            runThroughAllAssignments = true;
        }
        while (!assignmentChanged && !runThroughAllAssignments) {
            if(assignment[i] < sizes().size() - 1) {
                assignment[i] = assignment[i] + 1;
                assignmentChanged = true;
            }
            else {
                assignment[i] = 0;
                ++i;
                
                if(i == subsetItems.size()) {
                    runThroughAllAssignments = true;
                }
            }
        }
        
        if(!runThroughAllAssignments) {
            validAssignmentFound = testAssignment(assignment, subsetItems);
        }
    }
    
//     qDebug() << "Really tested" << numberOfTestedSubsets << "subsets";
    
    if(validAssignmentFound) {
        // Fill in the small items.
        QVector<int> completeAssignment(allItems.size(), -1);
        for(int i = 0; i < subsetItems.size(); ++i) {
            completeAssignment[subsetItems[i].index()] = assignment[i];
        }
        qDebug() << "Valid assignment";
        printAssignment(completeAssignment);
        FillRemainingMultipleKnapsack fillRemaining;
        fillRemaining.setStartAssignment(completeAssignment);
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

bool FastMultipleKnapsack::testAssignment(const QVector< int >& assignment,
                                          const QVector<ItemWithIndex>& assignmentItems)
{
    QVector<int> remainingSizes = sizesVector();
    int numberOfItems = assignment.size();
    for(int i = 0; i < assignment.size(); ++i)
    {
        if(assignment[i] < 0) {
            continue;
        }
        remainingSizes[assignment[i]] = remainingSizes[assignment[i]] - assignmentItems[i].size();
        if(remainingSizes[assignment[i]] < 0) {
            return false;
        }
    }
    
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
