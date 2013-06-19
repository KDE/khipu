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
    //connect(m_widget->buildCartesianGraphCurve, SIGNAL(toggled(bool)), SLOT(setupCartesianGraphCurveInfo()));
    
    //clear
    //connect(m_widget->buildCartesianGraphCurve, SIGNAL(leftUrl()), SLOT(clearInfoWidget()));

    // Setting up Mousetracking and Event-filter for all types of plots
    this->setMouseTracking(true);
    this->installEventFilter(this);

    m_widget->buildCartesianGraphCurve->setMouseTracking(true);
    m_widget->buildCartesianGraphCurve->installEventFilter(this);

    m_widget->buildCartesianImplicitCurve->setMouseTracking(true);
    m_widget->buildCartesianImplicitCurve->installEventFilter(this);

    m_widget->buildCartesianParametricCurve2D->setMouseTracking(true);
    m_widget->buildCartesianParametricCurve2D->installEventFilter(this);

    m_widget->buildPolarGraphCurve->setMouseTracking(true);
    m_widget->buildPolarGraphCurve->installEventFilter(this);

    m_widget->buildCartesianParametricCurve3D->setMouseTracking(true);
    m_widget->buildCartesianParametricCurve3D->installEventFilter(this);

    m_widget->buildCartesianGraphSurface->setMouseTracking(true);
    m_widget->buildCartesianGraphSurface->installEventFilter(this);

    m_widget->buildCartesianImplicitSurface->setMouseTracking(true);
    m_widget->buildCartesianImplicitSurface->installEventFilter(this);

    m_widget->buildCartesianParametricSurface->setMouseTracking(true);
    m_widget->buildCartesianParametricSurface->installEventFilter(this);

    m_widget->buildCylindricalGraphSurface->setMouseTracking(true);
    m_widget->buildCylindricalGraphSurface->installEventFilter(this);

    m_widget->buildSphericalGraphSurface->setMouseTracking(true);
    m_widget->buildSphericalGraphSurface->installEventFilter(this);

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
            clearInfoWidget();
            return false;
        }
    }

    setupInfo();
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

void PlotsBuilder::setupInfo()
{
    switch (m_currentTyppe)
    {
    case(CartesianGraphCurve):
        m_example = Analitza::Expression("x->sin(x)");
        break;
    case(CartesianImplicitCurve):
        m_example = Analitza::Expression("(x*x+y*y=1)");
        break;
    case(CartesianParametricCurve2D):
        m_example = Analitza::Expression("t->vector{t,t**2}");
        break;
    case(PolarGraphCurve):
        m_example = Analitza::Expression("q->sin(q)");
        break;

    //3D
    case(CartesianParametricCurve3D):
        m_example = Analitza::Expression("t->vector{t, t, t}");
        break;
    case(CartesianGraphSurface):
        m_example = Analitza::Expression("(x,y)->(x*x-y*y)/8");
        break;
    case(CartesianImplicitSurface):
        m_example = Analitza::Expression("(x*x+y*y)-z*z=1/2");
        break;
    case(CartesianParametricSurface):
        m_example = Analitza::Expression("(u,v)->vector{u,u+v,v}");
        break;
    case(CylindricalGraphSurface):
        m_example = Analitza::Expression("(r,p)->p");
        break;
    case(SphericalGraphSurface):
        m_example = Analitza::Expression("(t,p)->2");
        break;
    default:
        return;
    }
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
