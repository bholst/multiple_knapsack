//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// Self
#include "ProfitItemWithIndex.h"

ProfitItemWithIndex::ProfitItemWithIndex()
    : ProfitItem()
{
}

ProfitItemWithIndex::ProfitItemWithIndex(int index,
                                         ProfitItem item)
    : ProfitItem(item),
      m_index(index)
{
}

int ProfitItemWithIndex::index() const
{
    return m_index;
}

void ProfitItemWithIndex::setIndex(int index)
{
    m_index = index;
}

