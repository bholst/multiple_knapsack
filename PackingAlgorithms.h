//
// Copyright (C) 2011  Bastian Holst <bastianholst@gmx.de>
//

#ifndef PACKINGALGORITHMS_H
#define PACKINGALGORITHMS_H

// Standard
#include <cmath>

// Qt
#include <QtCore/QDebug>
#include <QtCore/QList>

// Project
#include "ItemVectorWithPredecessor.h"

template< typename T > int *findGroupedPacking(int *groupCount,
                                               T *groupSizes,
                                               T *remainingCapacities,
                                               int count,
                                               int binCount)
{
    QList<ItemVectorWithPredecessor *> allItemVectors;
    QList<ItemVectorWithPredecessor *> itemVectors;
    itemVectors.append(new ItemVectorWithPredecessor(count));
    
    ItemVectorWithPredecessor *sufficientFill = 0;
    int sufficientBinNumber = -1;
    for(int bin = 0; bin < binCount && sufficientFill == 0; ++bin) {
        T remainingCapacity = remainingCapacities[bin];
        
        QList<ItemVectorWithPredecessor *> nextItemVectors;
        ItemVector currentBinFill(count);
        
        bool allFills = false;
        while(!allFills && sufficientFill == 0) {
            int itemGroup = count - 1;
            bool foundNextFill = false;
            while(!allFills && !foundNextFill) {
                T itemSize = groupSizes[itemGroup];
                
                int currentItemCount = currentBinFill.itemCount(itemGroup);
                
                if(itemSize <= 0) {
                    itemGroup--;
                    if(itemGroup < 0) {
                        allFills = true;
                    }
                    continue;
                }
                else if(remainingCapacity > itemSize) {
                        currentBinFill.setItemCount(itemGroup, currentItemCount + 1);
                        remainingCapacity -= itemSize;
                        foundNextFill = true;
                }
                else {
                    remainingCapacity += itemSize * currentItemCount;
                    currentBinFill.setItemCount(itemGroup, 0);
                    itemGroup--;
                    if(itemGroup < 0) {
                        allFills = true;
                    }
                }
            }
            
            if(allFills) {
                break;
            }
            
            // Ensure that no items fit into the bin anymore.
            for(itemGroup = count - 1; itemGroup >= 0; --itemGroup) {
                T itemSize = groupSizes[itemGroup];
                if(itemSize <= 0) {
                    continue;
                }
                int currentItemCount = currentBinFill.itemCount(itemGroup);
                int fittingGroupItems = floor(((double) remainingCapacity) / ((double) itemSize)) + currentItemCount;
                if(fittingGroupItems <= currentItemCount) {
                    break;
                }
                if(fittingGroupItems > groupCount[itemGroup]) {
                    fittingGroupItems = groupCount[itemGroup];
                }
                
                currentBinFill.setItemCount(itemGroup, fittingGroupItems);
                remainingCapacity -= itemSize * (fittingGroupItems - currentItemCount);
            }
            
            // Now we have a new fill.
            foreach(ItemVectorWithPredecessor *vector, itemVectors) {
                ItemVectorWithPredecessor *nextFill = new ItemVectorWithPredecessor(currentBinFill, vector);
                nextItemVectors.append(nextFill);
                if(nextFill->isFull(groupCount)) {
                    sufficientFill = nextFill;
                    sufficientBinNumber = bin;
                    break;
                }
            }
        }
        
        allItemVectors.append(itemVectors);
        itemVectors = nextItemVectors;
    }
    
    int *packing = 0;
    
    if(sufficientFill != 0) {
        packing = new int[binCount * count];
        
        ItemVectorWithPredecessor *currentVector = sufficientFill;
        int remainingItemNumbers[count];
        memcpy(remainingItemNumbers, groupCount, count * sizeof(int));
        
        int bin;
        for(bin = binCount - 1; bin > sufficientBinNumber; --bin) {
            for(int i = 0; i < count; ++i) {
                packing[bin * count + i] = 0;
            }
        }
        for(bin; bin >= 0; --bin) {
            for(int i = 0; i < count; ++i) {
                int numberOfCurrentSizeItems;
                if(currentVector->predecessor() != 0) {
                    numberOfCurrentSizeItems = currentVector->itemCount(i) - currentVector->predecessor()->itemCount(i);
                }
                else {
                    numberOfCurrentSizeItems = currentVector->itemCount(i);
                }
                qDebug() << "Group" << i << "gets" << numberOfCurrentSizeItems << "set";
                
                if(numberOfCurrentSizeItems > remainingItemNumbers[i]) {
                    numberOfCurrentSizeItems = remainingItemNumbers[i];
                }
                remainingItemNumbers[i] -= numberOfCurrentSizeItems;
                
                packing[bin * count + i] = numberOfCurrentSizeItems;
            }
            currentVector = currentVector->predecessor();
        }
        
        qDebug() << "Found sufficient fill!";
    }
    
    foreach(ItemVectorWithPredecessor *vector, allItemVectors) {
        delete vector;
    }
    
    foreach(ItemVectorWithPredecessor *vector, itemVectors) {
        delete vector;
    }
    
    return packing;
}

#endif
