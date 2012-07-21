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
#include "document.h"
#include "spacesmodel.h"
#include "analitzagui/variablesmodel.h"
#include <analitzaplot/functiongraph.h>
#include <analitzaplot/planecurve.h>
#include "analitzaplot/plotsmodel.h"
#include "ui_plotseditor.h"

PlotsEditor::PlotsEditor(QWidget * parent)
    : QDockWidget(parent)
{
    m_widget = new Ui::PlotsEditorWidget;
    m_widget->setupUi(this);
    setObjectName("adasdds");
    
    
    
    Analitza::Expression e("x*x+3");
    
    m_widget->plotExample->setContent(e.toMathMLPresentation());
    m_widget->plotIcon->setPixmap(KIcon("kde").pixmap(32,32));
// costri r el selecto cmunto para view 
    
    //cons
    connect(m_widget->typesDialogBox->button(QDialogButtonBox::Cancel), SIGNAL(pressed()), SLOT(showList()));
    connect(m_widget->editorDialogBox->button(QDialogButtonBox::Cancel), SIGNAL(pressed()), SLOT(showTypes()));
    connect(m_widget->editorDialogBox->button(QDialogButtonBox::Ok), SIGNAL(pressed()), SLOT(savePlot()));

    connect(m_widget->createCartesianCurve, SIGNAL(leftClickedUrl(QString)), SLOT(createCartesianCurve()));
    connect(m_widget->createPolarCurve, SIGNAL(leftClickedUrl(QString)), SLOT(createPolarCurve()));
    connect(m_widget->createParametricCurve2D, SIGNAL(leftClickedUrl(QString)), SLOT(createParametricCurve2D()));
    connect(m_widget->createParametricCurve3D, SIGNAL(leftClickedUrl(QString)), SLOT(createParametricCurve3D()));
    connect(m_widget->createCartesianSurface, SIGNAL(leftClickedUrl(QString)), SLOT(createCartesianSurface()));
    connect(m_widget->createCylindricalSurface, SIGNAL(leftClickedUrl(QString)), SLOT(createCylindricalSurface()));
    connect(m_widget->createSphericalSurface, SIGNAL(leftClickedUrl(QString)), SLOT(createSphericalSurface()));
    connect(m_widget->createParametricSurface, SIGNAL(leftClickedUrl(QString)), SLOT(createParametricSurface()));
    
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
    m_widget->plotsView->setModel(m_document->spacePlotsFilterProxyModel());

}

void PlotsEditor::setCurrentSpace(int spaceidx)
{
    //proxy config
    m_document->spacePlotsFilterProxyModel()->setFilterSpace(m_document->spacesModel()->item(spaceidx));
    
//     qDebug() << m_document->currentSpace();
    //set dim profile ... esto depende del space actual
    int dim = m_document->spacesModel()->item(spaceidx)->dimension();
    
 switch (dim)
    {
        case 2:
        {
            m_widget->planeCurvesLinks->show();
            m_widget->spaceCurvesLinks->hide();
            m_widget->surfacesLinks->hide();
            m_widget->previews->setCurrentIndex(0);
            break;
        }
        
        case 3:
        {
            m_widget->surfacesLinks->show();
            m_widget->spaceCurvesLinks->show();
            m_widget->planeCurvesLinks->hide();
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
    showEditor();
    m_widget->previews->setCurrentIndex(0); //2d preview
}

void PlotsEditor::createPolarCurve()
{
    showEditor();
    m_widget->previews->setCurrentIndex(0); //2d preview
}

void PlotsEditor::createParametricCurve2D()
{
    showEditor();
    m_widget->previews->setCurrentIndex(0); //2d preview
}

void PlotsEditor::createParametricCurve3D()
{
    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::createCartesianSurface()
{
    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::createCylindricalSurface()
{
    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::createSphericalSurface()
{
    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::createParametricSurface()
{
    showEditor();
    m_widget->previews->setCurrentIndex(1); //3d preview
}

void PlotsEditor::savePlot()
{
    qDebug() << "asd 5" << PlaneCurve::canDraw(m_widget->f->expression());
    
    m_document->plotsModel()->addPlaneCurve(m_widget->f->expression(), m_widget->plotName->text(), m_widget->plotColor->color());
}

void PlotsEditor::removePlot()
{
    
    if (m_widget->plotsView->selectionModel()->hasSelection())
    {
        static_cast<PlotsModel*>(m_widget->plotsView->model())->removeItem(
            m_widget->plotsView->selectionModel()->currentIndex().row());
    }
}





