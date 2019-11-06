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
