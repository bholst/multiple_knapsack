/*
//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//
*/

// STD
#include <set>
#include <vector>
#include <stack>
#include <cmath>
#include <iostream>

// Project
#include "Item.h"

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
    // Make sizes vector
    list<int> allSizes = sizes();
    list<int>::iterator sizesEnd = allSizes.end();
    for(list<int>::iterator it = allSizes.begin();
        it != sizesEnd;
        ++it)
    {
        m_sizesVector.push_back(*it);
    }

    double rho = approximationLevel() / 5.0;
    MultipleKnapsack greedy;
    greedy.setApproximationLevel(rho/2.0);
    greedy.setItems(items());
    greedy.setSizes(sizes());
    
    int approximatedMaximum = greedy.maximumProfit();
    
    // Split the set of items into 3 groups:
    // Small and much profit:
    set<int> smallAndMuchProfitItems;
    // Large and much profit:
    set<int> largeAndMuchProfitItems;
    set<int> muchProfitItems;
    // Rest:
    set<int> littleProfitItems;
    
    list<int> sortedSizes = allSizes;
    sortedSizes.sort();
    vector<Item> allItems = items();
    int numberOfItems = allItems.size();
    int muchProfit = ceil(rho/sizes().size() * 2.0 * (1 + rho) * approximatedMaximum);
    cout << "Much profit means more than: " << muchProfit << endl;
    int small = rho * *(sortedSizes.begin());
    for(int i = 0; i < numberOfItems; ++i) {
        if(allItems[i].profit() >= muchProfit) {
            muchProfitItems.insert(i);
            if(allItems[i].size() <= small) {
                smallAndMuchProfitItems.insert(i);
            }
            else {
                largeAndMuchProfitItems.insert(i);
            }
        }
        else {
            littleProfitItems.insert(i);
        }
    }
    
    list< set<int> > subsets;
    set<int> emptySet;
    subsets.push_back(emptySet);
    handleSubset(emptySet);
    int itemLimit = allSizes.size() / rho;
    
    set<int>::iterator endIterator = muchProfitItems.end();
    for(set<int>::iterator it = muchProfitItems.begin();
        it != endIterator;
        ++it)
    {
        list< set<int> > expandedSubSets;
        list< set<int> >::iterator endSetIterator = subsets.end();
        for(list< set<int> >::iterator setIt = subsets.begin();
            setIt != endSetIterator;
            ++setIt)
        {
            if(setIt->size() < itemLimit) {
                set<int> expandedSet = *setIt;
                expandedSet.insert(*it);
                expandedSubSets.push_back(expandedSet);
                handleSubset(expandedSet);
            }
        }
        
        list< set<int> >::iterator endExpandedSubsets = expandedSubSets.end();
        for(list< set<int> >::iterator expandedSetIt = expandedSubSets.begin();
            expandedSetIt != endExpandedSubsets;
            ++expandedSetIt)
        {
            subsets.push_back(*expandedSetIt);
        }
    }
}

void FastMultipleKnapsack::handleSubset(const std::set< int >& subset)
{
    vector<int> assignment(items().size(), -1);
    cout << "Subset containing items:" << endl;
    set<int>::iterator endIterator = subset.end();
    for(set<int>::iterator it = subset.begin();
        it != endIterator;
        ++it)
    {
        assignment[*it] = 0;
        cout << *it << endl;
    }
    cout << endl;
    
    bool validAssignmentFound = testAssignment(assignment);
    bool runThroughAllAssignments = false;
    while(!validAssignmentFound && !runThroughAllAssignments) {
        bool assignmentChanged = false;
        endIterator = subset.end();
        set<int>::iterator it = subset.begin();
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
}

bool FastMultipleKnapsack::testAssignment(const std::vector< int >& assignment)
{
    vector<int> remainingSizes = m_sizesVector;
    vector<Item> allItems = items();
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

void FastMultipleKnapsack::printAssignment(const std::vector< int >& assignment)
{
    cout << "Assignment for subset:" << endl;
    int numberOfItems = assignment.size();
    for(int i = 0; i < numberOfItems; ++i)
    {
        cout << "Item " << i << " in bin " << assignment[i] << endl;
    }
    cout << endl;
}

