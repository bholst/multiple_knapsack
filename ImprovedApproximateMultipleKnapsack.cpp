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
    m_sortedSizes = sizes();
    qSort(m_sortedSizes);
    qDebug() << m_sortedSizes;
    
    m_numberOfBins = m_sortedSizes.size();
    m_largestBinCapacity = m_sortedSizes.last();
    
    m_rho = 1.0 / ceil(14.0/approximationLevel());
    m_highProfitSubsetSizeLimit = floor(1.0/m_rho);
    
    GreedyMultipleKnapsack greedy;
    greedy.setApproximationLevel(m_rho/2.0);
    greedy.setItems(items());
    greedy.setSizes(m_sortedSizes);
    
    int approximateMaximum = greedy.maximumProfit();
    
    groupItems(approximateMaximum);
    
    bool highProfitSubset[m_firstMediumProfitOrderIndex];
    for(int i = 0; i < m_firstMediumProfitOrderIndex; ++i) {
        highProfitSubset[i] = false;
    }
    int assignment[m_itemProfitOrder.size()];
    int remainingCapacity[m_numberOfBins];
    int highProfitSubsetProfit = 0;
    int subsetSize = 0;
    int subsetCounter = 0;
    while(1) {
        std::cout << "Subset Nr.: " << subsetCounter++ << std::endl
                  << "Profit: " << highProfitSubsetProfit << std::endl
                  << highProfitSubsetToString(highProfitSubset).toStdString() << std::endl;
        
        firstHighProfitSubsetAssignment(highProfitSubset, assignment, remainingCapacity);
        while(1) {
            if(subsetCounter == 82) {
                // Print the assignments
            }
            
            // Guess the next assignment of all high profit items in subset highProfitSubset
            if(!nextHighProfitSubsetAssignment(highProfitSubset, assignment, remainingCapacity)) {
                break;
            }
        }
        
        // Guess next subset of all high profit items.
        if(!nextHighProfitSubset(highProfitSubset, &subsetSize, &highProfitSubsetProfit)) {
            break;
        }
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

bool ImprovedApproximateMultipleKnapsack::nextHighProfitSubset(bool* highProfitSubset, int *subsetSize, int *profit)
{
    int i = 0;
    while(i < m_firstMediumProfitOrderIndex) {
        if(highProfitSubset[i] == false) {
            (*subsetSize)++;
            highProfitSubset[i] = true;
            (*profit) += items().at(m_itemProfitOrder[i]).profit();
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
            (*profit) -= items().at(m_itemProfitOrder[i]).profit();
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

void ImprovedApproximateMultipleKnapsack::firstHighProfitSubsetAssignment(bool* highProfitSubset, 
                                                                          int* assignment, 
                                                                          int* remainingCapacity)
{
    for(int bin = 0; bin < m_numberOfBins; ++bin) {
        remainingCapacity[bin] = m_sortedSizes[bin];
    }
    
    int item;
    for(item = 0; item < m_firstMediumProfitOrderIndex; ++item) {
        if(highProfitSubset[item] == true) {
            assignment[item] = 0;
            remainingCapacity[0] -= items().at(m_itemProfitOrder[item]).size();
        }
        else {
            assignment[item] = -1;
        }
    }
    
    for(; item < m_itemProfitOrder.size(); ++item) {
        assignment[item] = -1;
    }
}

bool ImprovedApproximateMultipleKnapsack::nextHighProfitSubsetAssignment(bool* highProfitSubset, 
                                                                         int* assignment, 
                                                                         int* remainingCapacity)
{
    int firstSubsetElement = 0;
    for(; firstSubsetElement < m_firstMediumProfitOrderIndex; ++firstSubsetElement) {
        if(highProfitSubset[firstSubsetElement] == true) {
            break;
        }
    }
    if(firstSubsetElement == m_firstMediumProfitOrderIndex) {
        return false;
    }
    
    int i = firstSubsetElement;
    while(i < m_firstMediumProfitOrderIndex) {
        int itemSize = items().at(m_itemProfitOrder[i]).size();
        if(assignment[i] < m_numberOfBins) {
            remainingCapacity[assignment[i]] += itemSize;
            assignment[i]++;
            remainingCapacity[assignment[i]] -= itemSize;
            
            bool feasibleAssignment = true;
            for(int bin = 0; bin < m_numberOfBins; ++bin) {
                if(remainingCapacity[bin] < 0) {
                    feasibleAssignment = false;
                    break;
                }
            }
            if(!feasibleAssignment) {
                i = firstSubsetElement;
                continue;
            }
            else {
                break;
            }
        }
        else {
            remainingCapacity[assignment[i]] += itemSize;
            assignment[i] = 0;
            remainingCapacity[0] -= itemSize;
            
            while(i < m_firstMediumProfitOrderIndex) {
                if(highProfitSubset[++i] == true) {
                    break;
                }
            }
        }
    }
    
    if(i >= m_firstMediumProfitOrderIndex) {
        return false;
    }
    else {
        return true;
    }
}

