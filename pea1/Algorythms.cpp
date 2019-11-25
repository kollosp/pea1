#include "Algorythms.h"
#include <bitset>

Algorythms::Algorythms()
{

}

std::vector<unsigned int> Algorythms::bruteforceTSPIter(int beginVert, const NeighbourMatrix &m, int &distance)
{
    class Destination {
    public:
        Destination(unsigned int _vert, unsigned int _depth): vert(_vert), depth(_depth) {}
        unsigned int vert;
        unsigned int depth;
    };

    std::vector<Destination> queue;
    queue.push_back(Destination(beginVert, 0));
    //parametry najlepsze osiagniete
    unsigned int minDistance = 0x0fffffff;
    std::vector<unsigned int> minPath;

    //parametry biezace
    std::vector<unsigned int> path;
    //path.push_back(beginVert);
    distance = 0;

    int lastDepth = -1;
    unsigned long long iterations = 0;
    unsigned long long  fullIterations = 1;

    //policz ilosc permutacji
    for(int i=2;i<=m.size();++i){
        fullIterations *=i;
    }

    std::cout<<"iteracji: "<<fullIterations<<std::endl;

    while (!queue.empty()) {

        //std::cout<<"iter: "<<iterations<<" size: "<<queue.size()<<std::endl;
        //iterations ++;

        //if(iterations % 1000 == 0)
            //std::cout<<"\r"<<iterations *100000/ fullIterations * 0.001<<" %, iteracja: "<<iterations;

        //kolejka FILO
        Destination d = queue[queue.size()-1];
        queue.erase(queue.end()-1);

        //jezeli nastapil powrot na wyzsza glebokosc
        while(lastDepth >= (signed) d.depth){
            //usuniecie dystansu
            distance -= m.edge(path[path.size()-2],path[path.size()-1]);
            path.erase(path.end()-1);
            lastDepth--;
        }

        //dopisz wierzcholek do sciezki i dodaj droge
        path.push_back(d.vert);

        if(path.size() >= 2)
            distance += m.edge(path[path.size()-2],path[path.size()-1]);

        //dodaj wszystkich sasiadow wierzcholka ktorzy nie wystapili jeszcze w
        //sciezce do kolejki
        std::vector<int> neighbours = m.neighbours(d.vert);
        bool allUsed = true;
        for(int i:neighbours){
            bool used = false;
            for(int j:path){
                if(i == j){
                    used = true;
                    break;
                }
            }

            //wierzcholek nie zosttal wczesniej uzyty
            if(!used){
                allUsed = false;
                queue.push_back(Destination(i,d.depth+1));
            }

        }

        //jezeli nie ma kolejnych wierzcholkow do rozpatrzenia
        //sprawdz czy nie zostala znaleziona nowa najlepsza opcja
        if(allUsed){

            //dodanie powrotu
            unsigned int fullD = distance + m.edge(path[path.size()-1],beginVert);

            if(fullD < minDistance){
                minDistance = fullD;
                minPath = path;
            }
        }

//        for(int i:path){
//            std::cout<<i<<" ";
//        }
//        std::cout<<"| "<<distance<<", "<<d.depth<<std::endl;

        //zapisanie poprzedniego stanu glegokosci zeby okreslic co nalezy zrobic ze sciezka
        lastDepth = d.depth;
    }

    distance = minDistance;
    minPath.push_back(beginVert);
    std::cout<<std::endl;
    return minPath;
}

std::vector<unsigned int> Algorythms::BAndBTSPIter(int beginVert, const NeighbourMatrix &m, int &distance)
{
    class Destination {
    public:
        Destination(unsigned int _vert, unsigned int _depth): vert(_vert), depth(_depth) {}
        unsigned int vert;
        unsigned int depth;
    };

    std::vector<Destination> queue;
    queue.push_back(Destination(beginVert, 0));
    //parametry najlepsze osiagniete
    unsigned int minDistance = 0x0fffffff;
    std::vector<unsigned int> minPath;

    //parametry biezace
    std::vector<unsigned int> path;
    //path.push_back(beginVert);
    distance = 0;

    int lastDepth = -1;
    unsigned long long iterations = 0;
    unsigned long long  fullIterations = 1;

    //policz ilosc permutacji
    for(int i=2;i<=m.size();++i){
        fullIterations *=i;
    }

    std::cout<<"iteracji: "<<fullIterations<<std::endl;

    while (!queue.empty()) {

        iterations ++;

        //if(iterations % 1000 == 0)
        //    std::cout<<"\r"<<iterations *100000/ fullIterations * 0.001<<" %, iteracja: "<<iterations;

        //kolejka FILO
        Destination d = queue[queue.size()-1];
        queue.erase(queue.end()-1);

        //jezeli nastapil powrot na wyzsza glebokosc
        while(lastDepth >= (signed) d.depth){
            //usuniecie dystansu
            distance -= m.edge(path[path.size()-2],path[path.size()-1]);
            path.erase(path.end()-1);
            lastDepth--;
        }

        //dopisz wierzcholek do sciezki i dodaj droge
        path.push_back(d.vert);

        if(path.size() >= 2)
            distance += m.edge(path[path.size()-2],path[path.size()-1]);


        //dodaj wszystkich sasiadow wierzcholka ktorzy nie wystapili jeszcze w
        //sciezce do kolejki i dla których przeszukiwanie ma jeszcze sens
        std::vector<int> neighbours = m.neighbours(d.vert);
        bool allUsed = true;
        for(int i:neighbours){
            bool used = false;
            for(int j:path){
                if(i == j){
                    used = true;
                    break;
                }
            }


            //wierzcholek nie zosttal wczesniej uzyty
            if(!used){
                allUsed = false;

                unsigned int lower = m.sumMinimumEdgesOfVerts(m.complement(path)) + distance;
                unsigned int higher = distance + m.edge(d.vert,  i);

                if(higher < minDistance && lower < minDistance)
                    queue.push_back(Destination(i,d.depth+1));
            }

        }

        //jezeli nie ma kolejnych wierzcholkow do rozpatrzenia
        //sprawdz czy nie zostala znaleziona nowa najlepsza opcja
        if(allUsed){

            //dodanie powrotu
            unsigned int fullD = distance + m.edge(path[path.size()-1],beginVert);

            if(fullD < minDistance){
                minDistance = fullD;
                minPath = path;
            }
        }

        //zapisanie poprzedniego stanu glegokosci zeby okreslic co nalezy zrobic ze sciezka
        lastDepth = d.depth;
    }

    distance = minDistance;
    minPath.push_back(beginVert);
    std::cout<<std::endl;
    return minPath;

}

std::vector<int> Algorythms::bruteforceTSP(int beginVert, const NeighbourMatrix &m, int&distance)
{

    std::vector<int> path;
    path.push_back(beginVert);
    std::vector<int> minPath;
    int minDistance = 0x0fffffff;
    bruteforceRecursive(beginVert, path, 0, m, minPath, minDistance);

    std::cout<<"podsumowanie: "<<std::endl;
    for(int i:minPath){
        std::cout<<i<<" ";
    }
    std::cout<<" | "<<minDistance<<std::endl;
    distance=minDistance;


    return minPath;
}

std::vector<int> Algorythms::bruteforceTSPLimited(int beginVert, const NeighbourMatrix &m, int &distance)
{
    std::vector<int> path;
    path.push_back(beginVert);
    std::vector<int> minPath;
    int minDistance = 0x0fffffff;
    bruteforceRecursiveLimited(beginVert, path, 0, m, minPath, minDistance);

    std::cout<<"podsumowanie: "<<std::endl;
    for(int i:minPath){
        std::cout<<i<<" ";
    }
    std::cout<<" | "<<minDistance - m.edge(minPath[minPath.size()-2], minPath[minPath.size()-1])<<std::endl;
    distance = minDistance;


    return minPath;
}

std::vector<int> Algorythms::greedyTSP(int beginVert, const NeighbourMatrix &m, int &distance)
{

    std::vector<int> path;
    path.push_back(beginVert);

    int currentVert = beginVert;

    for(int i=0;i<m.size()-1;++i){

        std::vector<int> neighbours = m.neighbours(currentVert);

        for(int& j:neighbours){
            for(int k:path){
                if(j == k) j=-1;
            }
        }

        int minDistance = 0x0fffffff;
        int minVert = currentVert;
        for(int j:neighbours){
            if(j>=0){
                if(minDistance > m.edge(currentVert, j)){
                    minVert = j;
                    minDistance = m.edge(currentVert, j);
                }
            }
        }

        if(currentVert == minVert){
            std::cout<<"brak sciezki"<<std::endl;

            for(int i:path){
                std::cout<<i<<" ";
            }
            std::cout<<" | "<<distance<<std::endl;
            return std::vector<int>();
        }

        distance += m.edge(currentVert, minVert);
        currentVert = minVert;
        path.push_back(currentVert);
    }



    distance += m.edge(path[path.size()-1], path[0]);
    path.push_back(path[0]);

    /*for(int i:path){
        std::cout<<i<<" ";
    }
    std::cout<<" | "<<distance<<std::endl;*/
    return path;
}

std::vector<int> Algorythms::dynamicTSP(int beginVert, const NeighbourMatrix &m, int &distance)
{
    int s=0;
    for(int i=0;i<m.size();++i){
        s<<=1;
        ++s;
    }

    unsigned int size = s+1;
    unsigned int *array = new unsigned int[size*size];
    unsigned int *paths = new unsigned int[size*size];

    std::cout<<"Zajeta pamiec: "<<sizeof(unsigned int) *size*size / 1024 / 1024<<" MB" <<std::endl;

    for(int i=0;i<size;++i){
        for(int j=0;j<size;++j){
            array[i+size*j] = 0;
            paths[i+size*j] = 0;
        }
    }

    try{

        distance = dynamicTSPDistanceF(beginVert, s, 1, m, array, paths, size);

    }
    catch(const char* err){
        std::cout<<err<<std::endl;
    }

    std::vector<int> p = refactorDynamicTSPPath(s, 1, paths, size);

    //dolicz powrot do wierzchołka startowego
    //distance += m.edge(p[p.size()-1], beginVert);
    p.push_back(beginVert);

/*    for(int i=0;i<size;++i){
        for(int j=0;j<size;++j){
            std::cout<<array[i+size*j]<<" ";
        }
        std::cout<<std::endl;
    }*/

    delete []array;
    delete []paths;

    return p;
}


Algorythms::BruteforceData Algorythms::bruteforceRecursive(int current, std::vector<int> path, int distance, const NeighbourMatrix &m, std::vector<int>&currentMinPath, int& currentMinDistance)
{
    std::vector<int> neighbours = m.neighbours(current);

    bool allUsed = true;

    for(int i:neighbours){

        bool used = false;
        for(unsigned int j=1;j<path.size();++j){
            if(i==path[j]) used = true;
        }

        if(used == false){
            allUsed = false;
            std::vector<int> newPath = path;
            newPath.push_back(i);
            bruteforceRecursive(i, newPath, distance + m.edge(current, i), m, currentMinPath, currentMinDistance);
        }
    }

    if(allUsed){

        if(path[0] != path[path.size()-1]) return BruteforceData();

        /*for(int i:path){
            std::cout<<i<<" ";
        }
        std::cout<<" | "<<distance<<std::endl;*/

        if(currentMinDistance > distance){
            currentMinDistance = distance;
            currentMinPath = path;
        }
    }
    return BruteforceData();

}

Algorythms::BruteforceData Algorythms::bruteforceRecursiveLimited(int current, std::vector<int> path, int distance, const NeighbourMatrix &m, std::vector<int>&currentMinPath, int& currentMinDistance)
{
    std::vector<int> neighbours = m.neighbours(current);

    bool allUsed = true;

    for(int i:neighbours){

        bool used = false;
        for(unsigned int j=1;j<path.size();++j){
            if(i==path[j]) used = true;
        }

        //dla pozostaly sprawdz sume minimalnych krawedzi
        int lmin = m.sumMinimumEdgesOfVerts(m.complement(path));

        if(used == false && (distance + m.edge(current, i)) < currentMinDistance && (distance + lmin) < currentMinDistance){
            allUsed = false;
            std::vector<int> newPath = path;
            newPath.push_back(i);
            bruteforceRecursiveLimited(i, newPath, distance + m.edge(current, i), m, currentMinPath, currentMinDistance);
        }
    }

    if(allUsed){

        if(path[0] != path[path.size()-1] || path.size() != m.size()+1) return BruteforceData();

        for(int i:path){
            std::cout<<i<<" ";
        }
        std::cout<<" | "<<distance<<" m:"<<currentMinDistance<<std::endl;

        if(currentMinDistance > distance){
            currentMinDistance = distance;
            currentMinPath = path;
        }
    }
    return BruteforceData();

}

int Algorythms::dynamicTSPDistanceF(int beginVert, unsigned int s, unsigned int dst, const NeighbourMatrix &m,
                                    unsigned int *array, unsigned int *paths, int arraySize, std::string string)
{
    class Destination {
    public:
        Destination(unsigned int _s, unsigned int _dst, unsigned int _depth) : s(_s), dst(_dst), depth(_depth) {}
        unsigned int s;
        unsigned int dst; //dst zawiera sie w s
        unsigned int depth; //glegokosc. jak duzo jedynek jest w s
    };

    //kolejka wszystkich przypadkow
    std::vector<Destination> queue;
    queue.push_back(Destination(s,dst, countOnes(s)));

    //kolejka sluzaca wygenerowaniu wszystkich przypadkow
    std::vector<Destination> q;
    q.push_back(Destination(s,dst, countOnes(s)));

    while (!q.empty()) {

        //pobierz z kolejki priorytetowej. Priorytet wyznaczony przez minimum depth
        Destination d = q[0];
        q.erase(q.begin());

        unsigned int newS = d.s - d.dst;

        //rozwiniecie przypadku podstawowego
        if(newS == 0){
            array[d.s + d.dst*arraySize] = m.edge(beginVert, log2(d.dst)-1);
        }

        unsigned int j=1;
        while(j <= newS){

            if((newS & j) > 0){
                if(array[newS + j*arraySize] == 0){
                    unsigned int depth = countOnes(newS);
                    q.push_back(Destination(newS,j, depth));

                    //bez przypadkow podstawowych. wszystkie sa rozwiazywane w powyzej
                    if(depth > 1)
                        queue.push_back(Destination(newS,j, depth));

                    //okreslenie ze dane rozwiniecie zostalo juz wykonane i nie
                    //nalezy go powtarzac
                    array[newS + j*arraySize]  = -1;
                }
            }
            j<<=1;
        }

        //std::bitset<8> x(d.s);
        //std::bitset<8> y(d.dst);
        //std::cout<<"s: "<<d.s<<"("<<x<<") "<<d.dst<<"("<<y<<") d: "<<d.depth<<std::endl;

    }

    //return 1;

    //queue zawiera przestrzen stanow do przebadania
    while (!queue.empty()) {

        //pobierz najnizszy element do analizy
        unsigned int min = 0x0fffffff;
        int index = -1;
        for(int i=queue.size()-1;i>=0;--i){
            if(queue[i].depth < min){
                index = i;
                min = queue[i].depth;
            }
        }

        Destination d = queue[index];
        queue.erase(queue.begin() + index);

        unsigned int j=1;
        unsigned int buf;
        min = 0x0fffffff;

        //usuniecie miasta docelowe z wektora s aby pozniej moc znalezc minimum
        //z sciezek od 0 do d.dst
        unsigned int newS = d.s-d.dst;
        unsigned int previous = 0;


        while(j <= newS){
            //
            if((newS & j) > 0){

                buf = array[newS + j*arraySize];

                int distanceTo = m.edge(log2(j)-1, log2(d.dst)-1);
                buf += distanceTo;

                if(min> buf){
                    min = buf;
                    previous = j;
                }
            }
            j<<=1;
        }

        array[d.s + d.dst*arraySize] = min; // + m.edge(log2(previous)-1, log2(d.dst)-1);
        paths[d.s + d.dst*arraySize] = previous;


        std::bitset<8> x(d.s);
        std::bitset<8> y(d.dst);
        std::cout<<"s: "<<d.s<<"("<<x<<") "<<d.dst<<"("<<y<<") d: "<<d.depth<<", v: "<<array[d.s + d.dst*arraySize]<<std::endl;

    }

    return array[s + dst * arraySize];
}

std::vector<int> Algorythms::refactorDynamicTSPPath(int start, int dst, unsigned int *paths, int arraySize)
{
    std::vector<int> path;

    int lastS = start;
    int lastDst = dst;

    while(lastDst > 0){

        int buf = paths[lastS + arraySize*lastDst];
        lastS &= ~lastDst;
        lastDst = buf;

        path.push_back(log2(lastDst)-1);


        std::bitset<8> x(lastS);
        std::bitset<8> y(lastDst);
        //std::cout<<"s: "<<lastS<<"("<<x<<") "<<lastDst<<"("<<y<<") "<<path[path.size()-1]<<std::endl;

    }

    std::vector<int> ret;

    //-2 bo na koncu path pojawia sie jeszcze (-1) znacznik konca sciezki w tablicy
    for(int i=path.size()-2;i>=0;--i){
        ret.push_back(path[i]);
    }

    return ret;
}

int Algorythms::log2(unsigned int a)
{
    int i=0;

    //if(a) a>>=1;

    while(a){
        a>>=1;
        ++i;
    }
    return i;
}

int Algorythms::countOnes(unsigned int a)
{
    int count = 0;
    int j=1;
    while(j){
        if((j & a) > 0){
            count ++;
        }

        j<<=1;
    }

    return count;

}

/*
int Algorythms::dynamicTSPDistanceF(int beginVert, unsigned int s, unsigned int dst, const NeighbourMatrix &m,
                                    unsigned int *array, unsigned int *paths, int arraySize, std::string string)
{
    int d = 0;
    int vert = 0;

    //jezeli s zawiera tylko jedna jedynke (jest potega dwojki) to znaczy ze
    //podana poszukiwana jest droga dlugosci 1
    if(s != 0 && (s & (s-1)) == 0 && s == dst) {
        d = m.edge(beginVert, log2(dst)-1);
        vert = -1;
    }
    else{

        //7(111) 2(10)
        int newS = s;
        int tDst = ~dst;
        //usun dst z aktualnego wektora miast przez ktore trzeba przejsc
        newS = newS & tDst; //usun bit miasta przeznaczenia z wektora miast do odwiedzenia

        //5(101)


        d = 0x0fffffff;
        int j=1;

        if(array[s + dst*arraySize] > 0){
            //return array[s + dst*arraySize];
        }


        for(unsigned int i=0;i<sizeof(s)*8;++i){

            //jezeli i-te miasto nalezy do wektora miast do odwiedzenia
            if((j & newS) > 0){

                //jezeli dana tablica zostala juz kiedys policzona
                int distance = dynamicTSPDistanceF(beginVert, newS, j, m, array, paths, arraySize, string + " ") +
                        m.edge(log2(j)-1,log2(dst)-1);

                if(d > distance){
                    d = distance;
                    vert = j;
                }
            }

            //aktualnie przetwarzany wierzcholek. j zawsze zawiera jedna jedynke
            j<<=1;
        }
    }

    std::bitset<8> x(s);
    std::bitset<8> y(dst);
    if(string.length() <= 2)
    std::cout<<string + "s: "<<s<<"("<<x<<") "<<dst<<"("<<y<<") d: "<<d<<" vert: "<<vert<<std::endl;

    array[s + dst*arraySize] = d;

    //zeby dosc do dst przechodzac przez wszystkie wierzcholki w s nalezy przejsc przez
    //vert (vert zawiera o jedna jedynke mniej niz s)
    paths[s + dst*arraySize] = vert;

    return d;
}*/
