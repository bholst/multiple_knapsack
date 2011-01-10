//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>
#include <QtCore/QtAlgorithms>

// Project
#include "ProfitItemWithIndex.h"
#include "ApproximatedKnapsack.h"

// Self
#include "MultipleKnapsack.h"

using namespace std;

MultipleKnapsack::MultipleKnapsack()
    : m_totalSizeDirty(true),
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

QVector<ProfitItem> MultipleKnapsack::items() const
{
    return m_items;
}

void MultipleKnapsack::setItems(QVector< ProfitItem > items)
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
    m_totalSizeDirty = true;
    update();
    m_sizes = sizes;
}

int MultipleKnapsack::totalSize()
{
    if(m_totalSizeDirty) {
        m_totalSize = 0;
        QList<int> allSizes = sizes();
        QList<int>::const_iterator sizesEnd = allSizes.constEnd();
        for(QList<int>::const_iterator it = allSizes.constBegin();
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
    QVector<ProfitItemWithIndex> remainingItems;
    for(int i = 0; i < m_items.size(); ++i) {
        remainingItems.push_back(ProfitItemWithIndex(i, m_items[i]));
    }
    
    m_assignment = QVector<int>(items().size(), -1);

    qSort(m_sizes);
    m_maximumProfit = 0;
    for(int i = 0; i < m_sizes.size(); ++i) {
        QVector<ProfitItem> itemsForKnapsack;
        for(int j = 0; j < remainingItems.size(); ++j) {
            itemsForKnapsack.append(remainingItems[j]);
        }
        ApproximatedKnapsack knapsack;
        knapsack.setSize(m_sizes[i]);
        knapsack.setApproximationLevel(m_approximationLevel);
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

QVector< int > MultipleKnapsack::assignment()
{
    if(dirty()) {
        recalculateValues();
    }
    return m_assignment;
}
