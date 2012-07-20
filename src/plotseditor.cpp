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
#include "plotsview.h"
// #include "keomath/solvers/solver.h"
// #include "keomath/solvers/solverfactory.h"
#include "functionlibraryedit.h"
#include "analitzagui/variablesmodel.h"
#include <analitzaplot/functiongraph.h>
#include <analitzaplot/planecurve.h>
#include "analitzaplot/plotsmodel.h"
#include "ui_plotseditor.h"

PlotsEditor::PlotsEditor(QWidget * parent)
    : QDockWidget(parent)
{
    Ui::DockWidget dd;
    dd.setupUi(this);
    setObjectName("adasdds");
    
//     m_plotsView = dd.listView;
// costri r el selecto cmunto para view 
    
    ///
    
    
    PlotsModel *model = new PlotsModel(this);
//     QItemSelectionModel *selection = new QItemSelectionModel(model);
        dd.treeView->setModel(model);

    PlaneCurve *item = model->addPlaneCurve(Analitza::Expression("x->x*x"), "para", Qt::cyan);
    model->addPlaneCurve(Analitza::Expression("q->q+2"), "polar simple", Qt::green);
    model->addPlaneCurve(Analitza::Expression("t->vector{t*t+1, t+2}"), "vec", Qt::yellow);
    PlaneCurve *item2 = model->addPlaneCurve(Analitza::Expression("5*(x**2+y**2)**3=15*(x*y*72)**2"), "impl", Qt::red);
    model->addPlaneCurve(Analitza::Expression("x->2+x*x"), "otra simple", Qt::blue);
//     
    model->addPlaneCurve(Analitza::Expression("(x**2+y**2)**3=4*(x**2)*(y**2)"), "otra simple", Qt::lightGray);
    model->addPlaneCurve(Analitza::Expression("(y-x**2)**2=x*y**3"), "otra simple", Qt::lightGray);
    model->addPlaneCurve(Analitza::Expression("sin(x)*sin(y)=1/2"), "otra simple", Qt::yellow);
    
    
//     qDebug() << "???" << model->rowCount();
//     QStringListModel *m = new QStringListModel(this);
//     m->setStringList(QStringList() << "adasd" << "234353");
//     
//     dd.treeView->setModel(m);
}

PlotsEditor::~PlotsEditor()
{

}

void PlotsEditor::setModel(PlotsModel* m)
{
    
    qDebug() << 555555555555555;
    
//     m_plotsView->setModel(m);
//     m_plotsView->update();

    
}


void PlotsEditor::showList()
{
}

void PlotsEditor::showCreateByName()
{

}

void PlotsEditor::showCreateByExpression()
{

}

void PlotsEditor::showEditor()
{

}




