//
// Copyright (C) 2011  Bastian Holst <bastianholst@gmx.de>
//

#ifndef ITEMVECTOR_H
#define ITEMVECTOR_H

class QString;
class ItemVector
{

public:
    explicit ItemVector(int numberOfItemSizes);
    ~ItemVector();
    void setItemCount(int sizeNumber, int itemNumber);
    int itemCount(int sizeNumber) const;
    
    QString toString() const;
    
private:
    int m_numberOfItemSizes;
    int *m_itemCount;
};

#endif // ITEMVECTOR_H
