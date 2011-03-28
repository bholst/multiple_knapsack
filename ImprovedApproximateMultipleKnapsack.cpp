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
#include "PackingAlgorithms.h"

// Self
#include "ImprovedApproximateMultipleKnapsack.h"

ImprovedApproximateMultipleKnapsack::ImprovedApproximateMultipleKnapsack()
    : m_itemProfitSizeOrder(0),
      m_orderedRelativeSizes(0),
      m_normalGroupSize(0),
      m_normalGroupCount(0),
      m_normalGroupStart(0),
      m_extraGroupSize(0),
      m_extraGroupCount(0),
      m_extraGroupStart(0)
{
    setApproximationLevel(0.7);
}

ImprovedApproximateMultipleKnapsack::~ImprovedApproximateMultipleKnapsack()
{
    delete m_itemProfitSizeOrder;
    delete m_orderedRelativeSizes;
    delete m_normalGroupSize;
    delete m_normalGroupCount;
    delete m_normalGroupStart;
    delete m_extraGroupSize;
    delete m_extraGroupCount;
    delete m_extraGroupStart;
}

void ImprovedApproximateMultipleKnapsack::recalculateValues()
{
    m_itemNumber = items().size();
    
    m_sortedSizes = sizes();
    qSort(m_sortedSizes);
    qDebug() << m_sortedSizes;
    
    m_numberOfBins = m_sortedSizes.size();
    m_largestBinCapacity = m_sortedSizes.last();
    
    m_rho = 1.0 / ceil(14.0/approximationLevel());
//     m_rho = 1.0 / ceil(4.0/approximationLevel());
    m_K = 56.0;
//     m_K = 1;
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
    
    int maxProfit = 0;
    int maxAssignment[items().size()];
    
    int assignment[items().size()];
    int remainingCapacity[m_numberOfBins];
    for(int i = 0; i < m_numberOfBins; ++i) {
        remainingCapacity[i] = sizes().at(i);
    }
    int highProfitSubsetProfits = 0;
    int highProfitSubsetSizes = 0;
    int highProfitSubsetCount = 0;
    int subsetCounter = 0;
    while(1) {
        std::cout << "Subset Nr.: " << subsetCounter++ << std::endl
                  << "Profit: " << highProfitSubsetProfits << std::endl
                  << subsetToString(highProfitSubset, m_itemProfitSizeOrder, m_firstMediumProfitOrderIndex).toStdString() << std::endl;
        
        bool foundHighProfitSubsetAssignment
            = firstSubsetAssignment(highProfitSubset, assignment, remainingCapacity, m_itemProfitSizeOrder, m_firstMediumProfitOrderIndex);
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
                                            m_itemProfitSizeOrder + m_firstMediumProfitOrderIndex,
                                            numberOfMediumProfitHighSizeItems).toStdString() << std::endl;
                int mediumProfitHighSizeRemainingCapacity[m_numberOfBins];
                memcpy(mediumProfitHighSizeRemainingCapacity, remainingCapacity, m_numberOfBins * sizeof(int));
                
                bool foundMediumProfitHighSizeSubsetAssignment
                    = firstSubsetAssignment(mediumProfitHighSizeSubset,
                                            assignment + m_firstMediumProfitOrderIndex,
                                            mediumProfitHighSizeRemainingCapacity,
                                            m_itemProfitSizeOrder + m_firstMediumProfitOrderIndex,
                                            numberOfMediumProfitHighSizeItems);
                if(foundMediumProfitHighSizeSubsetAssignment) {
                    std::cout << "Assignment following now:" << std::endl;
                }
                while(foundMediumProfitHighSizeSubsetAssignment) {
                    // Do stuff with the Assignment
                    std::cout << subsetAssignmentToString(assignment + m_firstMediumProfitOrderIndex,
                                                          m_itemProfitSizeOrder + m_firstMediumProfitOrderIndex,
                                                          numberOfMediumProfitHighSizeItems).toStdString()
                              << std::endl << std::endl;
                    int groupSubset[m_normalGroupNumber];
                    int groupSubsetCount = 0;
                    int groupSubsetSize = 0;
                    for(int i = 0; i < m_normalGroupNumber; ++i) {
                        groupSubset[i] = 0;
                    }
                    while(1) {
                        if(groupSubsetSize <= m_mediumProfitMediumSizeSubsetSizeLimit)
                        {
                            qDebug() << "This group is feasible";
                            // Do stuff with the group
                            int mediumProfitMediumSizeRemainingCapacity[m_numberOfBins];
                            memcpy(mediumProfitMediumSizeRemainingCapacity,
                                   mediumProfitHighSizeRemainingCapacity,
                                   m_numberOfBins * sizeof(int));
                            int mediumProfitMediumSizeAssignment[m_itemNumber];
                            memcpy(mediumProfitMediumSizeAssignment, assignment, m_itemNumber * sizeof(int));
                            
                            int mediumProfitMediumSizeProfit = 0;
                            if(packGroupItems(groupSubset,
                                              m_normalGroupSize,
                                              mediumProfitHighSizeRemainingCapacity,
                                              mediumProfitMediumSizeAssignment,
                                              m_normalGroupNumber,
                                              &mediumProfitMediumSizeProfit))
                            {
                                int totalProfit = highProfitSubsetProfits
                                                  + mediumProfitHighSizeSubsetProfits
                                                  + mediumProfitMediumSizeProfit;
                                qDebug() << "Total profit =" << totalProfit;
                                if(totalProfit > maxProfit) {
                                    qDebug() << "New max profit.";
                                    memcpy(maxAssignment, mediumProfitMediumSizeAssignment, m_itemNumber * sizeof(int));
                                    maxProfit = totalProfit;
                                }
                            }
                        }
                        
                        if(!nextGroupSubset(groupSubset,
                                            &groupSubsetCount,
                                            &groupSubsetSize,
                                            m_normalGroupSize,
                                            m_normalGroupCount,
                                            m_normalGroupNumber))
                        {
                            break;
                        }
                    }
                    
                    // Guess the next assignment of all medium profit item in subset mediumProfitHighSizeSubset.
                    foundMediumProfitHighSizeSubsetAssignment
                        = nextSubsetAssignment(mediumProfitHighSizeSubset,
                                               assignment + m_firstMediumProfitOrderIndex,
                                               mediumProfitHighSizeRemainingCapacity,
                                               m_itemProfitSizeOrder + m_firstMediumProfitOrderIndex,
                                               numberOfMediumProfitHighSizeItems);
                }
                
                
                if(!nextSubset(mediumProfitHighSizeSubset,
                               &mediumProfitHighSizeSubsetCount,
                               &mediumProfitHighSizeSubsetProfits,
                               &mediumProfitHighSizeSubsetSizes,
                               m_itemProfitSizeOrder + m_firstMediumProfitOrderIndex,
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
                                       m_itemProfitSizeOrder,
                                       m_firstMediumProfitOrderIndex);
        }
        
        // Guess next subset of all high profit items.
        if(!nextSubset(highProfitSubset,
                       &highProfitSubsetCount,
                       &highProfitSubsetProfits,
                       &highProfitSubsetSizes,
                       m_itemProfitSizeOrder,
                       m_firstMediumProfitOrderIndex,
                       m_highProfitSubsetSizeLimit))
        {
            break;
        }
    }
    
    m_assignment.clear();
    m_maximumProfit = maxProfit;
    for(int i = 0; i < m_itemNumber; ++i) {
        m_assignment.append(maxAssignment[i]);
    }
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
    
    delete m_itemProfitSizeOrder;
    m_itemProfitSizeOrder = itemProfitOrder();
    
    delete m_orderedRelativeSizes;
    m_orderedRelativeSizes = new double[m_itemNumber];
    
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
    qDebug() << "First medium profit order index =" << m_firstMediumProfitOrderIndex;
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
    for(; i < allItems.size(); ++i) {
        int itemNr = m_itemProfitSizeOrder[i];
    }
    
    sortItemSizeOrder(m_itemProfitSizeOrder + m_firstMediumProfitOrderIndex,
                      m_firstLowProfitOrderIndex - m_firstMediumProfitOrderIndex);
    qDebug() << "Sorted sizes:";
    for(int i = 0; i < items().size(); ++i) {
        m_orderedRelativeSizes[i] = ((double) allItems.at(m_itemProfitSizeOrder[i]).size()) / ((double) m_largestBinCapacity);
        std::cout << m_itemProfitSizeOrder[i] << ", ";
    }
    std::cout << std::endl;
}

void ImprovedApproximateMultipleKnapsack::groupMediumItems(int remainingArea)
{
    double relativeRemainingArea = ((double) remainingArea)/((double) m_largestBinCapacity);
    double minHighSize = m_rho * relativeRemainingArea/(2 * m_K * pow(log2(1.0/m_rho), 3.0));
    qDebug() << "Item has high size with at least" << minHighSize;
    double minMediumSize = pow(m_rho, 6.0) * relativeRemainingArea;
    qDebug() << "Item has medium size with at least" << minMediumSize;
    
    for(int i = 0; i < m_itemNumber; ++i) {
        qDebug() << "Order item" << i << "size" << m_orderedRelativeSizes[i];
    }
    
    qDebug() << "Medium items:";
    int item;
    for(item = m_firstMediumProfitOrderIndex; item < m_firstLowProfitOrderIndex; ++item) {
        double size = m_orderedRelativeSizes[item];
        if(size >= minHighSize) {
            // High size
        } else {
            break;
        }
    }
    
    m_firstMediumProfitMediumSizeOrderIndex = item;
    qDebug() << "First medium profit medium size order index =" << m_firstMediumProfitMediumSizeOrderIndex;
    
    // Finding out the number of groups of medium profit items with medium size.
    m_minR = floor(-log2(minHighSize));
    m_maxR = floor(-log2(minMediumSize));
    QVector<int> **mediumProfitMediumSizeGroupSize = (QVector<int> **) malloc((m_maxR - m_minR) * sizeof(void *));
    QVector<int> **mediumProfitMediumSizeGroupCount = (QVector<int> **) malloc((m_maxR - m_minR) * sizeof(void *));
    QVector<int> **mediumProfitMediumSizeGroupStart = (QVector<int> **) malloc((m_maxR - m_minR) * sizeof(void *));
    for(int i = 0; i < m_maxR - m_minR; ++i) {
        mediumProfitMediumSizeGroupCount[i] = 0;
        mediumProfitMediumSizeGroupSize[i] = 0;
        mediumProfitMediumSizeGroupStart[i] = 0;
    }
    
    int groupCountLimit = 0;
    int currentClass = -1;
    
    for(; item < m_firstLowProfitOrderIndex; ++item) {
        double size = m_orderedRelativeSizes[item];
        
        if(size >= minMediumSize) {
            // Medium size
            qDebug() << "Relative size:" << size;
            int itemClass = floor(-log2(size)) - m_minR;
            
            if(currentClass < itemClass) {
                while(currentClass < itemClass) {
                    currentClass++;
//                     m_mediumProfitMediumSizeGroupStart[currentClass] = item;
                }
                mediumProfitMediumSizeGroupCount[itemClass] = new QVector<int>();
                mediumProfitMediumSizeGroupSize[itemClass] = new QVector<int>();
                mediumProfitMediumSizeGroupStart[itemClass] = new QVector<int>();
                
                groupCountLimit = ceil(pow(2.0, itemClass + m_minR) * relativeRemainingArea 
                                       / m_K * pow(log2(1.0/m_rho), 3.0));
                qDebug() << "Group count limit is" << groupCountLimit;
                
                mediumProfitMediumSizeGroupSize[itemClass]
                    ->append(items().at(m_itemProfitSizeOrder[item]).size());
                mediumProfitMediumSizeGroupStart[itemClass]->append(item);
                mediumProfitMediumSizeGroupCount[itemClass]->append(1);
            }
            else {
                if(mediumProfitMediumSizeGroupCount[itemClass]->last() >= groupCountLimit) {
                    mediumProfitMediumSizeGroupSize[itemClass]
                        ->append(items().at(m_itemProfitSizeOrder[item]).size());
                    mediumProfitMediumSizeGroupStart[itemClass]->append(item);
                    mediumProfitMediumSizeGroupCount[itemClass]->append(1);
                }
                else {
                    (mediumProfitMediumSizeGroupCount[itemClass]->last())++;
                    qDebug() << "Last is now" << mediumProfitMediumSizeGroupCount[itemClass]->last();
                }
            }
            
            qDebug() << "Item" << m_itemProfitSizeOrder[item] << "in group" << itemClass;
        }
        else {
            break;
        }
    }
    
    m_normalGroupNumber = 0;
    m_extraGroupNumber = 0;
    for(int i = 0; i < m_maxR - m_minR; ++i) {
        if(mediumProfitMediumSizeGroupCount[i]) {
            int groupCount = mediumProfitMediumSizeGroupCount[i]->size();
            if(groupCount > 0) {
                m_normalGroupNumber += groupCount - 1;
                m_extraGroupNumber += 1;
            }
        }
    }
    
    delete m_normalGroupSize;
    delete m_normalGroupCount;
    delete m_normalGroupStart;
    delete m_extraGroupSize;
    delete m_extraGroupCount;
    delete m_extraGroupStart;
    m_normalGroupSize = new int[m_normalGroupNumber];
    m_normalGroupCount = new int[m_normalGroupNumber];
    m_normalGroupStart = new int[m_normalGroupNumber];
    m_extraGroupSize = new int[m_extraGroupNumber];
    m_extraGroupCount = new int[m_extraGroupNumber];
    m_extraGroupStart = new int[m_extraGroupNumber];
    
    int normalGroupIndex = 0;
    int extraGroupIndex = 0;
    for(int i = 0; i < m_maxR - m_minR; ++i) {
        if(mediumProfitMediumSizeGroupCount[i]
           && mediumProfitMediumSizeGroupCount[i]->size() > 0 )
        {
            m_extraGroupSize[extraGroupIndex] = mediumProfitMediumSizeGroupSize[i]->at(0);
            m_extraGroupCount[extraGroupIndex] = mediumProfitMediumSizeGroupCount[i]->at(0);
            m_extraGroupStart[extraGroupIndex] = mediumProfitMediumSizeGroupStart[i]->at(0);
            ++extraGroupIndex;
            for(int j = 1; j < mediumProfitMediumSizeGroupCount[i]->size(); ++j) {
                m_normalGroupSize[normalGroupIndex] = mediumProfitMediumSizeGroupSize[i]->at(j);
                m_normalGroupCount[normalGroupIndex] = mediumProfitMediumSizeGroupCount[i]->at(j);
                m_normalGroupStart[normalGroupIndex] = mediumProfitMediumSizeGroupStart[i]->at(j);
                ++normalGroupIndex;
            }
        }
    }
    
    for(int i = 0; i < m_maxR - m_minR; ++i) {
        delete mediumProfitMediumSizeGroupCount[i];
        delete mediumProfitMediumSizeGroupSize[i];
        delete mediumProfitMediumSizeGroupStart[i];
    }
    delete mediumProfitMediumSizeGroupCount;
    delete mediumProfitMediumSizeGroupSize;
    delete mediumProfitMediumSizeGroupStart;
    
    qDebug() << "Extra groups:";
    for(int i = 0; i < m_extraGroupNumber; ++i) {
        qDebug() << "Group" << i << "contains" << m_extraGroupCount[i] << "items"
                 << "and has size" << m_extraGroupSize[i];
    }
    
    qDebug() << "Normal groups:";
    for(int i = 0; i < m_normalGroupNumber; ++i) {
        qDebug() << "Group" << i << "contains" << m_normalGroupCount[i] << "items"
                 << "and has size" << m_normalGroupSize[i];
    }
    
    m_firstMediumProfitLowSizeOrderIndex = item;
    qDebug() << "First medium profit low size order index =" << m_firstMediumProfitLowSizeOrderIndex;
    
    m_mediumProfitLowSizeSize = 0;
    for(; item < m_firstLowProfitOrderIndex; ++item) {
        // Low size
        m_mediumProfitLowSizeSize++;
    }
    
    m_mediumProfitMediumSizeSubsetSizeLimit = m_largestBinCapacity * (m_rho * ((double) remainingArea)) / (4.0 * pow(log2(1 / m_rho), 2.0));
    m_mediumProfitMediumSizeSubsetSizeLimit -= m_mediumProfitLowSizeSize;
    qDebug() << "Medium profit medium size subset size limit:" << m_mediumProfitMediumSizeSubsetSizeLimit;
}

bool ImprovedApproximateMultipleKnapsack::nextSubset(bool *subset,
                                                     int *subsetSize, 
                                                     int *profit, 
                                                     int *size,
                                                     int *itemProfitSizeOrder,
                                                     int count,
                                                     int itemLimit)
{
    int i = 0;
    while(i < count) {
        ProfitItem item = items().at(itemProfitSizeOrder[i]);
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

bool ImprovedApproximateMultipleKnapsack::nextGroupSubset(int *subset,
                                                          int *subsetSize,
                                                          int *size,
                                                          int *groupSizes,
                                                          int *groupCounts,
                                                          int count)
{
    int i = 0;
    while(i < count) {
        if(subset[i] < groupCounts[i]) {
            (*subsetSize)++;
            subset[i]++;
            (*size) += groupSizes[i];
            break;
        }
        else {
            (*subsetSize) -= subset[i];
            (*size) -= subset[i] * groupSizes[i];
            subset[i] = 0;
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
                                                            int *itemProfitSizeOrder,
                                                            int count)
{
    QString result;
    for(int i = 0; i < count; ++i) {
        result += QString("%1|").arg(itemProfitSizeOrder[i], 5);
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

QString ImprovedApproximateMultipleKnapsack::subsetAssignmentToString(int *assignment,
                                                                      int *itemProfitSizeOrder,
                                                                      int count)
{
    QString result;
    for(int i = 0; i < count; ++i) {
        result += QString("%1|").arg(itemProfitSizeOrder[i], 5);
    }
    result += "\n";
    for(int i = 0; i < count; ++i) {
        result += QString("%1|").arg(assignment[i], 5);
    }
    return result;
}

bool ImprovedApproximateMultipleKnapsack::firstSubsetAssignment(bool* subset, 
                                                                int* assignment, 
                                                                int* remainingCapacity,
                                                                int *itemProfitSizeOrder,
                                                                int count)
{
    if(itemProfitSizeOrder == m_itemProfitSizeOrder) {
        // If we create a new assignment, reset the remaining capacities.
        for(int bin = 0; bin < m_numberOfBins; ++bin) {
            remainingCapacity[bin] = m_sortedSizes[bin];
        }
    }
    
    int item;
    for(item = 0; item < count; ++item) {
        // Working with absolute item numbers here.
        if(subset[item] == true) {
            assignment[item] = 0;
            remainingCapacity[0] -= items().at(itemProfitSizeOrder[item]).size();
        }
        else {
            assignment[item] = -1;
        }
    }
    
    if(itemProfitSizeOrder == m_itemProfitSizeOrder) {
        for(; item < m_itemNumber; ++item) {
            assignment[item] = -1;
        }
    }
    
    if(remainingCapacity[0] < 0) {
        return nextSubsetAssignment(subset, assignment, remainingCapacity, itemProfitSizeOrder, count);
    }
    else {
        return true;
    }
}

bool ImprovedApproximateMultipleKnapsack::nextSubsetAssignment(bool* subset, 
                                                               int* assignment, 
                                                               int* remainingCapacity,
                                                               int *itemProfitSizeOrder,
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
        int itemSize = items().at(itemProfitSizeOrder[i]).size();
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

bool ImprovedApproximateMultipleKnapsack::packGroupItems(int *groupCount,
                                                         int *groupSizes,
                                                         int *remainingCapacities,
                                                         int *assignment,
                                                         int count, 
                                                         int *profit)
{
    int *groupPacking = findGroupedPacking<int>(groupCount,
                                           groupSizes,
                                           remainingCapacities,
                                           count,
                                           m_numberOfBins);
    
    if(groupPacking != 0) {
        for(int bin = 0; bin < m_numberOfBins; ++bin) {
            for(int i = 0; i < count; ++i) {
                int numberOfCurrentSizeItems = groupPacking[bin * count + i];
                
                float currentSize = groupSizes[i];
                for(; numberOfCurrentSizeItems > 0; numberOfCurrentSizeItems--)
                {
                    // FIXME: The following line uses a class member.
                    int itemIndex = m_normalGroupStart[i] + numberOfCurrentSizeItems;
                    assignment[itemIndex] = bin;
                    qDebug() << "Item " << m_itemProfitSizeOrder[itemIndex] << "in bin" << bin;
                    remainingCapacities[bin] -= items().at(m_itemProfitSizeOrder[itemIndex]).size();
                    (*profit) += items().at(m_itemProfitSizeOrder[itemIndex]).profit();
                    qDebug() << "Bin space now:" << remainingCapacities[bin];
                }
            }
        }
        
        qDebug() << "Found sufficient fill!";
        delete groupPacking;
        return true;
    }
    else {
        return false;
    }
}
                                                                          

