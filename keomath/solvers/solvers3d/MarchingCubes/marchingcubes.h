#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H

#include <QList>
#include "octree.h"

struct sMarching_Cube
{
    sPunto_f centro;
    double medio_lado;
    unsigned short int tipo;
    double vertices[8];
};

typedef double (*superficie)(sPunto_f punto);

class MarchingCubes
{
private:
    
    double largo_mundo;
    double min_grid;
    sLimitesEspacio mundo;
    
    sMarching_Cube evaluar_cubo(sCubo cubo, superficie funcion);

    
    QList<sCubo> breadth_rec(int cubos_lado, superficie funcion);

    
    QList<sMarching_Cube> depth_rec(Octree *arbol, sNodo *nodo, superficie funcion);

public:
    
    
    MarchingCubes(double min_grid, double arista_mundo, sLimitesEspacio limites);

    
    ~MarchingCubes();

    
    QList<sMarching_Cube> ejecutar(superficie funcion);
};

#endif 
