#include "NeighbourMatrix.h"


NeighbourMatrix::NeighbourMatrix(int verts)
{

    this->verts = verts;
    data = new int*[this->verts];

    for(int i=0;i<this->verts;++i){
        data[i] = new int[verts];
    }

    for(int i=0;i<this->verts;++i){
        for(int j=0;j<verts;++j){
            data[i][j]=0;
        }
    }

}

NeighbourMatrix::~NeighbourMatrix()
{

    for(int i=0;i<verts;++i){
        if(data[i])
            delete[] data[i];
    }

    if(data)
        delete[] data;
}

bool NeighbourMatrix::addEdge(int vert1, int vert2, int factor, bool bidirectional)
{
    bool exist = false;

    if(vert1 < verts && vert2 < verts){
        if(data[vert1][vert2] != 0) exist = true;
        else{
            data[vert1][vert2] = factor;

            if(bidirectional)
                data[vert2][vert1] = factor;
        }
    }

    return exist;
}

int NeighbourMatrix::size() const
{
    return verts;
}

void NeighbourMatrix::init(int verts)
{
    for(int i=0;i<this->verts;++i){
        delete data[i];
    }
    delete data;

    this->verts = verts;
    data = new int*[this->verts];

    for(int i=0;i<this->verts;++i){
        data[i] = new int[verts];
    }

    for(int i=0;i<this->verts;++i){
        for(int j=0;j<verts;++j){
            data[i][j]=0;
        }
    }
}

int NeighbourMatrix::edge(int vert1, int vert2) const
{
    if(vert1 == vert2)
        return 0;

    if(vert1 < verts && vert2 < verts){
        return data[vert1][vert2];
    }

    throw("Przekroczenie zakresu. Nie istnieje podany wierzcholek. w int NeighbourMatrix::edge(int vert1, vert2)");
}

int NeighbourMatrix::edge(int vert1, int vert2)
{
    if(vert1 < verts && vert2 < verts){
        return data[vert1][vert2];
    }

    throw("Przekroczenie zakresu. Nie istnieje podany wierzcholek. w int NeighbourMatrix::edge(int vert1, vert2)");
}

std::vector<int> NeighbourMatrix::neighbours(int vert) const
{
    std::vector<int> nei;
    for(int i=0;i<size();++i){
        if(data[vert][i] > 0 && vert!=i)
            nei.push_back(i);
    }

    return nei;
}

std::vector<int> NeighbourMatrix::edges() const
{
    std::vector<int> edg;
    for(int i=0;i<verts;++i){
        for(int j=0;j<verts;++j){
            if(data[i][j] > 0 && i != j){
                edg.push_back(i);
                edg.push_back(j);
            }
        }
    }

    return edg;
}

int NeighbourMatrix::sumMinimumEdgesOfVerts(const std::vector<int> &verts) const
{
    //zusumuj minima
    int ret = 0;
    for(int i:verts){
        int minimum = 0x0fffffff;

        //sprawdz krawedz minimalnej dlugosci wychodzacej z wierzcholka i
        for(int j=0;j<size();++j){
            if(minimum  > data[i][j] && i!=j){
                minimum = data[i][j];
            }
        }

        ret += minimum;
    }

    return ret;
}

std::vector<int> NeighbourMatrix::complement(const std::vector<unsigned int> &verts) const
{
    std::vector<int> ret;
    for(int i=0;i<size();++i){
        bool contains = false;

        for(int j:verts){
            if(j == i){
                contains = true;
                break;
            }
        }

        if(contains == false)
            ret.push_back(i);
    }

    return ret;
}

std::vector<int> NeighbourMatrix::complement(const std::vector<int> &verts) const
{
    std::vector<int> ret;
    for(int i=0;i<size();++i){
        bool contains = false;

        for(int j:verts){
            if(j == i){
                contains = true;
                break;
            }
        }

        if(contains == false)
            ret.push_back(i);
    }

    return ret;
}

int NeighbourMatrix::calcPathDistance(const std::vector<int> &verts, int endVert) const
{
    int distance = 0;
    for(int i=1;i<verts.size();++i){
        distance += edge(verts[i-1], verts[i]);
    }

    //jezel endVert istnieje wtedy dodaj powrot
    if(endVert >= 0){
        distance+=edge(verts[verts.size()-1], endVert);
    }

    return distance;
}

std::ostream &operator <<(std::ostream &str, const NeighbourMatrix &matrix)
{
    str<<std::setw(4)<<"\\";

    for(int i=0;i<matrix.size();++i){
        str<<std::setw(4)<<i;
    }

    str<<"\n";

    for(int i=0;i<matrix.size();++i){

        str<<std::setw(3)<<i<<".";

        for(int j=0;j<matrix.size();++j){
            str<<std::setw(4)<<matrix.edge(i,j);
        }
        str<<"\n";
    }

    return str;
}
