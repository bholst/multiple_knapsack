//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#include "SmallBinPacking.h"

SmallBinPacking::SmallBinPacking()
    : m_maximumNumberOfBins(10),
      m_dirty(true),
      m_minimumNumberOfBins(-1)
{

}

QVector< Item > SmallBinPacking::items() const
{
    return m_items;
}

void SmallBinPacking::setItems(const QVector< Item >& items)
{
    m_dirty = true;
    m_items = items;
}

int SmallBinPacking::maximumNumberOfBins() const
{
    return m_maximumNumberOfBins;
}

void SmallBinPacking::setMaximumNumberOfBins(int maximumNumberOfBins)
{
    if(maximumNumberOfBins < m_minimumNumberOfBins
       || m_minimumNumberOfBins < 0)
    {
        m_dirty = true;
    }
    m_maximumNumberOfBins = maximumNumberOfBins;
}

int SmallBinPacking::minimumNumberOfBins()
{
    if(m_dirty) {
        recalculateValues();
    }
    return m_minimumNumberOfBins;
}

void SmallBinPacking::recalculateValues()
{
    int K = 8;
    // First calculate the sets of small, medium sized and large items.
}