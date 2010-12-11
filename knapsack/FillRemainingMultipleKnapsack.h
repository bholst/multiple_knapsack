//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef FILLREMAININGMULTIPLEKNAPSACK_H
#define FILLREMAININGMULTIPLEKNAPSACK_H

// STD
#include <QtCore/QVector>

// Project
#include "MultipleKnapsack.h"

class FillRemainingMultipleKnapsack : public MultipleKnapsack
{

public:
    FillRemainingMultipleKnapsack();
    
    void setStartAssignment(const QVector<int>& assignment);
    QVector<int> startAssignment() const;
    
    QVector<int> assignment();

    QVector<int> remainingSizes();
    int remainingSize();
    
    virtual void update();
    
private:
    virtual void recalculateValues();
    
    QVector<int> m_startAssignment;
    QVector<int> m_remainingSizes;
    bool m_remainingSizesDirty;
    int m_remainingSize;
    bool m_remainingSizeDirty;
    QVector<int> m_assignment;
};

#endif // FILLREMAININGMULTIPLEKNAPSACK_H
