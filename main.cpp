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
#include "SmallBinPacking.h"
#include "Test.h"
#include "ProfitItem.h"
#include "FloatItem.h"

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
        
        if(parser.type() == InstanceParser::MultipleKnapsack) {
            FastMultipleKnapsack multipleKnapsack;
            multipleKnapsack.setItems(parser.mkpItems());
            multipleKnapsack.setSizes(parser.sizes());
            qDebug() << QString("Maximum profit is %1").arg(multipleKnapsack.maximumProfit());
        }
        else if(parser.type() == InstanceParser::BinPacking) {
            SmallBinPacking binPacking;
            binPacking.setItems(parser.binItems());
            qDebug() << QString("Minimum number of bins is %1").arg(binPacking.minimumNumberOfBins());
            
            int *assignment = binPacking.assignment();
            if(assignment != 0) {
                qDebug() << QString("using the following packing:");
                int numberOfItems = parser.binItems().size();
                for(int i = 0; i < numberOfItems; ++i) {
                    qDebug() << QString("Pack item %1 in bin %2.").arg(i).arg(assignment[i]);
                }
            }
        }
    }
    return 0;
}
