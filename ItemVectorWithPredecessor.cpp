//
// Copyright (C) 2011  Bastian Holst <bastianholst@gmx.de>
//

#include "ItemVectorWithPredecessor.h"

template <int numberOfItemSizes>
ItemVectorWithPredecessor<numberOfItemSizes>::ItemVectorWithPredecessor()
    : m_predecessor(0)
{
    for(int i = 0; i < numberOfItemSizes; ++i) {
        m_itemCount[i] = 0;
    }
}

template <int numberOfItemSizes>
ItemVectorWithPredecessor<numberOfItemSizes>::ItemVectorWithPredecessor(const ItemVector<numberOfItemSizes>& add, ItemVectorWithPredecessor<numberOfItemSizes> *predecessor)
    : m_predecessor(predecessor)
{
    for(int i = 0; i < numberOfItemSizes; ++i) {
        m_itemCount[i] = predecessor->m_itemCount[i] + add.itemCount(i);
    }
}

