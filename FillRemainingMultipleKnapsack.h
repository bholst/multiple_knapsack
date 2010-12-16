//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef FILLREMAININGMULTIPLEKNAPSACK_H
#define FILLREMAININGMULTIPLEKNAPSACK_H

// STD
#include <QtCore/QVector>
#include <QtCore/QSet>

// Project
#include "MultipleKnapsack.h"

class FillRemainingMultipleKnapsack : public MultipleKnapsack
{

public:
    FillRemainingMultipleKnapsack();
    
    void setStartAssignment(const QVector<int>& assignment);
    QVector<int> startAssignment() const;

    QList<int> remainingSizes();
    int startProfit();
    int remainingSize();
    
    QSet<int> itemsToUse() const;
    void setItemsToUse(const QSet<int> itemsToUse);
    
    virtual void update();
    
private:
    virtual void recalculateValues();
    
    void recalculateRemainingSizes();
    
    QVector<int> m_startAssignment;
    QList<int> m_remainingSizes;
    int m_startProfit;
    bool m_remainingSizesDirty;
    int m_remainingSize;
    bool m_remainingSizeDirty;
    QSet<int> m_itemsToUse;
};

#endif // FILLREMAININGMULTIPLEKNAPSACK_H
