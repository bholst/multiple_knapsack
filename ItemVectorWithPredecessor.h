//
// Copyright (C) 2011  Bastian Holst <bastianholst@gmx.de>
//

#ifndef ITEMVECTORWITHPREDECESSOR_H
#define ITEMVECTORWITHPREDECESSOR_H

template <int numberOfItemSizes>
class ItemVectorWithPredecessor
{

public:
    ItemVectorWithPredecessor();
    
private:
    int itemNumber[numberOfItemSizes];
    ItemVectorWithPredecessor<numberOfItemSizes> *predecessor;
};

#endif // ITEMVECTORWITHPREDECESSOR_H
