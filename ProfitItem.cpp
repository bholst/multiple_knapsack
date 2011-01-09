//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>

// Self
#include "ProfitItem.h"

ProfitItem::ProfitItem()
    : Item(),
      m_profit(0)
{

}

int ProfitItem::profit() const
{
    return m_profit;
}

void ProfitItem::setProfit(int profit)
{
    if(profit < 0) {
        qDebug() << "Only profits >= 0 are allowed";
        m_profit = 0;
    }
    else {
        m_profit = profit;
    }
}