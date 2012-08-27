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

#include "plotsbuilder.h"

#include "ui_plotsbuilder.h"

PlotsBuilder::PlotsBuilder(QWidget* parent): QWidget(parent)
{
    m_widget = new Ui::PlotsBuilderWidget;
    m_widget->setupUi(this);
    
    //BEGIN TODO descomentar mas adelante 
    //NOTE see images/mockups of this GSoC
    //TODO
//     m_widget->advancedPlots->hide();
//     m_widget->inequalitiesPlots->hide();
    
    //END TODO
    
    //TODO support many examples
    //TODO agregar sistema de tipos a anlitza plot?? ... ya se tienen names .. filtrar por otro tipo adicional
//     QVBoxLayout *lay = new QVBoxLayout(m_widget->examples);
    
    //setup description/information area
    connect(m_widget->buildCartesianGraphCurve, SIGNAL(enteredUrl()), SLOT(setupCartesianGraphCurveInfo()));
    
    //clear
    connect(m_widget->buildCartesianGraphCurve, SIGNAL(leftUrl()), SLOT(clearInfoWidget()));
    
    //BEGIN setup icons
    #define setTypeIcon(tname, iconame) m_widget->build##tname##Icon->setPixmap(KIcon( iconame ).pixmap(16,16));
    

    setTypeIcon(CartesianGraphCurve, "newfunction");
    setTypeIcon(CartesianImplicitCurve, "newimplicit");
    setTypeIcon(CartesianParametricCurve2D, "newparametric");
    setTypeIcon(PolarGraphCurve, "newpolar");
    setTypeIcon(CartesianParametricCurve3D, "newparametric3d");
    setTypeIcon(CartesianGraphSurface, "newfunction3d");
    setTypeIcon(CartesianImplicitSurface, "draw-square-inverted-corners");
    setTypeIcon(CartesianParametricSurface, "draw-donut");
    setTypeIcon(CylindricalGraphSurface, "newcylindrical");
    setTypeIcon(SphericalGraphSurface, "newspherical");
    
    //END setup icons
}

PlotsBuilder::~PlotsBuilder()
{
    delete m_widget;
}

void PlotsBuilder::setupTypes(PlotsBuilder::PlotTypes t)
{
    m_types = t;
    
    setupTypes();
}


void PlotsBuilder::mapConnection(PlotsBuilder::PlotType pt, QObject* recvr, const char* slot)
{
    #define caseType(tname) case tname : connect(m_widget->build##tname , SIGNAL(leftClickedUrl()), recvr, slot); break;
    
    switch (pt)
    {
        caseType(CartesianGraphCurve);
        caseType(CartesianImplicitCurve);
        caseType(CartesianParametricCurve2D);
        caseType(PolarGraphCurve);
        //3D
        caseType(CartesianParametricCurve3D);
        caseType(CartesianGraphSurface);
        caseType(CartesianImplicitSurface);
        caseType(CartesianParametricSurface);
        caseType(CylindricalGraphSurface);
        caseType(SphericalGraphSurface);
    }
}

void PlotsBuilder::showAllTypes()
{
    #define showType(tname) m_widget->build##tname->show(); 
    
    showType(CartesianGraphCurve);
    showType(CartesianGraphCurve);
    showType(CartesianImplicitCurve);
    showType(CartesianParametricCurve2D);
    showType(PolarGraphCurve);
    //3D
    showType(CartesianParametricCurve3D);
    showType(CartesianGraphSurface);
    showType(CartesianImplicitSurface);
    showType(CartesianParametricSurface);
    showType(CylindricalGraphSurface);
    showType(SphericalGraphSurface);
    
    m_widget->cartesianCurvesLinks->show();
    m_widget->polarCurvesLinks->show();
    m_widget->spaceCurvesLinks->show();
    m_widget->cartesianCurvesLinks->show();
    m_widget->cylindricalSurfacesLinks->show();
    m_widget->sphericalSurfacesLinks->show();
}

void PlotsBuilder::hideAllTypes()
{
    #define hideType(tname) m_widget->build##tname->hide(); 
    
    hideType(CartesianGraphCurve);
    hideType(CartesianGraphCurve);
    hideType(CartesianImplicitCurve);
    hideType(CartesianParametricCurve2D);
    hideType(PolarGraphCurve);
    //3D
    hideType(CartesianParametricCurve3D);
    hideType(CartesianGraphSurface);
    hideType(CartesianImplicitSurface);
    hideType(CartesianParametricSurface);
    hideType(CylindricalGraphSurface);
    hideType(SphericalGraphSurface);
    
    m_widget->cartesianCurvesLinks->hide();
    m_widget->polarCurvesLinks->hide();
    m_widget->spaceCurvesLinks->hide();
    m_widget->cartesianCurvesLinks->hide();
    m_widget->cylindricalSurfacesLinks->hide();
    m_widget->sphericalSurfacesLinks->hide();
}

void PlotsBuilder::setupCartesianGraphCurveInfo()
{
    m_example = Analitza::Expression("x->x**2-sin(x)");
    
    m_widget->plotExample->setContent(m_example.toMathMLPresentation());
}

void PlotsBuilder::clearInfoWidget()
{
    m_widget->plotExample->clear();
    m_widget->plotExample->update();
}

void PlotsBuilder::setupTypes()
{
    #define testType(tname) if (m_types.testFlag( tname )) m_widget->build##tname->show(); else m_widget->build##tname->hide();

    testType(CartesianGraphCurve);
    testType(CartesianGraphCurve);
    testType(CartesianImplicitCurve);
    testType(CartesianParametricCurve2D);
    testType(PolarGraphCurve);
    //3D
    testType(CartesianParametricCurve3D);
    testType(CartesianGraphSurface);
    testType(CartesianImplicitSurface);
    testType(CartesianParametricSurface);
    testType(CylindricalGraphSurface);
    testType(SphericalGraphSurface);
    
    if (m_types.testFlag(CartesianGraphCurve) || m_types.testFlag(CartesianImplicitCurve) || m_types.testFlag(CartesianParametricCurve2D))
        m_widget->cartesianCurvesLinks->show();
    else
        m_widget->cartesianCurvesLinks->hide();
    
    if (m_types.testFlag(PolarGraphCurve))
        m_widget->polarCurvesLinks->show();
    else
        m_widget->polarCurvesLinks->hide();
    
    if (m_types.testFlag(CartesianParametricCurve3D))
        m_widget->spaceCurvesLinks->show();
    else
        m_widget->spaceCurvesLinks->hide();

    if (m_types.testFlag(CartesianGraphSurface) || m_types.testFlag(CartesianImplicitSurface) || m_types.testFlag(CartesianParametricSurface))
        m_widget->cartesianSurfacesLinks->show();
    else
        m_widget->cartesianSurfacesLinks->hide();

    if (m_types.testFlag(CylindricalGraphSurface))
        m_widget->cylindricalSurfacesLinks->show();
    else
        m_widget->cylindricalSurfacesLinks->hide();
    
    if (m_types.testFlag(SphericalGraphSurface))
        m_widget->sphericalSurfacesLinks->show();
    else
        m_widget->sphericalSurfacesLinks->hide();
}
