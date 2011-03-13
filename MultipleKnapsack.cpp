//
// Copyright (C) 2011  Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>

// Project
#include "ProfitItem.h"

// Self
#include "MultipleKnapsack.h"

MultipleKnapsack::MultipleKnapsack()
    : m_approximationLevel(0.5),
      m_dirty(true),
      m_totalSizeDirty(true)
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
