//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QPair>

// Project
#include "Item.h"
#include "ChainedSet.h"

// Self
#include "Knapsack.h"

typedef QPair<int,int> IntPair;

Knapsack::Knapsack()
    : m_dirty(true),
      m_size(0),
      m_maximumProfit(0)
{

}

Knapsack::~Knapsack()
{

}

int Knapsack::size() const
{
    return m_size;
}

void Knapsack::setSize ( int size )
{
    m_dirty = true;
    m_size = size;
}

QVector< ProfitItem > Knapsack::items() const
{
    return m_items;
}

void Knapsack::setItems ( const QVector< ProfitItem >& items )
{
    m_dirty = true;
    m_items = items;
}

void Knapsack::setItems(const QVector<int>& sizes, const QVector<int>& profits)
{
    if(sizes.size() == profits.size()) {
        QVector<ProfitItem> items;

        int length = sizes.size();
        for(int i = 0; i < length; ++i) {
            ProfitItem item;
            item.setSize(sizes[i]);
            item.setProfit(profits[i]);
            items.insert(items.end(), item);
        }
        setItems(items);
    }
}

int Knapsack::maximumProfit()
{
    if(m_dirty) {
        recalculateValues();
    }

    return m_maximumProfit;
}

QSet< int > Knapsack::maximumProfitItems()
{
    if(m_dirty) {
        recalculateValues();
    }

    return m_maximumProfitItems;
}

void Knapsack::recalculateValues()
{
    int profitSum = 0;
    for(int i=0; i < m_items.size(); ++i) {
        profitSum += m_items[i].profit();
        qDebug() << "Profit sum: " << profitSum;
    }

    qDebug() << QString("The size of the field is %1 x %2").arg(m_items.size()).arg(profitSum);
    //int minimumSize[m_items.size()][profitSum];
    QHash<IntPair,int> minimumSize;
    //QSet<int> *minimumSizeItems[m_items.size()][profitSum];
    QHash<IntPair,ChainedSet *> minimumSizeItems;
    QList< ChainedSet * > sets;
    /*
    for(int i = 0; i < m_items.size(); ++i) {
        for(int j = 0; j < profitSum; ++j) {
            minimumSizeItems[i][j] = 0;
        }
    }*/
    int maximumProfit = 0;
    ChainedSet *maximumProfitItems = 0;

    int profitFirst = m_items[0].profit();
    int sizeFirst = m_items[0].size();

    ChainedSet *zeroSet = new ChainedSet(0);
    sets.append(zeroSet);
    if( sizeFirst <= m_size ) {
        maximumProfit = profitFirst;
        maximumProfitItems = zeroSet;
    }

    for(int i = 1; i <= profitFirst; ++i) {
//         minimumSize[0][i-1] = sizeFirst;
//         minimumSizeItems[0][i-1] = zeroSet;
        minimumSize.insert(IntPair(0,i-1), sizeFirst);
        minimumSizeItems.insert(IntPair(0,i-1), zeroSet);
    }
    for(int i = profitFirst + 1; i <= profitSum; ++i) {
//         minimumSize[0][i-1] = -1;
        minimumSize.insert(IntPair(0,i-1), -1);
    }

    for(int j = 1; j < m_items.size(); ++j) {
        for(int i = 1; i <= profitSum; ++i) {
            int a, b = -1;
//             a = minimumSize[j-1][i-1];
            a = minimumSize.value(IntPair(j-1, i-1));
            ChainedSet *bItems = 0;

            if(i <= m_items[j].profit()) {
                // Item j is the only item in the set.
                b = m_items[j].size();
                bItems = new ChainedSet(j);
            }
            else {
                if(minimumSize.value(IntPair(j-1,i-1-m_items[j].profit())) >= 0) {
                    // Item j comes into the set of other items.
                    b = m_items[j].size() + minimumSize.value(IntPair(j-1,i-1-m_items[j].profit()));
                    bItems = new ChainedSet(minimumSizeItems.value(IntPair(j-1,i-1-m_items[j].profit())), j);
                }
            }

            if((a <= b && a >= 0) || b < 0) {
                // Item j does not come into the set of items.
                minimumSize.insert(IntPair(j, i-1), a);
                minimumSizeItems.insert(IntPair(j, i-1), minimumSizeItems.value(IntPair(j-1, i-1)));
                delete bItems;
            }
            else {
                // Item j comes into the set of items
                minimumSize.insert(IntPair(j, i-1), b);
                minimumSizeItems.insert(IntPair(j, i-1), bItems);
                sets.append(bItems);
            }

            if(minimumSize.value(IntPair(j,i-1)) >= 0
               && minimumSize.value(IntPair(j,i-1)) <= m_size
               && maximumProfit < i)
            {
                maximumProfit = i;
                maximumProfitItems = minimumSizeItems.value(IntPair(j, i-1));
            }
        }
    }

    m_maximumProfit = maximumProfit;
    if(maximumProfitItems) {
        m_maximumProfitItems = maximumProfitItems->set();
    }
    else {
        m_maximumProfitItems.clear();
    }
    
    QList< ChainedSet* >::const_iterator endIterator = sets.constEnd();
    for(QList< ChainedSet* >::const_iterator it = sets.constBegin();
        it != endIterator;
        ++it)
    {
        delete *it;
    }
    m_dirty = false;
}

QString Knapsack::toString() const
{
    QString returnString;
    returnString += "Bin size ";
    returnString += size();
    returnString += "\n";
    returnString += "Items:\n";
    for(int i = 0; i < m_items.size(); ++i) {
        returnString += i;
        returnString += ":\n";
        returnString += "Size: ";
        returnString += m_items[i].size();
        returnString += "\n";
        returnString += "Profit: ";
        returnString += m_items[i].profit();
        returnString += "\n";
    }

    return returnString;
}


