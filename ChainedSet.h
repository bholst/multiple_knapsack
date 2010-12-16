/*
//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//
*/

#ifndef CHAINEDSET_H
#define CHAINEDSET_H

// Qt
#include <QtCore/QSet>

class ChainedSet
{
public:
    ChainedSet(int current);
    ChainedSet(ChainedSet *next, int current);
    
    QSet<int> set();
    
private:
    ChainedSet *m_next;
    int         m_current;
};

#endif // CHAINEDSET_H
