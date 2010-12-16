//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#include "ItemWithIndex.h"

ItemWithIndex::ItemWithIndex()
    : Item()
{
}

ItemWithIndex::ItemWithIndex(int index, Item item)
    : Item(item),
      m_index(index)
{
}

int ItemWithIndex::index() const
{
    return m_index;
}

void ItemWithIndex::setIndex(int index)
{
    m_index = index;
}



