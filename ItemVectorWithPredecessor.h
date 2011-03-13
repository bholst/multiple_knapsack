//
// Copyright (C) 2011  Bastian Holst <bastianholst@gmx.de>
//

#ifndef ITEMVECTORWITHPREDECESSOR_H
#define ITEMVECTORWITHPREDECESSOR_H

// Qt
#include <QtCore/QVector>

// Project
#include "ItemVector.h"

class ItemVectorWithPredecessor
{

public:
    explicit ItemVectorWithPredecessor(int numberOfItemSizes);
    ItemVectorWithPredecessor(const ItemVector& add, ItemVectorWithPredecessor *predecessor);
    
    ~ItemVectorWithPredecessor();
    
    bool isFull(const QVector<int>& allItems);
    int itemCount(int sizeNumber) const;
    ItemVectorWithPredecessor *predecessor();
    
private:
    ItemVectorWithPredecessor *m_predecessor;
    int m_numberOfItemSizes;
    int *m_itemCount;
};

#endif // ITEMVECTORWITHPREDECESSOR_H
