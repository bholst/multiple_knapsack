//
// Copyright (C) 2011  Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <cmath>

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
    
    GreedyMultipleKnapsack greedy;
    greedy.setApproximationLevel(m_rho/2.0);
    greedy.setItems(items());
    greedy.setSizes(sortedSizes);
    
    int approximateMaximum = greedy.maximumProfit();
    
    groupItems(approximateMaximum);
    
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
    
    QVector<ProfitItem> allItems = items();
    
    for(int item = 0; item < allItems.size(); ++item) {
        if(allItems[item].profit() >= minHighProfit) {
            m_highProfitItems.insert(item);
            qDebug() << "Item" << item << "has high profit.";
        }
        else if(allItems[item].profit() >= minMediumProfit) {
            m_mediumProfitItems.insert(item);
            qDebug() << "Item" << item << "has medium profit.";
            
            // The medium profit items are categorized later in size categories,
            // because the remaining capacity is needed for that.
        }
        else {
            m_lowProfitItems.insert(item);
            qDebug() << "Item" << item << "has low profit.";
        }
    }
}

