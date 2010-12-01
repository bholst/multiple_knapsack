//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#ifndef TEST_H
#define TEST_H

#include <vector>

class KnapsackTestInstance;

class Test
{

public:
    Test();
    virtual ~Test();

    void run();
    void runMultipleKnapsackTests();

private:
    std::vector<KnapsackTestInstance> m_knapsackInstances;
};

#endif // TEST_H
