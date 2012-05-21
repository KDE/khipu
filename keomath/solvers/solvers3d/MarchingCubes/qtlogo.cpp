

#include <QGLWidget>
#include <QMatrix4x4>
#include <qmath.h>
#include "qtlogo.h"
#include <GL/glu.h>


struct Geometry
{
    QVector<GLuint> faces;
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    void appendSmooth(const QVector3D &a, const QVector3D &n, int from);
    void appendFaceted(const QVector3D &a, const QVector3D &n);
    void finalize();
    void loadArrays() const;
};



class Patch
{
public:
    enum Smoothing { Faceted, Smooth };
    Patch(Geometry *);
    void setSmoothing(Smoothing s)
    {
        sm = s;
    }
    void translate(const QVector3D &t);
    void rotate(qreal deg, QVector3D axis);
    void draw() const;
    void addTri(const QVector3D &a, const QVector3D &b, const QVector3D &c, const QVector3D &n);
    void addQuad(const QVector3D &a, const QVector3D &b,  const QVector3D &c, const QVector3D &d);

    GLuint start;
    GLuint count;
    GLuint initv;

    GLfloat faceColor[4];
    QMatrix4x4 mat;
    Smoothing sm;
    Geometry *geom;
};


static inline void qSetColor(float colorVec[], QColor c)
{
    colorVec[0] = c.redF();
    colorVec[1] = c.greenF();
    colorVec[2] = c.blueF();
    colorVec[3] = c.alphaF();
}

void Geometry::loadArrays() const
{
    glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
    glNormalPointer(GL_FLOAT, 0, normals.constData());
}

void Geometry::finalize()
{
    
    for (int i = 0; i < normals.count(); ++i)
        normals[i].normalize();
}

void Geometry::appendSmooth(const QVector3D &a, const QVector3D &n, int from)
{
    int v = vertices.count() - 1;
    for ( ; v >= from; --v)
        if (qFuzzyCompare(vertices[v], a))
            break;
    if (v < from)
    {
        v = vertices.count();
        vertices.append(a);
        normals.append(n);
    }
    else
    {
        normals[v] += n;
    }
    faces.append(v);
}

void Geometry::appendFaceted(const QVector3D &a, const QVector3D &n)
{
    int v = vertices.count();
    vertices.append(a);
    normals.append(n);
    faces.append(v);
}

Patch::Patch(Geometry *g)
    : start(g->faces.count())
    , count(0)
    , initv(g->vertices.count())
    , sm(Patch::Smooth)
    , geom(g)
{
    qSetColor(faceColor, QColor(Qt::darkGray));
}

void Patch::rotate(qreal deg, QVector3D axis)
{
    mat.rotate(deg, axis);
}

void Patch::translate(const QVector3D &t)
{
    mat.translate(t);
}

static inline void qMultMatrix(const QMatrix4x4 &mat)
{
    if (sizeof(qreal) == sizeof(GLfloat))
        glMultMatrixf((GLfloat*)mat.constData());
#ifndef QT_OPENGL_ES
    else if (sizeof(qreal) == sizeof(GLdouble))
        glMultMatrixd((GLdouble*)mat.constData());
#endif
    else
    {
        GLfloat fmat[16];
        qreal const *r = mat.constData();
        for (int i = 0; i < 16; ++i)
            fmat[i] = r[i];
        glMultMatrixf(fmat);
    }
}


void Patch::draw() const
{
    
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, faceColor);
    const GLuint *indices = geom->faces.constData();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, indices + start);
    
}


void Patch::addTri(const QVector3D &a, const QVector3D &b, const QVector3D &c, const QVector3D &n)
{
    QVector3D norm = n.isNull() ? QVector3D::normal(a, b, c) : n;
    if (sm == Smooth)
    {
        geom->appendSmooth(a, norm, initv);
        geom->appendSmooth(b, norm, initv);
        geom->appendSmooth(c, norm, initv);
    }
    else
    {
        geom->appendFaceted(a, norm);
        geom->appendFaceted(b, norm);
        geom->appendFaceted(c, norm);
    }
    count += 3;
}

void Patch::addQuad(const QVector3D &a, const QVector3D &b,  const QVector3D &c, const QVector3D &d)
{
    QVector3D norm = QVector3D::normal(a, b, c);
    if (sm == Smooth)
    {
        addTri(a, b, c, norm);
        addTri(a, c, d, norm);
    }
    else
    {
        addTri(a, b, c, norm);
        int k = geom->vertices.count();
        geom->appendSmooth(a, norm, k);
        geom->appendSmooth(c, norm, k);
        geom->appendFaceted(d, norm);
        count += 3;
    }
}

static inline QVector<QVector3D> extrude(const QVector<QVector3D> &verts, qreal depth)
{
    QVector<QVector3D> extr = verts;
    for (int v = 0; v < extr.count(); ++v)
        extr[v].setZ(extr[v].z() - depth);
    return extr;
}

QtLogo::QtLogo(QObject *parent, superficie funcion)
    : QObject(parent)
{
    this->funcion = funcion;
    geom = new Geometry();
}

QtLogo::~QtLogo()
{
    qDeleteAll(parts);
    delete geom;
}

void QtLogo::setColor(QColor c)
{
    for (int i = 0; i < parts.count(); ++i)
        qSetColor(parts[i]->faceColor, c);
}


superficie QtLogo::get_funcion()
{
    return funcion;
}

QList<sArista> QtLogo::calcular_cortes(sMarching_Cube cubo)
{
    QList<sArista> aristas;
    sArista temp;
    
    if(signo_opuesto(cubo.vertices[0],cubo.vertices[1]))
    {
        temp.corte = QVector3D(cubo.centro.x-cubo.medio_lado,
                               cubo.centro.y-cubo.medio_lado,
                               cubo.centro.z-cubo.medio_lado+2*cubo.medio_lado*lineal(cubo.vertices[0],cubo.vertices[1]));
        temp.vertices[0] = 0;
        temp.vertices[1] = 1;
        aristas.append(temp);
    }
    
    if(signo_opuesto(cubo.vertices[0],cubo.vertices[2]))
    {
        temp.corte = QVector3D(cubo.centro.x-cubo.medio_lado,
                               cubo.centro.y-cubo.medio_lado+2*cubo.medio_lado*lineal(cubo.vertices[0],cubo.vertices[2]),
                               cubo.centro.z-cubo.medio_lado);
        temp.vertices[0] = 0;
        temp.vertices[1] = 2;
        aristas.append(temp);
    }
    
    if(signo_opuesto(cubo.vertices[0],cubo.vertices[4]))
    {
        temp.corte = QVector3D(cubo.centro.x-cubo.medio_lado+2*cubo.medio_lado*lineal(cubo.vertices[0],cubo.vertices[4]),
                               cubo.centro.y-cubo.medio_lado,
                               cubo.centro.z-cubo.medio_lado);
        temp.vertices[0] = 0;
        temp.vertices[1] = 4;
        aristas.append(temp);
    }
    
    if(signo_opuesto(cubo.vertices[1],cubo.vertices[3]))
    {
        temp.corte = QVector3D(cubo.centro.x-cubo.medio_lado,
                               cubo.centro.y-cubo.medio_lado+2*cubo.medio_lado*lineal(cubo.vertices[1],cubo.vertices[3]),
                               cubo.centro.z+cubo.medio_lado);
        temp.vertices[0] = 1;
        temp.vertices[1] = 3;
        aristas.append(temp);
    }
    
    if(signo_opuesto(cubo.vertices[1],cubo.vertices[5]))
    {
        temp.corte = QVector3D(cubo.centro.x-cubo.medio_lado+2*cubo.medio_lado*lineal(cubo.vertices[1],cubo.vertices[5]),
                               cubo.centro.y-cubo.medio_lado,
                               cubo.centro.z+cubo.medio_lado);
        temp.vertices[0] = 1;
        temp.vertices[1] = 5;
        aristas.append(temp);
    }
    
    if(signo_opuesto(cubo.vertices[2],cubo.vertices[3]))
    {
        temp.corte = QVector3D(cubo.centro.x-cubo.medio_lado,
                               cubo.centro.y+cubo.medio_lado,
                               cubo.centro.z-cubo.medio_lado+2*cubo.medio_lado*lineal(cubo.vertices[2],cubo.vertices[3]));
        temp.vertices[0] = 2;
        temp.vertices[1] = 3;
        aristas.append(temp);
    }
    
    if(signo_opuesto(cubo.vertices[2],cubo.vertices[6]))
    {
        temp.corte = QVector3D(cubo.centro.x-cubo.medio_lado+2*cubo.medio_lado*lineal(cubo.vertices[2],cubo.vertices[6]),
                               cubo.centro.y+cubo.medio_lado,
                               cubo.centro.z-cubo.medio_lado);
        temp.vertices[0] = 2;
        temp.vertices[1] = 6;
        aristas.append(temp);
    }
    
    if(signo_opuesto(cubo.vertices[3],cubo.vertices[7]))
    {
        temp.corte = QVector3D(cubo.centro.x-cubo.medio_lado+2*cubo.medio_lado*lineal(cubo.vertices[3],cubo.vertices[7]),
                               cubo.centro.y+cubo.medio_lado,
                               cubo.centro.z+cubo.medio_lado);
        temp.vertices[0] = 3;
        temp.vertices[1] = 7;
        aristas.append(temp);
    }
    
    if(signo_opuesto(cubo.vertices[4],cubo.vertices[5]))
    {
        temp.corte = QVector3D(cubo.centro.x+cubo.medio_lado,
                               cubo.centro.y-cubo.medio_lado,
                               cubo.centro.z-cubo.medio_lado+2*cubo.medio_lado*lineal(cubo.vertices[4],cubo.vertices[5]));
        temp.vertices[0] = 4;
        temp.vertices[1] = 5;
        aristas.append(temp);
    }
    
    if(signo_opuesto(cubo.vertices[4],cubo.vertices[6]))
    {
        temp.corte = QVector3D(cubo.centro.x+cubo.medio_lado,
                               cubo.centro.y-cubo.medio_lado+2*cubo.medio_lado*lineal(cubo.vertices[4],cubo.vertices[6]),
                               cubo.centro.z-cubo.medio_lado);
        temp.vertices[0] = 4;
        temp.vertices[1] = 6;
        aristas.append(temp);
    }
    
    if(signo_opuesto(cubo.vertices[5],cubo.vertices[7]))
    {
        temp.corte = QVector3D(cubo.centro.x+cubo.medio_lado,
                               cubo.centro.y-cubo.medio_lado+2*cubo.medio_lado*lineal(cubo.vertices[5],cubo.vertices[7]),
                               cubo.centro.z+cubo.medio_lado);
        temp.vertices[0] = 5;
        temp.vertices[1] = 7;
        aristas.append(temp);
    }
    
    if(signo_opuesto(cubo.vertices[6],cubo.vertices[7]))
    {
        temp.corte = QVector3D(cubo.centro.x+cubo.medio_lado,
                               cubo.centro.y+cubo.medio_lado,
                               cubo.centro.z-cubo.medio_lado+2*cubo.medio_lado*lineal(cubo.vertices[6],cubo.vertices[7]));
        temp.vertices[0] = 6;
        temp.vertices[1] = 7;
        aristas.append(temp);
    }
    return aristas;
}

bool QtLogo::signo_opuesto(double a, double b)
{
    return ((a > 0 && b <= 0) || (a <= 0 && b > 0));
}

double QtLogo::lineal(double vert_1, double vert_2)
{
    
    return qAbs(vert_1/(vert_1 - vert_2));
}

Patch* QtLogo::agregar_triangulos(QList<QVector3D> &lista_triangulos)
{
    Patch *sides = new Patch(geom);
    sides->setSmoothing(Patch::Faceted);
    QVector3D normal;

    for(int i=0; i<lista_triangulos.count(); i+=3)
    {
        sides->addTri(lista_triangulos.at(i),lista_triangulos.at(i+1),lista_triangulos.at(i+2),normal);
    }

    return sides;
}

Patch* QtLogo::sumar_patch(Patch* patch_1, Patch* patch_2)
{
    patch_1->count += patch_2->count;
    delete patch_2;
    return patch_1;
}


Patch* QtLogo::identificar_tipo(sMarching_Cube cubo)
{
    QList<sArista> aristas;
    QList<unsigned int> vertices;
    unsigned int it;

    
    aristas = calcular_cortes(cubo);
    it=0;
    for(unsigned int i=1; i<129; i*=2)
    {
        if((cubo.tipo & i) == i)
        {
            vertices.append(it);
        }
        it++;
    }
    if(vertices.count() > 4)
    {
        it=0;
        vertices.clear();
        for(unsigned int i=1; i<129; i*=2)
        {
            if((cubo.tipo & i) != i)
            {
                vertices.append(it);
            }
            it++;
        }
    }

    
    switch(aristas.count())
    {
    case 3:
        
        return tipo01(aristas, vertices);
    case 4:
        
        if(vertices.count() == 2)
        {
            
            return tipo02(aristas);
        }
        else
        {
            
            return tipo05(aristas, vertices);
        }
    case 5:
        
        return tipo04(aristas, vertices);
    case 6:
        
        if(vertices.count() == 2)
        {
            
            return tipo01(aristas, vertices);
        }
        else
        {
            for(int i=0; i<vertices.count(); i++)
            {
                bool tiene_arista = false;
                for(int j=0; j<aristas.count(); j++)
                {
                    if(aristas.at(j).vertices[0] == vertices.at(i) || aristas.at(j).vertices[1] == vertices.at(i))
                    {
                        tiene_arista = true;
                        break;
                    }
                }
                if(!tiene_arista)
                {
                    
                    return tipo08(aristas, vertices, i);
                }
            }
            
            return tipo09(aristas,vertices);
        }
    case 7:
        
        return tipo11(aristas, vertices);
    case 8:
        
        bool encontrado;
        for(int i=0; i<vertices.count()-1; i++)
        {
            encontrado = true;
            for(int j=i+1; j<vertices.count(); j++)
            {
                if(vertices.at(j) - vertices.at(i) == 1 ||
                        vertices.at(j) - vertices.at(i) == 2 ||
                        vertices.at(j) - vertices.at(i) == 4 )
                {
                    encontrado = false;
                    break;
                }
            }
            if(encontrado)
            {
                
                return tipo06(aristas, vertices, i);
            }
        }
        
        return tipo13(aristas, vertices);
    case 9:
        
        return tipo01(aristas, vertices);
    case 12:
        
        return tipo01(aristas, vertices);
    default:
        printf("Error al calcular tipo\n");
    }
    return Cubo(cubo.centro,cubo.medio_lado);
}

Patch* QtLogo::tipo01(QList<sArista> aristas, QList<unsigned int> vertices)
{
    QList<QVector3D> triangulos;
    
    for(int i=0; i<vertices.count(); i++)
    {
        for(int j=0; j<aristas.count(); j++)
        {
            if(aristas.at(j).vertices[0]==vertices[i] || aristas.at(j).vertices[1]==vertices[i])
            {
                triangulos << aristas.at(j).corte;
            }
        }
    }
    return agregar_triangulos(triangulos);
}
Patch* QtLogo::tipo02(QList<sArista> aristas)
{
    QList<QVector3D> triangulos;
    triangulos << aristas.at(0).corte;
    triangulos << aristas.at(1).corte;
    triangulos << aristas.at(2).corte;
    triangulos << aristas.at(1).corte;
    triangulos << aristas.at(2).corte;
    triangulos << aristas.at(3).corte;
    return agregar_triangulos(triangulos);
}

Patch* QtLogo::tipo04(QList<sArista> aristas, QList<unsigned int> vertices)
{
    QList<QVector3D> triangulos;
    unsigned int encontrado, sentido, pos_arista;
    QList< QList<unsigned int> > pos;

    
    for(int i=0; i<vertices.count(); i++)
    {
        encontrado = 0;
        for(int j=0; j<aristas.count(); j++)
        {
            if(aristas.at(j).vertices[0]==vertices[i] || aristas.at(j).vertices[1]==vertices[i])
            {
                encontrado++;
                pos_arista = j;
                if(encontrado == 2)
                {
                    break;
                }
            }
        }
        if(encontrado == 1)
        {
            
            sentido = aristas.at(pos_arista).vertices[1]-aristas.at(pos_arista).vertices[0];
            for(int j=0; j<aristas.count(); j++)
            {
                if(aristas.at(j).vertices[1] - aristas.at(j).vertices[0] == sentido)
                {
                    triangulos << aristas.at(j).corte;
                }
            }
            aristas.removeAt(pos_arista);

            
            for(int k=0; k<vertices.count(); k++)
            {
                if(i==k)
                {
                    continue;
                }
                pos.append(QList<unsigned int>());
                for(int j=0; j<aristas.count(); j++)
                {
                    if(aristas.at(j).vertices[0] == vertices[k] || aristas.at(j).vertices[1] == vertices[k])
                    {
                        pos.back().append(j);
                    }
                }
            }

            
            triangulos << aristas.at(pos.at(0).at(0)).corte;
            triangulos << aristas.at(pos.at(0).at(1)).corte;
            for(int j=0; j<pos.at(1).count(); j++)
            {
                if(aristas.at(pos.at(1).at(j)).vertices[1] - aristas.at(pos.at(1).at(j)).vertices[0] == sentido)
                {
                    triangulos << aristas.at(pos.at(1).at(j)).corte;
                }
            }

            
            triangulos << aristas.at(pos.at(1).at(0)).corte;
            triangulos << aristas.at(pos.at(1).at(1)).corte;
            for(int j=0; j<pos.at(0).count(); j++)
            {
                if(aristas.at(pos.at(0).at(j)).vertices[1] - aristas.at(pos.at(0).at(j)).vertices[0] != sentido)
                {
                    triangulos << aristas.at(pos.at(0).at(j)).corte;
                }
            }
            break;
        }
    }
    return agregar_triangulos(triangulos);
}
Patch* QtLogo::tipo05(QList<sArista> aristas, QList<unsigned int> vertices)
{
    QList<QVector3D> triangulos;
    int vertice_arista[4];
    
    for(int i=0; i<vertices.count(); i++)
    {
        for(int j=0; j<aristas.count(); j++)
        {
            if(aristas.at(j).vertices[0] == vertices.at(i) || aristas.at(j).vertices[1] == vertices.at(i))
            {
                vertice_arista[i] = j;
                break;
            }
        }
    }
    
    triangulos << aristas.at(vertice_arista[0]).corte;
    triangulos << aristas.at(vertice_arista[1]).corte;
    triangulos << aristas.at(vertice_arista[2]).corte;
    triangulos << triangulos.at(1);
    triangulos << triangulos.at(2);
    triangulos << aristas.at(vertice_arista[3]).corte;

    return agregar_triangulos(triangulos);
}
Patch* QtLogo::tipo06(QList<sArista> aristas, QList<unsigned int> vertices, int ind_vertice_solitario)
{
    
    QList<sArista> aristas2;
    QList<unsigned int> vertices2;
    
    for(int i=0; i<aristas.count(); i++)
    {
        if(aristas.at(i).vertices[0] != vertices.at(ind_vertice_solitario)
                && aristas.at(i).vertices[1] != vertices.at(ind_vertice_solitario))
        {
            aristas2.append(aristas.at(i));
            aristas.removeAt(i);
            i--;
        }
    }
    vertices2.append(vertices.at(ind_vertice_solitario));
    vertices.removeAt(ind_vertice_solitario);
    return sumar_patch(tipo01(aristas, vertices2),tipo04(aristas2, vertices));
}

Patch* QtLogo::tipo08(QList<sArista> aristas, QList<unsigned int> vertices, unsigned int ind_vertice_solitario)
{
    QList<QVector3D> triangulos;
    unsigned int ind_vert = 0, sentido;
    QList<int> orden;
    if(ind_vertice_solitario == 0)
    {
        ind_vert = 1;
    }

    
    for(int j=0; j<aristas.count(); j++)
    {
        if(aristas.at(j).vertices[0] == vertices.at(ind_vert) || aristas.at(j).vertices[1] == vertices.at(ind_vert))
        {
            triangulos << aristas.at(j).corte;
            orden.append(j);
        }
    }
    
    sentido = aristas.at(orden.at(0)).vertices[1] - aristas.at(orden.at(0)).vertices[0];
    for(int j=0; j<aristas.count(); j++)
    {
        if(orden.at(0) == j || orden.at(1) == j)
        {
            continue;
        }
        if(aristas.at(j).vertices[1] - aristas.at(j).vertices[0] == sentido)
        {
            triangulos << aristas.at(j).corte;
            orden.append(j);
            for(int k=0; k<vertices.count(); k++)
            {
                if(aristas.at(j).vertices[0] == vertices.at(k) || aristas.at(j).vertices[1] == vertices.at(k))
                {
                    ind_vert = k;
                    break;
                }
            }
            break;
        }
    }
    triangulos << triangulos.at(1);
    triangulos << triangulos.at(2);
    for(int j=0; j<aristas.count(); j++)
    {
        if((aristas.at(j).vertices[0] == vertices.at(ind_vert) || aristas.at(j).vertices[1] == vertices.at(ind_vert))
                && (aristas.at(j).vertices[1] - aristas.at(j).vertices[0] != sentido))
        {
            triangulos << aristas.at(j).corte;
            orden.append(j);
            break;
        }
    }
    
    sentido = aristas.at(orden.at(1)).vertices[1] - aristas.at(orden.at(1)).vertices[0];
    triangulos << triangulos.at(0);
    triangulos << triangulos.at(2);

    for(int j=0; j<aristas.count(); j++)
    {
        if(orden.at(0) == j || orden.at(1) == j || orden.at(2) == j || orden.at(3) == j)
        {
            continue;
        }
        if(aristas.at(j).vertices[1] - aristas.at(j).vertices[0] == sentido)
        {
            triangulos << aristas.at(j).corte;
            orden.append(j);
            for(int k=0; k<vertices.count(); k++)
            {
                if(aristas.at(j).vertices[0] == vertices.at(k) || aristas.at(j).vertices[1] == vertices.at(k))
                {
                    ind_vert = k;
                    break;
                }
            }
            break;
        }
    }
    triangulos << triangulos.at(0);
    triangulos << triangulos.at(8);
    for(int j=0; j<aristas.count(); j++)
    {
        if((aristas.at(j).vertices[0] == vertices.at(ind_vert) || aristas.at(j).vertices[1] == vertices.at(ind_vert))
                && (aristas.at(j).vertices[1] - aristas.at(j).vertices[0] != sentido))
        {
            triangulos << aristas.at(j).corte;
            orden.append(j);
            break;
        }
    }

    return agregar_triangulos(triangulos);
}
Patch* QtLogo::tipo09(QList<sArista> aristas, QList<unsigned int> vertices)
{
    QList<QVector3D> triangulos;
    QList<int> vertices_doble;
    QList< QList<int> > aristas_doble;
    bool doble;
    unsigned int sentido, eje_compartido;

    
    for(int i=0; i<vertices.count(); i++)
    {
        doble = false;
        for(int j=0; j<aristas.count(); j++)
        {
            if(aristas.at(j).vertices[0] == vertices.at(i) || aristas.at(j).vertices[1] == vertices.at(i))
            {
                if(doble)
                {
                    vertices_doble.append(i);
                    break;
                }
                else
                {
                    doble = true;
                }
            }
        }
    }
    
    aristas_doble.append(QList<int>());
    aristas_doble.append(QList<int>());
    for(int j=0; j<aristas.count(); j++)
    {
        if(aristas.at(j).vertices[0] == vertices.at(vertices_doble.at(0)) || aristas.at(j).vertices[1] == vertices.at(vertices_doble.at(0)))
        {
            aristas_doble.front().append(j);
        }
        else if(aristas.at(j).vertices[0] == vertices.at(vertices_doble.at(1)) || aristas.at(j).vertices[1] == vertices.at(vertices_doble.at(1)))
        {
            aristas_doble.back().append(j);
        }
    }
    
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<2; j++)
        {
            if(aristas.at(aristas_doble.at(0).at(i)).vertices[1] - aristas.at(aristas_doble.at(0).at(i)).vertices[0] ==
                    aristas.at(aristas_doble.at(1).at(j)).vertices[1] - aristas.at(aristas_doble.at(1).at(j)).vertices[0])
            {
                eje_compartido = aristas.at(aristas_doble.at(0).at(i)).vertices[1] - aristas.at(aristas_doble.at(0).at(i)).vertices[0];
                break;
            }
        }
    }
    
    for(int i=0; i<2; i++)
    {
        triangulos << aristas.at(aristas_doble.at(0).at(i)).corte;
    }
    for(int i=0; i<2; i++)
    {
        if(aristas.at(aristas_doble.at(1).at(i)).vertices[1] - aristas.at(aristas_doble.at(1).at(i)).vertices[0] == eje_compartido)
        {
            triangulos << aristas.at(aristas_doble.at(1).at(i)).corte;
            break;
        }
    }
    
    for(int i=0; i<2; i++)
    {
        triangulos << aristas.at(aristas_doble.at(1).at(i)).corte;
    }
    for(int i=0; i<2; i++)
    {
        if(aristas.at(aristas_doble.at(0).at(i)).vertices[1] - aristas.at(aristas_doble.at(0).at(i)).vertices[0] == eje_compartido)
        {
            triangulos << aristas.at(aristas_doble.at(0).at(i)).corte;
            break;
        }
    }
    
    for(int i=0; i<2; i++)
    {
        if(aristas.at(aristas_doble.at(0).at(i)).vertices[1] - aristas.at(aristas_doble.at(0).at(i)).vertices[0] == eje_compartido)
        {
            triangulos << aristas.at(aristas_doble.at(0).at(i)).corte;
            break;
        }
    }
    for(int i=0; i<2; i++)
    {
        if(aristas.at(aristas_doble.at(1).at(i)).vertices[1] - aristas.at(aristas_doble.at(1).at(i)).vertices[0] != eje_compartido)
        {
            triangulos << aristas.at(aristas_doble.at(1).at(i)).corte;
            sentido = aristas.at(aristas_doble.at(1).at(i)).vertices[1] - aristas.at(aristas_doble.at(1).at(i)).vertices[0];
            break;
        }
    }
    for(int j=0; j<aristas.count(); j++)
    {
        if(aristas.at(j).vertices[1] - aristas.at(j).vertices[0] == sentido)
        {
            triangulos << aristas.at(j).corte;
        }
    }
    
    for(int i=0; i<2; i++)
    {
        if(aristas.at(aristas_doble.at(0).at(i)).vertices[1] - aristas.at(aristas_doble.at(0).at(i)).vertices[0] != eje_compartido)
        {
            triangulos << aristas.at(aristas_doble.at(0).at(i)).corte;
            sentido = aristas.at(aristas_doble.at(0).at(i)).vertices[1] - aristas.at(aristas_doble.at(0).at(i)).vertices[0];
            break;
        }
    }
    for(int i=0; i<2; i++)
    {
        if(aristas.at(aristas_doble.at(1).at(i)).vertices[1] - aristas.at(aristas_doble.at(1).at(i)).vertices[0] == eje_compartido)
        {
            triangulos << aristas.at(aristas_doble.at(1).at(i)).corte;
            break;
        }
    }
    for(int j=0; j<aristas.count(); j++)
    {
        if(aristas.at(j).vertices[1] - aristas.at(j).vertices[0] == sentido)
        {
            triangulos << aristas.at(j).corte;
        }
    }

    return agregar_triangulos(triangulos);
}

Patch* QtLogo::tipo11(QList<sArista> aristas, QList<unsigned int> vertices)
{
    
    unsigned int vert_solitario;
    bool encontrado;
    QList<sArista> aristas2;

    
    for(int i=0; i<vertices.count()-1; i++)
    {
        encontrado = true;
        for(int j=i+1; j<vertices.count(); j++)
        {
            if(vertices.at(j) - vertices.at(i) == 1 ||
                    vertices.at(j) - vertices.at(i) == 2 ||
                    vertices.at(j) - vertices.at(i) == 4 )
            {
                
                encontrado = false;
                break;
            }
        }
        if(encontrado)
        {
            vert_solitario = vertices.at(i);
            break;
        }
    }
    
    for(int i=0; i<aristas.count(); i++)
    {
        if(aristas.at(i).vertices[0] != vert_solitario
                && aristas.at(i).vertices[1] != vert_solitario)
        {
            aristas2.append(aristas.at(i));
            aristas.removeAt(i);
            i--;
        }
    }
    vertices.clear();
    vertices.append(vert_solitario);
    return sumar_patch(tipo01(aristas, vertices), tipo02(aristas2));
}

Patch* QtLogo::tipo13(QList<sArista> aristas, QList<unsigned int> vertices)
{
    
    unsigned int verts[2];
    QList<sArista> aristas2;

    
    verts[0] = vertices.at(0);
    for(int i=1; i<vertices.count(); i++)
    {
        if(vertices.at(i) - verts[0] == 1 ||
                vertices.at(i) - verts[0] == 2 ||
                vertices.at(i) - verts[0] == 4 )
        {
            verts[1] = vertices.at(i);
            break;
        }
    }
    
    for(int i=0; i<aristas.count(); i++)
    {
        if(aristas.at(i).vertices[0] != verts[0]
                && aristas.at(i).vertices[1] != verts[0]
                && aristas.at(i).vertices[0] != verts[1]
                && aristas.at(i).vertices[1] != verts[1])
        {
            aristas2.append(aristas.at(i));
            aristas.removeAt(i);
            i--;
        }
    }
    return sumar_patch(tipo02(aristas),tipo02(aristas2));
}

Patch* QtLogo::Cubo(sPunto_f centro, double medio_lado)
{
    Patch *sides = new Patch(geom);
    sides->setSmoothing(Patch::Faceted);
    QVector<QVector3D> cara;
    
    cara << QVector3D(centro.x-medio_lado,centro.y-medio_lado,centro.z-medio_lado);
    cara << QVector3D(centro.x-medio_lado,centro.y+medio_lado,centro.z-medio_lado);
    cara << QVector3D(centro.x+medio_lado,centro.y+medio_lado,centro.z-medio_lado);
    cara << QVector3D(centro.x+medio_lado,centro.y-medio_lado,centro.z-medio_lado);
    sides->addQuad(cara[0], cara[1], cara[2], cara[3]);
    
    cara[0]=QVector3D(centro.x-medio_lado,centro.y-medio_lado,centro.z-medio_lado);
    cara[1]=QVector3D(centro.x+medio_lado,centro.y-medio_lado,centro.z-medio_lado);
    cara[2]=QVector3D(centro.x+medio_lado,centro.y-medio_lado,centro.z+medio_lado);
    cara[3]=QVector3D(centro.x-medio_lado,centro.y-medio_lado,centro.z+medio_lado);
    sides->addQuad(cara[0], cara[1], cara[2], cara[3]);
    
    cara[0]=QVector3D(centro.x-medio_lado,centro.y-medio_lado,centro.z+medio_lado);
    cara[1]=QVector3D(centro.x+medio_lado,centro.y-medio_lado,centro.z+medio_lado);
    cara[2]=QVector3D(centro.x+medio_lado,centro.y+medio_lado,centro.z+medio_lado);
    cara[3]=QVector3D(centro.x-medio_lado,centro.y+medio_lado,centro.z+medio_lado);
    sides->addQuad(cara[0], cara[1], cara[2], cara[3]);
    
    cara[0]=QVector3D(centro.x-medio_lado,centro.y+medio_lado,centro.z-medio_lado);
    cara[1]=QVector3D(centro.x-medio_lado,centro.y+medio_lado,centro.z+medio_lado);
    cara[2]=QVector3D(centro.x+medio_lado,centro.y+medio_lado,centro.z+medio_lado);
    cara[3]=QVector3D(centro.x+medio_lado,centro.y+medio_lado,centro.z-medio_lado);
    sides->addQuad(cara[0], cara[1], cara[2], cara[3]);
    
    cara[0]=QVector3D(centro.x-medio_lado,centro.y-medio_lado,centro.z-medio_lado);
    cara[1]=QVector3D(centro.x-medio_lado,centro.y-medio_lado,centro.z+medio_lado);
    cara[2]=QVector3D(centro.x-medio_lado,centro.y+medio_lado,centro.z+medio_lado);
    cara[3]=QVector3D(centro.x-medio_lado,centro.y+medio_lado,centro.z-medio_lado);
    sides->addQuad(cara[0], cara[1], cara[2], cara[3]);
    
    cara[0]=QVector3D(centro.x+medio_lado,centro.y-medio_lado,centro.z-medio_lado);
    cara[1]=QVector3D(centro.x+medio_lado,centro.y+medio_lado,centro.z-medio_lado);
    cara[2]=QVector3D(centro.x+medio_lado,centro.y+medio_lado,centro.z+medio_lado);
    cara[3]=QVector3D(centro.x+medio_lado,centro.y-medio_lado,centro.z+medio_lado);
    sides->addQuad(cara[0], cara[1], cara[2], cara[3]);
    return sides;
}

QList<double> QtLogo::constantes;
int QtLogo::ejecentral;
bool QtLogo::esSolido;

double func_esfera(sPunto_f punto)
{

    if(QtLogo::esSolido)
    {
        if(punto.x==0 || punto.y==0 || punto.z==0)
            return qPow(punto.x-QtLogo::constantes.at(0),2) + qPow(punto.y-QtLogo::constantes.at(1),2) + qPow(punto.z-QtLogo::constantes.at(2),2) - qPow(QtLogo::constantes.at(3),2);
        else
            return 1;
    }
    else
    {
        return qPow(punto.x-QtLogo::constantes.at(0),2) + qPow(punto.y-QtLogo::constantes.at(1),2) + qPow(punto.z-QtLogo::constantes.at(2),2) - qPow(QtLogo::constantes.at(3),2);
    }
    return 0;
}

double func_elipsoide(sPunto_f punto)
{

    if(QtLogo::esSolido)
    {
        if(punto.x==0 || punto.y==0 || punto.z==0)
            return qPow((punto.x-QtLogo::constantes.at(0))/QtLogo::constantes.at(3),2) + qPow((punto.y-QtLogo::constantes.at(1))/QtLogo::constantes.at(4),2) + qPow((punto.z-QtLogo::constantes.at(2))/QtLogo::constantes.at(5),2) - 1;
        else
            return 1;

    }
    else
    {
        return qPow((punto.x-QtLogo::constantes.at(0))/QtLogo::constantes.at(3),2) + qPow((punto.y-QtLogo::constantes.at(1))/QtLogo::constantes.at(4),2) + qPow((punto.z-QtLogo::constantes.at(2))/QtLogo::constantes.at(5),2) - 1;
    }
    return 0;
}

double func_cilindro(sPunto_f punto)
{
    
    
    
    
    


    switch(QtLogo::ejecentral)
    {
    case sobreX:
        if(QtLogo::esSolido)
        {
            if(punto.x==0 || punto.y==0 || punto.z==0)
                return qPow(punto.y-QtLogo::constantes.at(1),2) + qPow(punto.z-QtLogo::constantes.at(2),2) - qPow(QtLogo::constantes.at(3),2);
            else
                return 1;
        }
        else
            return qPow(punto.y-QtLogo::constantes.at(1),2) + qPow(punto.z-QtLogo::constantes.at(2),2) - qPow(QtLogo::constantes.at(3),2);
        break;
    case sobreY:
        return qPow(punto.z-QtLogo::constantes.at(2),2) + qPow(punto.x-QtLogo::constantes.at(0),2) - qPow(QtLogo::constantes.at(3),2);
        break;
    case sobreZ:
        return qPow(punto.x-QtLogo::constantes.at(0),2) + qPow(punto.y-QtLogo::constantes.at(1),2) - qPow(QtLogo::constantes.at(3),2);
        break;
    }
    return 0;
}

double func_hiperboloide_1hoja(sPunto_f punto)
{

    switch(QtLogo::ejecentral)
    {
    case sobreX:
        return -qPow((punto.x-QtLogo::constantes.at(0))/QtLogo::constantes.at(3),2) + qPow((punto.y-QtLogo::constantes.at(1))/QtLogo::constantes.at(4),2) + qPow((punto.z-QtLogo::constantes.at(2))/QtLogo::constantes.at(5),2) - 1;
        break;
    case sobreY:
        return qPow((punto.x-QtLogo::constantes.at(0))/QtLogo::constantes.at(3),2) - qPow((punto.y-QtLogo::constantes.at(1))/QtLogo::constantes.at(4),2) + qPow((punto.z-QtLogo::constantes.at(2))/QtLogo::constantes.at(5),2) - 1;
        break;
    case sobreZ:
        return qPow((punto.x-QtLogo::constantes.at(0))/QtLogo::constantes.at(3),2) + qPow((punto.y-QtLogo::constantes.at(1))/QtLogo::constantes.at(4),2) - qPow((punto.z-QtLogo::constantes.at(2))/QtLogo::constantes.at(5),2) - 1;
        break;
    }
    return 0;
}

double func_hiperboloide_2hojas(sPunto_f punto)
{

    switch(QtLogo::ejecentral)
    {
    case sobreX:
        return -qPow((punto.x-QtLogo::constantes.at(0))/QtLogo::constantes.at(3),2) + qPow((punto.y-QtLogo::constantes.at(1))/QtLogo::constantes.at(4),2) + qPow((punto.z-QtLogo::constantes.at(2))/QtLogo::constantes.at(5),2) + 1;
        break;
    case sobreY:
        return qPow((punto.x-QtLogo::constantes.at(0))/QtLogo::constantes.at(3),2) - qPow((punto.y-QtLogo::constantes.at(1))/QtLogo::constantes.at(4),2) + qPow((punto.z-QtLogo::constantes.at(2))/QtLogo::constantes.at(5),2) + 1;
        break;
    case sobreZ:
        return qPow((punto.x-QtLogo::constantes.at(0))/QtLogo::constantes.at(3),2) + qPow((punto.y-QtLogo::constantes.at(1))/QtLogo::constantes.at(4),2) - qPow((punto.z-QtLogo::constantes.at(2))/QtLogo::constantes.at(5),2) + 1;
        break;
    }
    return 0;
}

double func_paraboloide_hiperbolico(sPunto_f punto)
{

    switch(QtLogo::ejecentral)
    {
    case sobreX:
        return -(punto.x - QtLogo::constantes.at(0)) + qPow((punto.y-QtLogo::constantes.at(1))/QtLogo::constantes.at(4),2) - qPow((punto.z-QtLogo::constantes.at(2))/QtLogo::constantes.at(5),2);
        break;
    case sobreY:
        return qPow((punto.x-QtLogo::constantes.at(0))/QtLogo::constantes.at(3),2) - (punto.y - QtLogo::constantes.at(1)) - qPow((punto.z-QtLogo::constantes.at(2))/QtLogo::constantes.at(5),2);
        break;
    case sobreZ:
        return qPow((punto.x-QtLogo::constantes.at(0))/QtLogo::constantes.at(3),2) - qPow((punto.y-QtLogo::constantes.at(1))/QtLogo::constantes.at(4),2) - (punto.z - QtLogo::constantes.at(2));
        break;
    }
    return 0;
}

double func_paraboloide_eliptico(sPunto_f punto)
{

    switch(QtLogo::ejecentral)
    {
    case sobreX:
        return -(punto.x-QtLogo::constantes.at(0))/(QtLogo::constantes.at(3))+qPow(punto.y-QtLogo::constantes.at(1),2)/qPow(QtLogo::constantes.at(4),2)+qPow(punto.z-QtLogo::constantes.at(2),2)/qPow(QtLogo::constantes.at(5),2);
        break;
    case sobreY:
        return qPow(punto.x-QtLogo::constantes.at(0),2)/qPow(QtLogo::constantes.at(3),2)-(punto.y-QtLogo::constantes.at(1))/QtLogo::constantes.at(4)+qPow(punto.z-QtLogo::constantes.at(2),2)/qPow(QtLogo::constantes.at(5),2);
        break;
    case sobreZ:
        return qPow(punto.x-QtLogo::constantes.at(0),2)/qPow(QtLogo::constantes.at(3),2)+qPow(punto.y-QtLogo::constantes.at(1),2)/qPow(QtLogo::constantes.at(4),2)-(punto.z-QtLogo::constantes.at(2))/QtLogo::constantes.at(5);
        break;
    }

    return 0;
}

double func_cono_eliptico(sPunto_f punto)
{

    switch(QtLogo::ejecentral)
    {
    case sobreX:
        return -qPow(punto.x-QtLogo::constantes.at(0),2)/qPow(QtLogo::constantes.at(3),2)+qPow(punto.y-QtLogo::constantes.at(1),2)/qPow(QtLogo::constantes.at(4),2)+qPow(punto.z-QtLogo::constantes.at(2),2)/qPow(QtLogo::constantes.at(5),2);
        break;
    case sobreY:
        return qPow(punto.x-QtLogo::constantes.at(0),2)/qPow(QtLogo::constantes.at(3),2)-qPow(punto.y-QtLogo::constantes.at(1),2)/qPow(QtLogo::constantes.at(4),2)+qPow(punto.z-QtLogo::constantes.at(2),2)/qPow(QtLogo::constantes.at(5),2);
        break;
    case sobreZ:
        return qPow(punto.x-QtLogo::constantes.at(0),2)/qPow(QtLogo::constantes.at(3),2)+qPow(punto.y-QtLogo::constantes.at(1),2)/qPow(QtLogo::constantes.at(4),2)-qPow(punto.z-QtLogo::constantes.at(2),2)/qPow(QtLogo::constantes.at(5),2);
        break;
    }

    return 0;
}

void QtLogo::buildGeometry()
{
    QList<sMarching_Cube> cubos;
    sMarching_Cube cubo;
    sLimitesEspacio _esp;

    switch(octante)
    {
    case 1:
        _esp.minX = 0;
        _esp.maxX = 5;
        _esp.minY = 0;
        _esp.maxY = 5;
        _esp.minZ = 0;
        _esp.maxZ = 5;
        break;
    case 2:
        _esp.minX = 0;
        _esp.maxX = 5;
        _esp.minY = -5;
        _esp.maxY = -1;
        _esp.minZ = 0;
        _esp.maxZ = 5;
        break;
    case 3:
        _esp.minX = 0;
        _esp.maxX = 5;
        _esp.minY = -5;
        _esp.maxY = -1;
        _esp.minZ = -5;
        _esp.maxZ = -1;
        break;
    case 4:
        _esp.minX = 0;
        _esp.maxX = 5;
        _esp.minY = 0;
        _esp.maxY = 5;
        _esp.minZ = -5;
        _esp.maxZ = -1;
        break;
    case 5:
        _esp.minX = -5;
        _esp.maxX = -1;
        _esp.minY = 0;
        _esp.maxY = 5;
        _esp.minZ = 0;
        _esp.maxZ = 5;
        break;
    case 6:
        _esp.minX = -5;
        _esp.maxX = -1;
        _esp.minY = -5;
        _esp.maxY = -1;
        _esp.minZ = 0;
        _esp.maxZ = 5;
        break;
    case 7:
        _esp.minX = -5;
        _esp.maxX = -1;
        _esp.minY = -5;
        _esp.maxY = -1;
        _esp.minZ = -5;
        _esp.maxZ = -1;
        break;
    case 8:
        _esp.minX = -5;
        _esp.maxX = -1;
        _esp.minY = 0;
        _esp.maxY = 5;
        _esp.minZ = -5;
        _esp.maxZ = -1;
        break;
    default:
        _esp.minX = -5;
        _esp.maxX = 5;
        _esp.minY = -5;
        _esp.maxY = 5;
        _esp.minZ = -5;
        _esp.maxZ = 5;
        break;
    }

    marching = new MarchingCubes(0.1,5.0,_esp);
    cubos = marching->ejecutar(funcion);
    printf("Cubos: %d\n",cubos.count());
    delete marching;

    foreach(cubo,cubos)
    {
        
        if(cubo.tipo > 127)
        {
            cubo.tipo = 255 - cubo.tipo;
        }
        parts << identificar_tipo(cubo);
    }

    printf("Vertices: %d\n",geom->vertices.count());
    geom->finalize();
}



void QtLogo::draw() const
{
    geom->loadArrays();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    for (int i = 0; i < parts.count(); ++i)
        parts[i]->draw();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

