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
#include "ProfitItem.h"
#include "FloatItem.h"

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

QVector< ProfitItem > InstanceParser::mkpItems() const
{    
    return m_mkpItems;
}

QVector< FloatItem > InstanceParser::binItems() const
{
    return m_binItems;
}

QList< int > InstanceParser::sizes() const
{    
    return m_sizes;
}

InstanceParser::ProblemType InstanceParser::type() const
{
    return m_type;
}

void InstanceParser::read()
{
    m_sizes.clear();
    m_mkpItems.clear();
    m_binItems.clear();
    
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
   
    QXmlStreamAttributes elementAttributes = attributes();
    if(elementAttributes.hasAttribute("name")) {
        if(elementAttributes.value("name").toString() == "mkp") {
            m_type = InstanceParser::MultipleKnapsack;
        }
        else if(elementAttributes.value("name").toString() == "BinPacking") {
            m_type = InstanceParser::BinPacking;
        }
    }
    
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
    if(m_type == InstanceParser::MultipleKnapsack) {
        if(elementAttributes.hasAttribute("height")
           && elementAttributes.hasAttribute("profit"))
        {
            ProfitItem item;
            item.setSize(elementAttributes.value("height").toString().toInt());
            item.setProfit(elementAttributes.value("profit").toString().toInt());
        
            m_mkpItems.append(item);
        }
    }
    else if(m_type == InstanceParser::BinPacking) {
        if(elementAttributes.hasAttribute("height")) {
            FloatItem item;
            item.setSize(elementAttributes.value("height").toString().toFloat());
            
            m_binItems.append(item);
        }
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




