//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef FLOATITEM_H
#define FLOATITEM_H

class FloatItem
{
public:
    FloatItem();
    
    float size() const;
    
    /**
     * Set the size of the item.
     * @param size The size of the item 0 <= size <= 1.
     */
    void setSize(float size);
private:
    float m_size;
};

#endif // FLOATITEM_H
