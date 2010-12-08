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
    double rho = approximationLevel() / 5.0;
    MultipleKnapsack greedy;
    greedy.setApproximationLevel(rho/2.0);
    greedy.setItems(items());
    greedy.setSizes(sizes());
    
    int approximatedMaximum = greedy.maximumProfit();
    cout << "The approximated Maximum is :" << approximatedMaximum << endl;
    
    // Split the set of items into 3 groups:
    // Small and much profit:
    set<int> smallAndMuchProfitItems;
    // Large and much profit:
    set<int> largeAndMuchProfitItems;
    set<int> muchProfitItems;
    // Rest:
    set<int> littleProfitItems;
    
    list<int> sortedSizes = sizes();
    sortedSizes.sort();
    vector<Item> allItems = items();
    int numberOfItems = allItems.size();
    int muchProfit = ceil(rho/sizes().size() * 2.0 * (1 + rho) * approximatedMaximum);
    cout << "Much profit means more than: " << muchProfit << endl;
    int small = rho * *(sortedSizes.begin()); // TODO: This seems to be bullshit
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
    SubsetAssignment largestSubsetAssignment = handleSubset(emptySet);
    int itemLimit = sizes().size() / rho;
    cout << "Please choose a maximum of " << itemLimit << " items only" << endl;
    
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
                SubsetAssignment otherSubsetAssignment = handleSubset(expandedSet);
                if(otherSubsetAssignment.valid()
                   && otherSubsetAssignment > largestSubsetAssignment)
                {
                    largestSubsetAssignment = otherSubsetAssignment;
                }
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
    
    if(largestSubsetAssignment.valid()) {
        m_maximumProfit = largestSubsetAssignment.profit();
    }
    else {
        m_maximumProfit = 0;
    }
}

SubsetAssignment FastMultipleKnapsack::handleSubset(const std::set< int >& subset)
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
    
    if(validAssignmentFound) {
        FillRemainingMultipleKnapsack fillRemaining;
        fillRemaining.setStartAssignment(assignment);
        fillRemaining.setItems(items());
        fillRemaining.setSizes(sizes());
        assignment = fillRemaining.assignment();
        int profitForAssignment = fillRemaining.maximumProfit();
        
        SubsetAssignment subsetAssignment;
        subsetAssignment.setSubset(subset);
        subsetAssignment.setProfit(profitForAssignment);
        subsetAssignment.setAssignment(assignment);
        return subsetAssignment;
    }
    
    return SubsetAssignment();
}

bool FastMultipleKnapsack::testAssignment(const std::vector< int >& assignment)
{
    vector<int> remainingSizes = sizesVector();
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
    int numberOfItems = assignment.size();
    for(int i = 0; i < numberOfItems; ++i)
    {
        cout << "Item " << i << " in bin " << assignment[i] << endl;
    }
    cout << endl;
}

