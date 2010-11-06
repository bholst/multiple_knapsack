//
// Copyright 2010      Bastian Holst <bastianholst@gmx.de>
//

#include <iostream>

#include "Item.h"
#include "Knapsack.h"

using namespace std;

int main(int argc, char **argv) {
    bool stop = false;

    cout << "Bitte geben Sie die Größe des Rucksacks an: ";
    int size = 0;
    cin >> size;
    cout << endl;

    vector<Item> items;

    while(!stop) {
        int size = 0;
        int profit = 0;

        cout << "Neues Item" << endl;
        cout << "Gewicht: "; cin >> size; cout << endl;
        cout << "Profit: "; cin >> profit; cout << endl;

        Item item;
        item.setSize(size);
        item.setProfit(profit);
        items.insert( items.end(), item );

        string stopString = "";
        cout << "Abbrechen (a)"; cin >> stopString; cout << endl;
        if(stopString.compare("a") == 0) {
            stop = true;
        }
    }

    Knapsack knapsack;
    knapsack.setSize(size);
    knapsack.setItems(items);

    cout << "Der maximale Profit ist: " << knapsack.maximumProfit() << endl;

    return 0;
}
