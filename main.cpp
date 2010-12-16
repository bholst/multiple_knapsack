//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>
#include <QtCore/QVector>
#include <QtCore/QList>

// Project
#include "InstanceParser.h"
#include "FastMultipleKnapsack.h"
#include "Test.h"
#include "Item.h"

using namespace std;

int main(int argc, char **argv) {
    if(argc < 2) {
        Test test;
        test.run();
        
        return 0;
    }
    
    for(int i = 1; i < argc; ++i) {
        InstanceParser parser;
        parser.setPath(QString(argv[i]));
        parser.read();
        
        FastMultipleKnapsack multipleKnapsack;
        multipleKnapsack.setItems(parser.items());
        multipleKnapsack.setSizes(parser.sizes());
        qDebug() << QString("Maximum profit is %1").arg(multipleKnapsack.maximumProfit());
    }
    return 0;
}
