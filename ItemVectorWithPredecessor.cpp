//
// Copyright (C) 2011  Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>
#include <QtCore/QVector>

// Self
#include "ItemVectorWithPredecessor.h"

ItemVectorWithPredecessor::ItemVectorWithPredecessor(int numberOfItemSizes)
    : m_predecessor(0),
      m_numberOfItemSizes(numberOfItemSizes)
{
    m_itemCount = new int[m_numberOfItemSizes];
    for(int i = 0; i < m_numberOfItemSizes; ++i) {
        m_itemCount[i] = 0;
    }
}

ItemVectorWithPredecessor::ItemVectorWithPredecessor(const ItemVector& add, ItemVectorWithPredecessor *predecessor)
    : m_predecessor(predecessor),
      m_numberOfItemSizes(predecessor->m_numberOfItemSizes)
{
    m_itemCount = new int[m_numberOfItemSizes];
    for(int i = 0; i < m_numberOfItemSizes; ++i) {
        m_itemCount[i] = predecessor->m_itemCount[i] + add.itemCount(i);
    }
}

ItemVectorWithPredecessor::~ItemVectorWithPredecessor()
{
    delete m_itemCount;
}

bool ItemVectorWithPredecessor::isFull(const QVector< int >& allItems)
{
    if(allItems.size() != m_numberOfItemSizes) {
        return false;
    }
    for(int i = 0; i < m_numberOfItemSizes; ++i) {
        if(allItems[i] > m_itemCount[i]) {
            return false;
        }
    }
    
    return true;
}

int ItemVectorWithPredecessor::itemCount(int sizeNumber) const
{
    if(sizeNumber >= m_numberOfItemSizes) {
        qDebug() << "Error: There are only" << m_numberOfItemSizes << "item sizes";
        return -1;
    }
    return m_itemCount[sizeNumber];
}

ItemVectorWithPredecessor* ItemVectorWithPredecessor::predecessor()
{
    return m_predecessor;
}
