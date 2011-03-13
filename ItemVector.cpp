//
// Copyright (C) 2011  Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>
#include <QtCore/QString>

// Self
#include "ItemVector.h"

ItemVector::ItemVector(int numberOfItemSizes)
    : m_numberOfItemSizes(numberOfItemSizes)
{
    m_itemCount = new int[numberOfItemSizes];
    for(int i = 0; i < numberOfItemSizes; ++i) {
        m_itemCount[i] = 0;
    }
}

ItemVector::~ItemVector()
{
    delete m_itemCount;
}

void ItemVector::setItemCount(int sizeNumber, int itemCount)
{
    if(sizeNumber >= m_numberOfItemSizes) {
        qDebug() << "Error: There are only" << m_numberOfItemSizes << "item sizes";
        return;
    }
    
    m_itemCount[sizeNumber] = itemCount;
}

int ItemVector::itemCount(int sizeNumber) const
{
    if(sizeNumber >= m_numberOfItemSizes) {
        qDebug() << "Error: There are only" << m_numberOfItemSizes << "item sizes";
        return -1;
    }
    
    return m_itemCount[sizeNumber];
}

QString ItemVector::toString() const
{
    QString result;
    for(int i = 0; i < m_numberOfItemSizes; i++) {
        result.append(QString("Of size %1 we have %2 items.").arg(i).arg(m_itemCount[i]));
    }
    
    return result;
}

