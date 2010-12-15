//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef ITEMWITHINDEX_H
#define ITEMWITHINDEX_H

// Project
#include "Item.h"

class ItemWithIndex : public Item
{
public:
    ItemWithIndex();
    ItemWithIndex(int index, Item item);
    
    void setIndex(int index);
    int index() const;
    
private:
    int m_index;
};

#endif // ITEMWITHINDEX_H
