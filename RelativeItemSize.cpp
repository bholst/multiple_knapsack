/*
//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//
*/

// Project
#include "ProfitItem.h"

// Self
#include "RelativeItemSize.h"

RelativeItemSize::RelativeItemSize()
    : m_id(-1),
      m_relativeItemSize(-1)
{
}

RelativeItemSize::RelativeItemSize(int id, ProfitItem item)
    : m_id(id),
      m_relativeItemSize(((double) item.size())/((double) item.profit()))
{
}

int RelativeItemSize::id() const
{
    return m_id;
}

void RelativeItemSize::setId(int id)
{
    m_id = id;
}

double RelativeItemSize::relativeItemSize() const
{
    return m_relativeItemSize;
}

void RelativeItemSize::setRelativeItemSize(double relativeItemSize)
{
    m_relativeItemSize = relativeItemSize;
}

bool RelativeItemSize::operator<(const RelativeItemSize& other) const
{
    return m_relativeItemSize < other.m_relativeItemSize;
}

