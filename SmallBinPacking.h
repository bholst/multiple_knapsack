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
    ~SmallBinPacking();

    QVector<FloatItem> items() const;
    void setItems(const QVector<FloatItem>& items);
    
    int maximumNumberOfBins() const;
    
    /**
     * Returns the number of bins that are needed to pack the items into.
     * Returns -1 if there is no assignment for maximumNumberOfBins.
     */
    int minimumNumberOfBins();
    
    int *assignment();
    
private:
    virtual void recalculateValues();
    bool handlePreassignment(int *preassignment, int numberOfBins,
                             QVector<float> normalItemSizes,
                             QVector<int> normalItemNumbers,
                             QMultiMap<float, int> grouping,
                             QMap<float, int> extraItems,
                             QSet<int> smallItems,
                             int *resultingAssignment,
                             float *resultingRemainingCapacities);
    bool packExtraItems(QMap<float, int> extraItems,
                        QMultiMap<float, int> grouping,
                        int *assignment,
                        float *remainingCapacities,
                        int numberOfBins);
    bool packSmallItems(QSet<int> smallItems,
                        int *assignment,
                        float *remainingCapacities,
                        int numberOfBins);

    float m_delta;
    int m_K;
    QVector<FloatItem> m_items;
    
    bool m_dirty;
    int m_minimumNumberOfBins;
    int *m_assignment;
};

#endif // SMALLBINPACKING_H
