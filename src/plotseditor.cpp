/*************************************************************************************
 *  Copyright (C) 2010-2014 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com> *
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
#include "ui_plotseditor.h"

//Analitza includes
#include <analitza/analyzer.h>
#include <analitza/expression.h>
#include <analitza/variables.h>
#include <analitza/value.h>
#include <analitzagui/expressionedit.h>
#include <analitzagui/algebrahighlighter.h>
#include <analitzagui/variablesmodel.h>
#include <analitzaplot/functiongraph.h>
#include <analitzaplot/planecurve.h>
#include <analitzaplot/plotsmodel.h>
#include <analitzaplot/spacecurve.h>
#include <analitzaplot/surface.h>
#include <analitzaplot/plotsfactory.h>

//Qt includes
#include <QStringListModel>
#include <QPushButton>
#include <qstyleditemdelegate.h>
#include <QPainter>

//KDE includes
#include <KRandom>

#include <cmath>

//local includes
#include "datastore.h"
#include "spacesmodel.h"
#include "spaceplotsproxymodel.h"
#include "spaceitem.h"

using namespace Analitza;

PlotsEditor::PlotsEditor(QWidget * parent)
    : QDockWidget(parent), isEditing(false), plotnumber(1)
{
    m_widget = new Ui::PlotsEditorWidget;
    m_widget->setupUi(this);
    setObjectName("adasdds");

    // random coloring of the plots
    m_widget->plotColor->setColor(randomFunctionColor());

    m_widget->focusPlot->setToolTip(i18n("check/uncheck to show/hide the Axes"));
    m_widget->focusPlot->setChecked(true);

    m_widget->editPlot->setToolTip(i18n("Click to edit the plot"));
    m_widget->removePlot->setToolTip(i18n("Click to remove the plot"));

    // no bounding by default
    m_widget->intervals->setChecked(false);

    connect(m_widget->fnameForGraphs, SIGNAL(currentIndexChanged(QString)), SLOT(setCurrentFunctionGraphs(QString)));

    m_widget->farrow->setText("=");
    m_widget->garrow->setText("=");
    m_widget->harrow->setText("=");

    connect(m_widget->builderDialogBox->button(QDialogButtonBox::Cancel), SIGNAL(pressed()), SLOT(showList()));
    connect(m_widget->editorDialogBox->button(QDialogButtonBox::Cancel), SIGNAL(pressed()), SLOT(cancelEditor()));
    connect(m_widget->editorDialogBox->button(QDialogButtonBox::Ok), SIGNAL(pressed()), SLOT(savePlot()));

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

    connect(m_widget->addPlots, SIGNAL(pressed()), SLOT(addPlots()));
    connect(m_widget->editPlot, SIGNAL(pressed()), SLOT(editPlot()));
    connect(m_widget->plotsView, SIGNAL(doubleClicked(QModelIndex)), SLOT(editPlot()));
    connect(m_widget->removePlot, SIGNAL(pressed()), SLOT(removePlot()));
    connect(m_widget->focusPlot,SIGNAL(stateChanged(int)),SLOT(showAxis(int)));
    connect(m_widget->plotnamecheck,SIGNAL(toggled(bool)),SLOT(plotnamecheckClicked(bool)));

    this->installEventFilter(this);
    m_widget->f->installEventFilter(this);
    plotnamecheckClicked(false);
}

bool PlotsEditor::eventFilter(QObject*, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key = static_cast<QKeyEvent *>(event);
        if(key->key() == Qt::Key_Enter || key->key() == Qt::Key_Return)
        {
            savePlot();
            m_widget->plotnamecheck->setChecked(false);
        }
    }
    return false;
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
    switch (m_document->spacesModel()->space(spaceidx)->dimension())
    {
    case Dim2D:
    {
        m_widget->builder->setupTypes(PlotsBuilder::CartesianGraphCurve |
                                      PlotsBuilder::CartesianImplicitCurve | PlotsBuilder::CartesianParametricCurve2D |
                                      PlotsBuilder::PolarGraphCurve);
        m_widget->styleWidget->hide();
        break;
    }
    case Dim3D:
    {
        m_widget->builder->setupTypes(PlotsBuilder::CartesianParametricCurve3D | PlotsBuilder::CartesianGraphSurface |
                                      PlotsBuilder::CartesianImplicitSurface | PlotsBuilder::CartesianParametricSurface | PlotsBuilder::CylindricalGraphSurface |
                                      PlotsBuilder::SphericalGraphSurface);
        m_widget->styleWidget->show();
        break;
    }
    default:
        break;
    }
}

void PlotsEditor::reset(bool clearBuilder)
{
    //clear widgets
    m_widget->plotName->clear();
    m_widget->plotnamecheck->setChecked(false);
    m_widget->plotColor->setColor(randomFunctionColor());

    m_widget->f->clear();
    m_widget->g->clear();
    m_widget->h->clear();
    m_widget->minx->clear();
    m_widget->maxx->clear();
    m_widget->miny->clear();
    m_widget->maxy->clear();
    m_widget->minz->clear();
    m_widget->maxz->clear();

    //reset widgets
    m_widget->widgets->setCurrentIndex(0);

    if (clearBuilder)
        m_widget->builder->hideAllTypes();

    //focus
    m_widget->plotName->setFocus();
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

    /// force right focus
    // while editing the plot , it is very likely that user needs to change the expression.
    m_widget->f->setFocus();
}

void PlotsEditor::cancelEditor()
{
    if (isEditing)
    {
        showList();
        isEditing = false;
        return;
    }
    if (m_cancelIsGoHome)
    {
        emit goHome();
        m_document->removeCurrentSpace();
    }
    else 
        showTypes();
}

void PlotsEditor::showAxis(int state)
{
    if (state==0) {
        if(m_document->spacesModel()->space(m_document->currentSpace())->dimension()==Dim2D)
            emit updateGridcolor(QColor(255,255,255)); // Axis willnot be visible when the color will be white.
        else
            emit updateGridcolor(QColor(Qt::black));  // Axis willnot be visible when the color will be black.
    }
    else if (state==2) {
        if(m_document->spacesModel()->space(m_document->currentSpace())->dimension()==Dim2D)
            emit updateGridcolor(QColor(230,230,230)); // Axis will be visible when the color will be grey.
        else
            emit updateGridcolor(QColor(Qt::darkGray)); // Axis will be visible when the color will be darkGrey.
    }
}

void PlotsEditor::addPlots()
{
    m_cancelIsGoHome = false;
    showTypes();
}

void PlotsEditor::editPlot(const QModelIndex &)
{
    // comes when double clicked on the items in the list shown with checkbox.!
    isEditing = true;

    reset();

    // while editing the plot , it is very likely that user needs to change the expression.
    m_widget->f->setFocus();

    if (m_widget->plotsView->selectionModel()->hasSelection()) // may be the plotsview is different .!!!
    {
        QModelIndex index = m_widget->plotsView->selectionModel()->currentIndex();
        PlotItem* item = index.data(PlotsModel::PlotRole).value<PlotItem*>();
        
        m_widget->plotColor->setColor(item->color());
        m_widget->plotName->setText(item->name());

        if (dynamic_cast<PlaneCurve*>(item))
        {
            PlaneCurve *curve = dynamic_cast<PlaneCurve*>(item);
            
            if (Expression(curve->display()).isEquation()) // implicit
            {
                m_widget->f->setExpression(Expression(curve->display()));
                buildCartesianImplicitCurve();

                if (curve->hasIntervals())
                {
                    QPair<Analitza::Expression, Analitza::Expression> interval = curve->interval(curve->parameters().at(1), false);
                
                    m_widget->miny->setExpression(interval.first);
                    m_widget->maxy->setExpression(interval.second);
                }
            }
            else if (curve->expression().lambdaBody().isVector()) //vectorvalued
            {
                m_widget->f->setExpression(curve->expression().lambdaBody().elementAt(0));
                m_widget->g->setExpression(curve->expression().lambdaBody().elementAt(1));

                buildCartesianParametricCurve2D();
            }
            else //graph
            {
                m_widget->f->setExpression(curve->expression().lambdaBody());

                if (curve->parameters().first() == "q") 
                {
                    buildPolarGraphCurve();
                }
                else
                {
                    buildCartesianGraphCurve();

                    if (curve->parameters().first() == "x")
                    {
                        m_currentVars = QStringList() << "x";
                        m_widget->fnameForGraphs->hide();
                        setupFuncName(1, "y", QStringList() << "x");
                        setupVarName(1, "x");
                        m_widget->fname->show();
                    }
                    else if (curve->parameters().first() == "y")
                    {
                        m_currentVars = QStringList() << "y";
                        m_widget->fnameForGraphs->hide();
                        setupFuncName(1, "x", QStringList() << "y");
                        setupVarName(1, "y");
                        m_widget->fname->show();
                    }
                }
            }

            if (curve->hasIntervals())
            {
                QPair<Analitza::Expression, Analitza::Expression> interval = curve->interval(curve->parameters().first(), false);
                
                m_widget->minx->setExpression(interval.first);
                m_widget->maxx->setExpression(interval.second);
            }
        }
        else if (dynamic_cast<SpaceCurve*>(item))
        {
            SpaceCurve *curve = dynamic_cast<SpaceCurve*>(item);

            m_widget->f->setExpression(curve->expression().lambdaBody().elementAt(0));
            m_widget->g->setExpression(curve->expression().lambdaBody().elementAt(1));
            m_widget->h->setExpression(curve->expression().lambdaBody().elementAt(2));

            buildCartesianParametricCurve3D();

            
            if (curve->hasIntervals())
            {
                QPair<Analitza::Expression, Analitza::Expression> interval = curve->interval(curve->parameters().first(), false);
                
                m_widget->minx->setExpression(interval.first);
                m_widget->maxx->setExpression(interval.second);
            }
        }
        else if (dynamic_cast<Surface*>(item))
        {
            Surface *surface = dynamic_cast<Surface*>(item);

            if (Expression(surface->display()).isEquation()) // implicit
            {
                m_widget->f->setExpression(Expression(surface->display()));

                buildCartesianImplicitSurface();

                if (surface->hasIntervals())
                {
                    QPair<Analitza::Expression, Analitza::Expression> interval = surface->interval(surface->parameters().at(2), false);
                    
                    m_widget->minz->setExpression(interval.first);
                    m_widget->maxz->setExpression(interval.second);
                }
            }
            else if (surface->expression().lambdaBody().isVector()) //vectorvalued
            {
                m_widget->f->setExpression(surface->expression().lambdaBody().elementAt(0));
                m_widget->g->setExpression(surface->expression().lambdaBody().elementAt(1));
                m_widget->h->setExpression(surface->expression().lambdaBody().elementAt(2));

                buildCartesianParametricSurface();
            }
            else //graph
            {
                m_widget->f->setExpression(surface->expression().lambdaBody());

                if (surface->parameters() == QStringList() << "r" << "p")
                    buildCylindricalGraphSurface();
                else if (surface->parameters() == QStringList() << "t" << "p")
                    buildSphericalGraphSurface();
                else
                {
                    buildCartesianGraphSurface();

                    if (surface->parameters() == QStringList() << "x" << "y")
                    {
                        m_currentVars = QStringList() << "x" << "y";
                        m_widget->fnameForGraphs->hide();
                        setupFuncName(1, "z", QStringList() << "x" << "y");
                        setupVarName(1, "x");
                        setupVarName(2, "y");
                        m_widget->fname->show();
                    }
                    else if (surface->parameters() == QStringList() << "x" << "z")
                    {
                        m_currentVars = QStringList() << "x" << "z";
                        m_widget->fnameForGraphs->hide();
                        setupFuncName(1, "y", QStringList() << "x" << "z");
                        setupVarName(1, "x");
                        setupVarName(2, "z");
                        m_widget->fname->show();
                    }
                    else if (surface->parameters() == QStringList() << "y" << "z")
                    {
                        m_currentVars = QStringList() << "y" << "z";
                        m_widget->fnameForGraphs->hide();
                        setupFuncName(1, "x", QStringList() << "y" << "z");
                        setupVarName(1, "y");
                        setupVarName(2, "z");
                        m_widget->fname->show();
                    }
                }
            }

            if (surface->hasIntervals())
            {
                QPair<Analitza::Expression, Analitza::Expression> interval = surface->interval(surface->parameters().first(), false);

                m_widget->minx->setExpression(interval.first);
                m_widget->maxx->setExpression(interval.second);

                interval = surface->interval(surface->parameters().at(1), false);

                m_widget->miny->setExpression(interval.first);
                m_widget->maxy->setExpression(interval.second);
            }
        }
    }
}

void PlotsEditor::buildCartesianGraphCurve(bool cancelIsGoHome)
{
    if (!isEditing)
        reset();

    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::CartesianGraphCurve;

    m_widget->plotIcon->setPixmap(QIcon::fromTheme("newfunction").pixmap(16, 16));

    setupExpressionType(QStringList() << "x" << "y", QStringList() << "x");

    m_widget->minx->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxx->setExpression(Analitza::Expression(Analitza::Cn(5)));
}

void PlotsEditor::buildCartesianImplicitCurve(bool cancelIsGoHome)
{
    if (!isEditing)
        reset();

    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::CartesianImplicitCurve;

    m_widget->plotIcon->setPixmap(QIcon::fromTheme("newimplicit").pixmap(16, 16));

    setupExpressionType(QStringList(), QStringList() << "x" << "y", true);

    m_widget->minx->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxx->setExpression(Analitza::Expression(Analitza::Cn(5)));

    m_widget->miny->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxy->setExpression(Analitza::Expression(Analitza::Cn(5)));
}

void PlotsEditor::buildCartesianParametricCurve2D(bool cancelIsGoHome)
{
    if (!isEditing)
        reset();

    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::CartesianParametricCurve2D;

    m_widget->plotIcon->setPixmap(QIcon::fromTheme("newparametric").pixmap(16, 16));

    setupExpressionType(QStringList() << "x" << "y", QStringList() << "t", false, true);

    m_widget->minx->setExpression(Analitza::Expression("-pi"));
    m_widget->maxx->setExpression(Analitza::Expression("pi"));
}

void PlotsEditor::buildPolarGraphCurve(bool cancelIsGoHome)
{
    if (!isEditing)
        reset();

    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::PolarGraphCurve;

    m_widget->plotIcon->setPixmap(QIcon::fromTheme("draw-spiral").pixmap(16, 16));

    setupExpressionType(QStringList()<<"q", QStringList() << "q");

    m_widget->minx->setExpression(Analitza::Expression(Analitza::Cn(0)));
    m_widget->maxx->setExpression(Analitza::Expression("2*pi"));
}

//3D
void PlotsEditor::buildCartesianParametricCurve3D(bool cancelIsGoHome)
{
    if (!isEditing)
        reset();

    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::CartesianParametricCurve3D;

    m_widget->plotIcon->setPixmap(QIcon::fromTheme("newparametric3d").pixmap(16, 16));

    setupExpressionType(QStringList() << "x" << "y" << "z", QStringList() << "t", false, true);

    m_widget->minx->setExpression(Analitza::Expression("-pi"));
    m_widget->maxx->setExpression(Analitza::Expression("pi"));
}

void PlotsEditor::buildCartesianGraphSurface(bool cancelIsGoHome)
{
    if (!isEditing)
        reset();

    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::CartesianGraphSurface;

    m_widget->plotIcon->setPixmap(QIcon::fromTheme("newfunction3d").pixmap(16, 16));

    setupExpressionType(QStringList() << "x,y" << "x,z" << "y,z", QStringList() << "x" << "y");

    m_widget->minx->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxx->setExpression(Analitza::Expression(Analitza::Cn(5)));

    m_widget->miny->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxy->setExpression(Analitza::Expression(Analitza::Cn(5)));
}

void PlotsEditor::buildCartesianImplicitSurface(bool cancelIsGoHome)
{
    if (!isEditing)
        reset();

    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::CartesianImplicitSurface;

    m_widget->plotIcon->setPixmap(QIcon::fromTheme("draw-square-inverted-corners").pixmap(16, 16));

    setupExpressionType(QStringList(), QStringList() << "x" << "y" << "z", true);

    m_widget->minx->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxx->setExpression(Analitza::Expression(Analitza::Cn(5)));

    m_widget->miny->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxy->setExpression(Analitza::Expression(Analitza::Cn(5)));

    m_widget->minz->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxz->setExpression(Analitza::Expression(Analitza::Cn(5)));
}

void PlotsEditor::buildCartesianParametricSurface(bool cancelIsGoHome)
{
    if (!isEditing)
        reset();

    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::CartesianParametricSurface;

    m_widget->plotIcon->setPixmap(QIcon::fromTheme("draw-donut").pixmap(16, 16));

    setupExpressionType(QStringList() << "x" << "y" << "z", QStringList() << "u" << "v", false, true);

    m_widget->minx->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxx->setExpression(Analitza::Expression(Analitza::Cn(5)));

    m_widget->miny->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxy->setExpression(Analitza::Expression(Analitza::Cn(5)));
}

void PlotsEditor::buildCylindricalGraphSurface(bool cancelIsGoHome)
{
    if (!isEditing)
        reset();

    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::CylindricalGraphSurface;

    m_widget->plotIcon->setPixmap(QIcon::fromTheme("newcylindrical").pixmap(16, 16));

    setupExpressionType(QStringList() << "r,p", QStringList() << "r" << "p");

    m_widget->minx->setExpression(Analitza::Expression(Analitza::Cn(0)));
    m_widget->maxx->setExpression(Analitza::Expression(Analitza::Cn(5)));

    m_widget->miny->setExpression(Analitza::Expression(Analitza::Cn(0)));
    m_widget->maxy->setExpression(Analitza::Expression("2*pi"));
}

void PlotsEditor::buildSphericalGraphSurface(bool cancelIsGoHome)
{
    if (!isEditing)
        reset();

    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::SphericalGraphSurface;

    m_widget->plotIcon->setPixmap(QIcon::fromTheme("newspherical").pixmap(16, 16));

    setupExpressionType(QStringList() << "t,p", QStringList() << "t" << "p");

    m_widget->minx->setExpression(Analitza::Expression(Analitza::Cn(0)));
    m_widget->maxx->setExpression(Analitza::Expression("2*pi"));

    m_widget->miny->setExpression(Analitza::Expression(Analitza::Cn(0)));
    m_widget->maxy->setExpression(Analitza::Expression("pi"));
}

void PlotsEditor::savePlot()
{
    QStringList errors;

    QString name = m_widget->plotName->text();

    if (name.isEmpty())
        name = 'f'+QString::number(m_document->currentPlots()->rowCount()+1);

    switch (m_currentType)
    {
        case PlotsBuilder::CartesianGraphCurve:
        case PlotsBuilder::PolarGraphCurve:
        {
            PlotBuilder req = PlotsFactory::self()->requestPlot(Analitza::Expression(QString(m_currentVars.first()+"->"+m_widget->f->expression().toString())), Dim2D);
            if (req.canDraw())
            {
                FunctionGraph *item =nullptr;

                if (isEditing) 
                {
                    //remove that first and then add the new one
                    m_document->unmapPlot(m_widget->plotsView->selectionModel()->currentIndex());
                }

                item = req.create(m_widget->plotColor->color(), m_widget->plotName->text());
                if (m_widget->intervals->isChecked())
                {
                    item->setInterval(item->parameters().first(), m_widget->minx->expression(), m_widget->maxx->expression());
                }
                else
                    item->clearIntervals();

                m_document->plotsModel()->addPlot(item);
                mapDataChanged();
            } 
            else
                errors = req.errors();

            break;
        }
        case PlotsBuilder::CartesianGraphSurface:
        case PlotsBuilder::CylindricalGraphSurface:
        case PlotsBuilder::SphericalGraphSurface:
        {
            PlotBuilder req = PlotsFactory::self()->requestPlot(m_widget->f->expression(), Dim3D);
            qDebug() << "------------" << m_widget->f->text() << m_widget->f->expression().toString() << req.errors();
            
            if (req.canDraw()) 
            {
                FunctionGraph *item = nullptr;

                if (isEditing) {
                    m_document->unmapPlot(m_widget->plotsView->selectionModel()->currentIndex());

                }

                item = req.create(m_widget->plotColor->color(), m_widget->plotName->text());
                
                if (m_widget->intervals->isChecked())
                {
                    item->setInterval(item->parameters().at(0), m_widget->minx->expression(), m_widget->maxx->expression());
                    item->setInterval(item->parameters().at(1), m_widget->miny->expression(), m_widget->maxy->expression());
                }
                else
                    item->clearIntervals();
                    
                m_document->plotsModel()->addPlot(item);
                mapDataChanged();
            } 
            else 
                errors = req.errors();

            break;
        }

        case PlotsBuilder::CartesianImplicitCurve:
        {
            PlotBuilder req = PlotsFactory::self()->requestPlot(m_widget->f->expression(), Dim2D);
            
            if (req.canDraw() && m_widget->f->expression().isEquation()) 
            {
                FunctionGraph *item = nullptr;

                if (isEditing)
                {
                    m_document->unmapPlot(m_widget->plotsView->selectionModel()->currentIndex());
                }

                item = req.create(m_widget->plotColor->color(), m_widget->plotName->text());

                if (m_widget->intervals->isChecked())
                {
                    item->setInterval(item->parameters().at(0), m_widget->minx->expression(), m_widget->maxx->expression());
                    item->setInterval(item->parameters().at(1), m_widget->miny->expression(), m_widget->maxy->expression());
                }
                else
                    item->clearIntervals();

                m_document->plotsModel()->addPlot(item);

                mapDataChanged();
            } 
            else
                errors = req.errors();

            break;
        }

        case PlotsBuilder::CartesianImplicitSurface:
        {
            PlotBuilder req = PlotsFactory::self()->requestPlot(m_widget->f->expression(), Dim3D);

            if (req.canDraw() && m_widget->f->expression().isEquation())
            {
                FunctionGraph *item = nullptr;

                if (isEditing)
                {
                    m_document->unmapPlot(m_widget->plotsView->selectionModel()->currentIndex());
                }

                item = req.create(m_widget->plotColor->color(), m_widget->plotName->text());

                if (m_widget->intervals->isChecked())
                {
                    item->setInterval(item->parameters().at(0), m_widget->minx->expression(), m_widget->maxx->expression());
                    item->setInterval(item->parameters().at(1), m_widget->miny->expression(), m_widget->maxy->expression());
                    item->setInterval(item->parameters().at(2), m_widget->minz->expression(), m_widget->maxz->expression());
                }
                else
                    item->clearIntervals();

                m_document->plotsModel()->addPlot(item);
                mapDataChanged();
            }
            else
                errors = req.errors();

            break;
        }

        case PlotsBuilder::CartesianParametricCurve2D:
        {
            PlotBuilder req = PlotsFactory::self()->requestPlot(Analitza::Expression(QString(m_currentVars.first()+"->vector{"+m_widget->f->expression().toString()+", "+
                            m_widget->g->expression().toString()+"}")), Dim2D);
            
            if (req.canDraw()) 
            {
                FunctionGraph *item = nullptr;
                
                if (isEditing) 
                {
                    m_document->unmapPlot(m_widget->plotsView->selectionModel()->currentIndex());
                }
                
                item = req.create(m_widget->plotColor->color(), m_widget->plotName->text());
                
                if (m_widget->intervals->isChecked())
                    item->setInterval(item->parameters().first(), m_widget->minx->expression(), m_widget->maxx->expression());
                else
                    item->clearIntervals();

                m_document->plotsModel()->addPlot(item);
                mapDataChanged();
            }
            else
                errors = req.errors();

            break;
        }

        case PlotsBuilder::CartesianParametricCurve3D:
        {
            PlotBuilder req = PlotsFactory::self()->requestPlot(Analitza::Expression(QString(m_currentVars.first()+"->vector{"+m_widget->f->expression().toString()+", "+m_widget->g->expression().toString()+", "+m_widget->h->expression().toString()+'}')), Dim3D);
            
            if (req.canDraw())
            {
                FunctionGraph *item = nullptr;
                
                if (isEditing) {
                    m_document->unmapPlot(m_widget->plotsView->selectionModel()->currentIndex());
                }
                item = req.create(m_widget->plotColor->color(), m_widget->plotName->text());

                if (m_widget->intervals->isChecked()) {
                    item->setInterval(item->parameters().first(), m_widget->minx->expression(), m_widget->maxx->expression());
                }
                else
                    item->clearIntervals();


                m_document->plotsModel()->addPlot(item);
                mapDataChanged();
            } 
            else
                errors = req.errors();

            break;
        }

        case PlotsBuilder::CartesianParametricSurface:
        {
            PlotBuilder req = PlotsFactory::self()->requestPlot(Analitza::Expression(QString("("+m_currentVars.join(",")+")->vector{"+m_widget->f->expression().toString()+", "+m_widget->g->expression().toString()+", "+m_widget->h->expression().toString()+'}')), Dim3D);
            
            if (req.canDraw()) 
            {
                FunctionGraph *item = nullptr;
                if (isEditing) {
                    m_document->unmapPlot(m_widget->plotsView->selectionModel()->currentIndex());
                }
                item = req.create(m_widget->plotColor->color(), m_widget->plotName->text());

                if (m_widget->intervals->isChecked())
                {
                    item->setInterval(item->parameters().at(0), m_widget->minx->expression(), m_widget->maxx->expression());
                    item->setInterval(item->parameters().at(1), m_widget->miny->expression(), m_widget->maxy->expression());
                }
                else
                    item->clearIntervals();

                m_document->plotsModel()->addPlot(item);
                mapDataChanged();
            } 
            else
                errors = req.errors();
            
            break;
        }
        
        default:
            break;
    }

    if (errors.isEmpty())
    {
        isEditing = false;
        reset();
        showList();
    }
    else
    {
        emit sendStatus(errors.last(), 2000); //2 secs for the user
    }
}

void PlotsEditor::removePlot()
{
    if (m_widget->plotsView->selectionModel()->hasSelection())
        m_document->unmapPlot(m_widget->plotsView->selectionModel()->currentIndex());
    emit mapDataChanged();
}

void PlotsEditor::setCurrentFunctionGraphs(const QString& txt)
{
    m_currentFunctionGraphs = txt.split(',');
    m_currentVars = m_currentFunctionGraphs;
    for (int var = 1; var <=m_currentFunctionGraphs.size(); ++var)
        setupVarName(var, m_currentFunctionGraphs[var-1]);
}

void PlotsEditor::setupVarName(int var, const QString &vvalue)
{
    switch (var)
    {
        case 1:
        {
            m_widget->x->setText("< "+vvalue+" <");
            break;
        }
        case 2:
        {
            m_widget->yinterval->show();
            m_widget->y->setText("< "+vvalue+" <");
            break;
        }
        case 3:
        {
            m_widget->zinterval->show();
            m_widget->z->setText("< "+vvalue+" <");
            break;
        }
    }
}

void PlotsEditor::setupFuncName(int var, const QString& vvalue, const QStringList& vars, bool withparenthesis)
{
    QString mmlhelper = vvalue;

    if (withparenthesis)
        mmlhelper.append("(");

    foreach(const QString &v, vars)
    {
        mmlhelper.append(v);

        if (v != vars.last())
            mmlhelper.append(",");
    }

    if (withparenthesis)
        mmlhelper.append(")");

    switch (var)
    {
        case 1:
        {
            m_widget->fname->setText(mmlhelper);
            m_widget->gexpression->hide();
            m_widget->hexpression->hide();
            break;
        }
        case 2:
        {
            m_widget->gname->setText(mmlhelper);
            m_widget->gexpression->show();
            m_widget->hexpression->hide();
            break;
        }
        case 3:
        {
            m_widget->hname->setText(mmlhelper);
            m_widget->hexpression->show();
            break;
        }
    }
}

void PlotsEditor::setupExpressionType(const QStringList &fvalues, const QStringList &vvalues, bool isimplicit, bool isvectorValued)
{
    //this vars track current type of plot (see saveslot)
    m_currentVars = vvalues;
    m_currentIsImplicit = isimplicit;
    m_currentIsVectorValued = isvectorValued;
    m_currentVectorSize = isvectorValued?fvalues.size():-1;

    m_widget->farrow->show();

    if (!isimplicit && !isvectorValued)
    {
        m_widget->fnameForGraphs->show();
        m_widget->fname->hide();
        m_widget->fnameForGraphs->clear();
        m_widget->fnameForGraphs->addItems(fvalues);
    } 
    else 
    {
        m_widget->fnameForGraphs->hide();
        m_widget->fname->show();
    }

    if (!isvectorValued || !isimplicit)
    {
        m_widget->gexpression->hide();
        m_widget->hexpression->hide();

        m_widget->yinterval->hide();
        m_widget->zinterval->hide();
    }

    if (isimplicit)
    {
        m_widget->fnameForGraphs->hide();
        m_widget->fname->hide();
        m_widget->farrow->hide();
    }

    for (int var = 1; var <=vvalues.size(); ++var)
        setupVarName(var, vvalues[var-1]);

    if (isvectorValued)
    {
        m_widget->fnameForGraphs->hide();

        m_widget->farrow->setText("=");

        for (int func = 1; func <= fvalues.size(); ++func)
        {
            setupFuncName(func, fvalues[func-1], vvalues);
        }
    }
    else
    {
        if (!isimplicit)
        {
            m_widget->farrow->setText("=");
        }
    }
    
    showEditor();
}

void PlotsEditor::plotnamecheckClicked(bool state)
{
    if(state==true) {
        m_widget->plotIcon->show();
        m_widget->plotName->show();
        if(m_widget->plotName->text().isEmpty()) {
            m_widget->plotName->setText(i18n("plot %1",plotnumber));
            plotnumber++;
        }
    } else {
        m_widget->plotIcon->hide();
        m_widget->plotName->hide();
    }
}

#include "moc_plotseditor.cpp"
