/*
//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//
*/

// Qt
#include <QtCore/QDebug>
#include <QtCore/QXmlStreamAttributes>
#include <QtCore/QFile>

// Project
#include "Item.h"

// Self
#include "InstanceParser.h"

InstanceParser::InstanceParser()
    : QXmlStreamReader()
{

}

InstanceParser::~InstanceParser()
{

}

void InstanceParser::setPath(const QString& path)
{
    m_path = path;
}

QVector< Item > InstanceParser::items() const
{    
    return m_items;
}

QList< int > InstanceParser::sizes() const
{    
    return m_sizes;
}

void InstanceParser::read()
{
    m_sizes.clear();
    m_items.clear();
    
    QFile file(m_path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file";
        return;
    }
    
    setDevice(&file);
    
    while(!atEnd()) {
        readNext();
        
        if(isStartElement()) {
            if(name() == "problem")
                readProblem();
            else
                raiseError("This file is not valid");
        }
    }
}

void InstanceParser::readProblem()
{
    Q_ASSERT(isStartElement()
             && name() == "problem");
    
    while(!atEnd()) {
        readNext();
        
        if(isEndElement()) 
            break;
        
        if(isStartElement()) {
            if(name() == "probleminstance")
                readProblemInstance();
            else 
                readUnknownElement();
        }
    }
}

void InstanceParser::readProblemInstance()
{
    Q_ASSERT(isStartElement()
             && name() == "probleminstance");
    
    while(!atEnd()) {
        readNext();
        
        if(isEndElement())
            break;
        
        if(isStartElement()) {
            if(name() == "data")
                readData();
            else 
                readUnknownElement();
        }
    }
}

void InstanceParser::readData()
{
    Q_ASSERT(isStartElement()
             && name() == "data");
    
    while(!atEnd()) {
        readNext();
        
        if(isEndElement())
            break;
        
        if(isStartElement()) {
            if(name() == "bin")
                readBin();
            else if(name() == "item")
                readItem();
            else 
                readUnknownElement();
        }
    }
}

void InstanceParser::readBin()
{
    Q_ASSERT(isStartElement()
             && name() == "bin");
    
    QXmlStreamAttributes elementAttributes = attributes();
    if(elementAttributes.hasAttribute("height")) {
        m_sizes.append(elementAttributes.value("height").toString().toInt());
    }
    
    while(!atEnd()) {
        readNext();
        
        if(isEndElement())
            break;
        
        if(isStartElement())
            readUnknownElement();
    }
}

void InstanceParser::readItem()
{
    Q_ASSERT(isStartElement()
             && name() == "item");
    
    QXmlStreamAttributes elementAttributes = attributes();
    if(elementAttributes.hasAttribute("height")
       && elementAttributes.hasAttribute("profit"))
    {
        Item item;
        item.setSize(elementAttributes.value("height").toString().toInt());
        item.setProfit(elementAttributes.value("profit").toString().toInt());
        
        m_items.append(item);
    }
    
    while(!atEnd()) {
        readNext();
        
        if(isEndElement())
            break;
        
        if(isStartElement())
            readUnknownElement();
    }
}

void InstanceParser::readUnknownElement()
{
    Q_ASSERT(isStartElement());
    
    while(!atEnd()) {
        readNext();
        
        if(isEndElement())
            break;
        
        if(isStartElement())
            readUnknownElement();
    }
}




