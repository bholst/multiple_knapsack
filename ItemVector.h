//
// Copyright (C) 2011  Bastian Holst <bastianholst@gmx.de>
//

#ifndef ITEMVECTOR_H
#define ITEMVECTOR_H

template <int numberOfItemSizes>
class ItemVector
{

public:
    ItemVector();
    void setItemCount(int sizeNumber, int itemNumber);
    int itemCount(int sizeNumber);
    
private:
    int m_itemCount[numberOfItemSizes];
};

#endif // ITEMVECTOR_H
