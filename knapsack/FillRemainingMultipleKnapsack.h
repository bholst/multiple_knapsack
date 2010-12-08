//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef FILLREMAININGMULTIPLEKNAPSACK_H
#define FILLREMAININGMULTIPLEKNAPSACK_H

// STD
#include <vector>

// Project
#include "MultipleKnapsack.h"

class FillRemainingMultipleKnapsack : public MultipleKnapsack
{

public:
    FillRemainingMultipleKnapsack();
    
    void setStartAssignment(const std::vector<int>& assignment);
    std::vector<int> startAssignment() const;
    
    std::vector<int> assignment();

    std::vector<int> remainingSizes();
    int remainingSize();
    
    virtual void update();
    
private:
    virtual void recalculateValues();
    
    std::vector<int> m_startAssignment;
    std::vector<int> m_remainingSizes;
    bool m_remainingSizesDirty;
    int m_remainingSize;
    bool m_remainingSizeDirty;
    std::vector<int> m_assignment;
};

#endif // FILLREMAININGMULTIPLEKNAPSACK_H
