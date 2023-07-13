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

PlotsBuilder::PlotsBuilder(QWidget* parent): QWidget(parent),
    m_currentTyppe(None)
{
    m_widget = new Ui::PlotsBuilderWidget;
    m_widget->setupUi(this);
    
    // Setting up Mousetracking and Event-filter for all types of plots
    this->setMouseTracking(true);
    this->installEventFilter(this);

    // can be done also by enter event and leave event using underMouse();
    m_widget->buildCartesianGraphCurve->setIcon(QIcon::fromTheme("newfunction"));
    m_widget->buildCartesianGraphCurve->setMouseTracking(true);
    m_widget->buildCartesianGraphCurve->installEventFilter(this);

    m_widget->buildCartesianImplicitCurve->setIcon(QIcon::fromTheme("newimplicit"));
    m_widget->buildCartesianImplicitCurve->setMouseTracking(true);
    m_widget->buildCartesianImplicitCurve->installEventFilter(this);

    m_widget->buildCartesianParametricCurve2D->setIcon(QIcon::fromTheme("newparametric"));
    m_widget->buildCartesianParametricCurve2D->setMouseTracking(true);
    m_widget->buildCartesianParametricCurve2D->installEventFilter(this);

    m_widget->buildPolarGraphCurve->setIcon(QIcon::fromTheme("newpolar"));
    m_widget->buildPolarGraphCurve->setMouseTracking(true);
    m_widget->buildPolarGraphCurve->installEventFilter(this);

    m_widget->buildCartesianParametricCurve3D->setIcon(QIcon::fromTheme("newparametric3d"));
    m_widget->buildCartesianParametricCurve3D->setMouseTracking(true);
    m_widget->buildCartesianParametricCurve3D->installEventFilter(this);

    m_widget->buildCartesianGraphSurface->setIcon(QIcon::fromTheme("newfunction3d"));
    m_widget->buildCartesianGraphSurface->setMouseTracking(true);
    m_widget->buildCartesianGraphSurface->installEventFilter(this);

    m_widget->buildCartesianImplicitSurface->setIcon(QIcon::fromTheme("draw-square-inverted-corners"));
    m_widget->buildCartesianImplicitSurface->setMouseTracking(true);
    m_widget->buildCartesianImplicitSurface->installEventFilter(this);

    m_widget->buildCartesianParametricSurface->setIcon(QIcon::fromTheme("draw-donut"));
    m_widget->buildCartesianParametricSurface->setMouseTracking(true);
    m_widget->buildCartesianParametricSurface->installEventFilter(this);

    m_widget->buildCylindricalGraphSurface->setIcon(QIcon::fromTheme("newcylindrical"));
    m_widget->buildCylindricalGraphSurface->setMouseTracking(true);
    m_widget->buildCylindricalGraphSurface->installEventFilter(this);

    m_widget->buildSphericalGraphSurface->setIcon(QIcon::fromTheme("newspherical"));
    m_widget->buildSphericalGraphSurface->setMouseTracking(true);
    m_widget->buildSphericalGraphSurface->installEventFilter(this);
}

bool PlotsBuilder::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        if(object==m_widget->buildCartesianGraphCurve)
            m_currentTyppe=CartesianGraphCurve;

        else if(object==m_widget->buildCartesianImplicitCurve)
            m_currentTyppe=CartesianImplicitCurve;

        else if(object==m_widget->buildCartesianParametricCurve2D)
            m_currentTyppe=CartesianParametricCurve2D;

        else if(object==m_widget->buildPolarGraphCurve)
            m_currentTyppe=PolarGraphCurve;

        else if(object==m_widget->buildCartesianParametricCurve3D)
            m_currentTyppe=CartesianParametricCurve3D;

        else if(object==m_widget->buildCartesianGraphSurface)
            m_currentTyppe=CartesianGraphSurface;

        else if(object==m_widget->buildCartesianImplicitSurface)
            m_currentTyppe=CartesianImplicitSurface;

        else if(object==m_widget->buildCartesianParametricSurface)
            m_currentTyppe=CartesianParametricSurface;

        else if(object==m_widget->buildCylindricalGraphSurface)
            m_currentTyppe=CylindricalGraphSurface;

        else if(object==m_widget->buildSphericalGraphSurface)
            m_currentTyppe=SphericalGraphSurface;

        else {
            m_currentTyppe=None;
            return false;
        }
    }
    return false;
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
    #define caseType(tname) case tname : connect(m_widget->build##tname , SIGNAL(clicked(bool)), recvr, slot); break;
    
    switch (pt)
    {
        //2D
        caseType(CartesianGraphCurve)
        caseType(CartesianImplicitCurve)
        caseType(CartesianParametricCurve2D)
        caseType(PolarGraphCurve)
        //3D
        caseType(CartesianParametricCurve3D)
        caseType(CartesianGraphSurface)
        caseType(CartesianImplicitSurface)
        caseType(CartesianParametricSurface)
        caseType(CylindricalGraphSurface)
        caseType(SphericalGraphSurface)
        case(None): break;
    }
}

void PlotsBuilder::showAllTypes()
{
    #define showType(tname) m_widget->build##tname->show(); 
    
    //2D
    showType(CartesianGraphCurve)
    showType(CartesianGraphCurve)
    showType(CartesianImplicitCurve)
    showType(CartesianParametricCurve2D)
    showType(PolarGraphCurve)
    //3D
    showType(CartesianParametricCurve3D)
    showType(CartesianGraphSurface)
    showType(CartesianImplicitSurface)
    showType(CartesianParametricSurface)
    showType(CylindricalGraphSurface)
    showType(SphericalGraphSurface)
    
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
    
    //2D
    hideType(CartesianGraphCurve)
    hideType(CartesianGraphCurve)
    hideType(CartesianImplicitCurve)
    hideType(CartesianParametricCurve2D)
    hideType(PolarGraphCurve)
    //3D
    hideType(CartesianParametricCurve3D)
    hideType(CartesianGraphSurface)
    hideType(CartesianImplicitSurface)
    hideType(CartesianParametricSurface)
    hideType(CylindricalGraphSurface)
    hideType(SphericalGraphSurface)
    
    m_widget->cartesianCurvesLinks->hide();
    m_widget->polarCurvesLinks->hide();
    m_widget->spaceCurvesLinks->hide();
    m_widget->cartesianCurvesLinks->hide();
    m_widget->cylindricalSurfacesLinks->hide();
    m_widget->sphericalSurfacesLinks->hide();
}

void PlotsBuilder::setupTypes()
{
    #define testType(tname) if (m_types.testFlag( tname )) m_widget->build##tname->show(); else m_widget->build##tname->hide();

    //2D
    testType(CartesianGraphCurve)
    testType(CartesianGraphCurve)
    testType(CartesianImplicitCurve)
    testType(CartesianParametricCurve2D)
    testType(PolarGraphCurve)
    //3D
    testType(CartesianParametricCurve3D)
    testType(CartesianGraphSurface)
    testType(CartesianImplicitSurface)
    testType(CartesianParametricSurface)
    testType(CylindricalGraphSurface)
    testType(SphericalGraphSurface)
    
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

#include "moc_plotsbuilder.cpp"
