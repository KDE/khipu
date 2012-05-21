#include "marchingcubes.h"
#include <stdio.h>

sMarching_Cube MarchingCubes::evaluar_cubo(sCubo cubo, superficie funcion)
{
    sMarching_Cube res;
    sPunto_f punto;
    unsigned short int val;


    
    res.centro = cubo.centro;
    res.medio_lado = cubo.medio_lado;

    
    punto.x = res.centro.x-res.medio_lado;
    punto.y = res.centro.y-res.medio_lado;
    punto.z = res.centro.z-res.medio_lado;
    res.vertices[0] = funcion(punto);

    punto.x = res.centro.x-res.medio_lado;
    punto.y = res.centro.y-res.medio_lado;
    punto.z = res.centro.z+res.medio_lado;
    res.vertices[1] = funcion(punto);

    punto.x = res.centro.x-res.medio_lado;
    punto.y = res.centro.y+res.medio_lado;
    punto.z = res.centro.z-res.medio_lado;
    res.vertices[2] = funcion(punto);

    punto.x = res.centro.x-res.medio_lado;
    punto.y = res.centro.y+res.medio_lado;
    punto.z = res.centro.z+res.medio_lado;
    res.vertices[3] = funcion(punto);

    punto.x = res.centro.x+res.medio_lado;
    punto.y = res.centro.y-res.medio_lado;
    punto.z = res.centro.z-res.medio_lado;
    res.vertices[4] = funcion(punto);

    punto.x = res.centro.x+res.medio_lado;
    punto.y = res.centro.y-res.medio_lado;
    punto.z = res.centro.z+res.medio_lado;
    res.vertices[5] = funcion(punto);

    punto.x = res.centro.x+res.medio_lado;
    punto.y = res.centro.y+res.medio_lado;
    punto.z = res.centro.z-res.medio_lado;
    res.vertices[6] = funcion(punto);

    punto.x = res.centro.x+res.medio_lado;
    punto.y = res.centro.y+res.medio_lado;
    punto.z = res.centro.z+res.medio_lado;
    res.vertices[7] = funcion(punto);

    
    res.tipo = 0;
    val=1;
    
    for(unsigned int i=0; i<8; i++)
    {
        if(res.vertices[i] > 0)
        {
            res.tipo += val;
        }
        val*=2;
    }
    return res;
}


QList<sCubo> MarchingCubes::breadth_rec(int cubos_lado, superficie funcion)
{
    sCubo cubo;
    sMarching_Cube m_cubo;
    bool salir = false;
    QList<sCubo> cubos;
    cubo.medio_lado = largo_mundo/(2*cubos_lado);

    for(int i=mundo.minX; i<=mundo.maxX; i++)
    {
        cubo.centro.x = (2*i+1)*cubo.medio_lado;
        for(int j=mundo.minY; j<=mundo.maxY; j++)
        {
            cubo.centro.y = (2*j+1)*cubo.medio_lado;
            for(int k=mundo.minZ; k<=mundo.maxZ; k++)
            {
                cubo.centro.z = (2*k+1)*cubo.medio_lado;
                m_cubo = evaluar_cubo(cubo,funcion);
                if(m_cubo.tipo != 0 && m_cubo.tipo != 255)
                {
                    
                    salir = true;
                    cubos.append(cubo);
                }
            }
        }
    }
    if(!salir && 2*cubo.medio_lado > min_grid)
    {
        cubos.append(breadth_rec(cubos_lado*2,funcion));
        
    }
    return cubos;
}

QList<sMarching_Cube> MarchingCubes::depth_rec(Octree *arbol, sNodo *nodo, superficie funcion)
{
    QList<sMarching_Cube> cubos;
    sMarching_Cube m_cubo;

    
    m_cubo = evaluar_cubo(nodo->cubo,funcion);

    if(m_cubo.tipo != 0 && m_cubo.tipo != 255)
    {
        
        if(m_cubo.medio_lado*2 > min_grid)
        {
            
            arbol->bajarNivel(nodo);
            for(unsigned int i=0; i<8; i++)
            {
                cubos.append(depth_rec(arbol,nodo->nodos[i],funcion));
            }
            
            
        }
        else
        {
            
            cubos.append(m_cubo);
        }
    }
    return cubos;
}

MarchingCubes::MarchingCubes(double min_grid, double arista_mundo, sLimitesEspacio limites)
{
    this->min_grid = arista_mundo;
    largo_mundo = arista_mundo;
    mundo = limites;
    while(this->min_grid > min_grid)
    {
        this->min_grid/=2;
    }
}

MarchingCubes::~MarchingCubes()
{
}

QList<sMarching_Cube> MarchingCubes::ejecutar(superficie funcion)
{
    QList<sMarching_Cube> cubos;
    QList<sCubo> encontrados;
    sCubo iterador;
    Octree *arbol;

    
    encontrados = breadth_rec(largo_mundo,funcion);
    
    if(encontrados.isEmpty())
    {
        return cubos;
    }
    printf("Encontrados: %d\n",encontrados.count());

    
    foreach(iterador, encontrados)
    {
        arbol = new Octree(iterador);
        cubos.append(depth_rec(arbol, arbol->get_raiz(),funcion));
        delete arbol;
    }

    
    return cubos;
}
