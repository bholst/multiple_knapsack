//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef ITEM_H
#define ITEM_H

class Item
{
    friend class GreedyMultipleKnapsack;

public:
    Item();
    virtual ~Item();

    int size() const;
    void setSize(int size);

private:
    int id() const;
    void setId(int id);

    int m_size;
    int m_id;
};

#endif // ITEM_H
