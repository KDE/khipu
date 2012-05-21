#ifndef OCTREE_H
#define OCTREE_H


struct sPunto_f
{
    double x,y,z;
};

struct sCubo
{
    sPunto_f centro;
    double medio_lado;
};

struct sLimitesEspacio
{
    int minX;
    int maxX;
    int minY;
    int maxY;
    int minZ;
    int maxZ;
};


struct sNodo
{
    sCubo cubo;
    sNodo* nodos[8];
};

class Octree
{
private:
    sNodo* raiz;

    
    void inicializar_nodos(sNodo* padre);

    void borrar_rec(sNodo* nodo);
    void crear_rec(sNodo* nodo, unsigned int nivel_actual, unsigned int nivel_max);
public:
    Octree(double largo_mundo);
    Octree(sCubo cubo);
    ~Octree();

    sNodo* get_raiz();
    void crearNivel(unsigned int nivel);
    void bajarNivel(sNodo* nodo);
    void borrarHijos(sNodo* padre);

};

#endif 
