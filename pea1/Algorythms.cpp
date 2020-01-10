#include "Algorythms.h"
#include <bitset>
#include <cmath>
#include <algorithm>

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

std::vector<int> Algorythms::tabuSearchTSP(int beginVert, const NeighbourMatrix &m, int &distance, int iterations, int tabuLength, int criticalLimit)
{
    int criticalEvents = 0;
    std::vector<std::vector<char>> tabu;

    for(int i=0;i<m.size();++i){
        tabu.push_back(std::vector<char>(m.size(), 0));
    }


    distance = INT32_MAX;

    std::vector<int> minPath;
    std::vector<int> returnPath;

    //najlepsza sciezka w jednej iteracji
    std::vector<int> bestCurrentPath(minPath.size(),0);

    //sprawdzana sciezka
    std::vector<int> path;

    //minPath.push_back(0);
    for(int i=0; i<m.size();++i){
        if(i != beginVert){
            minPath.push_back(i);
        }
    }

    std::random_shuffle(minPath.begin(), minPath.end());
    minPath.insert(minPath.begin(), beginVert);

    distance = m.calcPathDistance(minPath, beginVert);

    for(int i=0;i<iterations;++i){

        bestCurrentPath = minPath;
        int bestCurrentDistance = distance;
        path = minPath;

        int bestJ, bestK;
        for(int j=1;j<minPath.size();++j){
            ++i;
            for(int k=1;k<minPath.size();++k){



                //jezeli ruch nie jest zakazany to zamien
                if(tabu[j][k] == 0){
                    int buf = minPath[j];
                    minPath[j] = minPath[k];
                    minPath[k] = buf;

                    int d = m.calcPathDistance(minPath, beginVert);

                    if(d < bestCurrentDistance){
                        bestCurrentDistance = d;
                        bestCurrentPath = minPath;
                        bestJ = j;
                        bestK = k;
                    }


                    buf = minPath[j];
                    minPath[j] = minPath[k];
                    minPath[k] = buf;
                }else{
                    tabu[j][k]--;
                }
            }
        }

        tabu[bestJ][bestK] = tabuLength;

        if(bestCurrentDistance < distance){
            distance = bestCurrentDistance;
            minPath = bestCurrentPath;
            returnPath = bestCurrentPath;
        }else{
            if(criticalLimit <= criticalEvents){
                //przeladuj liste tabu
                for(unsigned int j=1;j<minPath.size();++j){
                    for(unsigned int k=1;k<minPath.size();++k){
                        tabu[j][k] = 0;
                    }
                }

                minPath.erase(minPath.begin());
                std::random_shuffle(minPath.begin(), minPath.end());
                minPath.insert(minPath.begin(), beginVert);
                //std::cout<<"blad krytyczny"<<std::endl;
                //std::cout<<minPath<<std::endl;
                criticalEvents = 0;
            }
            else criticalEvents++;
        }
    }

    //uwzglednij powrot
    //distance = m.calcPathDistance(minPath, beginVert);
    returnPath.push_back(beginVert);
    distance+=m.edge(returnPath[returnPath.size()-1], beginVert);

    /*int s = 0;
    for(int i=0;i<returnPath.size()-1;++i){
        std::cout<<m.edge(returnPath[i], returnPath[i+1])<<" + ";
        s+=m.edge(returnPath[i], returnPath[i+1]);
    }

    std::cout<<" = "<<distance<<" ("<<s<<")\n";*/

    return returnPath;
}

std::vector<int> Algorythms::simAnnealing(int beginVert, const NeighbourMatrix &m, int &distance, double TMax, double tempFactor, int iterations)
{
    int criticlaLimit=100, criticalEvents=criticlaLimit;

    std::vector<int> minPath;
    std::vector<int> retPath(m.size());
    int retDistance = 0x0fffffff;

    double t = TMax;

    for(int i=0; i<m.size();++i){
        if(i != beginVert){
            minPath.push_back(i);
        }
    }

    std::random_shuffle(minPath.begin(), minPath.end());
    minPath.insert(minPath.begin(), beginVert);

    distance = m.calcPathDistance(minPath, beginVert);

    while(iterations > 0){

        int x = (rand()%(m.size()-1)) + 1;
        int y = (rand()%(m.size()-1)) + 1;

        std::vector<int> p = minPath;
        std::swap(p[x], p[y]);

        int d=m.calcPathDistance(p, beginVert);
        int delta = d - distance;

        if(delta < 0){
            minPath = p;
            distance = d;
            criticalEvents=criticlaLimit;
        }else{
            double r = (rand()%100000)/100000.0;
            double e = exp((double)-delta/t);
            //std::cout<<"d: "<<d<<" delta: "<<delta<<" r: "<<r<<" t: "<<t<<" e: "<<e<<std::endl;
            if(r < e){
                //std::cout<<"done"<<std::endl;
                minPath = p;
                distance = d;
                criticalEvents=criticlaLimit;
            }else{
                criticalEvents--;
            }
        }

        if(distance < retDistance) {
            retPath = minPath;
            retDistance = distance;
        }

        t = tempFactor * t;
        iterations--;
    }

    distance = m.calcPathDistance(retPath, beginVert);
    retPath.push_back(beginVert);
    return retPath;

}

std::vector<int> Algorythms::generic(int beginVert, const NeighbourMatrix &m, int &distance, int seconds, int populationSize, float mutationProb)
{
    //generate first generation

    //while time < seconds
        //select best
        //reproduce new generation
        //mutate

    distance = 0x0ffffff;
    std::vector<int> minPath;
    std::vector<std::vector<int> > generation(populationSize);

    for(int i=0;i<generation.size();++i){
        std::vector<int> unit(m.size());

        unit[0] = beginVert;
        for(int j=1;j<m.size();++j){
            if(j != beginVert)
                unit[j] = j;
            else unit[j] = 0;
        }

        std::random_shuffle(unit.begin()+1, unit.end());
        std::cout<<unit<<std::endl;
        generation[i] = unit;
    }


    std::vector<int> costs(populationSize);
    Timer tm;
    tm.start();

    while (tm.elapsed() < seconds) {
        std::cout<<tm.elapsed()<<" "<<seconds<<std::endl;
        //caculate costs
        for(unsigned int i=0;i<generation.size();++i){
            costs[i] = m.calcPathDistance(generation[i],beginVert);
        }

        //order generation by cost
        for(int i=0;i<generation.size()-1;++i){
            for(int j=0;j<generation.size()-i-1;++j){
                if(costs[j]>costs[j+1]){
                    std::swap(costs[j], costs[j+1]);
                    std::swap(generation[j], generation[j+1]);
                }
            }
        }


        if(distance > costs[0]){
            distance = costs[0];
            minPath = generation[0];
        }

        //reproduce

        //rozne algorytmy reprodukcji
        //Algorythms::genericEdgeCrossover(generation, costs);
        Algorythms::genericPMXCrossover(generation, costs);

        //rozne algorytmy mutacji
        //Algorythms::genericReplaceMutation(generation, mutationProb);
        Algorythms::genericReplaceStringMutation(generation, mutationProb);

        std::cout<<std::endl;
        for(int j=0;j<generation.size();++j){
            std::cout<<generation[j]<<std::endl;

        }

        tm.stop();
    }

    minPath.push_back(beginVert);
    return minPath;
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

void Algorythms::genericEdgeCrossover(std::vector<std::vector<int> > &generation, std::vector<int> &costs)
{
    std::vector<std::vector<int> > newGeneration(generation.size());

    //calculate generation avg
    /*
    int avg = 0;
    for(int c:costs)
        avg+=c;
    avg/=costs.size();
    */

    //parents are 1 and 2
    int p1 = 0;
    int p2 = 1;
    std::vector<std::vector<int> > connections(generation[0].size());

    for(int j=1;j<connections.size();++j){
        std::vector<int> neighbours;

        //dodaj numer wierzcholka aby mozna bylo usuwac connections[i] podczas przetwarzania
        neighbours.push_back(j);

        //wygeneruj listy sasiadow dla wszystkich wierzcholkow. Sasiadow wez z dwoch najlepszych
        //(indeksy 0 i 1)
        for(int k=1;k<generation[p1].size();++k){
            if(j == generation[p1][k]){
                neighbours.push_back(generation[p1][(k+1)%connections.size()]);
                int index = k-1;
                if(index < 1) index = connections.size()-1;
                neighbours.push_back(generation[p1][index]);
            }
        }

        for(int k=1;k<generation[p2].size();++k){
            if(j == generation[p2][k]){
                int v = generation[p2][(k+1)%connections.size()];
                if(v != neighbours[0] && v != neighbours[1])
                    neighbours.push_back(v);
                int index = k-1;
                if(index < 1) index = connections.size()-1;
                v = generation[p2][index];
                if(v != neighbours[0] && v != neighbours[1])
                    neighbours.push_back(v);
            }
        }

        connections[j] = neighbours;
    }

    //std::cout<<std::endl;

    for(int i=0;i<generation.size();++i){

        std::vector<std::vector<int> > c(connections);
        std::vector<int> child(c.size());
        /*for(int j=0;j<connections.size();++j){
            std::cout<<connections[j]<<std::endl;
        }*/

        //rozpocznij tworzenie nowego potomka

        //wylosuj startowy wierzcholek
        int randIndex = (rand()%(c.size()-1)) + 1;

        int v = c[randIndex][0];

        for(int z=1;z<child.size();++z){

            child[z] = v;

            //usun uzyty wierzcholek
            for(int j=1;j<c.size();++j){
                if(c[j][0] == v)
                   c.erase(c.begin()+j);
            }

            //usun v z list sasiadow
            for(int j=1;j<c.size();++j){
                for(int k=0;k<c[j].size();++k)
                    if(c[j][k] == v)
                        c[j].erase(c[j].begin()+k);
            }

            //wybierz nowe v
            int indexOfmin = 1;
            int min = c[1].size();
            for(int j=1;j<c.size();++j){
                if(min > c[j].size()){
                    indexOfmin = j;
                    min = c[j].size();
                }
            }

            v = c[indexOfmin][0];
            //std::cout<<c.size()<<std::endl;
        }
        //std::cout<<child<<std::endl;
        newGeneration[i] = child;
    }

    generation = newGeneration;
}

void Algorythms::genericPMXCrossover(std::vector<std::vector<int> > &generation, std::vector<int> &costs)
{
    std::vector<int> p1(generation[0]);
    std::vector<int> p2(generation[1]);

    for(int i=0;i<generation.size();){
        int i1 = rand()%(generation[i].size()-1) +1;
        int i2 = rand()%(generation[i].size()-1) +1;

        if(i1>i2) std::swap(i1,i2);

        std::vector<int> r(p1);
        std::vector<int> q(p2);


        //std::cout<<"1."<<r<<" "<<q<<"|"<<i1<<" "<<i2<<std::endl;

        //zamien sekcje dopasowania
        for(int j=i1;j<i2;++j){
            std::swap(r[j], q[j]);
        }

        //std::cout<<"2."<<r<<" "<<q<<"|"<<i1<<" "<<i2<<std::endl;

        for(int k=1;k<p1.size();++k){
            if(k == i1) k=i2;
            for(int j=i1;j<i2;++j){
                if(r[k] == r[j]){

                    //znajdz brakujacy znak
                    for(int z=0;z<p1.size();++z){
                        bool ok=true;
                        for(int y=0;y<p1.size();++y){
                            if(q[z] == r[y]){
                                ok = false;
                                break;
                            }
                        }

                        if(ok){
                            r[k] = q[z];
                            break;
                        }
                    }
                }
            }
        }

        for(int k=1;k<p1.size();++k){
            if(k == i1) k=i2;
            for(int j=i1;j<i2;++j){
                if(q[k] == q[j]){

                    //znajdz brakujacy znak
                    for(int z=0;z<p1.size();++z){
                        bool ok=true;
                        for(int y=0;y<p1.size();++y){
                            if(r[z] == q[y]){
                                ok = false;
                                break;
                            }
                        }

                        if(ok){
                            q[k] = r[z];
                            break;
                        }
                    }
                }
            }
        }

        //std::cout<<"3."<<r<<" "<<q<<"|"<<i1<<" "<<i2<<std::endl;


        generation[i] = r;
        ++i;

        if(i<generation.size()){
            generation[i] = q;
            ++i;
        }
    }
}

void Algorythms::genericReplaceMutation(std::vector<std::vector<int> > &generation, float probability)
{
    for(int i=0;i<generation.size();++i){
        float p = (rand()%10000)/10000.0;

        if(p > probability){
            int x1 = (rand() % (generation[i].size()-1)) +1;
            int x2 = (rand() % (generation[i].size()-1)) + 1;

            std::swap(generation[i][x1],generation[i][x2]);
        }
    }
}

void Algorythms::genericReplaceStringMutation(std::vector<std::vector<int> > &generation, float probability)
{
    for(int i=0;i<generation.size();++i){
        float p = (rand()%10000)/10000.0;

        if(p > probability){
            int x1 = (rand() % (generation[i].size()-1)) +1;
            int x2 = (rand() % (generation[i].size()-1)) + 1;

            if(x1>x2) std::swap(x1,x2);

            std::vector<int> temp(generation[i]);

            int index = x1;

            //fragment 0..x1 pozostaje bez zmian

            for(int j=x2;j<temp.size();++j){
                generation[i][index] = temp[j];
                index++;
            }

            for(int j=x1;j<x2;++j){
                generation[i][index] = temp[j];
                index++;
            }

            //std::cout<<"----"<<std::endl;
            //std::cout<<temp<<" | "<<x1<<" "<<x2<<std::endl;
            //std::cout<<generation[i]<<std::endl;
        }
    }
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
