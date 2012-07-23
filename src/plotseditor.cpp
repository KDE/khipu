/*************************************************************************************
 *  Copyright (C) 2010-2012 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com> *
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
#include "functionlibraryedit.h"
#include "datastore.h"
#include "spacesmodel.h"
#include "spaceplotsproxymodel.h"
#include "analitzagui/variablesmodel.h"
#include <analitzaplot/functiongraph.h>
#include <analitzaplot/planecurve.h>
#include "analitzaplot/plotsmodel.h"
#include "ui_plotseditor.h"
#include <analitzaplot/spacecurve.h>
#include <analitzaplot/surface.h>

PlotsEditor::PlotsEditor(QWidget * parent)
    : QDockWidget(parent)
{
    m_widget = new Ui::PlotsEditorWidget;
    m_widget->setupUi(this);
    setObjectName("adasdds");
    
    //cons
    connect(m_widget->builderDialogBox->button(QDialogButtonBox::Cancel), SIGNAL(pressed()), SLOT(showList()));
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
//lo de arriba ya no va ahora todo es por el builderplots ... mira el metodo map y el siguiente codigo
    
    m_widget->builder->mapConnection(PlotsBuilder::CartesianGraphCurve, this, SLOT(buildCartesianGraphCurve()));
    m_widget->builder->mapConnection(PlotsBuilder::CartesianImplicitCurve, this, SLOT(buildCartesianImplicitCurve()));
    m_widget->builder->mapConnection(PlotsBuilder::CartesianParametricCurve2D, this, SLOT(buildCartesianParametricCurve2D()));
    m_widget->builder->mapConnection(PlotsBuilder::PolarGraphCurve, this, SLOT(buildPolarGraphCurve()));
    m_widget->builder->mapConnection(PlotsBuilder::CartesianParametricCurve3D, this, SLOT(buildCartesianParametricCurve3D()));
    m_widget->builder->mapConnection(PlotsBuilder::CartesianGraphSurface, this, SLOT(buildCartesianGraphSurface()));
    m_widget->builder->mapConnection(PlotsBuilder::CartesianImplicitSurface, this, SLOT(buildCartesianImplicitSurface()));
    m_widget->builder->mapConnection(PlotsBuilder::CartesianParametricSurface, this, SLOT(buildCartesianParametricSurface()));
    m_widget->builder->mapConnection(PlotsBuilder::CylindricalGraphSurface, this, SLOT(buildCylindricalGraphSurface()));
    m_widget->builder->mapConnection(PlotsBuilder::SphericalGraphSurface, this, SLOT(buildSphericalGraphSurface()));

    connect(m_widget->addPlots, SIGNAL(pressed()), SLOT(showTypes()));
    
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
//             m_widget->spaceCurvesLinks->hide();
//             m_widget->surfacesLinks->hide();
//lo de arriba que esta comentado se paso al builder
                m_widget->builder->setupTypes(PlotsBuilder::CartesianGraphCurve | 
                PlotsBuilder::CartesianImplicitCurve | PlotsBuilder::CartesianParametricCurve2D | 
                    PlotsBuilder::PolarGraphCurve);

///


                m_widget->previews->setCurrentIndex(0);
            
            
            break;
        }
        
        case 3:
        {
//             m_widget->surfacesLinks->show();
//             m_widget->spaceCurvesLinks->show();
//             m_widget->planeCurvesLinks->hide();

            m_widget->builder->setupTypes(PlotsBuilder::CartesianParametricCurve3D | PlotsBuilder::CartesianGraphSurface | 
                PlotsBuilder::CartesianImplicitSurface | PlotsBuilder::CartesianParametricSurface | PlotsBuilder::CylindricalGraphSurface |
                PlotsBuilder::SphericalGraphSurface);

            m_widget->previews->setCurrentIndex(1);
            break;
        }
    }
}


void PlotsEditor::reset(bool clearBuilder)
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

    if (clearBuilder)
        //// escondo todos los links solo muestro los necesario dependiendo del tipo de spac : 2d o 3d .. ver editorplots.setcurrentspace
        m_widget->builder->hideAllTypes(); 

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


void PlotsEditor::buildCartesianGraphCurve()
{
    setWindowIcon(KIcon("kde"));
    
    m_currentType = PlotsBuilder::CartesianGraphCurve;
    showEditor();
    m_widget->previews->setCurrentIndex(0); //2d preview
}

void PlotsEditor::buildCartesianImplicitCurve()
{
    m_currentType = PlotsBuilder::CartesianImplicitCurve;
    showEditor();
    m_widget->previews->setCurrentIndex(0); //2d preview
}

void PlotsEditor::buildCartesianParametricCurve2D()
{
    m_currentType = PlotsBuilder::CartesianParametricCurve2D;
    showEditor();
    m_widget->previews->setCurrentIndex(0); //2d preview
}

void PlotsEditor::buildPolarGraphCurve()
{
    m_currentType = PlotsBuilder::PolarGraphCurve;
    showEditor();
    m_widget->previews->setCurrentIndex(0); //2d preview
}

//3D
void PlotsEditor::buildCartesianParametricCurve3D()
{
    m_currentType = PlotsBuilder::CartesianParametricCurve3D;
    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::buildCartesianGraphSurface()
{
    m_currentType = PlotsBuilder::CartesianGraphSurface;
    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::buildCartesianImplicitSurface()
{
    m_currentType = PlotsBuilder::CartesianImplicitSurface;
    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::buildCartesianParametricSurface()
{
    m_currentType = PlotsBuilder::CartesianParametricSurface;
    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::buildCylindricalGraphSurface()
{
    m_currentType = PlotsBuilder::CylindricalGraphSurface;

    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::buildSphericalGraphSurface()
{
    m_currentType = PlotsBuilder::SphericalGraphSurface;
    
    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::savePlot()
{
    QStringList errors;
    
    switch (m_currentType)
    {
        case PlotsBuilder::CartesianGraphCurve:
        case PlotsBuilder::CartesianImplicitCurve:
        case PlotsBuilder::PolarGraphCurve:
            PlaneCurve::canDraw(m_widget->f->expression(), errors);
            break;
        case PlotsBuilder::CartesianParametricCurve2D:
        {
            QString es = "t->vector{"+m_widget->f->expression().toString()+","+
                m_widget->g->expression().toString()+"}";
            
            PlaneCurve::canDraw(Analitza::Expression(es), errors);
            break;
        }

        // 3D
        case PlotsBuilder::CartesianParametricCurve3D:
        {
            QString es = "t->vector{"+m_widget->f->expression().toString()+","+
                m_widget->g->expression().toString()+","+
                m_widget->h->expression().toString()+"}";
            
            SpaceCurve::canDraw(Analitza::Expression(es), errors);
            break;
        }

        case PlotsBuilder::CartesianGraphSurface:
        case PlotsBuilder::CartesianImplicitSurface:
        case PlotsBuilder::CylindricalGraphSurface:
        case PlotsBuilder::SphericalGraphSurface:
                Surface::canDraw(m_widget->f->expression(), errors);
            break;
        case PlotsBuilder::CartesianParametricSurface:
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
        switch (m_currentType)
        {
            case PlotsBuilder::CartesianGraphCurve:
            case PlotsBuilder::CartesianImplicitCurve:
            case PlotsBuilder::PolarGraphCurve:
            {
                PlaneCurve *item = m_document->plotsModel()->addPlaneCurve(m_widget->f->expression(), m_widget->plotName->text(), m_widget->plotColor->color());
                break;
            }
            case PlotsBuilder::CartesianParametricCurve2D:
            {
                QString es = "t->vector{"+m_widget->f->expression().toString()+","+
                    m_widget->g->expression().toString()+"}";
                
                PlaneCurve *item = m_document->plotsModel()->addPlaneCurve(Analitza::Expression(es), m_widget->plotName->text(), m_widget->plotColor->color());
                break;
            }

            // 3D
            case PlotsBuilder::CartesianParametricCurve3D:
            {
                QString es = "t->vector{"+m_widget->f->expression().toString()+","+
                    m_widget->g->expression().toString()+","+
                    m_widget->h->expression().toString()+"}";
                
                SpaceCurve *item = m_document->plotsModel()->addSpaceCurve(Analitza::Expression(es), m_widget->plotName->text(), m_widget->plotColor->color());
                break;
            }

            case PlotsBuilder::CartesianGraphSurface:
            case PlotsBuilder::CartesianImplicitSurface:
            case PlotsBuilder::CylindricalGraphSurface:
            case PlotsBuilder::SphericalGraphSurface:
            {
                Surface::canDraw(m_widget->f->expression(), errors);
                Surface *item = m_document->plotsModel()->addSurface(m_widget->f->expression(), m_widget->plotName->text(), m_widget->plotColor->color());

                break;
            }
            case PlotsBuilder::CartesianParametricSurface:
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





