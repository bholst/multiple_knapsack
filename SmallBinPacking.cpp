//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

// STL
#include <cmath>

// Qt
#include <QtCore/QDebug>
#include <QtCore/QSet>
#include <QtCore/QPair>

// Project
#include "FloatItem.h"
#include "ItemVectorWithPredecessor.h"
#include "ItemVector.h"

// Self
#include "SmallBinPacking.h"

SmallBinPacking::SmallBinPacking()
    : m_dirty(true),
      m_delta(0.38),
      m_K(8),
      m_minimumNumberOfBins(-1),
      m_assignment(0)
{

}

SmallBinPacking::~SmallBinPacking()
{
    delete m_assignment;
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

int *SmallBinPacking::assignment()
{
    if(m_dirty) {
        recalculateValues();
    }
    return m_assignment;
}

void SmallBinPacking::recalculateValues()
{
    qDebug() << "Starting to recalculate the values";
    m_minimumNumberOfBins = -1;

    // First calculate the sets of small, medium sized and large items.
    float small = pow(m_delta, 4);
    float large = 1.0/(2.0 * m_K) / log2(1/m_delta);
    qDebug() << "Small is smaller than" << small;
    qDebug() << "Large is larger as" << large;
    
    QSet<int> smallItems;
    QSet<int> mediumItems;
    QSet<int> largeItems;
    
    qDebug() << "Creating the sets";
    for(int i = 0; i < m_items.size(); ++i) {
        if(m_items[i].size() <= small) {
            qDebug() << "Item" << i << "is small";
            smallItems.insert(i);
        }
        else if(m_items[i].size() <= large) {
            qDebug() << "Item" << i << "is medium sized";
            mediumItems.insert(i);
        }
        else {
            qDebug() << "Item" << i << "is large";
            largeItems.insert(i);
        }
    }
    
    // Then calculate the sets of the medium sized items.
    int r0 = 0;
    double power = 1.0;
    double limit = m_K * log2(1/m_delta);
    while(power < limit) {
        r0++;
        power *= 2.0;
    }
    QList<double> upperBoundaries;
    QList<double> lowerBoundaries;
    int i = 0;
    do {
        lowerBoundaries.append(1/pow(2.0,r0+i+1));
        upperBoundaries.append(1/pow(2.0,r0+i));
        i++;
    } while(lowerBoundaries.last() > small);
    
    // Sort the medium sized items in their sets
    QVector< QMultiMap<float,int> > itemsI(lowerBoundaries.size());
    for(QSet<int>::iterator it = mediumItems.begin();
        it != mediumItems.end();
        ++it)
    {
        int setNr = ceil(log2(1/m_items[*it].size()) - r0 - 1);
        itemsI[setNr].insert(m_items[*it].size(),*it);
        qDebug() << "Item" << *it << "of size" << m_items[*it].size() << "in set" << setNr;
    }
    
    // Sort all items in the groups J
    QVector< QMultiMap<float, int> > groupItems(itemsI.size()); // The item numbers by rounded item size.
    QMultiMap<float, int> allGroupItems;
    QMap<float, int> extraItems; // counting all items out of a first group per set
    QMap<float, int> normalItems; // counting all other items
    QVector<float> normalItemsSizes;
    QVector<int> normalItemsNumbers;
    for(int i = 0; i < itemsI.size(); ++i) {
        int groupSize = ceil(pow(2,r0+i)/(m_K * log2(1/m_delta)));
        bool firstGroup = true;
        qDebug() << "For set" << i << "there are" << groupSize << "items per group";
        QMultiMap<float,int> items = itemsI[i];
        QMultiMap<float,int>::iterator it = items.end();
        while(it != items.begin()) {
            int groupItemNumber = 0;
            float groupItemSize = (it - 1).key();
            while(it != items.begin() && groupItemNumber < groupSize) {
                --it;
                groupItems[i].insert(groupItemSize, it.value());
                allGroupItems.insert(groupItemSize, it.value());
                qDebug() << "Rounding the size of item" << it.value() << "to" << groupItemSize;
                groupItemNumber++;
            }
            if(firstGroup) {
                extraItems.insert(groupItemSize, groupItemNumber);
                firstGroup = false;
            }
            else {
                normalItems.insert(groupItemSize, groupItemNumber);
                normalItemsSizes.append(groupItemSize);
                normalItemsNumbers.append(groupItemNumber);
            }
        }
    }
    qDebug() << "The set of extraItems is" << extraItems;
    qDebug() << "The set of normalItems is" << normalItems;
    
    int minimumBins = 1;
    int maximumBins = maximumNumberOfBins();
    bool foundNumber = false;
    
    int *endAssignment = new int[m_items.size()];
    float *endRemainingCapacities = 0;
    qDebug() << "Trying all large items";
    int mediumBins;
    while(!foundNumber) {
        mediumBins = (maximumBins + minimumBins)/2; // This will not exceed the size of int,
                                                        // because the maximum Number of bins is small.
        delete endRemainingCapacities;
        endRemainingCapacities = new float[mediumBins + 1];
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
            foundAssignment = handlePreassignment(assignment,
                                                  mediumBins,
                                                  normalItemsSizes,
                                                  normalItemsNumbers,
                                                  allGroupItems,
                                                  extraItems,
                                                  smallItems,
                                                  endAssignment,
                                                  endRemainingCapacities);
            if(foundAssignment) {
                delete m_assignment;
                m_assignment = endAssignment;
                endAssignment = new int[m_items.size()];
                
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
            if(maximumBins < maximumNumberOfBins()) {
                foundNumber = true;
                m_minimumNumberOfBins = maximumBins + 1;
            }
            else {
                break;
            }
        }
    }
    
    if(!foundNumber) {
        delete m_assignment;
        m_assignment = 0;
    }
    
    delete endAssignment;
    delete endRemainingCapacities;
    
    m_dirty = false;
}

bool SmallBinPacking::handlePreassignment(int* preassignment, int numberOfBins,
                                          QVector<float> normalItemSizes,
                                          QVector<int> normalItemNumbers,
                                          QMultiMap<float, int> grouping,
                                          QMap<float, int> extraItems,
                                          QSet<int> smallItems,
                                          int *resultingAssignment,
                                          float *resultingRemainingCapacities)
{
    // Initializing the capacities
    for(int i = 0; i <= numberOfBins; ++i) {
        resultingRemainingCapacities[i] = 1.0;
    }
    for(int i = 0; i < m_items.size(); ++i) {
//         qDebug() << "Item" << i << "in bin" << preassignment[i];
        if(preassignment[i] >= 0) {
            resultingRemainingCapacities[preassignment[i]] -= m_items[i].size();
            if(resultingRemainingCapacities[preassignment[i]] < 0) {
                return false;
            }
        }
    }
    
    QList<ItemVectorWithPredecessor *> allItemVectors;
    QList<ItemVectorWithPredecessor *> itemVectors;
    itemVectors.append(new ItemVectorWithPredecessor(normalItemNumbers.size()));
    
    ItemVectorWithPredecessor *sufficientFill = 0;
    int sufficientBinNumber = -1;
    for(int bin = 0; bin < numberOfBins && sufficientFill == 0; ++bin) {
        float remainingCapacity = resultingRemainingCapacities[bin];
        
        QList<ItemVectorWithPredecessor *> nextItemVectors;
        ItemVector currentBinFill(normalItemNumbers.size());
        
        bool allFills = false;
        while(!allFills && sufficientFill == 0) {
            int itemClass = normalItemSizes.size() - 1;
            bool foundNextFill = false;
            while(!allFills && !foundNextFill) {
                float itemSize = normalItemSizes[itemClass];
                int fittingClassItems = floor(remainingCapacity / itemSize);
                if(fittingClassItems > normalItemNumbers[itemClass]) {
                    fittingClassItems = normalItemNumbers[itemClass];
                }
                
                if(fittingClassItems > currentBinFill.itemCount(itemClass)) {
                    currentBinFill.setItemCount(itemClass, fittingClassItems);
                    remainingCapacity -= itemSize * fittingClassItems;
                    foundNextFill = true;
                }
                else {
                    remainingCapacity += itemSize * currentBinFill.itemCount(itemClass);
                    currentBinFill.setItemCount(itemClass, 0);
                    itemClass--;
                    if(itemClass < 0) {
                        allFills = true;
                    }
                }
            }
            
            if(!allFills) {
                // Now we have a new fill.
                foreach(ItemVectorWithPredecessor *vector, itemVectors) {
                    ItemVectorWithPredecessor *nextFill = new ItemVectorWithPredecessor(currentBinFill, vector);
                    nextItemVectors.append(nextFill);
                    if(nextFill->isFull(normalItemNumbers)) {
                        sufficientFill = nextFill;
                        sufficientBinNumber = bin;
                        break;
                    }
                }
            }
        }
        
        allItemVectors.append(itemVectors);
        itemVectors = nextItemVectors;
    }
    
    int result = false;
    for(int i = 0; i < m_items.size(); ++i) {
        resultingAssignment[i] = preassignment[i];
    }
    
    if(sufficientFill != 0) {
        ItemVectorWithPredecessor *currentVector = sufficientFill;
        QVector<int> remainingItemNumbers = normalItemNumbers;
        for(int bin = sufficientBinNumber; bin >= 0; --bin) {
            for(int i = 0; i < normalItemSizes.size(); ++i) {
                int numberOfCurrentSizeItems;
                if(currentVector->predecessor() != 0) {
                    numberOfCurrentSizeItems = currentVector->itemCount(i) - currentVector->predecessor()->itemCount(i);
                }
                else {
                    numberOfCurrentSizeItems = currentVector->itemCount(i);
                }
                qDebug() << "Group" << i << "gets" << numberOfCurrentSizeItems << "set";
                
                float currentSize = normalItemSizes[i];
                while(numberOfCurrentSizeItems > 0
                      && remainingItemNumbers[i] > 0)
                {
                    QMap<float, int>::const_iterator itemIndexIterator = grouping.find(currentSize);
                    if(itemIndexIterator == grouping.end()) {
                        qDebug() << "Group empty";
                        break;
                    }
                    int itemIndex = *itemIndexIterator;
//                     assignment[itemIndex] = bin;
                    resultingAssignment[itemIndex] = bin;
                    qDebug() << "Item " << itemIndex << "in bin" << bin;
                    resultingRemainingCapacities[bin] -= m_items[itemIndex].size();
                    qDebug() << "Bin space now:" << resultingRemainingCapacities[bin];
                    grouping.remove(currentSize, itemIndex);
                    
                    numberOfCurrentSizeItems--;
                }
            }
            currentVector = currentVector->predecessor();
        }
        
        qDebug() << "Found sufficient fill!";
        result = true;
    }
    
    foreach(ItemVectorWithPredecessor *vector, allItemVectors) {
        delete vector;
    }
    
    foreach(ItemVectorWithPredecessor *vector, itemVectors) {
        delete vector;
    }
    
    if(result == false) {
        return false;
    }
    
    bool extraResult = packExtraItems(extraItems,
                                      grouping,
                                      resultingAssignment,
                                      resultingRemainingCapacities,
                                      numberOfBins);
    
    if(!extraResult) {
        return false;
    }
    
    if(!packSmallItems(smallItems, resultingAssignment, resultingRemainingCapacities, numberOfBins)) {
        return false;
    }
    
    return true;
}


bool SmallBinPacking::packExtraItems(QMap< float, int > extraItems,
                                     QMultiMap< float, int > grouping,
                                     int* assignment,
                                     float* remainingCapacities,
                                     int numberOfBins)
{
    for(QMap<float, int>::iterator it = extraItems.begin();
        it != extraItems.end();
        ++it)
    {
        while(1) {
            QMap<float, int>::const_iterator itemIndexIterator = grouping.find(it.key());
            if(itemIndexIterator == grouping.end()) {
                qDebug() << "Group empty!";
                break;
            }
            
            int itemIndex = *itemIndexIterator;
            assignment[itemIndex] = numberOfBins;
            qDebug() << "Item " << itemIndex << "in bin" << numberOfBins;
            remainingCapacities[numberOfBins] -= m_items[itemIndex].size();
            qDebug() << "Bin space now:" << remainingCapacities[numberOfBins];
            grouping.remove(it.key(), itemIndex);
            if(remainingCapacities[numberOfBins] < 0.0) {
                return false;
            }
        }
    }
    
    return true;
}

bool SmallBinPacking::packSmallItems(QSet< int > smallItems,
                                     int* assignment,
                                     float* remainingCapacities,
                                     int numberOfBins)
{
    for(QSet<int>::iterator it = smallItems.begin(); 
        it != smallItems.end();
        ++it)
    {
        float currentSize = m_items[*it].size();
        bool foundBin = false;
        for(int bin = 0; bin <= numberOfBins; ++bin) {
            if(remainingCapacities[bin] >= currentSize) {
                assignment[*it] = bin;
                remainingCapacities[bin] -= currentSize;
                foundBin = true;
                qDebug() << "Item" << *it << "in bin" << bin;
                break;
            }
        }
        if(!foundBin) {
            qDebug() << "Small item did not fit into bin.";
            return false;
        }
    }
    
    return true;
}

