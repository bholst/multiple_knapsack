//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>

// Self
#include "FloatItem.h"

FloatItem::FloatItem()
    : m_size(0.0)
{
}

void FloatItem::setSize(float size)
{
    if(size > 1.0
       || size < 0.0)
    {
        m_size = 0.0;
        qDebug() << "The value for the size of a float item must be in [0, 1].";
    }
    m_size = size;
}

float FloatItem::size() const
{
    return m_size;
}