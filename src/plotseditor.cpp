/*************************************************************************************
 *  Copyright (C) 2010 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>      *
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


#include "plotseditor.h"

#include <KLocale>

#include "analitza/analyzer.h"
#include "analitza/expression.h"
#include "analitza/variables.h"
#include "analitza/value.h"
#include "analitzagui/expressionedit.h"
#include "analitzagui/algebrahighlighter.h"
#include <KRandom>
#include <math.h>
#include <QStringListModel>
#include <QPushButton>
#include <libkdeedu/qtmml/QtMmlWidget>
#include "plotsview.h"
// #include "keomath/solvers/solver.h"
// #include "keomath/solvers/solverfactory.h"
#include "functionlibraryedit.h"
#include "datastore.h"
#include "spacesmodel.h"
#include "spaceplotsproxymodel.h"
#include "analitzagui/variablesmodel.h"
#include <analitzaplot/functiongraph.h>
#include <analitzaplot/planecurve.h>
#include "analitzaplot/plotsmodel.h"
#include "ui_plotseditor.h"
#include <../analitza/analitzaplot/spacecurve.h>
#include <../analitza/analitzaplot/surface.h>

PlotsEditor::PlotsEditor(QWidget * parent)
    : QDockWidget(parent)
{
    m_widget = new Ui::PlotsEditorWidget;
    m_widget->setupUi(this);
    setObjectName("adasdds");
    
    Analitza::Expression e("x*x+3");
    
    m_widget->plotExample->setContent(e.toMathMLPresentation());
// costri r el selecto cmunto para view 
    
    //cons
    connect(m_widget->typesDialogBox->button(QDialogButtonBox::Cancel), SIGNAL(pressed()), SLOT(showList()));
    connect(m_widget->editorDialogBox->button(QDialogButtonBox::Cancel), SIGNAL(pressed()), SLOT(showTypes()));
    connect(m_widget->editorDialogBox->button(QDialogButtonBox::Ok), SIGNAL(pressed()), SLOT(savePlot()));

//     connect(m_widget->createCartesianCurve, SIGNAL(leftClickedUrl(QString)), SLOT(createCartesianCurve()));
//     connect(m_widget->createPolarCurve, SIGNAL(leftClickedUrl(QString)), SLOT(createPolarCurve()));
//     connect(m_widget->createParametricCurve2D, SIGNAL(leftClickedUrl(QString)), SLOT(createParametricCurve2D()));
//     connect(m_widget->createParametricCurve3D, SIGNAL(leftClickedUrl(QString)), SLOT(createParametricCurve3D()));
//     connect(m_widget->createCartesianSurface, SIGNAL(leftClickedUrl(QString)), SLOT(createCartesianSurface()));
//     connect(m_widget->createCylindricalSurface, SIGNAL(leftClickedUrl(QString)), SLOT(createCylindricalSurface()));
//     connect(m_widget->createSphericalSurface, SIGNAL(leftClickedUrl(QString)), SLOT(createSphericalSurface()));
//     connect(m_widget->createParametricSurface, SIGNAL(leftClickedUrl(QString)), SLOT(createParametricSurface()));
    
    connect(m_widget->addPlots, SIGNAL(pressed()), SLOT(showTypes()));
    
    connect(m_widget->roolPlot, SIGNAL(pressed()), SLOT(addPlots()));
    connect(m_widget->removePlot, SIGNAL(pressed()), SLOT(removePlot()));
    
}

PlotsEditor::~PlotsEditor()
{
    delete m_widget;
}

void PlotsEditor::setDocument(DataStore* doc)
{
    m_document  = doc;
    m_widget->plotsView->setModel(m_document->currentPlots());
    m_widget->plotsView->setSelectionModel(m_document->currentSelectionModel());
}

void PlotsEditor::setCurrentSpace(int spaceidx)
{
//     qDebug() << "2dd";
    //proxy config
//     m_document->currentPlots()->setFilterSpace(m_document->spacesModel()->item(spaceidx));
    
//     qDebug() << m_document->currentSpace();
    //set dim profile ... esto depende del space actual
    int dim = m_document->spacesModel()->item(spaceidx)->dimension();
    
 switch (dim)
    {
        case 2:
        {
//             m_widget->planeCurvesLinks->show();
            m_widget->spaceCurvesLinks->hide();
//             m_widget->surfacesLinks->hide();
            m_widget->previews->setCurrentIndex(0);
            break;
        }
        
        case 3:
        {
//             m_widget->surfacesLinks->show();
            m_widget->spaceCurvesLinks->show();
//             m_widget->planeCurvesLinks->hide();
            m_widget->previews->setCurrentIndex(1);
            break;
        }
    }
}


void PlotsEditor::reset()
{
    //clear widgets //TODO GSOC
    m_widget->f->clear();
    m_widget->g->clear();
    m_widget->h->clear();
    m_widget->minx->clear();
    m_widget->maxx->clear();
    m_widget->miny->clear();
    m_widget->maxy->clear();
    m_widget->minz->clear();
    m_widget->maxx->clear();
    
    
    //reset widgets
    m_widget->widgets->setCurrentIndex(0);
//     m_widget->preview3D->camera()/*->centerScene*/;
    m_widget->preview3D->stopAnimation();
    
    
}


void PlotsEditor::showList()
{
    m_widget->widgets->setCurrentIndex(0);
}

void PlotsEditor::showTypes()
{
    m_widget->widgets->setCurrentIndex(1);
}

void PlotsEditor::showEditor()
{
        /// clear actions 
    m_widget->widgets->setCurrentIndex(2);
}

void PlotsEditor::addPlots()
{
//     setCurrentIndex(1);
    
    
    //asert view-Zmodel not null
    
//     PlotsModel *model = qobject_cast< PlotsModel *>(m_plotsView->model());
    
//     model->addPlaneCurve(Analitza::Expression("x=y*y"), "adasd", Qt::red);
}

void PlotsEditor::createCartesianCurve()
{
    m_state = EditingCartesianCurve;
    
    showEditor();
    m_widget->previews->setCurrentIndex(0); //2d preview
}

void PlotsEditor::createPolarCurve()
{
    m_state = EditingPolarCurve;
    showEditor();
    m_widget->previews->setCurrentIndex(0); //2d preview
}

void PlotsEditor::createParametricCurve2D()
{
    m_state = EditingParametricCurve2D;
    showEditor();
    m_widget->previews->setCurrentIndex(0); //2d preview
}

void PlotsEditor::createParametricCurve3D()
{
    m_state = EditingParametricCurve3D;
    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::createCartesianSurface()
{
    m_state = EditingCartesianSurface;
    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::createCylindricalSurface()
{
    m_state = EditingCylindricalSurface;
    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::createSphericalSurface()
{
    m_state = EditingSphericalSurface;
    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::createImplicitSurface()
{
    m_state = EditingImplicitSurface;
    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::createParametricSurface()
{
    m_state = EditingParametricSurface;
    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::savePlot()
{
    QStringList errors;
    
    switch (m_state)
    {
        case EditingCartesianCurve:
        case EditingPolarCurve:
            PlaneCurve::canDraw(m_widget->f->expression(), errors);
            break;
        case EditingParametricCurve2D:
        {
            QString es = "t->vector{"+m_widget->f->expression().toString()+","+
                m_widget->g->expression().toString()+"}";
            
            PlaneCurve::canDraw(Analitza::Expression(es), errors);
            break;
        }

        // 3D
        case EditingParametricCurve3D:
        {
            QString es = "t->vector{"+m_widget->f->expression().toString()+","+
                m_widget->g->expression().toString()+","+
                m_widget->h->expression().toString()+"}";
            
            SpaceCurve::canDraw(Analitza::Expression(es), errors);
            break;
        }

        case EditingCartesianSurface:
        case EditingCylindricalSurface:
        case EditingSphericalSurface:
        case EditingImplicitSurface:
                Surface::canDraw(m_widget->f->expression(), errors);
            break;
        case EditingParametricSurface:
        {
            QString es = "(u,v)->vector{"+m_widget->f->expression().toString()+","+
                m_widget->g->expression().toString()+","+
                m_widget->h->expression().toString()+"}";
            
            Surface::canDraw(Analitza::Expression(es), errors);
            break;
        }
    }
    
    if (errors.isEmpty())
    {
        switch (m_state)
        {
            case EditingCartesianCurve:
            case EditingPolarCurve:
            {
                PlaneCurve *item = m_document->plotsModel()->addPlaneCurve(m_widget->f->expression(), m_widget->plotName->text(), m_widget->plotColor->color());
                break;
            }
            case EditingParametricCurve2D:
            {
                QString es = "t->vector{"+m_widget->f->expression().toString()+","+
                    m_widget->g->expression().toString()+"}";
                
                PlaneCurve *item = m_document->plotsModel()->addPlaneCurve(Analitza::Expression(es), m_widget->plotName->text(), m_widget->plotColor->color());
                break;
            }

            // 3D
            case EditingParametricCurve3D:
            {
                QString es = "t->vector{"+m_widget->f->expression().toString()+","+
                    m_widget->g->expression().toString()+","+
                    m_widget->h->expression().toString()+"}";
                
                SpaceCurve *item = m_document->plotsModel()->addSpaceCurve(Analitza::Expression(es), m_widget->plotName->text(), m_widget->plotColor->color());
                break;
            }

            case EditingCartesianSurface:
            case EditingCylindricalSurface:
            case EditingSphericalSurface:
            case EditingImplicitSurface:
            {
                Surface::canDraw(m_widget->f->expression(), errors);
                Surface *item = m_document->plotsModel()->addSurface(m_widget->f->expression(), m_widget->plotName->text(), m_widget->plotColor->color());

                break;
            }
            case EditingParametricSurface:
            {
                QString es = "(u,v)->vector{"+m_widget->f->expression().toString()+","+
                    m_widget->g->expression().toString()+","+
                    m_widget->h->expression().toString()+"}";
                
                Surface *item = m_document->plotsModel()->addSurface(Analitza::Expression(es), m_widget->plotName->text(), m_widget->plotColor->color());
                break;
            }
        }
        
        reset();
        showList();
    }    
    else
    {
//         setStatusTip(errors);
    }
}

void PlotsEditor::removePlot()
{
    
    if (m_widget->plotsView->selectionModel()->hasSelection())
    {
        static_cast<PlotsModel*>(m_widget->plotsView->model())->removeItem(
            m_widget->plotsView->selectionModel()->currentIndex().row());
    }
}





