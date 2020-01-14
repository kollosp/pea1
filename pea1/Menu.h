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
                           " 10. TSP Tabu search\n"
                           " 11. TSP Symulowane wyzarzanie\n"
                           " 12. TSP Genetyczny\n"
                           " 20. Policz dlugosc sciezki\n"
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

    void run(int argc, char*argv[]);
    void run();

    void pause();

    void bruteforce();
    void bruteforceLimited();
    void greedy();
    void dynamic();
    void tabuSearch();
    void simAnnealing();
    void generic();

    void simAnnealingTest();
    void tabuSearchTest();
    void genericTest();
    void countPath();

};



#endif // MENU_H
