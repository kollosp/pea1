#include "Menu.h"

Menu::Menu()
{

}

void Menu::initFromFile(const std::string& path)
{
    Algorythms::fromFileMatrixStyle(graphMatrix, path, false);
}



void Menu::fromFile()
{
    std::cout<<"Ladowanie grafu z pliku"<<std::endl;

    std::string file;
    std::cout<<"podaj sciezke do pliku: ";
    std::cin >> file;

    Algorythms::fromFile(graphMatrix, file, false);

    std::cout<<"Zaladowano"<<std::endl;

}

void Menu::fromFileMatrixStyle()
{
    std::cout<<"Ladowanie grafu z pliku (format macierzowy)"<<std::endl;

    std::string file;
    std::cout<<"podaj sciezke do pliku: ";
    std::cin >> file;

    Algorythms::fromFileMatrixStyle(graphMatrix, file, false);

    std::cout<<"Zaladowano"<<std::endl;

}

void Menu::toFile()
{
    std::cout<<"Zapisywanie grafu do pliku"<<std::endl;

    std::string file;
    std::cout<<"podaj sciezke do pliku: ";
    std::cin >> file;

     Algorythms::toFile(graphMatrix, file);


    std::cout<<"Zapisano"<<std::endl;
}

void Menu::random()
{
    std::cout<<"Generowanie losowego grafu"<<std::endl;

    int vert;
    std::cout<<"podaj ilosc wierzcholkow: ";
    std::cin >> vert;

    int factor = 100;
    //std::cout<<"podaj stopien wypelnienia grafu (0-100%): ";
    //std::cin >> factor;

    graphMatrix.init(vert);
    Algorythms::fillGraph(graphMatrix, factor);


    std::cout<<"Wygenerowano"<<std::endl;


}

void Menu::displayMatrix()
{

    std::cout<<"Wyswietlanie grafu: macierz sasiedztwa"<<std::endl;
    std::cout<<graphMatrix<<std::endl;

}



void Menu::run()
{
    int d = 0;


    do{

        std::cout<<"----------------------------------------------"<<std::endl;
        std::cout<<mainMenu<<std::endl;
        std::cin >> d;

        Timer tm;
        tm.start();

        switch(d){
            case 1: fromFileMatrixStyle(); break;
            case 2: fromFile();            break;
            case 3: toFile();              break;
            case 4: random();              break;
            case 5: displayMatrix();       break;
            case 6: bruteforce();          break;
            case 7: bruteforceLimited();   break;
            case 8: greedy();              break;
            case 9: dinamic();             break;
            default: continue;
        }

        tm.stop();
        std::cout<<"czas wykonania: "<<tm.elapsed()<<std::endl;
        pause();

    }while(d!=0);
}

void Menu::pause()
{
    std::cin.ignore(1024, '\n');
    std::cout << "Wcisnij enter aby kontynuowac...";
    std::cin.get();
}

void Menu::bruteforce()
{
    int distance = 0;
    std::cout<<"Rezultat metody brute force TSP: "<<Algorythms::bruteforceTSP(0,graphMatrix, distance);
    std::cout<<" dystans: "<<distance<<std::endl;
}

void Menu::bruteforceLimited()
{
    int distance = 0;
    std::cout<<"Rezultat metody z ograniczeniami TSP: "<<Algorythms::bruteforceTSPLimited(0,graphMatrix, distance);
    std::cout<<" dystans: "<<distance<<std::endl;
}

void Menu::greedy()
{
    int distance = 0;
    std::cout<<"Rezultat metody zachlannej TSP: "<<Algorythms::greedyTSP(0,graphMatrix, distance);
    std::cout<<" dystans: "<<distance<<std::endl;
}

void Menu::dinamic()
{
    int distance = 0;
    std::cout<<"Rezultat metody programowania dynamicznego TSP: "<<Algorythms::dinamicTSP(0,graphMatrix, distance);
    std::cout<<" dystans: "<<distance<<std::endl;
}
