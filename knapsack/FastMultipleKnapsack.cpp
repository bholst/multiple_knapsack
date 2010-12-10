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
    list<int> sortedSizes = sizes();
    sortedSizes.sort();

    double rho = approximationLevel() / 5.0;
    MultipleKnapsack greedy;
    greedy.setApproximationLevel(rho/2.0);
    greedy.setItems(items());
    greedy.setSizes(sortedSizes);
    
    int approximatedMaximum = greedy.maximumProfit();
    cout << "The approximated Maximum is :" << approximatedMaximum << endl;
    
    // Split the set of items into 2 groups:
    // Much profit:
    set<int> muchProfitItems;
    // Rest:
    set<int> littleProfitItems;
    
    vector<Item> allItems = items();
    int numberOfItems = allItems.size();
    int muchProfit = ceil(rho/sizes().size() * 2.0 * (1 + rho) * approximatedMaximum);
    //int small = rho * (*sortedSizes.begin());
    for(int i = 0; i < numberOfItems; ++i) {
        if(allItems[i].profit() >= muchProfit) {
            muchProfitItems.insert(i);
        }
        else {
            littleProfitItems.insert(i);
        }
    }
    
    // The list of the sets we tested until now
    list< set<int> > subsets;
    set<int> emptySet;
    subsets.push_back(emptySet);
    // Storing the subset with the largest profit until now
    SubsetAssignment largestSubsetAssignment = handleSubset(emptySet);

    // Only include itemLimit items per set
    int itemLimit = sizes().size() / rho;
    
    // Run though all subsets of items with much profit
    set<int>::iterator endIterator = muchProfitItems.end();
    for(set<int>::iterator it = muchProfitItems.begin();
        it != endIterator;
        ++it)
    {
        // Run throuh all subsets generated before and add the current item if possible
        // to create a new subset.
        
        // The list of new subsets
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
                
                // Test if we find a valid assignment for this subset.
                SubsetAssignment otherSubsetAssignment = handleSubset(expandedSet);

                if(otherSubsetAssignment.valid()
                   && otherSubsetAssignment > largestSubsetAssignment)
                {
                    largestSubsetAssignment = otherSubsetAssignment;
                }
            }
        }
        
        // Add the new subsets to the list.
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
        // We did not find any valid assignment for any subset.
        m_maximumProfit = 0;
    }
}

SubsetAssignment FastMultipleKnapsack::handleSubset(const std::set< int >& subset)
{
    int totalItemSize = 0;
    vector<Item> allItems = items();
    // Create a start assignment and calculate the total size of our subsets.
    vector<int> assignment(items().size(), -1);
    set<int>::iterator endIterator = subset.end();
    for(set<int>::iterator it = subset.begin();
        it != endIterator;
        ++it)
    {
        assignment[*it] = 0;
        totalItemSize += allItems[*it].size();
    }
    
    // There is no assignment for the items of our subset.
    if(totalItemSize > totalSize()) {
        return SubsetAssignment();
    }
    
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
        // Fill in the small items.
        FillRemainingMultipleKnapsack fillRemaining;
        fillRemaining.setStartAssignment(assignment);
        fillRemaining.setItems(items()); // FIXME: These are copys, we should use
        fillRemaining.setSizes(sizes()); // FIXME: copy-on-write containers instead.
        assignment = fillRemaining.assignment();
        int profitForAssignment = fillRemaining.maximumProfit();
        
        // Create the subset assignment.
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

