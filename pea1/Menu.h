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
                           " 1. Wczytaj z pliku (format: macierz)\n"
                           " 2. Wczytaj z pliku (format: lista krawedzi)\n"
                           " 3. Zapisz do pliku\n"
                           " 4. Wygeneruj losowy\n"
                           " 5. Wyswietl macierzowo\n"
                           " 6. TSP Metoda BruteForce\n"
                           " 7. TSP Metoda B&B wglab\n"
                           " 8. TSP Metoda zachlanna\n"
                           " 9. TSP Metoda prog. dynamicznego\n"
                           " 0. Wyjscie z programu";


    NeighbourMatrix graphMatrix;

public:
    Menu();

    void initFromFile(const std::__cxx11::string &path);
    void test();

    void fromFile();
    void fromFileMatrixStyle();
    void toFile();
    void random();

    void displayMatrix();
    void displayList();

    void run();

    void pause();

    void bruteforce();
    void bruteforceLimited();
    void greedy();
    void dynamic();
};

template <typename T>
std::ostream& operator <<(std::ostream&str, std::vector<T> obj){
    for(unsigned int i=0;i<obj.size();++i){
        str<<obj[i]<<" ";
    }

    return str;
}

#endif // MENU_H
