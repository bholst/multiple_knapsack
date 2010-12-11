//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

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
    list<RelativeItemSize> relativeSizes;
    
    QVector<Item> itemVector = items();
    int itemNumber = itemVector.size();
    for(int i = 0; i < itemNumber; ++i) {
        RelativeItemSize relativeSize(i, itemVector[i]);
        relativeSizes.push_back(relativeSize);
    };
    
    relativeSizes.sort();
    
    int completeSize = remainingSize();
    list<RelativeItemSize>::iterator endIterator = relativeSizes.end();
    list<RelativeItemSize>::iterator it = relativeSizes.begin();
    
    m_assignment = m_startAssignment;
    QVector<int> sizesStillAvailable = remainingSizes();
    int currentBin = 0;
    
    while((it != endIterator) && completeSize > 0) {
        int currentItemId = it->id();
        int currentItemSize = itemVector[currentItemId].size();
        completeSize -= currentItemSize;
        if(sizesStillAvailable[currentBin] >= currentItemSize) {
            m_assignment[currentItemId] = currentBin;
            sizesStillAvailable[currentBin] = sizesStillAvailable[currentBin] - currentItemSize;
        }
        else {
            currentBin++;
        }

        ++it;
    }
    
    m_maximumProfit = 0;
    for(int i = 0; i < itemNumber; ++i) {
        if(m_assignment[i] >= 0) {
            m_maximumProfit += itemVector[i].profit();
        }
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

QVector< int > FillRemainingMultipleKnapsack::assignment()
{
    if(dirty()) {
        recalculateValues();
    }
    return m_assignment;
}

QVector< int > FillRemainingMultipleKnapsack::remainingSizes()
{
    if(m_remainingSizesDirty) {
        m_remainingSizes = sizesVector();

        QVector<Item> allItems = items();
        int numberOfItems = m_startAssignment.size();
        for(int i = 0; i < numberOfItems; ++i)
        {
            if(m_startAssignment[i] < 0) {
                continue;
            }
            m_remainingSizes[m_startAssignment[i]] = m_remainingSizes[m_startAssignment[i]] - allItems[i].size();
        }
        m_remainingSizesDirty = false;
    }
    
    return m_remainingSizes;
}

int FillRemainingMultipleKnapsack::remainingSize()
{
    if(m_remainingSizeDirty) {
        m_remainingSize = 0;
        QVector<int> sizes = remainingSizes();
        
        QVector<int>::iterator endIterator = sizes.end();
        for(QVector<int>::iterator it = sizes.begin();
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

