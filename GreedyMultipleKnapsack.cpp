//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>
#include <QtCore/QtAlgorithms>

// Project
#include "ProfitItemWithIndex.h"
#include "ApproximateKnapsack.h"

// Self
#include "GreedyMultipleKnapsack.h"

using namespace std;

GreedyMultipleKnapsack::GreedyMultipleKnapsack()
    : MultipleKnapsack()
{

}

GreedyMultipleKnapsack::~GreedyMultipleKnapsack()
{

}

void GreedyMultipleKnapsack::recalculateValues()
{
    // Do the calculation stuff
    QVector<ProfitItemWithIndex> remainingItems;
    for(int i = 0; i < items().size(); ++i) {
        remainingItems.push_back(ProfitItemWithIndex(i, items()[i]));
    }
    
    m_assignment = QVector<int>(items().size(), -1);

    QList<int> allSizes = sizes();
    qSort(allSizes);
    m_maximumProfit = 0;
    for(int i = 0; i < allSizes.size(); ++i) {
        QVector<ProfitItem> itemsForKnapsack;
        for(int j = 0; j < remainingItems.size(); ++j) {
            itemsForKnapsack.append(remainingItems[j]);
        }
        ApproximateKnapsack knapsack;
        knapsack.setSize(allSizes[i]);
        knapsack.setApproximationLevel(approximationLevel());
        knapsack.setItems(itemsForKnapsack);
        
        m_maximumProfit += knapsack.maximumProfit();
        QSet<int> maximumProfitItems = knapsack.maximumProfitItems();
        int itemNumber = remainingItems.size();
        QVector<ProfitItemWithIndex> newRemainingItems;
        for(int j = 0; j < itemNumber; ++j) {
            if(maximumProfitItems.find(j) == maximumProfitItems.end()) {
                newRemainingItems.push_back(remainingItems[j]);
            }
            else {
                m_assignment[remainingItems[j].index()] = i;
            }
        }
        
        remainingItems = newRemainingItems;
    }
    setDirty(false);
}

