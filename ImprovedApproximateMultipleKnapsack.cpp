//
// Copyright (C) 2011  Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <cmath>
#include <iostream>

// Qt
#include <QtCore/QDebug>
#include <QtCore/QList>

// Project
#include "GreedyMultipleKnapsack.h"
#include "ProfitItem.h"

// Self
#include "ImprovedApproximateMultipleKnapsack.h"

ImprovedApproximateMultipleKnapsack::ImprovedApproximateMultipleKnapsack()
{

}

ImprovedApproximateMultipleKnapsack::~ImprovedApproximateMultipleKnapsack()
{

}

void ImprovedApproximateMultipleKnapsack::recalculateValues()
{
    QList<int> sortedSizes = sizes();
    qSort(sortedSizes);
    qDebug() << sortedSizes;
    
    m_largestBinCapacity = sortedSizes.last();
    
    m_rho = 1.0 / ceil(14.0/approximationLevel());
    m_highProfitSubsetSizeLimit = floor(1.0/m_rho);
    
    GreedyMultipleKnapsack greedy;
    greedy.setApproximationLevel(m_rho/2.0);
    greedy.setItems(items());
    greedy.setSizes(sortedSizes);
    
    int approximateMaximum = greedy.maximumProfit();
    
    groupItems(approximateMaximum);
    
    bool highProfitSubset[m_firstMediumProfitOrderIndex];
    for(int i = 0; i < m_firstMediumProfitOrderIndex; ++i) {
        highProfitSubset[i] = false;
    }
    int subsetSize = 0;
    int subsetCounter = 0;
    while(1) {
        // Guess next subset of all high profit items.
        if(!nextHighProfitSubset(highProfitSubset, &subsetSize)) {
            break;
        }
        std::cout << "Subset Nr.: " << subsetCounter++ << std::endl;
        std::cout << highProfitSubsetToString(highProfitSubset).toStdString() << std::endl;
    }
    
    m_assignment.clear();
    m_maximumProfit = 0;
    setDirty(false);
}

void ImprovedApproximateMultipleKnapsack::groupItems(int approximateMaximum)
{
    // Assigning items to their groups.
    
    // Profit categories:
    int minHighProfit = ceil(2.0 * m_rho * (1 + m_rho) * approximateMaximum);
    int minMediumProfit = ceil(2.0 * (m_rho / sizes().size()) * (1 + m_rho) * approximateMaximum);
    
    m_itemProfitOrder = itemProfitOrder();
    qDebug() << m_itemProfitOrder;
    
    QVector<ProfitItem> allItems = items();
    
    int i;
    // High profit items.
    for(i = 0; i < allItems.size(); ++i) {
        int itemNr = m_itemProfitOrder[i];
        int profit = allItems[itemNr].profit();
        if(profit >= minHighProfit) {
            m_highProfitItems.insert(itemNr);
        }
        else {
            break;
        }
    }
    
    // Medium Profit items.
    m_firstMediumProfitOrderIndex = i;
    for(; i < allItems.size(); ++i) {
        int itemNr = m_itemProfitOrder[i];
        int profit = allItems[itemNr].profit();
        if(profit >= minMediumProfit) {
            m_mediumProfitItems.insert(itemNr);
            
            // The medium profit items are categorized later in size categories,
            // because the remaining capacity is needed for that.
        }
        else {
            break;
        }
    }
    
    // Low profit items.
    m_firstLowProfitOrderIndex = i;
    for(; i < allItems.size(); ++i) {
        int itemNr = m_itemProfitOrder[i];
        int profit = allItems[itemNr].profit();
        m_lowProfitItems.insert(itemNr);
    }
}

bool ImprovedApproximateMultipleKnapsack::nextHighProfitSubset(bool* highProfitSubset, int *subsetSize)
{
    int i = 0;
    while(i < m_firstMediumProfitOrderIndex) {
        if(highProfitSubset[i] == false) {
            (*subsetSize)++;
            highProfitSubset[i] = true;
            if(*subsetSize > m_highProfitSubsetSizeLimit) {
                i = 0;
                continue;
            }
            else {
                break;
            }
        }
        else {
            (*subsetSize)--;
            highProfitSubset[i] = false;
            ++i;
        }
    }
    
    if(i >= m_firstMediumProfitOrderIndex) {
        return false;
    }
    else {
        return true;
    }
}

QString ImprovedApproximateMultipleKnapsack::highProfitSubsetToString(bool* highProfitSubset)
{
    QString result;
    for(int i = 0; i < m_firstMediumProfitOrderIndex; ++i) {
        result += QString("%1|").arg(m_itemProfitOrder[i], 5);
    }
    result += "\n";
    for(int i = 0; i < m_firstMediumProfitOrderIndex; ++i) {
        if(highProfitSubset[i]) {
            result += QString("  *  |");
        }
        else {
            result += QString("     |");
        }
    }
    return result;
}

