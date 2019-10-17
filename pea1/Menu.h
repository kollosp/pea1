#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <vector>
#include <functional>
#include <queue>
#include <stdio.h>
#include <Timer.h>

#include "NeighbourMatrix.h"
#include "Algorythms.h"

class Menu
{

    std::string mainMenu = "----Menu Glowne----\n"
                           " 1. Wczytaj z pliku\n"
                           " 2. Zapisz do pliku\n"
                           " 3. Wygeneruj losowy\n"
                           " 4. Wyswietl macierzowo\n"
                           " 5. TSP Metoda BruteForce\n"
                           " 6. TSP Metoda z ograniczeniami\n"
                           " 7. TSP Metoda zachlanna\n"
                           " 8. TSP Metoda prog. dynamicznego\n"
                           " 9. \n"
                           " 0. Wyjscie z programu";


    NeighbourMatrix graphMatrix;

public:
    Menu();

    void fromFile();
    void toFile();
    void random();

    void displayMatrix();
    void displayList();

    void run();

    void pause();

    void bruteforce();
    void bruteforceLimited();
    void greedy();
    void dinamic();
};

template <typename T>
std::ostream& operator <<(std::ostream&str, std::vector<T> obj){
    for(unsigned int i=0;i<obj.size();++i){
        str<<obj[i]<<" ";
    }

    return str;
}

#endif // MENU_H
