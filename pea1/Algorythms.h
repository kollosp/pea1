#ifndef ALGORYTHMS_H
#define ALGORYTHMS_H

#include "NeighbourMatrix.h"
#include "Timer.h"

//TSP15 B&B  61.5751  [0 12 1 14 8 4 6 2 11 13 9 7 5 3 10 0]


class Algorythms
{
    class BruteforceData{
    public:
        std::vector<int> path;
        int waigth;
    };


    static BruteforceData bruteforceRecursive(int current, std::vector<int> path, int distance, const NeighbourMatrix &m,
                                              std::vector<int>&currentMinPath, int& currentMinDistance);




    static BruteforceData bruteforceRecursiveLimited(int current, std::vector<int> path, int distance, const NeighbourMatrix &m,
                                              std::vector<int>&currentMinPath, int& currentMinDistance);
    /**
     * @brief dynamicTSPDistanceF
     * @param beginVert
     * @param s wierzcholki przez ktore nalezy przejsc do wierzcholka docelowego
     * @param dst
     * @param m
     * @param path
     * @param array tablica zawierajaca tyle elemntow ile istnieje sciezek czyli
     * @param arraySize - w c++ nie mozna przekazac tablicy dwuwymiarowej bez podania przynajmniej jednego wymiaru.
     *              dlatego tablica dwuwymiarowa zostala zastapiona jednowymiarowa z indeksowaniem [i + size*j] i jest indeksem wiersza
     *              a j jest indeksem kolumny
     * @return
     */
    static int dynamicTSPDistanceF(int beginVert, unsigned int s, unsigned int dst,
                                   const NeighbourMatrix &m, unsigned int *array, unsigned int *paths, int arraySize, std::string string="");
    static std::vector<int> refactorDynamicTSPPath(int start, int dst, unsigned int *paths, int arraySize);

    static void genericEdgeCrossover(std::vector<std::vector<int> > &generation, std::vector<int> &costs);
    static void genericPMXCrossover(std::vector<std::vector<int> > &generation, std::vector<int> &costs);

    /**
     * Funkcja zamienia dwa miasta ze soba
     * @brief genericReplaceMutation
     * @param generation
     * @param probability
     */
    static void genericReplaceMutation(std::vector<std::vector<int> > &generation, float probability);


    /**
     * Funkcja wydziela fragment genu <x1,x2> nastepnie skleja ciagi <0,x1) i (x2, n> i po n doklada <x1,x2>.
     * @brief genericReplaceStringMutation
     * @param generation
     * @param probability
     */
    static void genericReplaceStringMutation(std::vector<std::vector<int> > &generation, float probability);

public:
    static int log2(unsigned int a);
    static int countOnes(unsigned int a);

    Algorythms();

    template<typename T>
    static void fromFile(T& graph, const std::string& file, bool forceBidirectional=false)
    {
        std::ifstream f;
        f.open(file, std::ios::in);

        if(f.is_open()){
            int size, edges;
            f>>size>>edges;

            graph.init(size);

            for(int i=0;i<edges;++i){
                int vert1, vert2, factor;
                f>>vert1>>vert2>>factor;

                graph.addEdge(vert1, vert2, factor, forceBidirectional);
            }
        }
    }

    template<typename T>
    static void fromFileMatrixStyle(T& graph, const std::string& file, bool forceBidirectional=true)
    {
        std::ifstream f;
        f.open(file, std::ios::in);

        if(f.is_open()){
            int size;
            f>>size;

            graph.init(size);

            for(int j=0;j<size;++j){
                for(int i=0;i<size;++i){
                    int factor;
                    f>>factor;

                    graph.addEdge(j, i, factor, forceBidirectional);
                }
            }
        }
    }

    template<typename T1,typename T2>
    static void copy(const T1& src, T2& dst){
        dst.init(src.size());

        std::vector<int> edges = src.edges();

        for(unsigned int i=0;i<edges.size();i+=2){
            dst.addEdge(edges[i], edges[i+1], src.edge(edges[i], edges[i+1]), false);
        }
    }

    template<typename T>
    static void toFile(const T& graph, const std::string& file){
        std::ofstream f;
        f.open(file, std::ios::trunc);

        std::vector<int> edges = graph.edges();

        if(f.is_open()){
            f<<graph.size()<<" "<<edges.size()/2<<std::endl;

            for(unsigned int i=0;i<edges.size();i+=2){
                f<<edges[i]<<" "<<edges[i+1]<<" "<<graph.edge(edges[i], edges[i+1])<<std::endl;
            }
        }
    }

    template<typename T>
    static void fillGraph(T &graph, int fillFactor){
        int edges = ((float)graph.size()*(graph.size()-1)) * fillFactor/100.0;
        //edges /= 2;
        int size = graph.size();

        for(int i=0;i<edges;){
            int v1 = rand()%size;
            int v2 = rand()%size;
            if(v1!=v2)
                if(graph.addEdge(v1,v2, (rand()%9)+1, false) == false) ++i;
        }
    }

    static std::vector<unsigned int> bruteforceTSPIter(int beginVert, const NeighbourMatrix &m, int &distance);
    static std::vector<unsigned int> BAndBTSPIter(int beginVert, const NeighbourMatrix &m, int &distance);


    static std::vector<int> bruteforceTSP(int beginVert, const NeighbourMatrix &m, int &distance);
    static std::vector<int> bruteforceTSPLimited(int beginVert, const NeighbourMatrix &m, int &distance);
    static std::vector<int> greedyTSP(int beginVert, const NeighbourMatrix &m, int &distance);
    static std::vector<int> dynamicTSP(int beginVert, const NeighbourMatrix &m, int &distance);
    static std::vector<int> tabuSearchTSP(int beginVert, const NeighbourMatrix &m, int &distance, int iterations=100000, int tabuLength=20, int criticalLimit=20);
    static std::vector<int> simAnnealing(int beginVert, const NeighbourMatrix &m, int &distance, double TMax, double tempFactor, int iterations);


    static std::vector<int> generic(int beginVert, const NeighbourMatrix &m, int &distance, int seconds, int generationCount, float mutationProb, int &generationCounter);
};

template <typename T>
std::ostream& operator <<(std::ostream&str, std::vector<T> obj){
    for(unsigned int i=0;i<obj.size();++i){
        str<<obj[i]<<" ";
    }

    return str;
}

#endif // ALGORYTHMS_H
