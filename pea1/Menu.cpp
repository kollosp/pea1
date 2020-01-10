#include "Menu.h"

Menu::Menu()
{

}

void Menu::initFromFile(const std::string& path)
{
    Algorythms::fromFileMatrixStyle(graphMatrix, path, false);
}

void Menu::test()
{
    std::vector<int> v = {0,1,2};
    std::cout<<graphMatrix.complement(v)<<std::endl;

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


void Menu::run(int argc, char *argv[]) {


    if(argc >= 2){
        if(std::string(argv[2]) == "tabu"){
            tabuSearchTest();
        }
    }
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
            case 9: dynamic();             break;
            case 10: tabuSearch();             break;
            case 11: simAnnealing();             break;
            case 12: generic();             break;
            case 20: countPath();             break;
            default: continue;
        }

        tm.stop();
        std::cout<<"czas wykonania: "<<std::fixed<<tm.elapsed()*1000<<" ms"<<std::endl;
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
    std::cout<<"Rezultat metody brute force TSP: "<<Algorythms::bruteforceTSPIter(0,graphMatrix, distance);
    std::cout<<" dystans: "<<distance<<std::endl;
}

void Menu::bruteforceLimited()
{
    int distance = 0;
    std::cout<<"Rezultat metody z ograniczeniami TSP: "<<Algorythms::BAndBTSPIter(0,graphMatrix, distance);
    std::cout<<" dystans: "<<distance<<std::endl;
}

void Menu::greedy()
{
    int distance = 0;
    std::cout<<"Rezultat metody zachlannej TSP: "<<Algorythms::greedyTSP(0,graphMatrix, distance);
    std::cout<<" dystans: "<<distance<<std::endl;
}

void Menu::dynamic()
{
    int distance = 0;
    std::cout<<"Rezultat metody programowania dynamicznego TSP: "<<Algorythms::dynamicTSP(0,graphMatrix, distance);
    std::cout<<" dystans: "<<distance<<std::endl;
}

void Menu::tabuSearch()
{
    try{
        int distance = 0;
        std::cout<<"Rezultat tabu search TSP: "<<Algorythms::tabuSearchTSP(0,graphMatrix, distance, 500000, 30);
        std::cout<<" dystans: "<<distance<<std::endl;

    }catch(const char* e){
        std::cerr<<e<<std::endl;
    }
}

void Menu::simAnnealing()
{
    try{
        int distance = 0;
        std::cout<<"Rezultat symulowanego wyzarzania TSP: "<<Algorythms::simAnnealing(0,graphMatrix, distance, 20000.0,0.9998,100000);
        std::cout<<" dystans: "<<distance<<std::endl;

    }catch(const char* e){
        std::cerr<<e<<std::endl;
    }
}

void Menu::simAnnealingTest()
{
    int distance = 0;

    //std::vector<float> tmp = {1000, 5000, 10000, 15000, 20000, 25000, 30000, 50000, 100000, 150000};
    //std::vector<float> tmp = {2,5, 10,20};
    //std::vector<int> iterations = {1000, 5000, 10000, 15000, 20000, 25000, 30000, 50000};

    std::vector<float> factors = {0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.99, 0.9998};

    int minDistance = 2020;
    int maxIts = 100000;
    double factor = 0.9998;
    double tmpVal = 15000;


    for(float i:factors){
        int its = 0;
        float avgTime=0;
        float avgDistance=0;
        float avgError =0;

        //std::cout<<"====================================="<<std::endl;
        //std::cout<<"iterations: "<<i<<" f: "<<factor<<" its:"<<maxIts<<std::endl;

        for(int k=1;k<=20;++k){

            Timer tm;
            tm.start();

            Algorythms::simAnnealing(0,graphMatrix, distance, tmpVal, i, maxIts);

            tm.stop();
            float error = abs((float)(distance-minDistance)/distance*100.0);

            avgDistance +=distance;
            avgTime+=tm.elapsed()*1000;
            avgError+=error;

            its++;
            //std::cout<<k<<"; "<<std::fixed<<tm.elapsed()*1000<<";"<<distance<<";"<<error<<std::endl;
        }
        std::cout<<"srednio; "<<i<<";"<<std::fixed<<avgTime/its<<";"<<avgDistance/its<<";"<<avgError/its<<std::endl;
    }

}

void Menu::tabuSearchTest()
{
    std::vector<int> iterations = {500000}; //czas iterowania
    std::vector<float> tabuSize = {3.68,7.35,14.71,29.41,36.76,44.12,58.82,73.53,88.24}; //dlugosc listy w %
    std::vector<int> criticals = {20}; //ilosc bledow do resetu

    for(int it : iterations){
        for(int ts:tabuSize){
            for(int c : criticals){

                std::cout<<"====================================="<<std::endl;
                std::cout<<"iterations: "<<it<<" tabuSize: "<<ts<<" criticalEvents: "<<c<<std::endl;
                int distance = 0;

                for(int i=1;i<=20;++i){

                    Timer tm;
                    tm.start();

                    Algorythms::tabuSearchTSP(0,graphMatrix, distance, it, (graphMatrix.size()-1) * graphMatrix.size()* ts /200.0, c);

                    tm.stop();
                    std::cout<<i<<"; "<<std::fixed<<tm.elapsed()*1000<<";"<<distance<<";"<<std::endl;

                }
            }
        }
    }
}

void Menu::countPath()
{
    std::vector<int> verts;
    for(int i=0;i<graphMatrix.size();++i){
        int j;
        std::cin>>j;
        j--;
        verts.push_back(j);
    }

    std::cout<<"Dlugosc: "<<graphMatrix.calcPathDistance(verts, verts[0])<<std::endl;
}

void Menu::generic()
{
    try{
        int distance = 0;
        std::cout<<"Rezultat algorytmu generycznego: "<<Algorythms::generic(0,graphMatrix, distance, 20, 10, 0.5);
        std::cout<<" dystans: "<<distance<<std::endl;

    }catch(const char* e){
        std::cerr<<e<<std::endl;
    }
}

