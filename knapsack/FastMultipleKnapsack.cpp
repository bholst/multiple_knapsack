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
    
    list<int> allSizes = sizes();
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
    cout << "Subset containing items:" << endl;
    set<int>::iterator endIterator = subset.end();
    for(set<int>::iterator it = subset.begin();
        it != endIterator;
        ++it)
    {
        cout << *it << endl;
    }
    cout << endl;
}
