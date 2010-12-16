//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef TEST_H
#define TEST_H

// Qt
#include <QtCore/QVector>

class KnapsackTestInstance;

class Test
{

public:
    Test();
    virtual ~Test();

    void run();
    void runMultipleKnapsackTests();

private:
    QVector<KnapsackTestInstance> m_knapsackInstances;
};

#endif // TEST_H
