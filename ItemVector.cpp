//
// Copyright (C) 2011  Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>

// Self
#include "ItemVector.h"

template <int numberOfItemSizes>
ItemVector<numberOfItemSizes>::ItemVector()
{

}

template <int numberOfItemSizes>
void ItemVector<numberOfItemSizes>::setItemCount(int sizeNumber, int itemCount)
{
    if(sizeNumber >= numberOfItemSizes) {
        qDebug() << "Error: There are only" << numberOfItemSizes << "item sizes";
        return;
    }
    
    m_itemCount[sizeNumber] = itemCount;
}

template <int numberOfItemSizes>
int ItemVector<numberOfItemSizes>::itemCount(int sizeNumber)
{
    if(sizeNumber >= numberOfItemSizes) {
        qDebug() << "Error: There are only" << numberOfItemSizes << "item sizes";
        return;
    }
    
    return m_itemCount[sizeNumber];
}

