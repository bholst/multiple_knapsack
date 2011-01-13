//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// Self
#include "FloatItemWithIndex.h"

FloatItemWithIndex::FloatItemWithIndex()
    : FloatItem()
{
}

FloatItemWithIndex::FloatItemWithIndex(int index, const FloatItem& item)
    : FloatItem(item),
      m_index(index)
{

}

int FloatItemWithIndex::index() const
{
    return m_index;
}

void FloatItemWithIndex::setIndex(int index)
{
    m_index = index;
}

bool FloatItemWithIndex::operator<(const FloatItemWithIndex &other ) const
{
    return size() < other.size();
}