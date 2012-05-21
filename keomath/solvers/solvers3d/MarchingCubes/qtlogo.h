

#ifndef QTLOGO_H
#define QTLOGO_H

#include <QObject>
#include <QColor>
#include <QVector3D>

#include "marchingcubes.h"

class Patch;
struct Geometry;


double func_esfera(sPunto_f punto);
double func_elipsoide(sPunto_f punto);
double func_cilindro(sPunto_f punto);
double func_hiperboloide_1hoja(sPunto_f punto);
double func_hiperboloide_2hojas(sPunto_f punto);
double func_paraboloide_hiperbolico(sPunto_f punto);
double func_paraboloide_eliptico(sPunto_f punto);
double func_cono_eliptico(sPunto_f punto);

enum ejescentrales
{
    sobreX,
    sobreY,
    sobreZ
};

struct sArista
{
    QVector3D corte;
    unsigned int vertices[2];
};


class QtLogo : public QObject
{
public:
    QtLogo(QObject *parent, superficie funcion);
    ~QtLogo();
    void setColor(QColor c);
    void draw() const;

    
    superficie get_funcion();
    int octante;
    static int ejecentral;
    static bool esSolido;
    void buildGeometry();
    static QList<double> constantes;

private:

    QList<Patch *> parts;
    Geometry *geom;
    MarchingCubes *marching;
    superficie funcion;

    
    QList<sArista> calcular_cortes(sMarching_Cube cubo);
    bool signo_opuesto(double a, double b);

    
    double lineal(double vert_1, double vert_2);

    
    Patch* agregar_triangulos(QList<QVector3D> &lista_triangulos);

    
    Patch* sumar_patch(Patch* patch_1, Patch* patch_2);


    
    Patch* identificar_tipo(sMarching_Cube cubo);
    Patch* tipo01(QList<sArista> aristas, QList<unsigned int> vertices);
    Patch* tipo02(QList<sArista> aristas);
    Patch* tipo04(QList<sArista> aristas, QList<unsigned int> vertices);
    Patch* tipo05(QList<sArista> aristas, QList<unsigned int> vertices);
    Patch* tipo06(QList<sArista> aristas, QList<unsigned int> vertices, int ind_vertice_solitario);
    Patch* tipo08(QList<sArista> aristas, QList<unsigned int> vertices, unsigned int ind_vertice_solitario);
    Patch* tipo09(QList<sArista> aristas, QList<unsigned int> vertices);
    Patch* tipo11(QList<sArista> aristas, QList<unsigned int> vertices);
    Patch* tipo13(QList<sArista> aristas, QList<unsigned int> vertices);
    Patch* Cubo(sPunto_f centro, double medio_lado);
};


#endif 
