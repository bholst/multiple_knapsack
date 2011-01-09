/*
//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//
*/

#ifndef PROFITITEMWITHINDEX_H
#define PROFITITEMWITHINDEX_H

#include "ProfitItem.h"


class ProfitItemWithIndex : public ProfitItem
{
public:
    ProfitItemWithIndex();
    ProfitItemWithIndex(int index, ProfitItem item);
    
    int index() const;
    void setIndex(int index);
    
private:
    int m_index;
};

#endif // PROFITITEMWITHINDEX_H
