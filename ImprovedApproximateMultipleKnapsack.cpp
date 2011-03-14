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
                  << subsetToString(highProfitSubset, 0, m_firstMediumProfitOrderIndex).toStdString() << std::endl;
        
        bool foundHighProfitSubsetAssignment
            = firstSubsetAssignment(highProfitSubset, assignment, remainingCapacity, 0, m_firstMediumProfitOrderIndex);
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
            
            int mediumProfitHighSizeSubsetCounter = 0;
            
            // Creating the first subset (containing none of the items.
            bool mediumProfitHighSizeSubset[numberOfMediumProfitHighSizeItems];
            for(int i = 0; i < numberOfMediumProfitHighSizeItems; ++i) {
                mediumProfitHighSizeSubset[i] = false;
            }
            while(1) {
                // Do stuff with the subset.
                std::cout << "Subset Nr.: " << mediumProfitHighSizeSubsetCounter++ << std::endl
                          << "Profit: " << mediumProfitHighSizeSubsetProfits << std::endl
                          << subsetToString(mediumProfitHighSizeSubset,
                                            m_firstMediumProfitOrderIndex,
                                            numberOfMediumProfitHighSizeItems).toStdString() << std::endl;
                int mediumProfitHighSizeRemainingCapacity[m_numberOfBins];
                memcpy(mediumProfitHighSizeRemainingCapacity, remainingCapacity, m_numberOfBins);
                
                bool foundMediumProfitHighSizeSubsetAssignment
                    = firstSubsetAssignment(mediumProfitHighSizeSubset,
                                            assignment,
                                            mediumProfitHighSizeRemainingCapacity,
                                            m_firstMediumProfitOrderIndex,
                                            numberOfMediumProfitHighSizeItems);
//                 if(foundMediumProfitHighSizeSubsetAssignment) {
//                     std::cout << "Assignment following now:" << std::endl;
//                 }
                while(foundMediumProfitHighSizeSubsetAssignment) {
                    // Do stuff with the Assignment
//                     std::cout << subsetAssignmentToString(assignment,
//                                                           m_firstMediumProfitOrderIndex,
//                                                           numberOfMediumProfitHighSizeItems).toStdString()
//                               << std::endl << std::endl;
                    
                    // Guess the next assignment of all medium profit item in subset mediumProfitHighSizeSubset.
                    foundMediumProfitHighSizeSubsetAssignment
                        = nextSubsetAssignment(mediumProfitHighSizeSubset,
                                               assignment,
                                               mediumProfitHighSizeRemainingCapacity,
                                               m_firstMediumProfitOrderIndex,
                                               numberOfMediumProfitHighSizeItems);
                }
                
                
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
            // Now we have tried all subsets of the medium profit items with high size
            // Cleaning up
            for(int item = m_firstMediumProfitOrderIndex;
                item < m_firstMediumProfitMediumSizeOrderIndex;
                ++item)
            {
                assignment[item] = -1;
            }
            
            // Guess the next assignment of all high profit items in subset highProfitSubset
            foundHighProfitSubsetAssignment
                = nextSubsetAssignment(highProfitSubset,
                                       assignment,
                                       remainingCapacity,
                                       0,
                                       m_firstMediumProfitOrderIndex);
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

QString ImprovedApproximateMultipleKnapsack::subsetToString(bool* subset,
                                                            int first,
                                                            int count)
{
    QString result;
    for(int i = 0; i < count; ++i) {
        result += QString("%1|").arg(m_itemProfitSizeOrder[i+first], 5);
    }
    result += "\n";
    for(int i = 0; i < count; ++i) {
        if(subset[i]) {
            result += QString("  *  |");
        }
        else {
            result += QString("     |");
        }
    }
    return result;
}

QString ImprovedApproximateMultipleKnapsack::subsetAssignmentToString(int* assignment,
                                                                      int first,
                                                                      int count)
{
    QString result;
    for(int i = 0; i < count; ++i) {
        result += QString("%1|").arg(m_itemProfitSizeOrder[i+first], 5);
    }
    result += "\n";
    for(int i = 0; i < count; ++i) {
        result += QString("%1|").arg(assignment[i+first], 5);
    }
    return result;
}

bool ImprovedApproximateMultipleKnapsack::firstSubsetAssignment(bool* subset, 
                                                                int* assignment, 
                                                                int* remainingCapacity,
                                                                int first,
                                                                int count)
{
    if(first == 0) {
        // If we create a new assignment, reset the remaining capacities.
        for(int bin = 0; bin < m_numberOfBins; ++bin) {
            remainingCapacity[bin] = m_sortedSizes[bin];
        }
    }
    
    int item;
    for(item = first; item < first + count; ++item) {
        // Working with absolute item numbers here.
        if(subset[item-first] == true) {
            assignment[item] = 0;
            remainingCapacity[0] -= items().at(m_itemProfitSizeOrder[item]).size();
        }
        else {
            assignment[item] = -1;
        }
    }
    
    if(first == 0) {
        for(; item < m_itemProfitSizeOrder.size(); ++item) {
            assignment[item] = -1;
        }
    }
    
    if(remainingCapacity[0] < 0) {
        return nextSubsetAssignment(subset, assignment, remainingCapacity, first, count);
    }
    else {
        return true;
    }
}

bool ImprovedApproximateMultipleKnapsack::nextSubsetAssignment(bool* subset, 
                                                               int* assignment, 
                                                               int* remainingCapacity,
                                                               int first,
                                                               int count)
{
    int firstSubsetElement = 0;
    for(; firstSubsetElement < count; ++firstSubsetElement) {
        if(subset[firstSubsetElement] == true) {
            break;
        }
    }
    if(firstSubsetElement == count) {
        return false;
    }
    
    int i = firstSubsetElement;
    while(i < count) {
        int absoluteItemNr = i + first;
        int itemSize = items().at(m_itemProfitSizeOrder[absoluteItemNr]).size();
        if(assignment[absoluteItemNr] + 1 < m_numberOfBins) {
            remainingCapacity[assignment[absoluteItemNr]] += itemSize;
            assignment[absoluteItemNr]++;
            remainingCapacity[assignment[absoluteItemNr]] -= itemSize;
            
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
            remainingCapacity[assignment[absoluteItemNr]] += itemSize;
            assignment[absoluteItemNr] = 0;
            remainingCapacity[0] -= itemSize;
            
            while(i < count) {
                if(subset[++i] == true) {
                    break;
                }
            }
        }
    }
    
    if(i >= count) {
        return false;
    }
    else {
        return true;
    }
}

