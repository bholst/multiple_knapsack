//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>
#include <QtCore/QtAlgorithms>

// Project
#include "Item.h"
#include "ApproximatedKnapsack.h"

// Self
#include "MultipleKnapsack.h"

using namespace std;

MultipleKnapsack::MultipleKnapsack()
    : m_totalSizeDirty(true),
      m_sizesVectorDirty(true),
      m_approximationLevel(0.5),
      m_dirty(true)
{

}

MultipleKnapsack::~MultipleKnapsack()
{

}

double MultipleKnapsack::approximationLevel() const
{
    return m_approximationLevel;
}

void MultipleKnapsack::setApproximationLevel(double approximationLevel)
{
    if(approximationLevel <= 0) {
        qDebug() << "Invalid approximationLevel"; 
        return;
    }
    m_dirty = true;
    m_approximationLevel = approximationLevel;
}

QVector<Item> MultipleKnapsack::items() const
{
    return m_items;
}

void MultipleKnapsack::setItems(QVector< Item > items)
{
    m_dirty = true;
    m_items = items;
}

QList<int> MultipleKnapsack::sizes() const
{
    return m_sizes;
}

void MultipleKnapsack::setSizes(QList< int > sizes)
{
    m_sizesVectorDirty = true;
    m_totalSizeDirty = true;
    update();
    m_sizes = sizes;
}

QVector< int > MultipleKnapsack::sizesVector()
{
    if(m_sizesVectorDirty) {
        // Make sizes vector
        QList<int> allSizes = sizes();
        QList<int>::iterator sizesEnd = allSizes.end();
        for(QList<int>::iterator it = allSizes.begin();
            it != sizesEnd;
            ++it)
        {
            m_sizesVector.push_back(*it);
        }
    
        m_sizesVectorDirty = false;
    }
    
    return m_sizesVector;
}

int MultipleKnapsack::totalSize()
{
    if(m_totalSizeDirty) {
        m_totalSize = 0;
        QList<int> allSizes = sizes();
        QList<int>::iterator sizesEnd = allSizes.end();
        for(QList<int>::iterator it = allSizes.begin();
            it != sizesEnd;
            ++it)
        {
            m_totalSize += *it;
        }
        
        m_totalSizeDirty = false;
    }
    return m_totalSize;
}

int MultipleKnapsack::maximumProfit()
{
    if(m_dirty) {
        recalculateValues();
    }

    return m_maximumProfit;
}

void MultipleKnapsack::recalculateValues()
{
    // Do the calculation stuff
    QVector<Item> remainingItems = m_items;
    qSort(m_sizes);
    QList<int>::iterator end = m_sizes.end();
    m_maximumProfit = 0;
    for(QList<int>::iterator it = m_sizes.begin();
        it != end;
        ++it)
    {
        ApproximatedKnapsack knapsack;
        knapsack.setSize(*it);
        knapsack.setApproximationLevel(m_approximationLevel);
        knapsack.setItems(remainingItems);
        
        m_maximumProfit += knapsack.maximumProfit();
        QSet<int> maximumProfitItems = knapsack.maximumProfitItems();
        int itemNumber = remainingItems.size();
        QVector<Item> newRemainingItems;
        for(int i = 0; i < itemNumber; ++i) {
            if(maximumProfitItems.find(i) == maximumProfitItems.end()) {
                newRemainingItems.push_back(remainingItems[i]);
            }
        }
        
        remainingItems = newRemainingItems;
    }
    m_dirty = false;
}

void MultipleKnapsack::update()
{
    setDirty(true);
}

bool MultipleKnapsack::dirty()
{
    return m_dirty;
}

void MultipleKnapsack::setDirty(bool dirty)
{
    m_dirty = dirty;
}
