/*
//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//
*/

#ifndef INSTANCEPARSER_H
#define INSTANCEPARSER_H

// Qt
#include <QtCore/QXmlStreamReader>
#include <QtCore/QList>
#include <QtCore/QVector>

class QString;
class Item;

class InstanceParser : public QXmlStreamReader
{
public:
    InstanceParser();
    ~InstanceParser();
    
    void setPath(const QString& path);
    
    QVector<Item> items() const;
    QList<int> sizes() const;

    void read();
    
private:
    void readProblem();
    void readProblemInstance();
    void readData();
    void readUnknownElement();
    void readBin();
    void readItem();
    
    QString m_path;
    QVector<Item> m_items;
    QList<int> m_sizes;
};

#endif // INSTANCEPARSER_H
