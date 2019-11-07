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
    std::cout<<" | "<<minDistance<<std::endl;
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

std::vector<int> Algorythms::dinamicTSP(int beginVert, const NeighbourMatrix &m, int &distance)
{
    int s=1;
    for(int i=0;i<m.size();++i){
        s<<=1;
        ++s;
    }

    unsigned int array[s*s];
    unsigned int paths[s*s];

    for(int i=0;i<s;++i){
        for(int j=0;j<s;++j){
            array[i+s*j] = 0;
            paths[i+s*j] = 0;
        }
    }

    std::vector<int> path;

    try{

        distance = dynamicTSPDistanceF(beginVert, s, 1, m, path, array, paths, s);

        //dolicz powrot do wierzchołka startowego
        distance += m.edge(path[path.size()-1], beginVert);
        path.push_back(beginVert);
    }
    catch(const char* err){
        std::cout<<err<<std::endl;
    }

    return path;
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

        if(used == false && (distance + m.edge(current, i)) < currentMinDistance){
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

int Algorythms::dynamicTSPDistanceF(int beginVert, unsigned int s, unsigned int dst, const NeighbourMatrix &m, std::vector<int>& path,
                                    unsigned int *array, unsigned int *paths, int arraySize)
{
    //jezeli s zawiera tylko jedna jedynke (jest potega dwojki) to znaczy ze
    //podana poszukiwana jest droga dlugosci 1
    if(s != 0 && (s & (s-1)) == 0 && s == dst) {
        std::cout<<"koniec"<<std::endl;
        return m.edge(beginVert, log2(dst)-1);
    }
    else{
        int newS = s;

        //usun dst z aktualnego wektora miast przez ktore trzeba przejsc
        newS = newS & (~dst); //usun bit miasta przeznaczenia z wektora miast do odwiedzenia

        int minDistance = 0x0fffffff;
        int vert = -1;
        int j=1;


        for(unsigned int i=0;i<sizeof(s)*8;++i){

            //jezeli i-te miasto nalezy do wektora miast do odwiedzenia
            if((j & newS) > 0){
                std::vector<int> p;

                std::bitset<8> x(s);
                std::cout<<"s: "<<s<<"("<<x<<") "<<dst<<std::endl;
                std::cout<<"j "<<j<<" "<<log2(j)<<" "<<log2(dst)<<" "<<newS<<std::endl;

                int d = array[newS + j*arraySize];

                //jezeli dana tablica zostala juz kiedys policzona
                if(d == 0){
                    d = dynamicTSPDistanceF(beginVert, newS, j, m, p, array, paths, arraySize);
                    array[newS + j*arraySize] = d;
                    paths[s + j*arraySize] = newS;
                }

                d += m.edge(log2(j)-1,log2(dst)-1);

                if(minDistance > d){
                    minDistance = d;
                    vert = log2(j) -1;
                    path = p;
                }
            }

            //aktualnie przetwarzany wierzcholek. j zawsze zawiera jedna jedynke
            j<<=1;
        }

        //std::cout<<vert<<std::endl;
        path.insert(path.end(), vert);
        return minDistance;
    }
}

int Algorythms::log2(int a)
{
    int i=0;
    if(a>0) a >>= 1;

    while(a){
        a>>=1;
        ++i;
    }
    return i;
}

