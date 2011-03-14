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
    
//     m_rho = 1.0 / ceil(14.0/approximationLevel());
    m_rho = 1.0 / ceil(4.0/approximationLevel());
    m_K = 5.0;
    m_highProfitSubsetSizeLimit = floor(1.0/m_rho);
    
    GreedyMultipleKnapsack greedy;
    greedy.setApproximationLevel(m_rho/2.0);
    greedy.setItems(items());
    greedy.setSizes(m_sortedSizes);
    
    m_totalCapacity = 0;
    for(int bin = 0; bin < m_numberOfBins; ++bin) {
        m_totalCapacity += m_sortedSizes[bin];
    }
    qDebug() << "Total capacity is:" << m_totalCapacity;
    
    int approximateMaximum = greedy.maximumProfit();
    
    groupItems(approximateMaximum);
    
    bool highProfitSubset[m_firstMediumProfitOrderIndex];
    for(int i = 0; i < m_firstMediumProfitOrderIndex; ++i) {
        highProfitSubset[i] = false;
    }
    int assignment[m_itemProfitSizeOrder.size()];
    int remainingCapacity[m_numberOfBins];
    int highProfitSubsetProfits = 0;
    int highProfitSubsetSizes = 0;
    int highProfitSubsetCount = 0;
    int subsetCounter = 0;
    while(1) {
        std::cout << "Subset Nr.: " << subsetCounter++ << std::endl
                  << "Profit: " << highProfitSubsetProfits << std::endl
                  << highProfitSubsetToString(highProfitSubset).toStdString() << std::endl;
        
        bool foundHighProfitSubsetAssignment
            = firstHighProfitSubsetAssignment(highProfitSubset, assignment, remainingCapacity);
        int numberOfMediumProfitHighSizeItems;
        if(foundHighProfitSubsetAssignment) {
            groupMediumItems(m_totalCapacity - highProfitSubsetSizes);
            numberOfMediumProfitHighSizeItems = m_firstMediumProfitMediumSizeOrderIndex - m_firstMediumProfitOrderIndex;
        }
        while(foundHighProfitSubsetAssignment) {
            // Do stuff with the assignment.
            int mediumProfitHighSizeSubsetProfits = 0;
            int mediumProfitHighSizeSubsetSizes = 0;
            int mediumProfitHighSizeSubsetCount = 0;
            
            // Creating the first subset (containing none of the items.
            bool mediumProfitHighSizeSubset[numberOfMediumProfitHighSizeItems];
            for(int i = 0; i < numberOfMediumProfitHighSizeItems; ++i) {
                mediumProfitHighSizeSubset[i] = false;
            }
            while(1) {
                // Do stuff with the subset.
                
                if(!nextSubset(mediumProfitHighSizeSubset,
                               &mediumProfitHighSizeSubsetCount,
                               &mediumProfitHighSizeSubsetProfits,
                               &mediumProfitHighSizeSubsetSizes,
                               m_firstMediumProfitOrderIndex,
                               numberOfMediumProfitHighSizeItems,
                               numberOfMediumProfitHighSizeItems))
                {
                    break;
                }   
            }
            
            // Guess the next assignment of all high profit items in subset highProfitSubset
            foundHighProfitSubsetAssignment
                = nextHighProfitSubsetAssignment(highProfitSubset, assignment, remainingCapacity);
        }
        
        // Guess next subset of all high profit items.
        if(!nextSubset(highProfitSubset,
                       &highProfitSubsetCount,
                       &highProfitSubsetProfits,
                       &highProfitSubsetSizes,
                       0,
                       m_firstMediumProfitOrderIndex,
                       m_highProfitSubsetSizeLimit))
        {
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
    qDebug() << "Item has high profit with:" << minHighProfit;
    int minMediumProfit = ceil(2.0 * (m_rho / sizes().size()) * (1 + m_rho) * approximateMaximum);
    qDebug() << "Item has medium profit with:" << minMediumProfit;
    
    m_itemProfitSizeOrder = itemProfitOrder();
    qDebug() << m_itemProfitSizeOrder;
    
    QVector<ProfitItem> allItems = items();
    
    int i;
    // High profit items.
    for(i = 0; i < allItems.size(); ++i) {
        int itemNr = m_itemProfitSizeOrder[i];
        int profit = allItems[itemNr].profit();
        if(profit >= minHighProfit) {
//             m_highProfitItems.insert(itemNr);
        }
        else {
            break;
        }
    }
    
    // Medium Profit items.
    m_firstMediumProfitOrderIndex = i;
    for(; i < allItems.size(); ++i) {
        int itemNr = m_itemProfitSizeOrder[i];
        int profit = allItems[itemNr].profit();
        if(profit >= minMediumProfit) {
//             m_mediumProfitItems.insert(itemNr);
            
            // The medium profit items are categorized later in size categories,
            // because the remaining capacity is needed for that.
        }
        else {
            break;
        }
    }
    
    // Low profit items.
    m_firstLowProfitOrderIndex = i;
//     for(; i < allItems.size(); ++i) {
//         m_lowProfitItems.insert(itemNr);
//     }
    
    sortItemSizeOrder(&m_itemProfitSizeOrder,
                      m_firstMediumProfitOrderIndex,
                      m_firstLowProfitOrderIndex - m_firstMediumProfitOrderIndex);
    qDebug() << "Sorted sizes:";
    qDebug() << m_itemProfitSizeOrder;
}

void ImprovedApproximateMultipleKnapsack::groupMediumItems(int remainingArea)
{
    int maxMediumSize = floor((m_rho * remainingArea)/(2 * m_K * pow(log2(1.0/m_rho), 3.0)));
    int minMediumSize = ceil(pow(m_rho, 6.0) * remainingArea);
    
//     m_mediumProfitHighSizeItems.clear();
//     m_mediumProfitMediumSizeItems.clear();
//     m_mediumProfitLowSizeItems.clear();
    
    qDebug() << "Medium items:";
    int item;
    for(item = m_firstMediumProfitOrderIndex; item < m_firstLowProfitOrderIndex; ++item) {
        int size = items().at(m_itemProfitSizeOrder[item]).size();
        if(size > maxMediumSize) {
            // High size
//             m_mediumProfitHighSizeItems.insert(m_itemProfitSizeOrder[item]);
        } else {
            break;
        }
    }
    
    m_firstMediumProfitMediumSizeOrderIndex = item;
    
    for(; item < m_firstLowProfitOrderIndex; ++item) {
        int size = items().at(m_itemProfitSizeOrder[item]).size();
        
        if(size >= minMediumSize) {
            // Medium size
//             m_mediumProfitMediumSizeItems.insert(m_itemProfitSizeOrder[item]);
        }
        else {
            break;
        }
    }
    
    m_firstMediumProfitLowSizeOrderIndex = item;
    
//     for(; item < m_firstLowProfitOrderIndex; ++item) {
        // Low size
//         m_mediumProfitLowSizeItems.insert(m_itemProfitSizeOrder[item]);
//     }
}

bool ImprovedApproximateMultipleKnapsack::nextSubset(bool *subset,
                                                     int *subsetSize, 
                                                     int *profit, 
                                                     int *size,
                                                     int first,
                                                     int count,
                                                     int itemLimit)
{
    int i = 0;
    while(i < count) {
        ProfitItem item = items().at(m_itemProfitSizeOrder[i+first]);
        if(subset[i] == false) {
            (*subsetSize)++;
            subset[i] = true;
            (*profit) += item.profit();
            (*size) += item.size();
            if(*subsetSize > itemLimit) {
                i = 0;
                continue;
            }
            else {
                break;
            }
        }
        else {
            (*subsetSize)--;
            subset[i] = false;
            (*profit) -= item.profit();
            (*size) -= item.size();
            ++i;
        }
    }
    
    if(i >= count) {
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
        result += QString("%1|").arg(m_itemProfitSizeOrder[i], 5);
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

QString ImprovedApproximateMultipleKnapsack::highProfitSubsetAssignmentToString(int* assignment)
{
    QString result;
    for(int i = 0; i < m_firstMediumProfitOrderIndex; ++i) {
        result += QString("%1|").arg(m_itemProfitSizeOrder[i], 5);
    }
    result += "\n";
    for(int i = 0; i < m_firstMediumProfitOrderIndex; ++i) {
        result += QString("%1|").arg(assignment[i], 5);
    }
    return result;
}

bool ImprovedApproximateMultipleKnapsack::firstHighProfitSubsetAssignment(bool* highProfitSubset, 
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
            remainingCapacity[0] -= items().at(m_itemProfitSizeOrder[item]).size();
        }
        else {
            assignment[item] = -1;
        }
    }
    
    for(; item < m_itemProfitSizeOrder.size(); ++item) {
        assignment[item] = -1;
    }
    
    if(remainingCapacity[0] < 0) {
        return nextHighProfitSubsetAssignment(highProfitSubset, assignment, remainingCapacity);
    }
    else {
        return true;
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
        int itemSize = items().at(m_itemProfitSizeOrder[i]).size();
        if(assignment[i] + 1 < m_numberOfBins) {
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

