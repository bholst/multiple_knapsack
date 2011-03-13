//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>
#include <QtCore/QVector>
#include <QtCore/QList>

// Project
#include "InstanceParser.h"
#include "ApproximateMultipleKnapsack.h"
#include "ImprovedApproximateMultipleKnapsack.h"
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
    
    bool extra = false;
    
    for(int i = 1; i < argc; ++i) {
        qDebug() << "Arg" << i;
        if(QString(argv[i]) == "--extra") {
            qDebug() << "Using the extra algorithm.";
            extra = true;
            continue;
        }
        
        InstanceParser parser;
        parser.setPath(QString(argv[i]));
        parser.read();
        
        if(parser.type() == InstanceParser::MultipleKnapsack) {
            qDebug() << "Knapsack";
            if(!extra) {
                ApproximateMultipleKnapsack multipleKnapsack;
                multipleKnapsack.setItems(parser.mkpItems());
                multipleKnapsack.setSizes(parser.sizes());
                qDebug() << QString("Maximum profit is %1").arg(multipleKnapsack.maximumProfit());
            }
            else {
                ImprovedApproximateMultipleKnapsack multipleKnapsack;
                multipleKnapsack.setItems(parser.mkpItems());
                multipleKnapsack.setSizes(parser.sizes());
                multipleKnapsack.maximumProfit();
            }
        }
        else if(parser.type() == InstanceParser::BinPacking) {
            qDebug() << "Bin packing";
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
