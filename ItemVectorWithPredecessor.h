//
// Copyright (C) 2011  Bastian Holst <bastianholst@gmx.de>
//

#ifndef ITEMVECTORWITHPREDECESSOR_H
#define ITEMVECTORWITHPREDECESSOR_H

#include "ItemVector.h"

template <int numberOfItemSizes>
class ItemVectorWithPredecessor
{

public:
    ItemVectorWithPredecessor();
    ItemVectorWithPredecessor(const ItemVector<numberOfItemSizes>& add, ItemVectorWithPredecessor<numberOfItemSizes> *predecessor);
    
private:
    ItemVectorWithPredecessor<numberOfItemSizes> *m_predecessor;
    int m_itemCount[numberOfItemSizes];
};

#endif // ITEMVECTORWITHPREDECESSOR_H
