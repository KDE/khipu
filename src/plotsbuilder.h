/*************************************************************************************
 *  Copyright (C) 2012 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>      *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 2                   *
 *  of the License, or (at your option) any later version.                           *
 *                                                                                   *
 *  This program is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 *  GNU General Public License for more details.                                     *
 *                                                                                   *
 *  You should have received a copy of the GNU General Public License                *
 *  along with this program; if not, write to the Free Software                      *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 *************************************************************************************/


#ifndef FUNCTIONEDITOR_H_builder
#define FUNCTIONEDITOR_H_builder

#include <QWidget>
#include "analitza/expression.h"

namespace Ui
{
    class PlotsBuilderWidget;
}


//TODO ANALITZAPLOT
// typename deberia devolver una lista de cadenas no una cadena: Superfices, SuperficesEsfericas ... etc en vez de solo SurfaEs
//...de esta manera se forma un arbol de categorias, de tipos de plots que la libreria puede dibujar
class PlotsBuilder : public QWidget
{
    Q_OBJECT
public:
    //NOTE AvailablePlots ... more to come .. in the future
    enum PlotType
    {
        //2D
        CartesianGraphCurve = 0x1,
        CartesianImplicitCurve = 0x2,
        CartesianParametricCurve2D = 0x4,
        PolarGraphCurve = 0x8,
        //3D
        CartesianParametricCurve3D = 0x10,
        CartesianGraphSurface = 0x20,
        CartesianImplicitSurface = 0x40,
        CartesianParametricSurface = 0x80,
        CylindricalGraphSurface = 0x100,
        SphericalGraphSurface = 0x200,
        //profiles
    };
    
    Q_DECLARE_FLAGS(PlotTypes, PlotType)
    

    PlotsBuilder(QWidget *parent);
    ~ PlotsBuilder();

    void setupTypes(PlotTypes t);
    PlotTypes types() const { return m_types; }
    void mapConnection(PlotType pt, QObject *recvr, const char * slot);
    
public slots:
    void showAllTypes(); // show all types
    void hideAllTypes(); // hide all types
    
private slots: // better names
    void setupCartesianGraphCurveInfo();
    
    
    void clearInfoWidget();
    
private:
    void setupTypes();
    
    
    Ui::PlotsBuilderWidget *m_widget;
    PlotTypes m_types;
    Analitza::Expression m_example;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PlotsBuilder::PlotTypes)

#endif 


