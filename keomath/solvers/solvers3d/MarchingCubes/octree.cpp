#include "octree.h"
#include <stdlib.h>


Octree::Octree(double largo_mundo)
{
    raiz = new sNodo;
    raiz->cubo.centro.x = largo_mundo/2;
    raiz->cubo.centro.y = largo_mundo/2;
    raiz->cubo.centro.z = largo_mundo/2;
    raiz->cubo.medio_lado = largo_mundo/2;
    for(unsigned int i=0; i<8; i++)
    {
        raiz->nodos[i]=NULL;
    }
}

Octree::Octree(sCubo cubo)
{
    raiz = new sNodo;
    raiz->cubo = cubo;
    for(unsigned int i=0; i<8; i++)
    {
        raiz->nodos[i]=NULL;
    }
}
Octree::~Octree()
{
    borrar_rec(raiz);
}

void Octree::inicializar_nodos(sNodo* padre)
{
    for(unsigned int i=0; i<8; i++)
    {
        padre->nodos[i] = new sNodo;
        padre->nodos[i]->cubo.medio_lado = padre->cubo.medio_lado/2;
        for(unsigned int j=0; j<8; j++)
        {
            padre->nodos[i]->nodos[j]=NULL;
        }
    }
    padre->nodos[0]->cubo.centro.x = padre->cubo.centro.x-padre->cubo.medio_lado/2;
    padre->nodos[0]->cubo.centro.y = padre->cubo.centro.y-padre->cubo.medio_lado/2;
    padre->nodos[0]->cubo.centro.z = padre->cubo.centro.z-padre->cubo.medio_lado/2;

    padre->nodos[1]->cubo.centro.x = padre->cubo.centro.x-padre->cubo.medio_lado/2;
    padre->nodos[1]->cubo.centro.y = padre->cubo.centro.y-padre->cubo.medio_lado/2;
    padre->nodos[1]->cubo.centro.z = padre->cubo.centro.z+padre->cubo.medio_lado/2;

    padre->nodos[2]->cubo.centro.x = padre->cubo.centro.x-padre->cubo.medio_lado/2;
    padre->nodos[2]->cubo.centro.y = padre->cubo.centro.y+padre->cubo.medio_lado/2;
    padre->nodos[2]->cubo.centro.z = padre->cubo.centro.z-padre->cubo.medio_lado/2;

    padre->nodos[3]->cubo.centro.x = padre->cubo.centro.x-padre->cubo.medio_lado/2;
    padre->nodos[3]->cubo.centro.y = padre->cubo.centro.y+padre->cubo.medio_lado/2;
    padre->nodos[3]->cubo.centro.z = padre->cubo.centro.z+padre->cubo.medio_lado/2;

    padre->nodos[4]->cubo.centro.x = padre->cubo.centro.x+padre->cubo.medio_lado/2;
    padre->nodos[4]->cubo.centro.y = padre->cubo.centro.y-padre->cubo.medio_lado/2;
    padre->nodos[4]->cubo.centro.z = padre->cubo.centro.z-padre->cubo.medio_lado/2;

    padre->nodos[5]->cubo.centro.x = padre->cubo.centro.x+padre->cubo.medio_lado/2;
    padre->nodos[5]->cubo.centro.y = padre->cubo.centro.y-padre->cubo.medio_lado/2;
    padre->nodos[5]->cubo.centro.z = padre->cubo.centro.z+padre->cubo.medio_lado/2;

    padre->nodos[6]->cubo.centro.x = padre->cubo.centro.x+padre->cubo.medio_lado/2;
    padre->nodos[6]->cubo.centro.y = padre->cubo.centro.y+padre->cubo.medio_lado/2;
    padre->nodos[6]->cubo.centro.z = padre->cubo.centro.z-padre->cubo.medio_lado/2;

    padre->nodos[7]->cubo.centro.x = padre->cubo.centro.x+padre->cubo.medio_lado/2;
    padre->nodos[7]->cubo.centro.y = padre->cubo.centro.y+padre->cubo.medio_lado/2;
    padre->nodos[7]->cubo.centro.z = padre->cubo.centro.z+padre->cubo.medio_lado/2;
}

void Octree::borrar_rec(sNodo* nodo)
{
    if(nodo == NULL)
    {
        return;
    }
    for(unsigned int i=0; i<8; i++)
    {
        borrar_rec(nodo->nodos[i]);
    }
    delete nodo;
}


void Octree::crear_rec(sNodo* nodo, unsigned int nivel_actual, unsigned int nivel_max)
{
    if(nivel_actual > nivel_max)
    {
        return;
    }
    inicializar_nodos(nodo);
    for(unsigned int i=0; i<8; i++)
    {
        crear_rec(nodo->nodos[i],nivel_actual+1,nivel_max);
    }
}

sNodo* Octree::get_raiz()
{
    return raiz;
}

void Octree::crearNivel(unsigned int nivel)
{
    crear_rec(raiz,0,nivel);
}

void Octree::bajarNivel(sNodo* nodo)
{
    inicializar_nodos(nodo);
}

void Octree::borrarHijos(sNodo* padre)
{
    for(unsigned int i=0; i<8; i++)
    {
        borrar_rec(padre->nodos[i]);
        padre->nodos[i] = NULL;
    }
}
