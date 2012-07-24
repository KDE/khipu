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
    qDebug() << mathMLRenderer.size() << funcs << m_cacheText;
    
    //TODO add the combo arrow size properly
//     QStyleOptionComboBox opt;
//     opt.initFrom(this);
    
    return QSize(mathMLRenderer.size().width()+24, mathMLRenderer.size().height()+8);
}

void ComboBox::paintEvent(QPaintEvent* e)
{
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
    
    //dibujo el combo pero borrando el texto temporalmente, luego reinserto el texto ... la idea es dibujar el combo sin el text
    QString itemtext = currentText();
    m_cacheText = itemtext;
    setItemText(currentIndex(), "");
    QComboBox::paintEvent(e);
    setItemText(currentIndex(), itemtext);    
}

void ComboBox::setupCache(const QString& currtext)
{
    m_cacheText = currtext;
    qDebug() << m_cacheText;
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
    : QDockWidget(parent)
{
    m_widget = new Ui::PlotsEditorWidget;
    m_widget->setupUi(this);
    setObjectName("adasdds");
    
//     m_widget->fnameForGraphs->setMouseTracking(true);
//     m_widget->fnameForGraphs->view()->setMouseTracking(true);
    m_widget->fnameForGraphs->setItemDelegate(new FunctionDelegate(m_widget->fnameForGraphs));

    connect(m_widget->fnameForGraphs, SIGNAL(currentIndexChanged(QString)), SLOT(setCurrentFunctionGraphs(QString)));
    
    m_widget->farrow->setContent("<math display='block'> <mrow> <mo>&rarr;</mo> </mrow> </math>");
    m_widget->garrow->setContent("<math display='block'> <mrow> <mo>=</mo> </mrow> </math>");
    m_widget->harrow->setContent("<math display='block'> <mrow> <mo>=</mo> </mrow> </math>");

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
    m_currentType = PlotsBuilder::CartesianGraphCurve;
    
    m_widget->plotIcon->setPixmap(KIcon("kde").pixmap(16.16));

    setupExpressionType(QStringList() << "x" << "y", QStringList() << "x");
}

void PlotsEditor::buildCartesianImplicitCurve()
{
    m_currentType = PlotsBuilder::CartesianImplicitCurve;

    m_widget->plotIcon->setPixmap(KIcon("list-add").pixmap(16.16));

    setupExpressionType(QStringList(), QStringList() << "x" << "y", true);
}

void PlotsEditor::buildCartesianParametricCurve2D()
{
    m_currentType = PlotsBuilder::CartesianParametricCurve2D;

    m_widget->plotIcon->setPixmap(KIcon("list-add").pixmap(16.16));

    setupExpressionType(QStringList() << "x" << "y", QStringList() << "t", false, true, 2);
}

void PlotsEditor::buildPolarGraphCurve()
{
    m_currentType = PlotsBuilder::PolarGraphCurve;

    m_widget->plotIcon->setPixmap(KIcon("list-add").pixmap(16.16));

    setupExpressionType(QStringList()<<"p", QStringList() << "p");
}

//3D
void PlotsEditor::buildCartesianParametricCurve3D()
{
    m_currentType = PlotsBuilder::CartesianParametricCurve3D;
    
    m_widget->plotIcon->setPixmap(KIcon("list-add").pixmap(16.16));

    setupExpressionType(QStringList() << "x" << "y" << "z", QStringList() << "t", false, true, 3);
}

void PlotsEditor::buildCartesianGraphSurface()
{
    m_currentType = PlotsBuilder::CartesianGraphSurface;
    
    m_widget->plotIcon->setPixmap(KIcon("kde").pixmap(16.16));

    setupExpressionType(QStringList() << "x,y" << "x,z" << "y,z", QStringList() << "x" << "y");
}

void PlotsEditor::buildCartesianImplicitSurface()
{
    m_currentType = PlotsBuilder::CartesianImplicitSurface;

    m_widget->plotIcon->setPixmap(KIcon("list-add").pixmap(16.16));

    setupExpressionType(QStringList(), QStringList() << "x" << "y" << "z", true);
}

void PlotsEditor::buildCartesianParametricSurface()
{
    m_currentType = PlotsBuilder::CartesianParametricSurface;
    
    m_widget->plotIcon->setPixmap(KIcon("list-add").pixmap(16.16));

    setupExpressionType(QStringList() << "x" << "y" << "z", QStringList() << "u" << "v", false, true, 3);
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

void PlotsEditor::setCurrentFunctionGraphs(const QString& txt)
{
    m_currentFunctionGraphs = txt.split(",");
    
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

void PlotsEditor::setupFuncName(int func, const QString &funcvalue, const QStringList &vars)
{
    QString mmlhelper;
    mmlhelper.append("<mi>"+funcvalue+"</mi>");
    mmlhelper.append("<mo>(</mo>");
        
    foreach(const QString &var, vars)
    {
        mmlhelper.append("<mi>"+var+"</mi>");
        
        if (var != vars.last()) // no agregar comas al final
            mmlhelper.append("<mtext>,</mtext>");
    }

    switch (func)
    {
        case 1: 
        {
            m_widget->fname->setContent("<math display='block'><mrow>"+mmlhelper+"<mo>)</mo></mrow></math>");
            m_widget->gexpression->hide();
            m_widget->hexpression->hide();
            break;
        }
        case 2: 
        {
            m_widget->gname->setContent("<math display='block'><mrow>"+mmlhelper+"<mo>)</mo></mrow></math>");
            m_widget->gexpression->show();
            m_widget->hexpression->hide();
            break;
        }
        case 3: 
        {
            m_widget->hname->setContent("<math display='block'><mrow>"+mmlhelper+"<mo>)</mo></mrow></math>");
            m_widget->hexpression->show();
            break;
        }
    }
}

void PlotsEditor::setupExpressionType(const QStringList &fvalues, const QStringList &vvalues, bool isimplicit, bool isvectorValued, bool m_vectorSize)
{
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
//         if (m_vectorSize == 2)
//         {
//             m_widget->gexpression->show();
//             m_widget->hexpression->hide();
//         }
//         else
//         {
//             m_widget->gexpression->show();
//             m_widget->hexpression->show();
//         }
//         
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

    if ((isvectorValued && m_vectorSize == 2) || (isimplicit && vvalues.size() == 2) ||(!isimplicit && !isimplicit && vvalues.size() == 1))
        m_widget->previews->setCurrentIndex(0); //2d preview
    else // 3D
        m_widget->previews->setCurrentIndex(1); //3d preview

    showEditor();
}




