//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// STL
#include <iostream>

// Qt
#include <QtCore/QVector>
#include <QtCore/QList>
#include <QtCore/QSet>

// Project
#include "Knapsack.h"
#include "KnapsackTestInstance.h"
#include "ApproximateKnapsack.h"
#include "GreedyMultipleKnapsack.h"
#include "ApproximateMultipleKnapsack.h"

// Self
#include "Test.h"

using namespace std;

Test::Test()
{
    KnapsackTestInstance instance;
    instance.setSize(5);
    QVector<int> sizes(3, 0), profit(3, 0);
    sizes[0] = 2;
    sizes[1] = 4;
    sizes[2] = 1;
    profit[0] = 5;
    profit[1] = 6;
    profit[2] = 4;
    instance.setItems(sizes, profit);
    instance.setExpectedMaximumProfit(10);
    QList< QSet<int> > itemList;
    QSet<int> set1;
    set1.insert(1);
    set1.insert(2);
    itemList.push_back(set1);
    instance.setExpectedMaximumProfitItems(itemList);
    m_knapsackInstances.insert(m_knapsackInstances.end(), instance);

    KnapsackTestInstance instance2;
    instance2.setSize(100);
    QVector<int> sizes2(2, 0), profit2(2, 0);
    sizes2[0] = 1;
    sizes2[1] = 100;
    profit2[0] = 1;
    profit2[1] = 2;
    instance2.setItems(sizes2, profit2);
    instance2.setExpectedMaximumProfit(2);
    QList< QSet<int> > itemList2;
    QSet<int> set2;
    set2.insert(1);
    itemList2.push_back(set2);
    instance2.setExpectedMaximumProfitItems(itemList2);
    m_knapsackInstances.insert(m_knapsackInstances.end(), instance2);

    KnapsackTestInstance instance3;
    instance3.setSize(1024);
    QVector<int> sizes3(5, 0), profit3(5, 0);
    sizes3[0] = 1024;
    sizes3[1] = 2048;
    sizes3[2] = 512;
    sizes3[3] = 512;
    sizes3[4] = 512;
    profit3[0] = 2048;
    profit3[1] = 4096;
    profit3[2] = 1023;
    profit3[3] = 1020;
    profit3[4] = 1025;
    instance3.setItems(sizes3, profit3);
    instance3.setExpectedMaximumProfit(2048);
    QList< QSet<int> > itemList3;
    QSet<int> set3;
    QSet<int> set4;
    set3.insert(0);
    set4.insert(2);
    set4.insert(4);
    itemList3.push_back(set3);
    itemList3.push_back(set4);
    instance3.setExpectedMaximumProfitItems(itemList3);
    m_knapsackInstances.insert(m_knapsackInstances.end(), instance3);

    KnapsackTestInstance instance4;
    instance4.setSize(10);
    QVector<int> sizes4(3, 0), profit4(3, 0);
    sizes4[0] = 128;
    sizes4[1] = 16;
    sizes4[2] = 11;
    profit4[0] = 4096;
    profit4[1] = 8192;
    profit4[2] = 16;
    instance4.setItems(sizes4, profit4);
    instance4.setExpectedMaximumProfit(0);
    QList< QSet<int> > itemList4;
    itemList4.append(QSet<int>());
    instance4.setExpectedMaximumProfitItems(itemList4);
    m_knapsackInstances.insert(m_knapsackInstances.end(), instance4);
}

Test::~Test()
{

}

void Test::run()
{
    runMultipleKnapsackTests();
    
    int testsFailed = 0;
    for(QVector<KnapsackTestInstance>::iterator it = m_knapsackInstances.begin();
        it != m_knapsackInstances.end();
        ++it)
    {
        QList< QSet<int> > expectedList = it->expectedMaximumProfitItems();
        QSet<int> returnedSet = it->maximumProfitItems();
        bool foundRightSet = false;

        for(QList< QSet<int> >::iterator expectedSetIt = expectedList.begin();
            expectedSetIt != expectedList.end();
            ++expectedSetIt)
        {
            bool rightSet = true;
            if(expectedSetIt->size() == returnedSet.size()) {
                for(QSet<int>::iterator it = expectedSetIt->begin();
                    it != expectedSetIt->end();
                    ++it)
                {
                    if(returnedSet.find(*it) == returnedSet.end()) {
                        rightSet = false;
                        break;
                    }
                }
            }
            else {
                rightSet = false;
            }

            if(rightSet) {
                foundRightSet = true;
                break;
            }
        }

        if(it->maximumProfit() != it->expectedMaximumProfit()) {
            cerr << "Test failed:" << endl;
            cerr << "The calculated profit is " << it->maximumProfit()
                 << ", but the expected Profit is "
                 << it->expectedMaximumProfit() << ".\n";
            ++testsFailed;
        }
        else if(!foundRightSet) {
            cerr << "Wrong set of items" << endl;
            ++testsFailed;
        }
        else {
            cerr << "Test successful!" << endl;
        }

        cerr << "Approximation:";
        ApproximateKnapsack approx(*it);
        approx.setApproximationLevel(0.1);

        cerr << "The approximated value is " << approx.maximumProfit()
             << ", whereas the exact value is " << it->maximumProfit()
             << " and it would be sufficient to get a value like "
             << (1.0 - approx.approximationLevel()) * it->maximumProfit() << endl;
        if(approx.maximumProfit() <
           (1.0 - approx.approximationLevel()) * it->maximumProfit())
        {
            cerr << "The approximation is not sufficient!\n";
            ++testsFailed;
        }
        else {
            cerr << "The approximation is sufficient!\n";
        }
    }

    cerr << testsFailed << " tests failed.\n";
}

void Test::runMultipleKnapsackTests()
{
    cout << "Multiple Knapsack test starting" << endl << endl;
    QList<int> bins;
    bins.push_back(133);
    bins.push_back(1200);
    bins.push_back(421);
    QVector<ProfitItem> items;
    {
        ProfitItem item;
        item.setSize(102);
        item.setProfit(142);
        items.push_back(item);
    }
    {
        ProfitItem item;
        item.setSize(102);
        item.setProfit(12);
        items.push_back(item);
    }
    {
        ProfitItem item;
        item.setSize(699);
        item.setProfit(799);
        items.push_back(item);
    }
    {
        ProfitItem item;
        item.setSize(688);
        item.setProfit(608);
        items.push_back(item);
    }
    {
        ProfitItem item;
        item.setSize(306);
        item.setProfit(346);
        items.push_back(item);
    }
    {
        ProfitItem item;
        item.setSize(983);
        item.setProfit(1073);
        items.push_back(item);
    }
    {
        ProfitItem item;
        item.setSize(555);
        item.setProfit(545);
        items.push_back(item);
    }
    
    GreedyMultipleKnapsack knapsacks;
    knapsacks.setSizes(bins);
    knapsacks.setItems(items);
    knapsacks.setApproximationLevel(0.1);
    
    if(knapsacks.maximumProfit() == 1561) {
        cout << "Test successful" << endl;
    }
    else {
        cout << "Test failed" << endl;
    }
    cout << "Multiple Knapsack 1, Profit: " << knapsacks.maximumProfit() << endl;
    
    ApproximateMultipleKnapsack fastKnapsacks;
    fastKnapsacks.setSizes(bins);
    fastKnapsacks.setItems(items);
    fastKnapsacks.setApproximationLevel(0.1);
    cout << "Better approximation: " << fastKnapsacks.maximumProfit() << endl;
}


