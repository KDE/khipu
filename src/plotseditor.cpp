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
#include <qstyleditemdelegate.h>
#include <QPainter>
#include <libkdeedu/qtmml/QtMmlWidget>
#include "functionlibraryedit.h"
#include "datastore.h"
#include "dictionariesmodel.h"
#include "spaceplotsproxymodel.h"
#include "analitzagui/variablesmodel.h"
#include <analitzaplot/functiongraph.h>
#include <analitzaplot/planecurve.h>
#include "analitzaplot/plotsmodel.h"
#include <analitzaplot/spacecurve.h>
#include <analitzaplot/surface.h>
#include <dictionaryitem.h>
#include <analitzaplot/plotsfactory.h>
#include "ui_plotseditor.h"

Q_DECLARE_METATYPE(Analitza::PlotItem*);

using namespace Analitza;

ComboBox::ComboBox(QWidget* parent): QComboBox(parent)
{
    connect(this, SIGNAL(currentIndexChanged(QString)), SLOT(setupCache(QString)));
}

QSize ComboBox::sizeHint() const
{
    //TODO GSOC fix magic numbers heres

    QStringList funcs = m_cacheText.split(",");
    QString mmlhelper;

    foreach(const QString &func, funcs)
    {
        mmlhelper.append("<mi>"+func+"</mi>");

        if (func != funcs.last()) // no agregar comas al final
            mmlhelper.append("<mtext>,</mtext>");
    }

    QtMmlDocument mathMLRenderer;
    mathMLRenderer.setContent("<math display='block'><mrow>"+mmlhelper+"</mrow></math>");

// return QSize(258, 32);
//     qDebug() << mathMLRenderer.size() << funcs << m_cacheText;

    //TODO add the combo arrow size properly
//     QStyleOptionComboBox opt;
//     opt.initFrom(this);

    return QSize(mathMLRenderer.size().width()+24, mathMLRenderer.size().height()+8);
}

void ComboBox::paintEvent(QPaintEvent* e)
{
    //dibujo el combo pero borrando el texto temporalmente, luego reinserto el texto ... la idea es dibujar el combo sin el text
    QString itemtext = currentText();
    m_cacheText = itemtext;
    setItemText(currentIndex(), "");
    QComboBox::paintEvent(e);
    setItemText(currentIndex(), itemtext);

    //TODO GSOC fix magic numbers heres

    QPainter p(this);

    QStringList funcs = currentText().split(",");
    QString mmlhelper;

    foreach(const QString &func, funcs)
    {
        mmlhelper.append("<mi>"+func+"</mi>");

        if (func != funcs.last()) // no agregar comas al final
            mmlhelper.append("<mtext>,</mtext>");
    }

    QtMmlDocument mathMLRenderer;
    mathMLRenderer.setContent("<math display='block'><mrow>"+mmlhelper+"</mrow></math>");

    QStyleOptionComboBox opt;
    opt.initFrom(this);

    mathMLRenderer.paint(&p, opt.rect.topLeft()+QPoint(4,4));

}

void ComboBox::setupCache(const QString& currtext)
{
    m_cacheText = currtext;
//     qDebug() << m_cacheText;
    setMinimumSize(sizeHint());
    update();
//     resize(sizeHint());
//     update();
}

class FunctionDelegate : public QStyledItemDelegate
{
public:
    FunctionDelegate(ComboBox *parent = 0);
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

FunctionDelegate::FunctionDelegate(ComboBox* parent): QStyledItemDelegate(parent)
{

}

QSize FunctionDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStringList funcs = index.data().toString().split(",");
    QString mmlhelper;

    foreach(const QString &func, funcs)
    {
        mmlhelper.append("<mi>"+func+"</mi>");

        if (func != funcs.last()) // no agregar comas al final
            mmlhelper.append("<mtext>,</mtext>");
    }

    QtMmlDocument mathMLRenderer;
    mathMLRenderer.setContent("<math display='block'><mrow>"+mmlhelper+"</mrow></math>");

    return QSize(258, 32);
    return QSize(mathMLRenderer.size().width(), mathMLRenderer.size().height()+8);
}

void FunctionDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStringList funcs = index.data().toString().split(",");
    QString mmlhelper;

    foreach(const QString &func, funcs)
    {
        mmlhelper.append("<mi>"+func+"</mi>");

        if (func != funcs.last()) // no agregar comas al final
            mmlhelper.append("<mtext>,</mtext>");
    }

    if (option.state & QStyle::State_Selected)
    {
        painter->save();
        painter->setPen(option.palette.highlight().color());
        painter->setBrush(option.palette.highlight());

        QRect selRect = static_cast<ComboBox*>(parent())->view()->visualRect(index);
        selRect.setHeight(selRect.height() - 1); //TODO see above
        painter->drawRect(selRect);
        painter->restore();
        painter->setPen(option.palette.highlightedText().color());
    }

    QtMmlDocument mathMLRenderer;
    mathMLRenderer.setContent("<math display='block'><mrow>"+mmlhelper+"</mrow></math>");
    mathMLRenderer.paint(painter, QPoint(option.rect.left()+2, option.rect.top()+4));
}

PlotsEditor::PlotsEditor(QWidget * parent)
    : QDockWidget(parent), isEditing(false)
{
    m_widget = new Ui::PlotsEditorWidget;
    m_widget->setupUi(this);
    setObjectName("adasdds");

    

    
//     m_widget->intervals->setChecked(false); // por defecto usaremos el viewpor no los intervalos

//     m_widget->fnameForGraphs->setMouseTracking(true);
//     m_widget->fnameForGraphs->view()->setMouseTracking(true);
    m_widget->fnameForGraphs->setItemDelegate(new FunctionDelegate(m_widget->fnameForGraphs));

    connect(m_widget->fnameForGraphs, SIGNAL(currentIndexChanged(QString)), SLOT(setCurrentFunctionGraphs(QString)));

    m_widget->farrow->setContent("<math display='block'> <mrow> <mo>&rarr;</mo> </mrow> </math>");
    m_widget->garrow->setContent("<math display='block'> <mrow> <mo>=</mo> </mrow> </math>");
    m_widget->harrow->setContent("<math display='block'> <mrow> <mo>=</mo> </mrow> </math>");

    //cons
    connect(m_widget->builderDialogBox->button(QDialogButtonBox::Cancel), SIGNAL(pressed()), SLOT(showList()));
    connect(m_widget->editorDialogBox->button(QDialogButtonBox::Cancel), SIGNAL(pressed()), SLOT(cancelEditor()));
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

    connect(m_widget->addPlots, SIGNAL(pressed()), SLOT(addPlots()));
    connect(m_widget->editPlot, SIGNAL(pressed()), SLOT(editPlot()));
    connect(m_widget->plotsView, SIGNAL(doubleClicked(QModelIndex)), SLOT(editPlot()));
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
    m_widget->quickPlot->setModel(m_document->plotsDictionaryModel());
}

void PlotsEditor::setCurrentSpace(int spaceidx)
{
    m_widget->quickPlot->setFilterDimension(m_document->spacesModel()->space(spaceidx)->dimension());
    
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
    }
}

void PlotsEditor::reset(bool clearBuilder)
{
    //clear widgets //TODO GSOC
    m_widget->plotName->clear();

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
//     m_widget->preview3D->camera()/*->centerScene*/;
//     m_widget->preview3D->stopAnimation();

    if (clearBuilder)
        //// escondo todos los links solo muestro los necesario dependiendo del tipo de spac : 2d o 3d .. ver editorplots.setcurrentspace
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
}

void PlotsEditor::cancelEditor()
{
    if (isEditing)
    {
        showList();
        isEditing = false;
        return;
    }
    if (m_cancelIsGoHome) // si he llegado desde el dock del mainwnd => el cancel regresa al mainwnd
    {
        emit goHome();

        //NOTE eliminamos el ultimo espacio si el usuario cancelo y no hay nuevosplos
        m_document->removeCurrentSpace();
    }
    else // caso contrario se entiende que estoy en el contexto de un space (editando la lista de plots de un space)
        showTypes();

}

void PlotsEditor::addPlots()
{
    m_cancelIsGoHome = false;

    showTypes();
}

void PlotsEditor::editPlot(const QModelIndex &index)
{
    isEditing = true;

    reset();

    if (m_widget->plotsView->selectionModel()->hasSelection())
    {
        QModelIndex index = m_widget->plotsView->selectionModel()->currentIndex();
        PlotItem* item = index.data(PlotsModel::PlotRole).value<PlotItem*>();

        if (dynamic_cast<PlaneCurve*>(item))
        {
            PlaneCurve *curve = dynamic_cast<PlaneCurve*>(item);

            if (curve->expression().isEquation()) // implicit
            {
                m_widget->f->setExpression(curve->expression());

                buildCartesianImplicitCurve();
                
                QPair<Analitza::Expression, Analitza::Expression> interval = curve->interval(curve->parameters().at(1), false);
                m_widget->miny->setExpression(interval.first);
                m_widget->maxy->setExpression(interval.second);
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
                    buildPolarGraphCurve();
                else
                {
                    buildCartesianGraphCurve();

                    if (curve->parameters().first() == "x")
                    {
                        m_currentVars = QStringList() << "x";
                        // no mostramos el combo pues ya el tipo de la func esta elejido
                        m_widget->fnameForGraphs->hide();
                        setupFuncName(1, "", QStringList() << "x", false);
                        m_widget->fname->show();
                    }
                    else if (curve->parameters().first() == "y")
                    {
                        m_currentVars = QStringList() << "y";
                        m_widget->fnameForGraphs->hide();
                        setupFuncName(1, "", QStringList() << "y", false);
                        m_widget->fname->show();
                    }
                }
            }

            QPair<Analitza::Expression, Analitza::Expression> interval = curve->interval(curve->parameters().first(), false);
            m_widget->minx->setExpression(interval.first);
            m_widget->maxx->setExpression(interval.second);
        }
        else if (dynamic_cast<SpaceCurve*>(item))
        {
            SpaceCurve *curve = dynamic_cast<SpaceCurve*>(item);

            m_widget->f->setExpression(curve->expression().lambdaBody().elementAt(0));
            m_widget->g->setExpression(curve->expression().lambdaBody().elementAt(1));
            m_widget->h->setExpression(curve->expression().lambdaBody().elementAt(2));

            buildCartesianParametricCurve3D();
            
            QPair<Analitza::Expression, Analitza::Expression> interval = curve->interval(curve->parameters().first(), false);
            m_widget->minx->setExpression(interval.first);
            m_widget->maxx->setExpression(interval.second);
        }
        else if (dynamic_cast<Surface*>(item))
        {
            Surface *surface = dynamic_cast<Surface*>(item);

            if (surface->expression().isEquation()) // implicit
            {
                m_widget->f->setExpression(surface->expression());

                buildCartesianImplicitSurface();

                QPair<Analitza::Expression, Analitza::Expression> interval = surface->interval(surface->parameters().at(2), false);
                m_widget->minz->setExpression(interval.first);
                m_widget->maxz->setExpression(interval.second);
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
                        // no mostramos el combo pues ya el tipo de la func esta elejido
                        m_widget->fnameForGraphs->hide();
                        setupFuncName(1, "", QStringList() << "x" << "y", false);
                        m_widget->fname->show();
                    }
                    else if (surface->parameters() == QStringList() << "x" << "z")
                    {
                        m_currentVars = QStringList() << "x" << "z";
                        m_widget->fnameForGraphs->hide();
                        setupFuncName(1, "", QStringList() << "x" << "z", false);
                        m_widget->fname->show();
                    }
                    else if (surface->parameters() == QStringList() << "y" << "z")
                    {
                        m_currentVars = QStringList() << "y" << "z";
                        m_widget->fnameForGraphs->hide();
                        setupFuncName(1, "", QStringList() << "y" << "z", false);
                        m_widget->fname->show();
                    }
                }
            }
            
            QPair<Analitza::Expression, Analitza::Expression> interval = surface->interval(surface->parameters().first(), false);
            m_widget->minx->setExpression(interval.first);
            m_widget->maxx->setExpression(interval.second);
            
            interval = surface->interval(surface->parameters().at(1), false);
            m_widget->miny->setExpression(interval.first);
            m_widget->maxy->setExpression(interval.second);
        }
    }

}

void PlotsEditor::buildCartesianGraphCurve(bool cancelIsGoHome)
{
    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::CartesianGraphCurve;

    m_widget->plotIcon->setPixmap(KIcon("newfunction").pixmap(16.16));

    setupExpressionType(QStringList() << "x" << "y", QStringList() << "x");
    
    m_widget->minx->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxx->setExpression(Analitza::Expression(Analitza::Cn(5)));
}

void PlotsEditor::buildCartesianImplicitCurve(bool cancelIsGoHome)
{
    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::CartesianImplicitCurve;

    m_widget->plotIcon->setPixmap(KIcon("newimplicit").pixmap(16.16));

    setupExpressionType(QStringList(), QStringList() << "x" << "y", true);
    
    m_widget->minx->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxx->setExpression(Analitza::Expression(Analitza::Cn(5)));
    
    m_widget->miny->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxy->setExpression(Analitza::Expression(Analitza::Cn(5)));
}

void PlotsEditor::buildCartesianParametricCurve2D(bool cancelIsGoHome)
{
    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::CartesianParametricCurve2D;

    m_widget->plotIcon->setPixmap(KIcon("newparametric").pixmap(16.16));

    setupExpressionType(QStringList() << "x" << "y", QStringList() << "t", false, true);
    
    m_widget->minx->setExpression(Analitza::Expression("-pi"));
    m_widget->maxx->setExpression(Analitza::Expression("pi"));
}

void PlotsEditor::buildPolarGraphCurve(bool cancelIsGoHome)
{
    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::PolarGraphCurve;

    m_widget->plotIcon->setPixmap(KIcon("draw-spiral").pixmap(16.16));

    setupExpressionType(QStringList()<<"q", QStringList() << "q");
    
    m_widget->minx->setExpression(Analitza::Expression(Analitza::Cn(0)));
    m_widget->maxx->setExpression(Analitza::Expression("2*pi"));
}

//3D
void PlotsEditor::buildCartesianParametricCurve3D(bool cancelIsGoHome)
{
    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::CartesianParametricCurve3D;

    m_widget->plotIcon->setPixmap(KIcon("newparametric3d").pixmap(16.16));

    setupExpressionType(QStringList() << "x" << "y" << "z", QStringList() << "t", false, true);
    
    m_widget->minx->setExpression(Analitza::Expression("-pi"));
    m_widget->maxx->setExpression(Analitza::Expression("pi"));
}

void PlotsEditor::buildCartesianGraphSurface(bool cancelIsGoHome)
{
    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::CartesianGraphSurface;

    m_widget->plotIcon->setPixmap(KIcon("newfunction3d").pixmap(16.16));

    setupExpressionType(QStringList() << "x,y" << "x,z" << "y,z", QStringList() << "x" << "y");
    
    m_widget->minx->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxx->setExpression(Analitza::Expression(Analitza::Cn(5)));
    
    m_widget->miny->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxy->setExpression(Analitza::Expression(Analitza::Cn(5)));
}

void PlotsEditor::buildCartesianImplicitSurface(bool cancelIsGoHome)
{
    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::CartesianImplicitSurface;

    m_widget->plotIcon->setPixmap(KIcon("draw-square-inverted-corners").pixmap(16.16));

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
    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::CartesianParametricSurface;

    m_widget->plotIcon->setPixmap(KIcon("draw-donut").pixmap(16.16));

    setupExpressionType(QStringList() << "x" << "y" << "z", QStringList() << "u" << "v", false, true);
    
    m_widget->minx->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxx->setExpression(Analitza::Expression(Analitza::Cn(5)));
    
    m_widget->miny->setExpression(Analitza::Expression(Analitza::Cn(-5)));
    m_widget->maxy->setExpression(Analitza::Expression(Analitza::Cn(5)));
}

void PlotsEditor::buildCylindricalGraphSurface(bool cancelIsGoHome)
{
    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::CylindricalGraphSurface;

    m_widget->plotIcon->setPixmap(KIcon("newcylindrical").pixmap(16.16));

    setupExpressionType(QStringList() << "r,p", QStringList() << "r" << "p");
    
    m_widget->minx->setExpression(Analitza::Expression(Analitza::Cn(0)));
    m_widget->maxx->setExpression(Analitza::Expression(Analitza::Cn(5)));
    
    m_widget->miny->setExpression(Analitza::Expression(Analitza::Cn(0)));
    m_widget->maxy->setExpression(Analitza::Expression("2*pi"));
}

void PlotsEditor::buildSphericalGraphSurface(bool cancelIsGoHome)
{
    m_cancelIsGoHome = cancelIsGoHome;

    m_currentType = PlotsBuilder::SphericalGraphSurface;

    m_widget->plotIcon->setPixmap(KIcon("newspherical").pixmap(16.16));

    setupExpressionType(QStringList() << "t,p", QStringList() << "t" << "p");
    
    m_widget->minx->setExpression(Analitza::Expression(Analitza::Cn(0)));
    m_widget->maxx->setExpression(Analitza::Expression("2*pi"));

    m_widget->miny->setExpression(Analitza::Expression(Analitza::Cn(0)));
    m_widget->maxy->setExpression(Analitza::Expression("pi"));
}

FunctionGraph* PlotsEditor::editCurrentFunction(const Analitza::Expression& exp)
{
    QModelIndex idx = m_widget->plotsView->selectionModel()->currentIndex().sibling(0, 1);

    m_widget->plotsView->model()->setData(idx, AnalitzaUtils::expressionToVariant(exp));
    m_widget->plotsView->model()->setData(idx, m_widget->plotColor->color(), Qt::DecorationRole);
    m_widget->plotsView->model()->setData(idx.sibling(idx.row(), 0), m_widget->plotName->text());

    return static_cast<FunctionGraph*>(idx.data(PlotsModel::PlotRole).value<PlotItem*>());
}

void PlotsEditor::savePlot()
{
    QStringList errors;

    QString name = m_widget->plotName->text();

    if (name.isEmpty())
        name = "f"+QString::number(m_document->currentPlots()->rowCount()+1);

    switch (m_currentType)
    {
        case PlotsBuilder::CartesianGraphCurve:
        case PlotsBuilder::PolarGraphCurve:
        {
            PlotBuilder req = PlotsFactory::self()->requestPlot(Analitza::Expression(QString(m_currentVars.first()+"->"+m_widget->f->expression().toString())), Dim2D);
            if (req.canDraw()) {
                FunctionGraph *item = 0;
                if (isEditing) {
                    item = editCurrentFunction(req.expression());
                } else
                    item = req.create(m_widget->plotColor->color(), m_widget->plotName->text());
                
                item->setInterval(item->parameters().first(), m_widget->minx->expression(), m_widget->maxx->expression());

                if(!isEditing)
                    m_document->plotsModel()->addPlot(item);
            } else
                errors = req.errors();

            break;
        }
        case PlotsBuilder::CartesianGraphSurface:
        case PlotsBuilder::CylindricalGraphSurface:
        case PlotsBuilder::SphericalGraphSurface:
        {
            PlotBuilder req = PlotsFactory::self()->requestPlot(m_widget->f->expression(), Dim3D);
			qDebug() << "------------" << m_widget->f->text() << m_widget->f->expression().toString() << req.errors();
            if (req.canDraw()) {
                FunctionGraph *item = 0;
                if (isEditing) {
                    item = editCurrentFunction(req.expression());
                } else
                    item = req.create(m_widget->plotColor->color(), m_widget->plotName->text());
               
                item->setInterval(item->parameters().at(0), m_widget->minx->expression(), m_widget->maxx->expression());
                item->setInterval(item->parameters().at(1), m_widget->miny->expression(), m_widget->maxy->expression());

                if (!isEditing)
                    m_document->plotsModel()->addPlot(item);
            } else {
                errors = req.errors();
			}

            break;
        }

        case PlotsBuilder::CartesianImplicitCurve:
        {
            PlotBuilder req = PlotsFactory::self()->requestPlot(m_widget->f->expression(), Dim2D);
            if (req.canDraw() && m_widget->f->expression().isEquation()) {
                FunctionGraph *item = 0;
                if (isEditing) {
                    item = editCurrentFunction(req.expression());
                } else {
                    item = req.create(m_widget->plotColor->color(), m_widget->plotName->text());
                }
                
                item->setInterval(item->parameters().at(0), m_widget->minx->expression(), m_widget->maxx->expression());
                item->setInterval(item->parameters().at(1), m_widget->miny->expression(), m_widget->maxy->expression());

                if (!isEditing)
                    m_document->plotsModel()->addPlot(item);
            } else
                errors = req.errors();

            break;
        }

        case PlotsBuilder::CartesianImplicitSurface:
        {
            PlotBuilder req = PlotsFactory::self()->requestPlot(m_widget->f->expression(), Dim3D);
            if (req.canDraw() && m_widget->f->expression().isEquation()) {
                FunctionGraph *item = 0;
                if (isEditing) {
                    item = editCurrentFunction(req.expression());
                } else {
                    item = req.create(m_widget->plotColor->color(), m_widget->plotName->text());
                }
                
                item->setInterval(item->parameters().at(0), m_widget->minx->expression(), m_widget->maxx->expression());
                item->setInterval(item->parameters().at(1), m_widget->miny->expression(), m_widget->maxy->expression());
                item->setInterval(item->parameters().at(2), m_widget->minz->expression(), m_widget->maxz->expression());

                if (!isEditing)
                    m_document->plotsModel()->addPlot(item);
            } else
                errors = req.errors();

            break;
        }

        case PlotsBuilder::CartesianParametricCurve2D:
        {
            PlotBuilder req = PlotsFactory::self()->requestPlot(Analitza::Expression(QString(m_currentVars.first()+"->vector{"+m_widget->f->expression().toString()+", "+
                                    m_widget->g->expression().toString()+"}")), Dim2D);
            if (req.canDraw()) {
                FunctionGraph *item = 0;
                if (isEditing) {
                    item = editCurrentFunction(req.expression());
                } else {
                    item = req.create(m_widget->plotColor->color(), m_widget->plotName->text());
                }
               
                item->setInterval(item->parameters().first(), m_widget->minx->expression(), m_widget->maxx->expression());

                if (!isEditing)
                    m_document->plotsModel()->addPlot(item);
            } else
                errors = req.errors();

            break;
        }

        case PlotsBuilder::CartesianParametricCurve3D:
        {
            PlotBuilder req = PlotsFactory::self()->requestPlot(Analitza::Expression(QString(m_currentVars.first()+"->vector{"+m_widget->f->expression().toString()+", "+m_widget->g->expression().toString()+", "+m_widget->h->expression().toString()+"}")), Dim3D);
            if (req.canDraw())
            {
                FunctionGraph *item = 0;
                if (isEditing) {
                    item = editCurrentFunction(req.expression());
                } else {
                    item = req.create(m_widget->plotColor->color(), m_widget->plotName->text());
                }
                
                item->setInterval(item->parameters().first(), m_widget->minx->expression(), m_widget->maxx->expression());

                if (!isEditing)
                    m_document->plotsModel()->addPlot(item);
            } else
                errors = req.errors();
            
            break;
        }

        case PlotsBuilder::CartesianParametricSurface:
        {
            PlotBuilder req = PlotsFactory::self()->requestPlot(Analitza::Expression(QString("("+m_currentVars.join(",")+")->vector{"+m_widget->f->expression().toString()+", "+m_widget->g->expression().toString()+", "+m_widget->h->expression().toString()+"}")), Dim3D);
            if (req.canDraw()) {
                FunctionGraph *item = 0;
                if (isEditing) {
                    item = editCurrentFunction(req.expression());
                } else {
                    item = req.create(m_widget->plotColor->color(), m_widget->plotName->text());
                }
                
                item->setInterval(item->parameters().at(0), m_widget->minx->expression(), m_widget->maxx->expression());
                item->setInterval(item->parameters().at(1), m_widget->miny->expression(), m_widget->maxy->expression());

                if (!isEditing)
                    m_document->plotsModel()->addPlot(item);
                
            } else
                errors = req.errors();
            break;
        }
    }

    if (errors.isEmpty())
    {
        isEditing = false;
        reset();
        showList();
    }
    else
    {
//         setStatusTip(errors);
        emit sendStatus(errors.last(), 2000); //2 secs for the user
    }
}

void PlotsEditor::removePlot()
{
    if (m_widget->plotsView->selectionModel()->hasSelection())
        m_document->unmapPlot(m_widget->plotsView->selectionModel()->currentIndex());
}

void PlotsEditor::setCurrentFunctionGraphs(const QString& txt)
{
    m_currentFunctionGraphs = txt.split(",");

    m_currentVars = m_currentFunctionGraphs;

    //mostramos a demanda las variables usadas
    for (int var = 1; var <=m_currentFunctionGraphs.size(); ++var)
        setupVarName(var, m_currentFunctionGraphs[var-1]);
}

void PlotsEditor::setupVarName(int var, const QString &vvalue)
{
    switch (var)
    {
    case 1:
    {
        m_widget->x->setContent("<math display='block'> <mrow> <mo>&#x02264;</mo> <mi>"+vvalue+"</mi> <mo>&#x02264;</mo> </mrow> </math>");
        break;
    }
    case 2:
    {
        m_widget->yinterval->show();
        m_widget->y->setContent("<math display='block'> <mrow> <mo>&#x02264;</mo> <mi>"+vvalue+"</mi> <mo>&#x02264;</mo> </mrow> </math>");
        break;
    }
    case 3:
    {
        m_widget->zinterval->show();
        m_widget->z->setContent("<math display='block'> <mrow> <mo>&#x02264;</mo> <mi>"+vvalue+"</mi> <mo>&#x02264;</mo> </mrow> </math>");
        break;
    }
    }
}

void PlotsEditor::setupFuncName(int var, const QString& vvalue, const QStringList& vars, bool withparenthesis)
{
    QString mmlhelper;
    mmlhelper.append("<mi>"+vvalue+"</mi>");

    if (withparenthesis)
        mmlhelper.append("<mo>(</mo>");

    foreach(const QString &var, vars)
    {
        mmlhelper.append("<mi>"+var+"</mi>");

        if (var != vars.last()) // no agregar comas al final
            mmlhelper.append("<mtext>,</mtext>");
    }

    if (withparenthesis)
        mmlhelper.append("<mo>)</mo>");

    switch (var)
    {
    case 1:
    {
        m_widget->fname->setContent("<math display='block'><mrow>"+mmlhelper+"</mrow></math>");
        m_widget->gexpression->hide();
        m_widget->hexpression->hide();
        break;
    }
    case 2:
    {
        m_widget->gname->setContent("<math display='block'><mrow>"+mmlhelper+"</mrow></math>");
        m_widget->gexpression->show();
        m_widget->hexpression->hide();
        break;
    }
    case 3:
    {
        m_widget->hname->setContent("<math display='block'><mrow>"+mmlhelper+"</mrow></math>");
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

    //
    m_widget->farrow->show();

    if (!isimplicit && !isvectorValued) // es decir, si es realvalued ... osea un graph
    {
        m_widget->fnameForGraphs->show();
        m_widget->fname->hide();
//         m_widget->farrow->show();
        m_widget->fnameForGraphs->clear();
        m_widget->fnameForGraphs->addItems(fvalues);

    }
    else
    {
        m_widget->fnameForGraphs->hide();
        m_widget->fname->show();
//         m_widget->farrow->hide();

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

    //mostramos a demanda las variables usadas
    for (int var = 1; var <=vvalues.size(); ++var)
        setupVarName(var, vvalues[var-1]);

    if (isvectorValued)
    {
        m_widget->fnameForGraphs->hide();

        m_widget->farrow->setContent("<math display='block'> <mrow> <mo>=</mo> </mrow> </math>");

        for (int func = 1; func <= fvalues.size(); ++func)
        {
            setupFuncName(func, fvalues[func-1], vvalues);
        }
    }
    else
    {
        if (!isimplicit) //flechas en ves de = pues es un graph
        {
            m_widget->farrow->setContent("<math display='block'> <mrow> <mo>&rarr;</mo> </mrow> </math>");
        }
    }

    //clear this next iter
//     if (isvectorValued && m_vectorSize == 2 || (isimplicit && vvalues.size() == 2) || (!isimplicit && !isimplicit && vvalues.size() == 1))
//         m_widget->previews->setCurrentIndex(0); //2d preview
//     else // 3D
//         m_widget->previews->setCurrentIndex(1); //3d preview

    showEditor();
}
