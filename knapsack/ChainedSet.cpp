/*
//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//
*/

#include "ChainedSet.h"

ChainedSet::ChainedSet(int current)
    : m_next(0),
      m_current(current)
{

}

ChainedSet::ChainedSet(ChainedSet* next, int current)
    : m_next(next),
      m_current(current)
{
}

QSet< int > ChainedSet::set()
{
    ChainedSet *currentSet = this;
    QSet<int> returnSet;

    while(currentSet != 0) {
        returnSet.insert(currentSet->m_current);
        currentSet = currentSet->m_next;
    }
    
    return returnSet;
}


