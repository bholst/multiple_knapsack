//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// STL
#include <iostream>

// Self
#include "Item.h"

using namespace std;

Item::Item()
    : m_size(0),
      m_id(-1)
{

}

Item::~Item()
{

}

int Item::size() const
{
    return m_size;
}

void Item::setSize(int size)
{
    if(size < 0) {
        cerr << "Only sizes >= 0 are allowed";
        m_size = 0;
    }
    else {
        m_size = size;
    }
}

int Item::id() const
{
    return m_id;
}

void Item::setId(int id)
{
    m_id = id;
}