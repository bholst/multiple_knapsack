//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef SMALLBINPACKING_H
#define SMALLBINPACKING_H

#include <QtCore/QVector>
#include "Item.h"

class SmallBinPacking
{
public:
    SmallBinPacking();

    QVector<Item> items() const;
    void setItems(const QVector<Item>& items);
    
    int maximumNumberOfBins() const;
    /**
     * Set the maximum number of bins.
     * The default value is 10.
     */
    void setMaximumNumberOfBins(int maximumNumberOfBins);
    
    /**
     * Returns the number of bins that are needed to pack the items into.
     * Returns -1 if there is no assignment for maximumNumberOfBins.
     */
    int minimumNumberOfBins();
    
private:
    void recalculateValues();

    int m_maximumNumberOfBins;
    QVector<Item> m_items;
    
    bool m_dirty;
    int m_minimumNumberOfBins;
};

#endif // SMALLBINPACKING_H
