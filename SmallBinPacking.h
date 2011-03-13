//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef SMALLBINPACKING_H
#define SMALLBINPACKING_H

#include <QtCore/QVector>
#include <QtCore/QMultiMap>

class FloatItem;

class SmallBinPacking
{
public:
    SmallBinPacking();

    QVector<FloatItem> items() const;
    void setItems(const QVector<FloatItem>& items);
    
    int maximumNumberOfBins() const;
    
    /**
     * Returns the number of bins that are needed to pack the items into.
     * Returns -1 if there is no assignment for maximumNumberOfBins.
     */
    int minimumNumberOfBins();
    
private:
    virtual void recalculateValues();
    bool handlePreassignment(int *preassignment, int numberOfBins,
                             QVector<float> normalItemSizes,
                             QVector<int> normalItemNumbers,
                             QMultiMap<float, int> grouping,
                             int *resultingAssignment,
                             float *resultingRemainingCapacities);

    float m_delta;
    int m_K;
    QVector<FloatItem> m_items;
    
    bool m_dirty;
    int m_minimumNumberOfBins;
};

#endif // SMALLBINPACKING_H
