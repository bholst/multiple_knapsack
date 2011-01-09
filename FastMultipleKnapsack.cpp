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
#include <QtCore/QTime>
#include <QtCore/QList>
#include <QtCore/QtAlgorithms>

// Project
#include "ProfitItem.h"
#include "FillRemainingMultipleKnapsack.h"
#include "SubsetAssignment.h"
#include "ProfitItemWithIndex.h"

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
    
    QVector<ProfitItem> allItems = items();
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
    QSet<int>::const_iterator endIterator = muchProfitItems.constEnd();
    for(QSet<int>::const_iterator it = muchProfitItems.constBegin();
        it != endIterator;
        ++it)
    {
        // Run throuh all subsets generated before and add the current item if possible
        // to create a new subset.
        
        int numberOfSubsets = subsets.size();
        qDebug() << "Number of subsets for item" << *it << "is" << numberOfSubsets;
        
        for(int i = 0; i < numberOfSubsets; ++i)
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
        m_assignment = largestSubsetAssignment.assingment();
    }
    else {
        // We did not find any valid assignment for any subset.
        m_maximumProfit = 0;
        m_assignment.clear();
    }
}

SubsetAssignment FastMultipleKnapsack::handleSubset(const QSet< int >& subset,
                                                    const QSet<int>& remainingItems,
                                                    int remainingItemsProfit,
                                                    int minimumProfit)
{
    int totalItemSize = 0;
    int totalItemProfit = 0;
    QVector<ProfitItem> allItems = items();
    QVector<ProfitItemWithIndex> subsetItems;
    subsetItems.reserve(subset.size());
    // Create a start assignment and calculate the total size of our subsets.
    QVector<int> assignment(subset.size(), 0);
    QSet<int>::const_iterator endIterator = subset.constEnd();
    for(QSet<int>::const_iterator it = subset.constBegin();
        it != endIterator;
        ++it)
    {
        ProfitItemWithIndex currentItem(*it, allItems[*it]);
        subsetItems.push_back(currentItem);
        totalItemSize += currentItem.size();
        totalItemProfit += currentItem.profit();
    }
    
    // There is no assignment for the items of our subset.
    if(totalItemSize > totalSize()) {
        SubsetAssignment subsetAssignment;
        subsetAssignment.setNoAssignmentPossible(true);
        return subsetAssignment;
    }
    
    QList<int> remainingSizes = sizes();
    int numberOfOverfullBins = 0;
    remainingSizes[0] = remainingSizes[0] - totalItemSize;
    if(remainingSizes[0] < 0) {
        numberOfOverfullBins++;
    }
//     bool validAssignmentFound = testAssignment(assignment, subsetItems);
    bool runThroughAllAssignments = false;
    while(numberOfOverfullBins != 0 && !runThroughAllAssignments) {
        bool assignmentChanged = false;
        endIterator = subset.end();
        int i = 0;
        if(i == subsetItems.size()) {
            runThroughAllAssignments = true;
        }
        while (!assignmentChanged && !runThroughAllAssignments) {
            int itemSize = subsetItems[i].size();
            // Take the item out of the old bin
            remainingSizes[assignment[i]] += itemSize;
            if(remainingSizes[assignment[i]] >= 0
               && remainingSizes[assignment[i]] < itemSize)
            {
                numberOfOverfullBins--;
            }

            // Change assignment
            int nextAssignment;
            if(assignment[i] < sizes().size() - 1) {
                nextAssignment = assignment[i] + 1;
                assignment[i] = nextAssignment;
                
                assignmentChanged = true;
            }
            else {
                nextAssignment = 0;
                assignment[i] = nextAssignment;
                ++i;
                
                if(i == subsetItems.size()) {
                    runThroughAllAssignments = true;
                }
            }
            
            // Put the item into the new bin
            
            remainingSizes[nextAssignment] = remainingSizes[nextAssignment] - itemSize;
            if(remainingSizes[nextAssignment] < 0
               && remainingSizes[nextAssignment] >= -itemSize)
            {
                numberOfOverfullBins++;
            }
        }
        
        if(runThroughAllAssignments) {
//             validAssignmentFound = testAssignment(assignment, subsetItems);
            numberOfOverfullBins = -1;
        }
    }
    
    if(numberOfOverfullBins == 0) {
        if((totalItemProfit + remainingItemsProfit) < minimumProfit) {
// //         qDebug() << "Total profit is" << totalItemProfit + remainingItemsProfit << "but we need a profit of" << minimumProfit; 
//             m_totalTimeSum += totalTime.elapsed();
            return SubsetAssignment();
        }

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
//         m_totalTimeSum += totalTime.elapsed();
        return subsetAssignment;
    }
    
    SubsetAssignment subsetAssignment;
    subsetAssignment.setNoAssignmentPossible(true);
//     m_totalTimeSum += totalTime.elapsed();
    return subsetAssignment;
}

bool FastMultipleKnapsack::testAssignment(const QVector< int >& assignment,
                                          const QVector<ProfitItemWithIndex>& assignmentItems)
{
    QList<int> remainingSizes = sizes();
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
