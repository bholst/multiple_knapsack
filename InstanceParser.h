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
class ProfitItem;
class FloatItem;

class InstanceParser : public QXmlStreamReader
{
public:
    enum ProblemType {
        MultipleKnapsack,
        BinPacking
    };
    
    InstanceParser();
    ~InstanceParser();
    
    void setPath(const QString& path);
    
    QVector<ProfitItem> mkpItems() const;
    QVector<FloatItem> binItems() const;
    QList<int> sizes() const;
    ProblemType type() const;

    void read();
    
private:
    void readProblem();
    void readProblemInstance();
    void readData();
    void readUnknownElement();
    void readBin();
    void readItem();
    
    QString m_path;
    QVector<ProfitItem> m_mkpItems;
    QVector<FloatItem> m_binItems;
    QList<int> m_sizes;
    ProblemType m_type;
};

#endif // INSTANCEPARSER_H
