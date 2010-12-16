//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QtAlgorithms>

// Project
#include "Item.h"
#include "RelativeItemSize.h"

// Self
#include "FillRemainingMultipleKnapsack.h"

using namespace std;

FillRemainingMultipleKnapsack::FillRemainingMultipleKnapsack()
    : MultipleKnapsack(),
      m_remainingSizesDirty(true),
      m_remainingSizeDirty(true)
{

}

void FillRemainingMultipleKnapsack::recalculateValues()
{
    QList<RelativeItemSize> relativeSizes;
    
    QVector<Item> itemVector = items();
    QSet<int>::const_iterator itemsToUseEnd = m_itemsToUse.constEnd();
    for(QSet<int>::const_iterator it = m_itemsToUse.constBegin(); it != itemsToUseEnd; ++it) {
        if(m_startAssignment[*it] < 0) {
            RelativeItemSize relativeSize(*it, itemVector[*it]);
            relativeSizes.push_back(relativeSize);
        }
    }
    
    qSort(relativeSizes);
    
    int completeSize = remainingSize();
    QList<RelativeItemSize>::const_iterator endIterator = relativeSizes.constEnd();
    QList<RelativeItemSize>::const_iterator it = relativeSizes.constBegin();
    
    m_assignment = m_startAssignment;
    QList<int> sizesStillAvailable = remainingSizes();
    int currentBin = 0;
    
    m_maximumProfit = startProfit();
    qDebug() << "Start profit is" << m_maximumProfit;
    while((it != endIterator) && completeSize > 0) {
        int currentItemId = it->id();
        int currentItemSize = itemVector[currentItemId].size();
        completeSize -= currentItemSize;
        if(sizesStillAvailable[currentBin] >= currentItemSize) {
            m_assignment[currentItemId] = currentBin;
            m_maximumProfit += itemVector[currentItemId].profit();
            sizesStillAvailable[currentBin] = sizesStillAvailable[currentBin] - currentItemSize;
        }
        else {
            currentBin++;
        }

        ++it;
    }

    setDirty(false);
}

QVector<int> FillRemainingMultipleKnapsack::startAssignment() const
{
    return m_startAssignment;
}

void FillRemainingMultipleKnapsack::setStartAssignment(const QVector< int >& assignment)
{
    update();
    m_startAssignment = assignment;
}

QList< int > FillRemainingMultipleKnapsack::remainingSizes()
{
    if(m_remainingSizesDirty) {
        recalculateRemainingSizes();
    }
    
    return m_remainingSizes;
}

int FillRemainingMultipleKnapsack::startProfit()
{
    if(m_remainingSizesDirty) {
        recalculateRemainingSizes();
    }
    
    return m_startProfit;
}

void FillRemainingMultipleKnapsack::recalculateRemainingSizes()
{
    m_remainingSizes = sizes();
    m_startProfit = 0;

    QVector<Item> allItems = items();
    int numberOfItems = m_startAssignment.size();
    for(int i = 0; i < numberOfItems; ++i)
    {
        if(m_startAssignment[i] < 0) {
            continue;
        }
        m_remainingSizes[m_startAssignment[i]] = m_remainingSizes[m_startAssignment[i]] - allItems[i].size();
        m_startProfit += allItems[i].profit();
    }
    m_remainingSizesDirty = false;
}


int FillRemainingMultipleKnapsack::remainingSize()
{
    if(m_remainingSizeDirty) {
        m_remainingSize = 0;
        QList<int> sizes = remainingSizes();
        
        QList<int>::const_iterator endIterator = sizes.constEnd();
        for(QList<int>::const_iterator it = sizes.constBegin();
            it != endIterator;
            ++it)
        {
            m_remainingSize += *it;
        }
        
        m_remainingSizeDirty = false;
    }
    
    return m_remainingSize;
}

void FillRemainingMultipleKnapsack::update()
{
    MultipleKnapsack::update();
    m_remainingSizeDirty = true;
    m_remainingSizesDirty = true;
}

QSet< int > FillRemainingMultipleKnapsack::itemsToUse() const
{
    return m_itemsToUse;
}

void FillRemainingMultipleKnapsack::setItemsToUse(const QSet< int > itemsToUse)
{
    m_itemsToUse = itemsToUse;
}

