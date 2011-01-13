//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef FLOATITEMWITHINDEX_H
#define FLOATITEMWITHINDEX_H

#include "FloatItem.h"


class FloatItemWithIndex : public FloatItem
{
public:
    FloatItemWithIndex();
    FloatItemWithIndex(int index, const FloatItem& item);
    
    int index() const;
    void setIndex(int index);
    
    bool operator<(const FloatItemWithIndex&) const;
private:
    int m_index;
};

#endif // FLOATITEMWITHINDEX_H
