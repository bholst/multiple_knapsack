//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// STL
#include <cmath>

// Qt
#include <QtCore/QDebug>
#include <QtCore/QSet>

// Project
#include "FloatItem.h"

// Self
#include "SmallBinPacking.h"

SmallBinPacking::SmallBinPacking()
    : m_dirty(true),
      m_delta(0.6),
      m_K(8),
      m_minimumNumberOfBins(-1)
{

}

QVector< FloatItem > SmallBinPacking::items() const
{
    return m_items;
}

void SmallBinPacking::setItems(const QVector< FloatItem >& items)
{
    m_dirty = true;
    m_items = items;
}

int SmallBinPacking::maximumNumberOfBins() const
{
    return floor(pow(1/m_delta, 3));
}

int SmallBinPacking::minimumNumberOfBins()
{
    if(m_dirty) {
        recalculateValues();
    }
    return m_minimumNumberOfBins;
}

void SmallBinPacking::recalculateValues()
{
    qDebug() << "Starting to recalculate the values";
    m_minimumNumberOfBins = -1;

    // First calculate the sets of small, medium sized and large items.
    float small = pow(m_delta, 4);
    float large = 1/(2 * m_K) / log2(1/m_delta);
    
    QSet<int> smallItems;
    QSet<int> mediumItems;
    QSet<int> largeItems;
    
    qDebug() << "Creating the sets";
    for(int i = 0; i < m_items.size(); ++i) {
        if(m_items[i].size() <= small) {
            smallItems.insert(i);
        }
        else if(m_items[i].size() <= large) {
            mediumItems.insert(i);
        }
        else {
            largeItems.insert(i);
        }
    }
    
    int minimumBins = 0;
    int maximumBins = maximumNumberOfBins();
    bool foundNumber = false;
    
    qDebug() << "Trying all large items";
    while(!foundNumber) {
        int mediumBins = (maximumBins + minimumBins)/2; // This will not exceed the size of int,
                                                        // because the maximum Number of bins is small.
        int assignment[m_items.size()];
        for(int i = 0; i < m_items.size(); ++i) {
            if(largeItems.contains(i)) {
                assignment[i] = 0;
            }
            else {
                assignment[i] = -1;
            }
        }
        
        qDebug() << "Now trying to handle all assignments for" << mediumBins << "bins";
        bool foundAssignment = false;
        bool noAssignmentPossible = false;
        while(!noAssignmentPossible) {
            foundAssignment = handlePreassignment(assignment, mediumBins);
            if(foundAssignment) {
                break;
            }
            
            bool assignmentChanged = false;
            QSet<int>::iterator it = largeItems.begin();
            while(1) {
                int current = *it;
                
                if(assignment[current] < (mediumBins - 1)) {
                    assignment[current] = assignment[current] + 1;
                    break;
                }
                else {
                    assignment[current] = 0;
                    ++it;
                    if(it == largeItems.end()) {
                        noAssignmentPossible = true;
                        break;
                    }
                }
            }
        }
        
        if(foundAssignment) {
            maximumBins = mediumBins;
        }
        else {
            minimumBins = mediumBins + 1;
        }

        if(maximumBins == minimumBins) {
            if(foundAssignment) {
                foundNumber = true;
                m_minimumNumberOfBins = minimumBins;
            }
            else {
                break;
            }
        }
    }
}

bool SmallBinPacking::handlePreassignment(int* preassignment, int numberOfBins
)
{
    qDebug() << "Testing a preassignment";
    float remainingCapacity[numberOfBins];
    // Initializing the capacities
    for(int i = 0; i < numberOfBins; ++i) {
        remainingCapacity[i] = 1.0;
    }
    for(int i = 0; i < m_items.size(); ++i) {
        qDebug() << "Item" << i << "in bin" << preassignment[i];
        if(preassignment[i] >= 0) {
            remainingCapacity[preassignment[i]] -= m_items[i].size();
            if(remainingCapacity[preassignment[i]] < 0) {
                return false;
            }
        }
    }
    
    return true;
}