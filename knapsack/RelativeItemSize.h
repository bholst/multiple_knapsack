/*
//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//
*/

#ifndef RELATIVEITEMSIZE_H
#define RELATIVEITEMSIZE_H

class Item;

class RelativeItemSize
{
public:
    RelativeItemSize();
    RelativeItemSize(int id, Item item);
    
    int id() const;
    void setId(int id);
    
    double relativeItemSize() const;
    void setRelativeItemSize(double relativeItemSize);
    
    bool operator<(const RelativeItemSize&) const;
private:
    int m_id;
    double m_relativeItemSize;
};

#endif // RELATIVEITEMSIZE_H
