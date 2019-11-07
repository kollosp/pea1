#include "Algorythms.h"
#include <bitset>

Algorythms::Algorythms()
{

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

    //7(111)/4(100)
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
    s++;
    unsigned int *array = new unsigned int[s*s];
    unsigned int *paths = new unsigned int[s*s];

    for(int i=0;i<s;++i){
        for(int j=0;j<s;++j){
            array[i+s*j] = 0;
            paths[i+s*j] = 0;
        }
    }

    try{

        distance = dynamicTSPDistanceF(beginVert, s-1, 1, m, array, paths, s);


    }
    catch(const char* err){
        std::cout<<err<<std::endl;
    }

    /*std::vector<int> p = refactorDynamicTSPPath(s, 1, paths, s);

    //dolicz powrot do wierzchołka startowego
    distance += m.edge(p[p.size()-1], beginVert);
    p.push_back(beginVert);
*/
/*    for(int i=0;i<s;++i){
        for(int j=0;j<s;++j){
            std::cout<<array[i+s*j]<<" ";
        }
        std::cout<<std::endl;
    }*/

    delete []array;
    delete []paths;

    return std::vector<int>();
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

        if(newS == 0){
            array[d.s + d.dst*arraySize] = m.edge(beginVert, log2(d.dst)-1);
            //std::cout<<"koniec "<<beginVert<<" "<<d.dst<<" "<<log2(d.dst)-1<<" "<<m.edge(beginVert, log2(d.dst)-1)<<std::endl;
        }

        unsigned int j=1;
        while(j <= newS){

            if((newS & j) > 0){
                if(array[newS + j*arraySize] == 0){
                    unsigned int depth = countOnes(newS);
                    q.push_back(Destination(newS,j, depth));

                    if(depth > 1)
                        queue.push_back(Destination(newS,j, depth));

                    array[newS + j*arraySize]  = -1;
                }
            }
            j<<=1;
        }

//        std::bitset<8> x(d.s);
//        std::bitset<8> y(d.dst);
//        std::cout<<"s: "<<d.s<<"("<<x<<") "<<d.dst<<"("<<y<<") d: "<<d.depth<<std::endl;

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
        unsigned int newS = d.s-d.dst;
        unsigned int previous = 0;

        while(j <= newS){
            if((newS & j) > 0){
                buf = array[newS + j*arraySize];

                //std::cout<<newS<<" "<<j*arraySize<<" "<<buf<<std::endl;
                if(min > buf){
                    min = buf;
                    previous = j;
                }
            }
            j<<=1;
        }

        array[d.s + d.dst*arraySize] = min + m.edge(log2(previous)-1, log2(d.dst)-1);

   }

    return array[s + dst * arraySize];
}

std::vector<int> Algorythms::refactorDynamicTSPPath(int start, int dst, unsigned int *paths, int arraySize)
{
    std::vector<int> path;

    int lastS = start;
    int lastDst = dst;

    while(lastDst > 0){

        int buf= paths[lastS + arraySize*lastDst];
        lastS &= ~lastDst;
        lastDst = buf;

        path.push_back(log2(lastS)-1);
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
