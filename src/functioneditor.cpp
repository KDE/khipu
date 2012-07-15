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


#include "functioneditor.h"

#include <QtGui/QPainter>
#include <KMenu>
#include <KLocale>
#include <KColorScheme>
#include <KColorCombo>

#include "analitza/analyzer.h"
#include "analitza/expression.h"
#include "analitza/variables.h"
#include "analitza/value.h"
#include "analitzagui/expressionedit.h"
#include "analitzagui/algebrahighlighter.h"
#include <KRandom>
#include <math.h>
#include "functionsmodel.h"
#include "functionsview.h"
// #include "keomath/solvers/solver.h"
// #include "keomath/solvers/solverfactory.h"
#include "functionlibraryedit.h"
#include "analitzagui/variablesmodel.h"
#include <analitzaplot/private/functiongraph.h>
#include <analitzaplot/planecurve.h>

#include <KStandardDirs>
#include "ui_functioneditorwidget.h"
#include <QMessageBox>
#include "ui_functioneditorwidget.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDoubleValidator>

using namespace std;

namespace Keomath
{



class FunctionEditorWidget : public QTabWidget, public Ui::FunctionEditorWidget
{
public:
    FunctionEditorWidget(QWidget *parent = 0)
        : QTabWidget(parent)
    {
        setupUi(this);

        cartesianCurveType->setItemIcon(0, KIcon("newfunction"));
        cartesianCurveType->setItemData(0, QStringList() << QString("x"), Qt::UserRole); 
        cartesianCurveType->setItemIcon(1, KIcon("newfunction"));
        cartesianCurveType->setItemData(1, QStringList() << QString("y"), Qt::UserRole);
        cartesianCurveType->setItemIcon(2, KIcon("newfunction"));
        cartesianCurveType->setItemData(2, QStringList() << QString("x") << QString("y"), Qt::UserRole);








        






        




        

        cmbTipoAnim->addItem("AntiHoraria");
        cmbTipoAnim->addItem("Horaria");

        




        setTabText(0, i18n("Funciones"));
        setTabIcon(0, KIcon("format-list-unordered"));

        setTabText(1, i18nc("", "Crear"));
        setTabIcon(1, KIcon("list-add"));




        

        setTabText(2, i18n("Area"));
        setTabIcon(2,KIcon("office-chart-area"));

        







        createCartesianCurve->setIcon(KIcon("newfunction"));
        createPolarCurve->setIcon(KIcon("newpolar"));
        createParametric2DCurve->setIcon(KIcon("newparametric"));


        createCartesianSurface->setIcon(KIcon("newfunction3d"));
        createCylindricalSurface->setIcon(KIcon("newcylindrical"));
        createSphericalSurface->setIcon(KIcon("newspherical"));
        createParametricSurface->setIcon(KIcon("newfunction3d"));
        createParametric3DCurve->setIcon(KIcon("newparametric3d"));

        clickOk->setIcon(KIcon("dialog-ok"));
        clickCancel->setIcon(KIcon("dialog-cancel"));

        argument1MinValue->setExpression(Analitza::Expression("-2pi", false));
        argument1MaxValue->setExpression(Analitza::Expression("2pi", false));

        argument2->hide();;
        argument2MinValue->hide();
        argument2MaxValue->hide();
        argument2MinValue->setExpression(Analitza::Expression("-2pi", false));
        argument2MaxValue->setExpression(Analitza::Expression("2pi", false));

        surfaceResolutionValue->setRange(1, 100);
        surfaceResolutionValue->setValue(50);



        curveResolutionValue->setRange(1, 100);
        curveResolutionValue->setValue(50);


        errorIcon->setPixmap(KIcon("emblem-important").pixmap(24,24));
        errorIcon->hide();
        errorMessage->hide();




        

        cmbFuncion1->addItem("Ninguna Funcion");
        cmbFuncion2->addItem("Ninguna Funcion");

        cmbEjemploArea->addItem("Funcion Creciente");
        cmbEjemploArea->addItem("Dos Funciones");
        cmbEjemploArea->addItem("Funcion Decreciente");
        cmbDivEjemplo->setCurrentIndex(0);


        cmbDivEjemplo->addItem("10");
        cmbDivEjemplo->addItem("20");
        cmbDivEjemplo->addItem("30");

        




        



        
    }
};










FunctionEditor::FunctionEditor(QWidget * parent)
    : QWidget(parent), m_functionsFilterProxyModel(0), m_isEditing(false), fi(0), liArea(-1000), lsArea(1000)
{



    m_editor = new FunctionEditorWidget(this);

    connect(m_editor->surfaceResolutionValue, SIGNAL(valueChanged(int)), m_editor->surfaceResolutionLcd, SLOT(display(int)));
    connect(m_editor->curveResolutionValue, SIGNAL(valueChanged(int)), m_editor->curveResolutionLcd, SLOT(display(int)));

    

    connect(m_editor->createCartesianCurve, SIGNAL(mouseHovered()), SLOT(showCartesianInfo()));
    connect(m_editor->createPolarCurve, SIGNAL(mouseHovered()), SLOT(showPolarInfo()));
    connect(m_editor->createParametric2DCurve, SIGNAL(mouseHovered()), SLOT(showParametricInfo()));


    connect(m_editor->createCartesianCurve, SIGNAL(mouseLeft()), SLOT(clearFunctions2DInfo()));
    connect(m_editor->createPolarCurve, SIGNAL(mouseLeft()), SLOT(clearFunctions2DInfo()));
    connect(m_editor->createParametric2DCurve, SIGNAL(mouseLeft()), SLOT(clearFunctions2DInfo()));



    

    connect(m_editor->createCartesianSurface, SIGNAL(mouseHovered()), SLOT(showCartesianSurfaceInfo()));
    connect(m_editor->createCylindricalSurface, SIGNAL(mouseHovered()), SLOT(showCylindricalSurfaceInfo()));
    connect(m_editor->createSphericalSurface, SIGNAL(mouseHovered()), SLOT(showSphericalSurfaceInfo()));
    connect(m_editor->createParametricSurface, SIGNAL(mouseHovered()), SLOT(showParametricSurfaceInfo()));
    connect(m_editor->createParametric3DCurve, SIGNAL(mouseHovered()), SLOT(showParametric3DInfo()));

    connect(m_editor->createCartesianSurface, SIGNAL(mouseLeft()), SLOT(clearFunctions2DInfo()));
    connect(m_editor->createCylindricalSurface, SIGNAL(mouseLeft()), SLOT(clearFunctions2DInfo()));
    connect(m_editor->createSphericalSurface, SIGNAL(mouseLeft()), SLOT(clearFunctions2DInfo()));
    connect(m_editor->createParametricSurface, SIGNAL(mouseLeft()), SLOT(clearFunctions2DInfo()));
    connect(m_editor->createParametric3DCurve, SIGNAL(mouseLeft()), SLOT(clearFunctions2DInfo()));


    
    m_editor->implicitCombobox->insertItem(0,"Seleccionar");
    m_editor->implicitCombobox->insertItem(1,"Esfera");
    m_editor->implicitCombobox->insertItem(2,"Elipsoide");
    m_editor->implicitCombobox->insertItem(3,"Cilindro");
    m_editor->implicitCombobox->insertItem(4,"Hiperboloide de una hoja");
    m_editor->implicitCombobox->insertItem(5,"Hiperboloide de dos hojas");
    m_editor->implicitCombobox->insertItem(6,"Paraboloide hiperbolico");
    m_editor->implicitCombobox->insertItem(7,"Paraboloide eliptico");
    m_editor->implicitCombobox->insertItem(8,"Cono eliptico");
    m_editor->octantecombobox->insertItem(0,"All");
    for(int i=0; i<8; i++) m_editor->octantecombobox->insertItem(i+1,QString("%1").arg(i+1));
    m_editor->ejecombobox->insertItem(0,"X");
    m_editor->ejecombobox->insertItem(1,"Y");
    m_editor->ejecombobox->insertItem(2,"Z");
    m_editor->ejecombobox->setCurrentIndex(2);
    m_editor->lblFormula->hide();
    implicitfunctionsId = 1000;
    
    

    m_editor->gbImplicitInfo->hide();
    m_editor->gbImplicitParameters->hide();
    currentImplicitIndex=0;

    

    connect(m_editor->cmbTipoAnim,SIGNAL(currentIndexChanged(int)),SLOT(setDirAnim()));
    connect(m_editor->chkModoAnim,SIGNAL(clicked()),SLOT(setModAnActivo()));
    connect(m_editor->btnPlayAnim,SIGNAL(clicked()),SLOT(setAnimActiva()));
    connect(m_editor->btnReiniciar,SIGNAL(clicked()),SLOT(reiniciarAnimacion()));
    connect(m_editor,SIGNAL(currentChanged(int)),SLOT(llenarFuncionesR()));
    connect(m_editor->btnHacerCalculos,SIGNAL(clicked()),SLOT(calcularAreaR()));
    connect(m_editor->btnCalcularEjemplo,SIGNAL(clicked()),SLOT(calcularAreaE()));
    connect(m_editor->cmbFuncion1,SIGNAL(currentIndexChanged(int)),SLOT(cambiarAreaF()));
    connect(m_editor->cmbFuncion2,SIGNAL(currentIndexChanged(int)),SLOT(cambiarAreaF()));




    m_editor->cmbTipoAnim->setCurrentIndex(0);

    

    
    connect(m_editor->implicitCombobox,SIGNAL(currentIndexChanged(int)), SLOT(changeCurrentImplicitIndex(int)));
    connect(m_editor->ejecombobox,SIGNAL(currentIndexChanged(int)),SLOT(changeCurrentAxis(int)));
    connect(m_editor->chkboxCurvas,SIGNAL(stateChanged(int)),SLOT(chkboxCurvasOnSelect(int)));
    
    

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_editor);





    connect(m_editor->cartesianCurveType, SIGNAL(currentIndexChanged(int)), SLOT(setCartesianCurveDomainInfo(int)));





    connect(m_editor->createPolarCurve, SIGNAL(clicked()), SLOT(setPolarCurveDomainInfo()));
    connect(m_editor->createParametric2DCurve, SIGNAL(clicked()), SLOT(setParametricCurveDomainInfo()));























    

    m_editor->setCurrentIndex(1);



    QDoubleValidator* qevaluate = new QDoubleValidator(-5.00, 5.00, 2,  m_editor->txtedtParameter1);

    qevaluate->setNotation(QDoubleValidator::StandardNotation);
    m_editor->txtedtParameter1->setValidator(qevaluate);
    m_editor->txtedtParameter2->setValidator(qevaluate);
    m_editor->txtedtParameter3->setValidator(qevaluate);
    m_editor->txtedtParameter4->setValidator(qevaluate);
    m_editor->txtedtParameter5->setValidator(qevaluate);
    m_editor->txtedtParameter6->setValidator(qevaluate);

    m_editor->txtedtParameter1->setMaxLength(4);
    m_editor->txtedtParameter2->setMaxLength(4);
    m_editor->txtedtParameter3->setMaxLength(4);
    m_editor->txtedtParameter4->setMaxLength(4);
    m_editor->txtedtParameter5->setMaxLength(4);
    m_editor->txtedtParameter6->setMaxLength(4);



}




double FunctionEditor::devFuncion(int tipo, double val)
{
    switch(tipo)
    {
    case 1:
        return((-1 * val) + 7);
        break;
    case 2:
        return(val);
        break;
    case 3:
        return(sin(val/10)*40);
        break;
    case 4:
        return(cos(val/10)*40);
        break;
    case 5:
        return(5*log(val)+3);
        break;
    case 6:
        return((-1 * pow(val/4,2)) + 5);
        break;
    default:
        return -40;
        break;

    }

}

FunctionEditor::~FunctionEditor()
{
}

FunctionsView * FunctionEditor::functionsView() const
{
    return m_editor->functionsView;
}

void FunctionEditor::setFunctionsModel(FunctionsFilterProxyModel * functionsFilterProxyModel)
{
    m_functionsFilterProxyModel = functionsFilterProxyModel;
    




    

    
    m_editor->functionsView->setModel(m_functionsFilterProxyModel);

    

    m_editor->functionsView->header()->setResizeMode(1, QHeaderView::Interactive	);
    m_editor->functionsView->showColumn(0);
    m_editor->functionsView->showColumn(1);
    m_editor->functionsView->hideColumn(2);
    m_editor->functionsView->hideColumn(3);
}

void FunctionEditor::setVariables(Analitza::Variables* v)
{
    m_vars = v;


}








void FunctionEditor::setupEditorBehaviour(int dimension)
{
    m_dimensionBehaviour = dimension;

    if (m_dimensionBehaviour == 2)
    {
        m_editor->functions->setCurrentIndex(0);
        
        m_editor->createCartesianSurface->hide();
        m_editor->createCylindricalSurface->hide();
        m_editor->createSphericalSurface->hide();
        m_editor->createParametricSurface->hide();
        m_editor->createParametric3DCurve->hide();
        m_editor->createImplicitSurface->hide();

        

        connect(m_editor->createCartesianCurve, SIGNAL(clicked()), SLOT(initCartesianCurve()));
        connect(m_editor->createPolarCurve, SIGNAL(clicked()), SLOT(initPolarCurve()));
        connect(m_editor->createParametric2DCurve, SIGNAL(clicked()), SLOT(initParametricCurve2D()));
        
    }
    else if (m_dimensionBehaviour == 3)
    {
        m_editor->functions->setCurrentIndex(1);

        
        m_editor->createCartesianCurve->hide();
        m_editor->createPolarCurve->hide();
        m_editor->createParametric2DCurve->hide();


        connect(m_editor->createCartesianSurface, SIGNAL(clicked()), SLOT(initCartesianSurface()));
        connect(m_editor->createCylindricalSurface, SIGNAL(clicked()), SLOT(initCylindricalSurface()));
        connect(m_editor->createSphericalSurface, SIGNAL(clicked()), SLOT(initSphericalSurface()));
        connect(m_editor->createParametricSurface, SIGNAL(clicked()), SLOT(initParametricSurface()));
        connect(m_editor->createParametric3DCurve, SIGNAL(clicked()), SLOT(initParametricCurve3D()));
        connect(m_editor->createImplicitSurface, SIGNAL(clicked()), SLOT(initImplicitSurface()));

        
































        
    }

    m_editor->functionName->setFilterDimension(m_dimensionBehaviour);




    m_editor->quickNewFunctionByName->setFilterDimension(m_dimensionBehaviour);











    connect(m_editor->quickNewFunctionByName, SIGNAL(selectedFunction(QString,int,QString,QStringList)),
            SLOT(saveFromQuick(QString,int,QString,QStringList)));



    connect(m_editor->functionName, SIGNAL(selectedFunction(QString,int,QString,QStringList)),
            SLOT(completeFunctionInfoFromQuickName(QString,int,QString,QStringList)));


    m_editor->appearanceCurve->hide();
    m_editor->appearanceSurface->hide();
    m_editor->domain->hide();
    m_editor->functionInfo->hide();
    m_editor->clickOk->hide();
    m_editor->clickCancel->hide();

    connect(m_editor->clickOk, SIGNAL(pressed()), SLOT(createFunction()));
    connect(m_editor->clickCancel, SIGNAL(pressed()), SLOT(resetAll()));




    connect(m_editor->functionsView, SIGNAL(doubleClicked(const QModelIndex &)), SLOT(editFunction(const QModelIndex &)));








    m_editor->infoFunctionPicture->setPixmap(QPixmap());
    m_editor->infoFunctionPicture->show();


    m_editor->functions->hide();





}

void FunctionEditor::initCartesianCurve()
{
    m_state = EditingCartesianCurve;


    m_editor->functions2D->setCurrentIndex(1);
    m_editor->cartesianCurveExpression->setFocus();
    m_editor->functionName->show();
    m_editor->cartesianCurveType->setCurrentIndex(0); 


    m_editor->argument1->setText("x");
    m_editor->argument2->setText("y");







    m_editor->functionName->setFilterArguments(QStringList() << "x");



    initFunction();
}

void FunctionEditor::initPolarCurve(const RealInterval::List &functionDomain)
{
    m_state = EditingPolarCurve;

    m_editor->functions2D->setCurrentIndex(2);
    m_editor->polarCurveExpression->setFocus();
    m_editor->functionName->show();
    m_editor->createImplicitSurface->hide();


    initDomainInfo(QStringList() << "q", functionDomain);

    initFunction();
}

void FunctionEditor::initParametricCurve2D(const RealInterval::List &functionDomain)
{
    m_state = EditingParametricCurve2D;

    m_editor->functions2D->setCurrentIndex(3);
    m_editor->parametricCurve2DXExpression->setFocus();
    m_editor->functionName->show();
    m_editor->createImplicitSurface->hide();


    initDomainInfo(QStringList() << "t", functionDomain);


    initFunction();
}











void FunctionEditor::initCartesianSurface(const RealInterval::List &functionDomain)
{
    m_state = EditingCartesianSurface;

    m_editor->functions3D->setCurrentIndex(1);
    m_editor->cartesianSurfaceExpression->setFocus();
    m_editor->functionName->show();
    m_editor->createImplicitSurface->hide();



    initDomainInfo(QStringList() << "x" << "y", functionDomain);

    initFunction();
}

void FunctionEditor::initCylindricalSurface(const RealInterval::List &functionDomain)
{
    m_state = EditingCylindricalSurface;

    m_editor->functions3D->setCurrentIndex(2);
    m_editor->cylindricalSurfaceExpression->setFocus();
    m_editor->functionName->show();
    m_editor->createImplicitSurface->hide();


    initDomainInfo(QStringList() << "r" << "t", functionDomain);

    initFunction();
}

void FunctionEditor::initSphericalSurface(const RealInterval::List &functionDomain)
{
    m_state = EditingSphericalSurface;

    m_editor->functions3D->setCurrentIndex(3);
    m_editor->sphericalSurfaceExpression->setFocus();
    m_editor->functionName->show();
    m_editor->createImplicitSurface->hide();

    initDomainInfo(QStringList() << "s" << "t", functionDomain);

    initFunction();
}

void FunctionEditor::initParametricSurface(const RealInterval::List &functionDomain)
{
    m_state = EditingParametricSurface;

    m_editor->functions3D->setCurrentIndex(4);
    m_editor->parametricSurfaceXExpression->setFocus();
    m_editor->functionName->show();
    m_editor->createImplicitSurface->hide();

    initDomainInfo(QStringList() << "u" << "v", functionDomain);

    initFunction();
}

void FunctionEditor::initParametricCurve3D(const RealInterval::List &functionDomain)
{
    m_state = EditingParametricCurve3D;
    m_editor->functions3D->setCurrentIndex(5);
    m_editor->functionName->show();
    m_editor->createImplicitSurface->hide();
    initDomainInfo(QStringList() << "t", functionDomain);
    initFunction();
}

void FunctionEditor::initImplicitSurface(const RealInterval::List &functionDomain)
{
    m_state = EditingImplicitSurface;
    m_editor->createImplicitSurface->hide();
    m_editor->label_3->hide();
    m_editor->domain->hide();
    m_editor->functionInfo->hide();
    m_editor->implicitCombobox->show();
    m_editor->gbImplicitInfo->show();
    m_editor->functionName->hide();
    m_editor->lblFormula->hide();
    m_editor->argument1->hide();
    m_editor->label_23->hide();
    m_editor->label_34->hide();
    m_editor->argument1MinValue->hide();
    m_editor->argument1MaxValue->hide();
    m_editor->argument2MinValue->hide();
    m_editor->argument2MaxValue->hide();
    m_editor->chkboxSolido->hide();
    m_editor->chkboxCurvas->hide();
    m_editor->gbCurvas->hide();
    m_editor->functionsView->showColumn(0);
    m_editor->functionsView->showColumn(2);
    m_editor->functionsView->header()->resizeSection(0,200);
    
    
    m_editor->groupBox_21->hide();
    m_editor->groupBox_23->hide();
    initFunction();
}


void FunctionEditor::changeCurrentImplicitIndex(int index)
{

    m_editor->lblParameter1->hide();
    m_editor->lblParameter2->hide();
    m_editor->lblParameter3->hide();
    m_editor->lblParameter4->hide();
    m_editor->lblParameter5->hide();
    m_editor->lblParameter6->hide();
    m_editor->txtedtParameter1->hide();
    m_editor->txtedtParameter2->hide();
    m_editor->txtedtParameter3->hide();
    m_editor->txtedtParameter4->hide();
    m_editor->txtedtParameter5->hide();
    m_editor->txtedtParameter6->hide();
    m_editor->chkboxCurvas->setChecked(false);
    m_editor->lblFormula->show();
    m_editor->chkboxSolido->hide();
    m_editor->chkboxCurvas->show();
    m_editor->gbCurvas->hide();
    m_editor->txtedtParameter1->setEnabled(true);
    m_editor->txtedtParameter2->setEnabled(true);
    m_editor->txtedtParameter3->setEnabled(true);
    m_editor->txtedtParameter4->setEnabled(true);
    m_editor->txtedtParameter5->setEnabled(true);
    m_editor->txtedtParameter6->setEnabled(true);

    if(index!=0)
    {
        currentImplicitIndex = index-1;
    }
    else
        currentImplicitIndex = index;

    switch(currentImplicitIndex)
    {

    case 0:
        m_editor->lblFormula->setText("(X-Xo)^2 + (Y-Yo)^2 + (Z-Zo)^2 = r^2");
        m_editor->lblParameter1->setText("Xo");
        m_editor->lblParameter2->setText("Yo");
        m_editor->lblParameter3->setText("Zo");
        m_editor->lblParameter4->setText("r");
        m_editor->lblParameter1->show();
        m_editor->lblParameter2->show();
        m_editor->lblParameter3->show();
        m_editor->lblParameter4->show();
        m_editor->txtedtParameter1->show();
        m_editor->txtedtParameter2->show();
        m_editor->txtedtParameter3->show();
        m_editor->txtedtParameter4->show();
        m_editor->chkboxSolido->show();
        break;
    case 1:
        m_editor->lblFormula->setText("((X-Xo)/a)^2 + ((Y-Yo)/b)^2 + ((Z-Zo)/c)^2 = r^2");
        m_editor->lblParameter1->setText("Xo");
        m_editor->lblParameter2->setText("Yo");
        m_editor->lblParameter3->setText("Zo");
        m_editor->lblParameter4->setText("a");
        m_editor->lblParameter5->setText("b");
        m_editor->lblParameter6->setText("c");
        m_editor->lblParameter1->show();
        m_editor->lblParameter2->show();
        m_editor->lblParameter3->show();
        m_editor->lblParameter4->show();
        m_editor->lblParameter5->show();
        m_editor->lblParameter6->show();
        m_editor->txtedtParameter1->show();
        m_editor->txtedtParameter2->show();
        m_editor->txtedtParameter3->show();
        m_editor->txtedtParameter4->show();
        m_editor->txtedtParameter5->show();
        m_editor->txtedtParameter6->show();
        m_editor->chkboxSolido->show();
        break;
    case 2:
        m_editor->lblFormula->setText("(X-Xo)^2+(Y-Yo)^2 = r^2");
        m_editor->lblParameter1->setText("Xo");
        m_editor->lblParameter2->setText("Yo");
        m_editor->lblParameter3->setText("Zo");
        m_editor->lblParameter4->setText("r");
        m_editor->lblParameter1->show();
        m_editor->lblParameter2->show();
        m_editor->lblParameter3->show();
        m_editor->lblParameter4->show();
        m_editor->txtedtParameter1->show();
        m_editor->txtedtParameter2->show();
        m_editor->txtedtParameter3->show();
        m_editor->txtedtParameter4->show();
        break;
    case 3:
        m_editor->lblFormula->setText("((X-Xo)/a)^2 + ((Y-Yo)/b)^2 - ((Z-Zo)/c)^2 = 1");
        m_editor->lblParameter1->setText("Xo");
        m_editor->lblParameter2->setText("Yo");
        m_editor->lblParameter3->setText("Zo");
        m_editor->lblParameter4->setText("a");
        m_editor->lblParameter5->setText("b");
        m_editor->lblParameter6->setText("c");
        m_editor->lblParameter1->show();
        m_editor->lblParameter2->show();
        m_editor->lblParameter3->show();
        m_editor->lblParameter4->show();
        m_editor->lblParameter5->show();
        m_editor->lblParameter6->show();
        m_editor->txtedtParameter1->show();
        m_editor->txtedtParameter2->show();
        m_editor->txtedtParameter3->show();
        m_editor->txtedtParameter4->show();
        m_editor->txtedtParameter5->show();
        m_editor->txtedtParameter6->show();
        break;
    case 4:
        m_editor->lblFormula->setText("((X-Xo)/a)^2 + ((Y-Yo)/b)^2 - ((Z-Zo)/c)^2 = -1");
        m_editor->lblParameter1->setText("Xo");
        m_editor->lblParameter2->setText("Yo");
        m_editor->lblParameter3->setText("Zo");
        m_editor->lblParameter4->setText("a");
        m_editor->lblParameter5->setText("b");
        m_editor->lblParameter6->setText("c");
        m_editor->lblParameter1->show();
        m_editor->lblParameter2->show();
        m_editor->lblParameter3->show();
        m_editor->lblParameter4->show();
        m_editor->lblParameter5->show();
        m_editor->lblParameter6->show();
        m_editor->txtedtParameter1->show();
        m_editor->txtedtParameter2->show();
        m_editor->txtedtParameter3->show();
        m_editor->txtedtParameter4->show();
        m_editor->txtedtParameter5->show();
        m_editor->txtedtParameter6->show();
        break;
    case 5:
        m_editor->lblFormula->setText("((X-Xo)/a)^2 - ((Y-Yo)/b)^2 = (Z-Zo)");
        m_editor->lblParameter1->setText("Xo");
        m_editor->lblParameter2->setText("Yo");
        m_editor->lblParameter3->setText("Zo");
        m_editor->lblParameter4->setText("a");
        m_editor->lblParameter5->setText("b");
        m_editor->lblParameter6->setText("c");
        m_editor->lblParameter1->show();
        m_editor->lblParameter2->show();
        m_editor->lblParameter3->show();
        m_editor->lblParameter4->show();
        m_editor->lblParameter5->show();
        m_editor->lblParameter6->show();
        m_editor->txtedtParameter1->show();
        m_editor->txtedtParameter2->show();
        m_editor->txtedtParameter3->show();
        m_editor->txtedtParameter4->show();
        m_editor->txtedtParameter5->show();
        m_editor->txtedtParameter6->show();
        break;
    case 6:
        m_editor->lblFormula->setText("(X-Xo)^2/a^2 - (Y-Yo)^2/b^2 = (Z-Zo)/c");
        m_editor->lblParameter1->setText("Xo");
        m_editor->lblParameter2->setText("Yo");
        m_editor->lblParameter3->setText("Zo");
        m_editor->lblParameter4->setText("a");
        m_editor->lblParameter5->setText("b");
        m_editor->lblParameter6->setText("c");
        m_editor->lblParameter1->show();
        m_editor->lblParameter2->show();
        m_editor->lblParameter3->show();
        m_editor->lblParameter4->show();
        m_editor->lblParameter5->show();
        m_editor->lblParameter6->show();
        m_editor->txtedtParameter1->show();
        m_editor->txtedtParameter2->show();
        m_editor->txtedtParameter3->show();
        m_editor->txtedtParameter4->show();
        m_editor->txtedtParameter5->show();
        m_editor->txtedtParameter6->show();
        break;
    case 7:
        m_editor->lblFormula->setText("(X-Xo)^2/a^2 - (Y-Yo)^2/b^2 = (Z-Zo)^2/c^2");
        m_editor->lblParameter1->setText("Xo");
        m_editor->lblParameter2->setText("Yo");
        m_editor->lblParameter3->setText("Zo");
        m_editor->lblParameter4->setText("a");
        m_editor->lblParameter5->setText("b");
        m_editor->lblParameter6->setText("c");
        m_editor->lblParameter1->show();
        m_editor->lblParameter2->show();
        m_editor->lblParameter3->show();
        m_editor->lblParameter4->show();
        m_editor->lblParameter5->show();
        m_editor->lblParameter6->show();
        m_editor->txtedtParameter1->show();
        m_editor->txtedtParameter2->show();
        m_editor->txtedtParameter3->show();
        m_editor->txtedtParameter4->show();
        m_editor->txtedtParameter5->show();
        m_editor->txtedtParameter6->show();
        break;
    }

    m_editor->gbImplicitParameters->show();
    m_editor->ejecombobox->setCurrentIndex(2);
    this->changeCurrentAxis(2);
}


void FunctionEditor::chkboxCurvasOnSelect(int index)
{

    if(index==2)
    {
        m_editor->gbCurvas->show();
        m_editor->rbtnXY->setChecked(true);
        m_editor->gbImplicitInfo->setMinimumHeight(200);
        m_editor->ejecombobox->setEnabled(false);
        m_editor->octantecombobox->setEnabled(false);
    }
    else
    {
        m_editor->gbCurvas->hide();
        m_editor->gbImplicitInfo->setMinimumHeight(170);
        m_editor->ejecombobox->setEnabled(true);
        m_editor->octantecombobox->setEnabled(true);
    }
}

void FunctionEditor::changeCurrentAxis(int index)
{

    switch(currentImplicitIndex)
    {

    case 2:
        m_editor->txtedtParameter1->setEnabled(true);
        m_editor->txtedtParameter2->setEnabled(true);
        m_editor->txtedtParameter3->setEnabled(true);
        switch(index)
        {
        case 0:
            m_editor->lblFormula->setText("(Y-Yo)^2+(Z-Zo)^2 = r^2");
            m_editor->txtedtParameter1->setEnabled(false);
            break;
        case 1:
            m_editor->lblFormula->setText("(X-Xo)^2+(Z-Zo)^2 = r^2");
            m_editor->txtedtParameter2->setEnabled(false);
            break;
        case 2:
            m_editor->lblFormula->setText("(X-Xo)^2+(Y-Yo)^2 = r^2");
            m_editor->txtedtParameter3->setEnabled(false);
            break;
        }
        break;

    case 3:
        switch(index)
        {
        case 0:
            m_editor->lblFormula->setText("((Y-Yo)/b)^2 + ((Z-Zo)/c)^2 - ((X-Xo)/a)^2 = 1");
            break;
        case 1:
            m_editor->lblFormula->setText("((X-Xo)/a)^2 + ((Z-Zo)/c)^2 - ((Y-Yo)/b)^2 = 1");
            break;
        case 2:
            m_editor->lblFormula->setText("((X-Xo)/a)^2 + ((Y-Yo)/b)^2 - ((Z-Zo)/c)^2 = 1");
            break;
        }
        break;

    case 4:
        switch(index)
        {
        case 0:
            m_editor->lblFormula->setText("((Y-Yo)/b)^2 + ((Z-Zo)/c)^2 - ((X-Xo)/a)^2 = -1");
            break;
        case 1:
            m_editor->lblFormula->setText("((X-Xo)/a)^2 + ((Z-Zo)/c)^2 - ((Y-Yo)/b)^2 = -1");
            break;
        case 2:
            m_editor->lblFormula->setText("((X-Xo)/a)^2 + ((Y-Yo)/b)^2 - ((Z-Zo)/c)^2 = -1");
            break;
        }
        break;

    case 5:
        m_editor->txtedtParameter3->setEnabled(true);
        m_editor->txtedtParameter4->setEnabled(true);
        m_editor->txtedtParameter5->setEnabled(true);
        switch(index)
        {
        case 0:
            m_editor->lblFormula->setText("((Y-Yo)/b)^2 - ((Z-Zo)/c)^2 = (X-Xo)");
            
            break;
        case 1:
            m_editor->lblFormula->setText("((X-Xo)/a)^2 - ((Z-Zo)/c)^2 = (Y-Yo)");
            
            break;
        case 2:
            m_editor->lblFormula->setText("((X-Xo)/a)^2 - ((Y-Yo)/b)^2 = (Z-Zo)");
            
            break;
        }
        break;

    case 6:
        switch(index)
        {
        case 0:
            m_editor->lblFormula->setText("(Y-Yo)^2/b^2 - (Z-Zo)^2/c^2 = (X-Xo)/a");
            break;
        case 1:
            m_editor->lblFormula->setText("(X-Xo)^2/a^2 - (Z-Zo)^2/c^2 = (Y-Yo)/b");
            break;
        case 2:
            m_editor->lblFormula->setText("(X-Xo)^2/a^2 - (Y-Yo)^2/b^2 = (Z-Zo)/c");
            break;
        }
        break;

    case 7:
        switch(index)
        {
        case 0:
            m_editor->lblFormula->setText("(Y-Yo)^2/b^2 - (Z-Zo)^2/c^2 = (X-Xo)^2/a^2");
            break;
        case 1:
            m_editor->lblFormula->setText("(X-Xo)^2/a^2 - (Z-Zo)^2/c^2 = (Y-Yo)^2/b^2");
            break;
        case 2:
            m_editor->lblFormula->setText("(X-Xo)^2/a^2 - (Y-Yo)^2/b^2 = (Z-Zo)^2/c^2");
            break;
        }
        break;

    }
}




void FunctionEditor::createFunction()
{

    bool valid = false;

    switch (m_state)
    {
        

    case EditingCartesianCurve:
        valid = createCartesianCurve();
        break;
    case EditingPolarCurve:
        valid = createPolarCurve();
        break;
    case EditingParametricCurve2D:
        valid = createParametricCurve2D();
        break;

        
    case EditingCartesianSurface:
        valid = createCartesianSurface();
        break;
    case EditingCylindricalSurface:
        valid = createCylindricalSurface();
        break;
    case EditingSphericalSurface:
        valid = createSphericalSurface();
        break;
    case EditingParametricSurface:
        valid = createParametricSurface();
        break;
    case EditingParametricCurve3D:
        valid = createParametricCurve3D();
        break;
//     case EditingImplicitSurface:
//         valid = createImplicitSurface(currentImplicitIndex);
//         break;
    }

    if (valid)
    {
        m_editor->setCurrentIndex(0);
        resetAll();
    }
    else
    {
        showErrorMessage();
        m_errors.clear(); 
    }
}

void FunctionEditor::resetAll()
{
    m_errors.clear();

    m_editor->errorIcon->hide();
    m_editor->errorMessage->clear();
    m_editor->errorMessage->hide();
    m_editor->createImplicitSurface->hide();

    m_isEditing = false;

    m_editor->createCartesianCurve->show();
    m_editor->createPolarCurve->show();
    m_editor->createParametric2DCurve->show();
    m_editor->quickNewFunctionByName->show();
    m_editor->infoFunctionPicture->show();

    m_editor->appearanceCurve->hide();
    m_editor->appearanceSurface->hide();
    m_editor->domain->hide();
    m_editor->functionInfo->hide();

    
    m_editor->gbImplicitInfo->hide();
    m_editor->gbImplicitParameters->hide();
    
    
    m_editor->clickOk->hide();
    m_editor->clickCancel->hide();

    m_editor->setTabText(1, i18n("&Crear"));

    m_editor->functions2D->setCurrentIndex(0);
    m_editor->functions3D->setCurrentIndex(0);

    if (m_dimensionBehaviour == 2)
    {

        m_editor->createCartesianCurve->show();
        m_editor->createPolarCurve->show();
        m_editor->createParametric2DCurve->show();
        m_editor->createCartesianSurface->hide();
        m_editor->createCylindricalSurface->hide();
        m_editor->createSphericalSurface->hide();
        m_editor->createParametricSurface->hide();
        m_editor->createParametric3DCurve->hide();
        m_editor->groupBox_23->hide();

        clearCartesianCurve();
        clearPolarCurve();
        clearParametricCurve2D();
        m_editor->chkModoAnim->setChecked(false);
        m_editor->clickOk->setEnabled(true);

    }
    else if (m_dimensionBehaviour == 3)
    {
        m_editor->createCartesianSurface->show();
        m_editor->createCylindricalSurface->show();
        m_editor->createSphericalSurface->show();
        m_editor->createParametricSurface->show();
        m_editor->createParametric3DCurve->show();
        m_editor->createCartesianCurve->hide();
        m_editor->createPolarCurve->hide();
        m_editor->createParametric2DCurve->hide();
        
        m_editor->setTabEnabled(2,false);
        m_editor->createImplicitSurface->show();
        m_editor->implicitCombobox->setCurrentIndex(0);
        m_editor->gbImplicitParameters->hide();
        m_editor->chkboxCurvas->hide();
        m_editor->lblFormula->hide();
        
        clearCartesianSurface();
        clearCylindricalSurface();
        clearSphericalSurface();
        clearParametricSurface();
        clearParametricCurve3D();
    }





    m_editor->infoFunctionPicture->setPixmap(QPixmap());
    m_editor->functions->hide();

}

void FunctionEditor::editFunction(const QModelIndex &index)
{
    int sourceRow = m_functionsFilterProxyModel->mapToSource(index).row();

    m_isEditing = true;

    m_editor->setTabText(1, i18n("&Editando"));
    m_editor->setCurrentIndex(1);

    FunctionsModel *functionModel = static_cast<FunctionsModel*>(m_functionsFilterProxyModel->sourceModel());

    FunctionGraph *fn = (FunctionGraph *)(functionModel->item(sourceRow));

    m_editor->functionName->setText(fn->name());

//     m_currentEditFunctionId = fn->id();
    QStringList arguments = fn->parameters();

    if (fn->spaceDimension() == 2)
    {
        if (fn->expression().lambdaBody().isVector())
        {
            if (arguments == QStringList() << QString("t"))
            {

                //NOTE quick FIX 
                RealInterval::List dd; 
                foreach (const QString &param, fn->parameters())
                {
                    QPair<double, double> ii_ = fn->interval(param);
                    dd << RealInterval(ii_.first, ii_.second);
                }   
           
                initParametricCurve2D(dd);

                m_editor->parametricCurve2DXExpression->setExpression(fn->expression().lambdaBody().elementAt(0));
                m_editor->parametricCurve2DYExpression->setExpression(fn->expression().lambdaBody().elementAt(1));


                setParametricCurveDomainInfo();
            }
        }
        else
        {
            if ((arguments == QStringList() << QString("x")) ||
                    (arguments == QStringList() << QString("y")) ||
                    (arguments == QStringList() << QString("x") << QString("y")))
            {
                initCartesianCurve();
                m_editor->cartesianCurveExpression->setExpression(fn->expression().lambdaBody());

                if (arguments == QStringList() << QString("x"))
                {
                    m_editor->cartesianCurveType->setCurrentIndex(0);


                    //TODO
//                     m_editor->argument1MinValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).lower())));
//                     m_editor->argument1MaxValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).upper())));

                }
                else if (arguments == QStringList() << QString("y"))
                {
                    m_editor->cartesianCurveType->setCurrentIndex(1);

                    //TODO
//                     m_editor->argument1MinValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).lower())));
//                     m_editor->argument1MaxValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).upper())));

                }
                else if (arguments == QStringList() << QString("x") << QString("y"))
                {
                    m_editor->cartesianCurveType->setCurrentIndex(2);

                    //TODO
//                     m_editor->argument1MinValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).lower())));
//                     m_editor->argument1MaxValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).upper())));

                    //TODO
//                     m_editor->argument2MinValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(1).lower())));
//                     m_editor->argument2MaxValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(1).upper())));


                }
            }
            else if (arguments == QStringList() << QString("q"))
            {
                //TODO
//                 initPolarCurve(fn->domain());
                m_editor->polarCurveExpression->setExpression(fn->expression().lambdaBody());


                //TODO
//                 m_editor->argument1MinValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).lower())));
//                 m_editor->argument1MaxValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).upper())));


                setPolarCurveDomainInfo();
            }
        }
    }
    else if (fn->spaceDimension() == 3)
    {
        if (fn->expression().lambdaBody().isVector())
        {
            if (arguments == QStringList() << QString("u") << QString("v"))
            {
                //TODO
//                 initParametricSurface(fn->domain());

                m_editor->parametricSurfaceXExpression->setExpression(fn->expression().lambdaBody().elementAt(0));
                m_editor->parametricSurfaceYExpression->setExpression(fn->expression().lambdaBody().elementAt(1));
                m_editor->parametricSurfaceZExpression->setExpression(fn->expression().lambdaBody().elementAt(2));

                //TODO
//                 m_editor->argument1MinValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).lower())));
//                 m_editor->argument1MaxValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).upper())));

//                 m_editor->argument2MinValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(1).lower())));
//                 m_editor->argument2MaxValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(1).upper())));


                setParametricSurfaceDomainInfo();
            }
            else if (arguments == QStringList() << QString("t"))
            {
                //TODO
//                 initParametricCurve3D(fn->domain());

                m_editor->parametricCurve3DXExpression->setExpression(fn->expression().lambdaBody().elementAt(0));
                m_editor->parametricCurve3DYExpression->setExpression(fn->expression().lambdaBody().elementAt(1));
                m_editor->parametricCurve3DZExpression->setExpression(fn->expression().lambdaBody().elementAt(2));

                //TODO
//                 m_editor->argument1MinValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).lower())));
//                 m_editor->argument1MaxValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).upper())));



                setParametricCurve3DDomainInfo();

            }
        }
        else
        {
            if ((arguments == QStringList() << QString("x") << QString("y")))
            {
                //TODO
//                 initCartesianSurface(fn->domain());
                m_editor->cartesianSurfaceExpression->setExpression(fn->expression().lambdaBody());


                //TODO
//                 m_editor->argument1MinValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).lower())));
//                 m_editor->argument1MaxValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).upper())));

//                 m_editor->argument2MinValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(1).lower())));
//                 m_editor->argument2MaxValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(1).upper())));


                setCartesianSurfaceDomainInfo();
            }
            else if (arguments == QStringList() << QString("r") << QString("t"))
            {
                //TODO
//                 initCylindricalSurface(fn->domain());
                m_editor->cylindricalSurfaceExpression->setExpression(fn->expression().lambdaBody());


//                 m_editor->argument1MinValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).lower())));
//                 m_editor->argument1MaxValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).upper())));

//                 m_editor->argument2MinValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(1).lower())));
//                 m_editor->argument2MaxValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(1).upper())));

                setCylindricalSurfaceDomainInfo();
            }
            else if (arguments == QStringList() << QString("s") << QString("t"))
            {
                //TODO
//                 initSphericalSurface(fn->domain());
                m_editor->sphericalSurfaceExpression->setExpression(fn->expression().lambdaBody());

                //TODO
//                 m_editor->argument1MinValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).lower())));
//                 m_editor->argument1MaxValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).upper())));

//                 m_editor->argument2MinValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(1).lower())));
//                 m_editor->argument2MaxValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(1).upper())));

                setSphericalSurfaceDomainInfo();
            }
        }
    }



    if (!fn->parameters().isEmpty())
    {
        if (arguments.size() == 1)
        {
            m_editor->argument1->setText(arguments.at(0));

            //TODO
//             m_editor->argument1MinValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).lower()), false));
//             m_editor->argument1MaxValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(0).upper()), false));
        }
        else if (arguments.size() == 2)
        {
            m_editor->argument2->setText(arguments.at(1));

            m_editor->argument2->show();
            m_editor->argument2MinValue->show();
            m_editor->argument2MaxValue->show();


            {
                //TODO
//                 m_editor->argument2MinValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(1).lower()), false));
//                 m_editor->argument2MaxValue->setExpression(Analitza::Expression(QString::number(fn->domain().at(1).upper()), false));
            }
        }
    }

    




    if (fn->spaceDimension() == 2)
    {
        m_editor->curveColor->setColor(fn->color());
        //NOTE gsoc2012 DEPRECATED
//         m_editor->curveLineWidth->setValue(fn->lineWidth());
    }
    else if (fn->spaceDimension() == 3)
    {
        if (arguments == QStringList() << QString("t"))
        {
            m_editor->curveColor->setColor(fn->color());
            //NOTE gsoc2012 DEPRECATED
//             m_editor->curveLineWidth->setValue(fn->lineWidth());
        }
        else
        {
            //NOTE gsoc2012 DEPRECATED
//             m_editor->surfaceResolutionValue->setValue(fn->resolution());
//             m_editor->surfaceDrawingType->setCurrentIndex((int)fn->drawingType());
            m_editor->surfaceColor->setColor(fn->color());
        }
    }
}

void FunctionEditor::showCartesianInfo()
{

    QString kflpath = KStandardDirs::locate("data", QString("gplacs/pics/info-cartesian-curves.png"));
    m_editor->infoFunctionPicture->setAlignment(Qt::AlignJustify);
    m_editor->infoFunctionPicture->setPixmap(QPixmap(kflpath, "PNG"));

}

void FunctionEditor::showParametricInfo()
{

    QString kflpath = KStandardDirs::locate("data", QString("gplacs/pics/info-parametric2d-curves.png"));
    m_editor->infoFunctionPicture->setAlignment(Qt::AlignJustify);
    m_editor->infoFunctionPicture->setPixmap(QPixmap(kflpath, "PNG"));

}

void FunctionEditor::showPolarInfo()
{


    QString kflpath = KStandardDirs::locate("data", QString("gplacs/pics/info-polar-curves.png"));
    m_editor->infoFunctionPicture->setAlignment(Qt::AlignJustify);
    m_editor->infoFunctionPicture->setPixmap(QPixmap(kflpath, "PNG"));

}


void FunctionEditor::showCartesianSurfaceInfo()
{
    QString kflpath = KStandardDirs::locate("data", QString("gplacs/pics/info-cartesian-surfaces.png"));
    m_editor->infoFunctionPicture->setAlignment(Qt::AlignJustify);
    m_editor->infoFunctionPicture->setPixmap(QPixmap(kflpath, "PNG"));
}

void FunctionEditor::showCylindricalSurfaceInfo()
{
    QString kflpath = KStandardDirs::locate("data", QString("gplacs/pics/info-cylindrical-surfaces.png"));
    m_editor->infoFunctionPicture->setAlignment(Qt::AlignJustify);
    m_editor->infoFunctionPicture->setPixmap(QPixmap(kflpath, "PNG"));

}

void FunctionEditor::showSphericalSurfaceInfo()
{
    QString kflpath = KStandardDirs::locate("data", QString("gplacs/pics/info-spherical-surfaces.png"));
    m_editor->infoFunctionPicture->setAlignment(Qt::AlignJustify);
    m_editor->infoFunctionPicture->setPixmap(QPixmap(kflpath, "PNG"));

}

void FunctionEditor::showParametricSurfaceInfo()
{
    QString kflpath = KStandardDirs::locate("data", QString("gplacs/pics/info-parametric-surfaces.png"));
    m_editor->infoFunctionPicture->setAlignment(Qt::AlignJustify);
    m_editor->infoFunctionPicture->setPixmap(QPixmap(kflpath, "PNG"));

}

void FunctionEditor::showParametric3DInfo()
{
    QString kflpath = KStandardDirs::locate("data", QString("gplacs/pics/info-parametric3d-curves.png"));
    m_editor->infoFunctionPicture->setAlignment(Qt::AlignJustify);
    m_editor->infoFunctionPicture->setPixmap(QPixmap(kflpath, "PNG"));

}



void FunctionEditor::clearFunctions2DInfo()
{
    m_editor->infoFunctionPicture->clear();
}

void FunctionEditor::setCartesianCurveDomainInfo(int index)
{
    initDomainInfo(m_editor->cartesianCurveType->itemData(index, Qt::UserRole).toStringList());
}

void FunctionEditor::setPolarCurveDomainInfo()
{
    m_editor->functionName->setFilterArguments(QStringList() << "q");
    initDomainInfo(QStringList() << "q");
}

void FunctionEditor::setParametricCurveDomainInfo()
{
    m_editor->functionName->setFilterArguments(QStringList() << "t");
    initDomainInfo(QStringList() << "t");
}

void FunctionEditor::setCartesianSurfaceDomainInfo()
{

}

void FunctionEditor::setCylindricalSurfaceDomainInfo()
{

}

void FunctionEditor::setSphericalSurfaceDomainInfo()
{

}

void FunctionEditor::setParametricSurfaceDomainInfo()
{

}

void FunctionEditor::setParametricCurve3DDomainInfo()
{

}


void FunctionEditor::varsContextMenu(const QPoint& p)
{
    QMenu m;
    m.addAction(i18n("Add variable"));

    QAction* ac=m.exec(m_editor->mapToGlobal(p));

    if(ac)
    {




    }
}


void FunctionEditor::saveFromQuick(const QString & newExp, int dimension,
                                   const QString &name, const QStringList &bvars)
{
    Analitza::Expression expr(newExp, false);

    if (!expr.isCorrect()) return ;

    if (saveFunction(expr, dimension, RealInterval::List(), true, name, bvars))
    {
        qDebug() << "QKICKKKKK " << newExp << dimension << name;


    }
    else
        qDebug() << "ERROR EN ADD QUICK ";

}

void FunctionEditor::completeFunctionInfoFromQuickName(const QString & newExp, int dimension,
        const QString &name, const QStringList &arguments)
{

    

    

    
    

    

    

    Analitza::Expression lambda(newExp, false);

    m_editor->functionName->setText(name);

    
    

    if (dimension == 2)
    {
        if (lambda.lambdaBody().isVector())
        {
            if (arguments == QStringList() << QString("t"))
            {
                initParametricCurve2D();

                m_editor->parametricCurve2DXExpression->setExpression(lambda.lambdaBody().elementAt(0));
                m_editor->parametricCurve2DYExpression->setExpression(lambda.lambdaBody().elementAt(1));

                setParametricCurveDomainInfo();
            }
        }
        else
        {
            if ((arguments == QStringList() << QString("x")) ||
                    (arguments == QStringList() << QString("y")) ||
                    (arguments == QStringList() << QString("x") << QString("y")))
            {
                initCartesianCurve();
                m_editor->cartesianCurveExpression->setExpression(lambda.lambdaBody());

                if (arguments == QStringList() << QString("x"))
                {
                    m_editor->cartesianCurveType->setCurrentIndex(0);
                }
                else if (arguments == QStringList() << QString("y"))
                {
                    m_editor->cartesianCurveType->setCurrentIndex(1);
                }
                else if (arguments == QStringList() << QString("x") << QString("y"))
                {
                    m_editor->cartesianCurveType->setCurrentIndex(2);
                }
            }
            else if (arguments == QStringList() << QString("q"))
            {
                initPolarCurve();
                m_editor->polarCurveExpression->setExpression(lambda.lambdaBody());

                setPolarCurveDomainInfo();









            }
        }
    }
    else if (dimension == 3)
    {
        if (lambda.lambdaBody().isVector())
        {
            if (arguments == QStringList() << QString("u") << QString("v"))
            {
                initParametricSurface();

                m_editor->parametricSurfaceXExpression->setExpression(lambda.lambdaBody().elementAt(0));
                m_editor->parametricSurfaceYExpression->setExpression(lambda.lambdaBody().elementAt(1));
                m_editor->parametricSurfaceZExpression->setExpression(lambda.lambdaBody().elementAt(2));

                setParametricSurfaceDomainInfo();
            }
            else if (arguments == QStringList() << QString("t"))
            {
                initParametricCurve3D();


                m_editor->parametricCurve3DXExpression->setExpression(lambda.lambdaBody().elementAt(0));
                m_editor->parametricCurve3DYExpression->setExpression(lambda.lambdaBody().elementAt(1));
                m_editor->parametricCurve3DZExpression->setExpression(lambda.lambdaBody().elementAt(2));

            }
        }
        else
        {
            if ((arguments == QStringList() << QString("x") << QString("y")))
            {
                initCartesianSurface();
                m_editor->cartesianSurfaceExpression->setExpression(lambda.lambdaBody());
                setCartesianSurfaceDomainInfo();
            }
            else if (arguments == QStringList() << QString("r") << QString("t"))
            {
                initCylindricalSurface();
                m_editor->cylindricalSurfaceExpression->setExpression(lambda.lambdaBody());

                setCylindricalSurfaceDomainInfo();
            }
            else if (arguments == QStringList() << QString("s") << QString("t"))
            {
                initSphericalSurface();
                m_editor->sphericalSurfaceExpression->setExpression(lambda.lambdaBody());

                setSphericalSurfaceDomainInfo();
            }
        }
    }















}


void FunctionEditor::initFunction()
{
    if (m_dimensionBehaviour == 2)
    {
        m_editor->appearanceCurve->show();
        m_editor->appearanceSurface->hide();
    }
    else if (m_dimensionBehaviour == 3)
    {
        
        if ( m_state == EditingParametricCurve3D)
        {
            m_editor->appearanceCurve->show();
            m_editor->appearanceSurface->hide();
        }
        else if( m_state == EditingImplicitSurface)
        {

        }
        else
        {
            m_editor->appearanceCurve->hide();
            m_editor->appearanceSurface->show();
        }
    }

    m_editor->domain->show();
    m_editor->functionInfo->show();
    m_editor->clickOk->show();
    m_editor->clickCancel->show();




    m_editor->quickNewFunctionByName->hide();
    m_editor->infoFunctionPicture->hide();

    m_editor->createCartesianCurve->hide();
    m_editor->createPolarCurve->hide();
    m_editor->createParametric2DCurve->hide();
    m_editor->createCartesianSurface->hide();
    m_editor->createCylindricalSurface->hide();
    m_editor->createSphericalSurface->hide();
    m_editor->createParametricSurface->hide();
    m_editor->createParametric3DCurve->hide();





    m_editor->infoFunctionPicture->hide();

    m_editor->infoFunctionPicture->setPixmap(QPixmap());
    m_editor->functions->show();


    if( m_state == EditingImplicitSurface)
    {

        m_editor->gbImplicitInfo->show();
        m_editor->functionInfo->hide();
        m_editor->gbImplicitParameters->hide();
        m_editor->domain->hide();
    }

}

bool FunctionEditor::createCartesianCurve()
{
    if (m_editor->cartesianCurveExpression->toPlainText().isEmpty())
    {
        m_errors << i18n("La ecuación está vacia");

        return false;
    }
    else
    {
        if (m_editor->cartesianCurveExpression->expression().isCorrect())
        {
            Analitza::Analyzer a;
            a.setExpression(m_editor->cartesianCurveExpression->expression());
            a.setExpression(a.dependenciesToLambda());

            QStringList arguments = a.expression().bvarList();
            QString expressionLiteral = m_editor->cartesianCurveExpression->expression().toString();


            qreal min, max;
            evalDomainExpression(m_editor->argument1MinValue, m_editor->argument1MaxValue,
                                 min, max);

            RealInterval::List domain;
            domain << RealInterval(min, max);

            switch (m_editor->cartesianCurveType->currentIndex())
            {
            case 0: 
            {
                if (arguments.size() > 1)
                {
                    m_errors << i18n("Se necesita un argumento: x");
                }

                if (arguments.size() == 1)
                {
                    if (!arguments.contains(QString("x")))
                    {
                        m_errors << i18n("El argumento debe ser x");
                    }
                }
                else if (arguments.isEmpty())
                    expressionLiteral += QString("+x*0"); 
            }
            break;

            case 1: 
            {
                if (arguments.size() > 1)
                {
                    m_errors << i18n("Se necesita un argumento: y");
                }

                if (arguments.size() == 1)
                {
                    if (!arguments.contains(QString("y")))
                    {
                        m_errors << i18n("El argumento debe ser y");
                    }
                }
                else if (arguments.isEmpty())
                    expressionLiteral += QString("+y*0"); 
            }
            break;

            case 2: 
            {
                if (arguments.size() != 2)
                {
                    m_errors << i18n("Se necesitan dos argumentos: x e y");
                }

                if (!arguments.contains("x") || !arguments.contains("y"))
                {
                    m_errors << i18n("Los argumentos deben ser x e y");
                }

                

                evalDomainExpression(m_editor->argument2MinValue, m_editor->argument2MaxValue,
                                     min, max);


                domain << RealInterval(min, max);
            }
            break;
            }

            if (m_errors.isEmpty())
            {
                Analitza::Expression expression(expressionLiteral, false);

                return saveFunction(expression, 2, domain);
            }
            else
                return false;
        }
        else
        {
            m_errors << i18n("La ecuación no es correcta");

            return false;
        }
    }
}

bool FunctionEditor::createPolarCurve()
{
    if (m_editor->polarCurveExpression->toPlainText().isEmpty())
    {
        m_errors << i18n("La ecuación está vacia");

        return false;
    }
    else
    {
        if (m_editor->polarCurveExpression->expression().isCorrect())
        {
            Analitza::Analyzer a;
            a.setExpression(m_editor->polarCurveExpression->expression());
            a.setExpression(a.dependenciesToLambda());

            QStringList arguments = a.expression().bvarList();
            QString expressionLiteral = m_editor->polarCurveExpression->expression().toString();

            qreal min, max;
            evalDomainExpression(m_editor->argument1MinValue, m_editor->argument1MaxValue,
                                 min, max);

            RealInterval::List domain;
            domain << RealInterval(min, max);



            {
                if (arguments.size() > 1)
                {
                    m_errors << i18n("Se necesita un argumento: q (theta)");
                }

                if (arguments.size() == 1)
                {
                    if (!arguments.contains(QString("q")))
                    {
                        m_errors << i18n("El argumento debe ser q (theta)");
                    }
                }
                else if (arguments.isEmpty())
                    expressionLiteral += QString("+q*0"); 
            }


            if (m_errors.isEmpty())
            {
                Analitza::Expression expression(expressionLiteral, false);

                return saveFunction(expression, 2, domain);
            }
            else
                return false;
        }
        else
        {
            m_errors << i18n("La ecuación no es correcta");

            return false;
        }
    }
}

bool FunctionEditor::createParametricCurve2D()
{
    if (m_editor->parametricCurve2DXExpression->toPlainText().isEmpty() ||
            m_editor->parametricCurve2DYExpression->toPlainText().isEmpty() )
    {
        m_errors << i18n("La ecuación está vacia");

        return false;
    }
    else
    {
        if ((m_editor->parametricCurve2DXExpression->expression().isCorrect()) &&
                (m_editor->parametricCurve2DYExpression->expression().isCorrect()))
        {

            Analitza::Analyzer a;
            a.setExpression(m_editor->parametricCurve2DXExpression->expression());
            a.setExpression(a.dependenciesToLambda());

            QStringList argumentsComponentX = a.expression().bvarList();
            QString expressionLiteralComponentX = m_editor->parametricCurve2DXExpression->expression().toString();

            a.setExpression(m_editor->parametricCurve2DYExpression->expression());
            a.setExpression(a.dependenciesToLambda());

            QStringList argumentsComponentY = a.expression().bvarList();
            QString expressionLiteralComponentY = m_editor->parametricCurve2DYExpression->expression().toString();

            qreal min, max;
            evalDomainExpression(m_editor->argument1MinValue, m_editor->argument1MaxValue,
                                 min, max);

            RealInterval::List domain;
            domain << RealInterval(min, max);

            if ((argumentsComponentX.size() > 1) || (argumentsComponentY.size() > 1) )
            {
                m_errors << i18n("Se necesita un argumento: t");
            }

            if ((argumentsComponentX.size() == 1) && (argumentsComponentY.size() == 1))
            {
                if (!argumentsComponentX.contains(QString("t")) || !argumentsComponentY.contains(QString("t")))
                {
                    m_errors << i18n("El argumento debe ser t");
                }
            }

            if (argumentsComponentX.isEmpty())
                expressionLiteralComponentX += QString("+t*0"); 

            if (argumentsComponentY.isEmpty())
                expressionLiteralComponentY += QString("+t*0"); 

            QString expressionLiteral = "vector{ " + expressionLiteralComponentX + ", " +
                                        expressionLiteralComponentY + " }";

            if (m_errors.isEmpty())
            {
                Analitza::Expression expression(expressionLiteral, false);

                return saveFunction(expression, 2, domain);
            }
            else
                return false;
        }
        else
        {
            m_errors << i18n("La ecuación no es correcta");

            return false;
        }
    }
}
















static const uchar fncspal[][4] =
{
    { 160, 160, 160 }, 
    { 128, 128, 128 }, 
    { 0, 0, 0 }, 
    { 255, 128, 128 }, 
    { 255, 192, 128 }, 
    { 255, 255, 128 }, 
    { 128, 255, 128 }, 
    { 128, 255, 255 }, 
    { 128, 128, 255 }, 
    { 255, 128, 255 }, 
    { 255, 0, 0 }, 
    { 255, 128, 0 }, 
    { 255, 255, 0 }, 
    { 0, 255, 0 }, 
    { 0, 255, 255 }, 
    { 0, 0, 255 }, 
    { 255, 0, 255 }, 
    { 128, 0, 0 }, 
    { 128, 64, 0 }, 
    { 128, 128, 0 }, 
    { 0, 128, 0 }, 
    { 0, 128, 128 }, 
    { 0, 0, 128 }, 
    { 128, 0, 128 } 
};

#define FNCS_PALETTE_SIZE (int(sizeof(fncspal) / sizeof(*fncspal)))

void FunctionEditor::clearCartesianCurve()
{
    m_editor->cartesianCurveExpression->setExpression(Analitza::Expression());

    clearAppearanceAndDomainSettings();
}

void FunctionEditor::clearPolarCurve()
{
    m_editor->polarCurveExpression->setExpression(Analitza::Expression());

    clearAppearanceAndDomainSettings();
}

void FunctionEditor::clearParametricCurve2D()
{
    m_editor->parametricCurve2DXExpression->setExpression(Analitza::Expression());
    m_editor->parametricCurve2DYExpression->setExpression(Analitza::Expression());

    clearAppearanceAndDomainSettings();
}








bool FunctionEditor::createCartesianSurface()
{
    if (m_editor->cartesianSurfaceExpression->toPlainText().isEmpty())
    {
        m_errors << i18n("La ecuación está vacia");

        return false;
    }
    else
    {
        if (m_editor->cartesianSurfaceExpression->expression().isCorrect())
        {
            Analitza::Analyzer a;
            a.setExpression(m_editor->cartesianSurfaceExpression->expression());
            a.setExpression(a.dependenciesToLambda());

            QStringList arguments = a.expression().bvarList();
            QString expressionLiteral = m_editor->cartesianSurfaceExpression->expression().toString();

            qreal min1, max1;
            evalDomainExpression(m_editor->argument1MinValue, m_editor->argument1MaxValue,
                                 min1, max1);

            qreal min2, max2;
            evalDomainExpression(m_editor->argument2MinValue, m_editor->argument2MaxValue,
                                 min2, max2);

            RealInterval::List domain;
            domain << RealInterval(min1, max1) << RealInterval(min2, max2);

            switch (arguments.size())
            {
            case 0:
            {
                expressionLiteral += QString("+x*y*0");
            }
            break;

            case 1:
            {
                if (arguments.contains("x") || arguments.contains("y"))
                {
                    if (arguments.contains("x"))
                        expressionLiteral += QString("+x*y*0");

                    if (arguments.contains("y"))
                        expressionLiteral += QString("+x*y*0");
                }
                else
                    m_errors << i18n("The arguments must be x or y");
            }
            break;

            case 2:
            {
                if (!(arguments.contains("x") && arguments.contains("y")))
                    m_errors << i18n("The arguments must be x or y");
            }
            break;

            default:
            {
                m_errors << i18n("Wrong number of arguments for z=f(x,y)");

                return false;
            }
            break;
            }

            if (m_errors.isEmpty())
            {
                Analitza::Expression expression(expressionLiteral, false);

                return saveFunction(expression, 3, domain);
            }
            else
                return false;
        }
        else
        {
            m_errors << i18n("La ecuación no es correcta");

            return false;
        }
    }
}

bool FunctionEditor::createCylindricalSurface()
{
    if (m_editor->cylindricalSurfaceExpression->toPlainText().isEmpty())
    {
        m_errors << i18n("La ecuación está vacia");

        return false;
    }
    else
    {
        if (m_editor->cylindricalSurfaceExpression->expression().isCorrect())
        {
            Analitza::Analyzer a;
            a.setExpression(m_editor->cylindricalSurfaceExpression->expression());
            a.setExpression(a.dependenciesToLambda());

            QStringList arguments = a.expression().bvarList();
            QString expressionLiteral = m_editor->cylindricalSurfaceExpression->expression().toString();

            qreal min1, max1;
            evalDomainExpression(m_editor->argument1MinValue, m_editor->argument1MaxValue,
                                 min1, max1);

            qreal min2, max2;
            evalDomainExpression(m_editor->argument2MinValue, m_editor->argument2MaxValue,
                                 min2, max2);

            RealInterval::List domain;
            domain << RealInterval(min1, max1) << RealInterval(min2, max2);

            switch (arguments.size())
            {
            case 0:
            {
                expressionLiteral += QString("+r*t*0");
            }
            break;

            case 1:
            {
                if (arguments.contains("r") || arguments.contains("t"))
                {
                    if (arguments.contains("r"))
                        expressionLiteral += QString("+r*t*0");

                    if (arguments.contains("t"))
                        expressionLiteral += QString("+r*t*0");
                }
                else
                    m_errors << i18n("The arguments must be r or t");
            }
            break;

            case 2:
            {
                if (!(arguments.contains("r") && arguments.contains("t")))
                    m_errors << i18n("The arguments must be r or t");
            }
            break;

            default:
            {
                m_errors << i18n("Wrong number of arguments for z=f(r,t)");

                return false;
            }
            break;
            }

            if (m_errors.isEmpty())
            {
                Analitza::Expression expression(expressionLiteral, false);

                return saveFunction(expression, 3, domain);
            }
            else
                return false;
        }
        else
        {
            m_errors << i18n("La ecuación no es correcta");

            return false;
        }
    }
}

bool FunctionEditor::createSphericalSurface()
{
    if (m_editor->sphericalSurfaceExpression->toPlainText().isEmpty())
    {
        m_errors << i18n("La ecuación está vacia");

        return false;
    }
    else
    {
        if (m_editor->sphericalSurfaceExpression->expression().isCorrect())
        {
            Analitza::Analyzer a;
            a.setExpression(m_editor->sphericalSurfaceExpression->expression());
            a.setExpression(a.dependenciesToLambda());

            QStringList arguments = a.expression().bvarList();
            QString expressionLiteral = m_editor->sphericalSurfaceExpression->expression().toString();

            qreal min1, max1;
            evalDomainExpression(m_editor->argument1MinValue, m_editor->argument1MaxValue,
                                 min1, max1);

            qreal min2, max2;
            evalDomainExpression(m_editor->argument2MinValue, m_editor->argument2MaxValue,
                                 min2, max2);

            RealInterval::List domain;
            domain << RealInterval(min1, max1) << RealInterval(min2, max2);

            switch (arguments.size())
            {
            case 0:
            {
                expressionLiteral += QString("+s*t*0");
            }
            break;

            case 1:
            {
                if (arguments.contains("s") || arguments.contains("t"))
                {
                    if (arguments.contains("s"))
                        expressionLiteral += QString("+s*t*0");

                    if (arguments.contains("t"))
                        expressionLiteral += QString("+s*t*0");
                }
                else
                    m_errors << i18n("The arguments must be s or t");
            }
            break;

            case 2:
            {
                if (!(arguments.contains("s") && arguments.contains("t")))
                    m_errors << i18n("The arguments must be s or t");
            }
            break;

            default:
            {
                m_errors << i18n("Wrong number of arguments for r=f(s,t)");

                return false;
            }
            break;
            }

            if (m_errors.isEmpty())
            {
                Analitza::Expression expression(expressionLiteral, false);

                return saveFunction(expression, 3, domain);
            }
            else
                return false;
        }
        else
        {
            m_errors << i18n("La ecuación no es correcta");

            return false;
        }
    }
}

bool FunctionEditor::createParametricSurface()
{





























    if (m_editor->parametricSurfaceXExpression->toPlainText().isEmpty() ||
            m_editor->parametricSurfaceYExpression->toPlainText().isEmpty() ||
            m_editor->parametricSurfaceZExpression->toPlainText().isEmpty())
    {
        m_errors << i18n("La ecuación está vacia");

        return false;
    }
    else
    {
        if ((m_editor->parametricSurfaceXExpression->expression().isCorrect()) &&
                (m_editor->parametricSurfaceYExpression->expression().isCorrect()) &&
                (m_editor->parametricSurfaceZExpression->expression().isCorrect()))
        {
            Analitza::Analyzer a;
            a.setExpression(m_editor->parametricSurfaceXExpression->expression());
            a.setExpression(a.dependenciesToLambda());

            QStringList argumentsComponentX = a.expression().bvarList();
            QString expressionLiteralComponentX = m_editor->parametricSurfaceXExpression->expression().toString();

            a.setExpression(m_editor->parametricSurfaceYExpression->expression());
            a.setExpression(a.dependenciesToLambda());

            QStringList argumentsComponentY = a.expression().bvarList();
            QString expressionLiteralComponentY = m_editor->parametricSurfaceYExpression->expression().toString();

            a.setExpression(m_editor->parametricSurfaceZExpression->expression());
            a.setExpression(a.dependenciesToLambda());

            QStringList argumentsComponentZ = a.expression().bvarList();
            QString expressionLiteralComponentZ = m_editor->parametricSurfaceZExpression->expression().toString();

            qreal min1, max1;
            evalDomainExpression(m_editor->argument1MinValue, m_editor->argument1MaxValue,
                                 min1, max1);

            qreal min2, max2;
            evalDomainExpression(m_editor->argument2MinValue, m_editor->argument2MaxValue,
                                 min2, max2);

            RealInterval::List domain;
            domain << RealInterval(min1, max1) << RealInterval(min2, max2);


            if ((argumentsComponentX.size() > 2) || (argumentsComponentY.size() > 2) || (argumentsComponentZ.size() > 2))
            {
                m_errors << i18n("Wrong number of arguments for f(u,v)");
            }

            if ((argumentsComponentX.size() == 2) && (argumentsComponentY.size() == 2) && (argumentsComponentZ.size() == 2) )
            {
                if (!argumentsComponentX.contains(QString("u")) || !argumentsComponentX.contains(QString("v")) ||
                        !argumentsComponentY.contains(QString("u")) || !argumentsComponentY.contains(QString("v")) ||
                        !argumentsComponentZ.contains(QString("u")) || !argumentsComponentZ.contains(QString("v")) )
                {
                    m_errors << i18n("The arguments are u,v");
                }
            }

            
            if ((argumentsComponentX.size() == 1) && (argumentsComponentY.size() == 1) && (argumentsComponentZ.size() == 1) )
            {
                if ((argumentsComponentX == argumentsComponentY) && (argumentsComponentY == argumentsComponentZ))
                {
                    m_errors << i18n("Min to be a surf is 2 arguments");
                }
                else if (!(argumentsComponentX.contains(QString("u")) || argumentsComponentX.contains(QString("v")) &&
                           argumentsComponentY.contains(QString("u")) || argumentsComponentY.contains(QString("v")) &&
                           argumentsComponentZ.contains(QString("u")) || argumentsComponentZ.contains(QString("v"))))
                {
                    m_errors << i18n("The arguments are u,v");
                }
            }

            if (argumentsComponentX.isEmpty())
                expressionLiteralComponentX += QString("+u*v*0"); 

            if (argumentsComponentY.isEmpty())
                expressionLiteralComponentY += QString("+u*v*0"); 

            if (argumentsComponentZ.isEmpty())
                expressionLiteralComponentY += QString("+u*v*0"); 

            QString expressionLiteral = "vector{ " + expressionLiteralComponentX + ", " +
                                        expressionLiteralComponentY + ", " + expressionLiteralComponentZ + " }";

            if (m_errors.isEmpty())
            {
                Analitza::Expression expression(expressionLiteral, false);

                return saveFunction(expression, 3, domain);
            }
            else
                return false;
        }
        else
        {
            m_errors << i18n("La ecuación no es correcta");

            return false;
        }
    }
}

bool FunctionEditor::createParametricCurve3D()
{




    if (m_editor->parametricCurve3DXExpression->toPlainText().isEmpty() ||
            m_editor->parametricCurve3DYExpression->toPlainText().isEmpty() ||
            m_editor->parametricCurve3DZExpression->toPlainText().isEmpty() )
    {
        m_errors << i18n("La ecuación está vacia");

        return false;
    }
    else
    {
        if ((m_editor->parametricCurve3DXExpression->expression().isCorrect()) &&
                (m_editor->parametricCurve3DYExpression->expression().isCorrect()) &&
                (m_editor->parametricCurve3DZExpression->expression().isCorrect()))
        {

            Analitza::Analyzer a;
            a.setExpression(m_editor->parametricCurve3DXExpression->expression());
            a.setExpression(a.dependenciesToLambda());

            QStringList argumentsComponentX = a.expression().bvarList();
            QString expressionLiteralComponentX = m_editor->parametricCurve3DXExpression->expression().toString();

            a.setExpression(m_editor->parametricCurve3DYExpression->expression());
            a.setExpression(a.dependenciesToLambda());

            QStringList argumentsComponentY = a.expression().bvarList();
            QString expressionLiteralComponentY = m_editor->parametricCurve3DYExpression->expression().toString();


            a.setExpression(m_editor->parametricCurve3DZExpression->expression());
            a.setExpression(a.dependenciesToLambda());

            QStringList argumentsComponentZ = a.expression().bvarList();
            QString expressionLiteralComponentZ = m_editor->parametricCurve3DZExpression->expression().toString();


            qreal min, max;
            evalDomainExpression(m_editor->argument1MinValue, m_editor->argument1MaxValue,
                                 min, max);

            RealInterval::List domain;
            domain << RealInterval(min, max);

            if ((argumentsComponentX.size() > 1) || (argumentsComponentY.size() > 1) || (argumentsComponentZ.size() > 1))
            {
                m_errors << i18n("Se necesita un argumento: t");
            }

            if ((argumentsComponentX.size() == 1) && (argumentsComponentY.size() == 1) && (argumentsComponentZ.size() == 1))
            {
                if (!argumentsComponentX.contains(QString("t")) || !argumentsComponentY.contains(QString("t")) ||
                        !argumentsComponentZ.contains(QString("t")))
                {
                    m_errors << i18n("El argumento debe ser t");
                }
            }

            if (argumentsComponentX.isEmpty())
                expressionLiteralComponentX += QString("+t*0"); 

            if (argumentsComponentY.isEmpty())
                expressionLiteralComponentY += QString("+t*0"); 

            if (argumentsComponentZ.isEmpty())
                expressionLiteralComponentZ += QString("+t*0"); 


            QString expressionLiteral = "vector{ " + expressionLiteralComponentX + ", " +
                                        expressionLiteralComponentY + ","+ expressionLiteralComponentZ  + "}";

            if (m_errors.isEmpty())
            {
                Analitza::Expression expression(expressionLiteral, false);

                return saveFunction(expression, 3, domain);
            }
            else
                return false;
        }
        else
        {
            m_errors << i18n("La ecuación no es correcta");

            return false;
        }
    }













































}

bool FunctionEditor::areImplicitParametersOK(int index)
{

    bool ans = true;
    switch(index)
    {
    case 0:
        if(m_editor->txtedtParameter1->text()=="" ||
                m_editor->txtedtParameter2->text()=="" ||
                m_editor->txtedtParameter3->text()=="" ||
                m_editor->txtedtParameter4->text()=="")
            return false;
        break;
    case 1:
        if(m_editor->txtedtParameter1->text()=="" ||
                m_editor->txtedtParameter2->text()=="" ||
                m_editor->txtedtParameter3->text()=="" ||
                m_editor->txtedtParameter4->text()=="" ||
                m_editor->txtedtParameter5->text()=="" ||
                m_editor->txtedtParameter6->text()=="")
            return false;
        break;
    case 2:
        if(m_editor->txtedtParameter1->text()=="" ||
                m_editor->txtedtParameter2->text()=="" ||
                m_editor->txtedtParameter3->text()=="" ||
                m_editor->txtedtParameter4->text()=="" ||
                m_editor->txtedtParameter5->text()=="")
            return false;
        break;
    case 3:
        if(m_editor->txtedtParameter1->text()=="" ||
                m_editor->txtedtParameter2->text()=="" ||
                m_editor->txtedtParameter3->text()=="" ||
                m_editor->txtedtParameter4->text()=="" ||
                m_editor->txtedtParameter5->text()=="" ||
                m_editor->txtedtParameter6->text()=="")
            return false;
        break;
    case 4:
        if(m_editor->txtedtParameter1->text()=="" ||
                m_editor->txtedtParameter2->text()=="" ||
                m_editor->txtedtParameter3->text()=="" ||
                m_editor->txtedtParameter4->text()=="" ||
                m_editor->txtedtParameter5->text()=="" ||
                m_editor->txtedtParameter6->text()=="")
            return false;
        break;
    case 5:
        if(m_editor->txtedtParameter1->text()=="" ||
                m_editor->txtedtParameter2->text()=="" ||
                m_editor->txtedtParameter3->text()=="" ||
                m_editor->txtedtParameter4->text()=="" ||
                m_editor->txtedtParameter5->text()=="")
            return false;
        break;
    case 6:
        if(m_editor->txtedtParameter1->text()=="" ||
                m_editor->txtedtParameter2->text()=="" ||
                m_editor->txtedtParameter3->text()=="" ||
                m_editor->txtedtParameter4->text()=="" ||
                m_editor->txtedtParameter5->text()=="" ||
                m_editor->txtedtParameter6->text()=="")
            return false;
        break;
    case 7:
        if(m_editor->txtedtParameter1->text()=="" ||
                m_editor->txtedtParameter2->text()=="" ||
                m_editor->txtedtParameter3->text()=="" ||
                m_editor->txtedtParameter4->text()=="" ||
                m_editor->txtedtParameter5->text()=="" ||
                m_editor->txtedtParameter6->text()=="")
            return false;
        break;
    }
    return ans;
}
/*
bool FunctionEditor::createImplicitSurface(int index)
{
    
    QString nombre;
    QList<double> p;
    switch(index)
    {
    case 0:
        p.append(m_editor->txtedtParameter1->text().toDouble());
        p.append(m_editor->txtedtParameter2->text().toDouble());
        p.append(m_editor->txtedtParameter3->text().toDouble());
        p.append(m_editor->txtedtParameter4->text().toDouble());
        nombre = "Esfera";
        break;
    case 1:
        p.append(m_editor->txtedtParameter1->text().toDouble());
        p.append(m_editor->txtedtParameter2->text().toDouble());
        p.append(m_editor->txtedtParameter3->text().toDouble());
        p.append(m_editor->txtedtParameter4->text().toDouble());
        p.append(m_editor->txtedtParameter5->text().toDouble());
        p.append(m_editor->txtedtParameter6->text().toDouble());
        nombre = "Elipsoide";
        break;
    case 2:
        p.append(m_editor->txtedtParameter1->text().toDouble());
        p.append(m_editor->txtedtParameter2->text().toDouble());
        p.append(m_editor->txtedtParameter3->text().toDouble());
        p.append(m_editor->txtedtParameter4->text().toDouble());
        p.append(m_editor->txtedtParameter5->text().toDouble());
        nombre = "Cilindro";
        break;
    case 3:
        p.append(m_editor->txtedtParameter1->text().toDouble());
        p.append(m_editor->txtedtParameter2->text().toDouble());
        p.append(m_editor->txtedtParameter3->text().toDouble());
        p.append(m_editor->txtedtParameter4->text().toDouble());
        p.append(m_editor->txtedtParameter5->text().toDouble());
        p.append(m_editor->txtedtParameter6->text().toDouble());
        nombre = "Hiperboloide una hoja";
        break;
    case 4:
        p.append(m_editor->txtedtParameter1->text().toDouble());
        p.append(m_editor->txtedtParameter2->text().toDouble());
        p.append(m_editor->txtedtParameter3->text().toDouble());
        p.append(m_editor->txtedtParameter4->text().toDouble());
        p.append(m_editor->txtedtParameter5->text().toDouble());
        p.append(m_editor->txtedtParameter6->text().toDouble());
        nombre = "Hiperboloide dos hojas";
        break;
    case 5:
        p.append(m_editor->txtedtParameter1->text().toDouble());
        p.append(m_editor->txtedtParameter2->text().toDouble());
        p.append(m_editor->txtedtParameter3->text().toDouble());
        p.append(m_editor->txtedtParameter4->text().toDouble());
        p.append(m_editor->txtedtParameter5->text().toDouble());
        p.append(m_editor->txtedtParameter6->text().toDouble());
        nombre = "Paraboloide hiperbolico";
        break;
    case 6:
        p.append(m_editor->txtedtParameter1->text().toDouble());
        p.append(m_editor->txtedtParameter2->text().toDouble());
        p.append(m_editor->txtedtParameter3->text().toDouble());
        p.append(m_editor->txtedtParameter4->text().toDouble());
        p.append(m_editor->txtedtParameter5->text().toDouble());
        p.append(m_editor->txtedtParameter6->text().toDouble());
        nombre = "Paraboloide eliptico";
        break;
    case 7:
        p.append(m_editor->txtedtParameter1->text().toDouble());
        p.append(m_editor->txtedtParameter2->text().toDouble());
        p.append(m_editor->txtedtParameter3->text().toDouble());
        p.append(m_editor->txtedtParameter4->text().toDouble());
        p.append(m_editor->txtedtParameter5->text().toDouble());
        p.append(m_editor->txtedtParameter6->text().toDouble());
        nombre = "Cono eliptico";
        break;
    }

    implicitfunctionsId+=1;
    int num=implicitfunctionsId;
    QUuid uid(0,0,0,0,0,0,0,(num&0xFF000000)>>24,(num&0xFF0000)>>16,(num&0xFF00)>>8,(num&0xFF));
    RealInterval::List domfun;
    domfun << RealInterval(-2.0*M_PI,2.0*M_PI);
    QColor col = QColor(rand()%255+1,rand()%255+1,rand()%255+1);
    Analitza::Expression expr("x*y*0", false);
    FunctionGraph f(expr,3,domfun,m_vars,nombre,col);
    
    f.setColor(col);
//     f.setSpaceId(m_functionsFilterProxyModel->filterSpaceId());
    //NOTE gsoc2012 DEPRECATED
//     f.setResolution(m_editor->surfaceResolutionValue->value());
//     f.setDrawingType(Function::Solid);
    FunctionsModel *functionModel = static_cast<FunctionsModel*>(m_functionsFilterProxyModel->sourceModel());
//     functionModel->addFunction(f,currentImplicitIndex,p,m_editor->octantecombobox->currentIndex(),m_editor->ejecombobox->currentIndex(),
//                                m_editor->chkboxSolido->isChecked(),m_editor->chkboxCurvas->isChecked(),m_editor->rbtnXY->isChecked(),0.2);

    functionModel->addItem(f.expression(), f.spaceDimension(), f.name(), f.color());
    functionModel->setSpaceId(functionModel->rowCount()-1, m_functionsFilterProxyModel->filterSpaceId());
    
    
    
    
    
    
    
    
    

    return true;
}
*/

void FunctionEditor::clearCartesianSurface()
{
    m_editor->cartesianSurfaceExpression->setExpression(Analitza::Expression());

    clearAppearanceAndDomainSettings();
}

void FunctionEditor::clearCylindricalSurface()
{
    m_editor->cylindricalSurfaceExpression->setExpression(Analitza::Expression());

    clearAppearanceAndDomainSettings();
}

void FunctionEditor::clearSphericalSurface()
{
    m_editor->sphericalSurfaceExpression->setExpression(Analitza::Expression());

    clearAppearanceAndDomainSettings();
}

void FunctionEditor::clearParametricSurface()
{
    m_editor->parametricSurfaceXExpression->setExpression(Analitza::Expression());
    m_editor->parametricSurfaceYExpression->setExpression(Analitza::Expression());
    m_editor->parametricSurfaceZExpression->setExpression(Analitza::Expression());

    clearAppearanceAndDomainSettings();
}

void FunctionEditor::clearParametricCurve3D()
{
    m_editor->parametricCurve3DXExpression->setExpression(Analitza::Expression());
    m_editor->parametricCurve3DYExpression->setExpression(Analitza::Expression());
    m_editor->parametricCurve3DZExpression->setExpression(Analitza::Expression());

    clearAppearanceAndDomainSettings();
}

void FunctionEditor::clearImplicitSurface()
{

}

bool FunctionEditor::saveFunction(const Analitza::Expression& newExp, int dimension,
                                  const RealInterval::List &domain, bool useDefaults, const QString &quickname, const QStringList &bvars) 
{
    
    if (quickname=="implicit")
    {
    }

    QColor col;

    if (useDefaults)
    {
        int ra = KRandom::random() % FNCS_PALETTE_SIZE;

        col = QColor(fncspal[ra][0], fncspal[ra][1], fncspal[ra][2]);
    }
    else
    {
        col = m_editor->curveColor->color();

        if (dimension == 3)
        {

            if (m_state == EditingParametricCurve3D)
            {

                col = m_editor->curveColor->color();

            }
            else
                col = m_editor->surfaceColor->color();

        }
    }

    QString namefun;
    RealInterval::List domfun;

    if (useDefaults)
    {
        namefun = quickname;

        if (dimension == 2)
        {
            domfun << RealInterval(-2.0*M_PI,2.0*M_PI);
            if (bvars.size() == 2)
                domfun << RealInterval(-2.0*M_PI,2.0*M_PI);
        }
        else if (dimension == 3)
        {
            domfun << RealInterval(-2.0*M_PI,2.0*M_PI);

            if (bvars.size() == 2)
                domfun << RealInterval(-2.0*M_PI,2.0*M_PI);
        }
    }
    else
    {
        namefun = m_editor->functionName->text();
        domfun = domain;
    }

    FunctionGraph f(newExp, dimension/*, domfun, m_vars*/, namefun, col);
    
    

    if(dimension == 3)
    {
        f.setName(newExp.toString());

    }
    
    {
        if (dimension == 2)
        {
            //NOTE gsoc2012 DEPRECATED
//             f.setLineWidth(m_editor->curveLineWidth->value());
            

//             f.setResolution(m_editor->curveResolutionValue->value());
        }
        else if (dimension == 3)
        {

            if (m_state == EditingParametricCurve3D)
            {
                //NOTE gsoc2012 DEPRECATED
//                 f.setLineWidth(m_editor->curveLineWidth->value());
                

//                 f.setResolution(m_editor->curveResolutionValue->value());
            }
            else
            {
                PlotItem::PlotStyle dt;

                switch (m_editor->surfaceDrawingType->currentIndex())
                {
                case 0:
                    dt = PlotItem::Solid;
                    break;
                case 1:
                    dt = PlotItem::Wired;
                    break;
                case 2:
                    dt = PlotItem::Dots;
                    break;
                }

                //NOTE gsoc2012 DEPRECATED
//                 f.setResolution(m_editor->surfaceResolutionValue->value());
//                 f.setDrawingType(dt);

//                 if (useDefaults)
                    //NOTE gsoc2012 DEPRECATED
//                     f.setDrawingType(Function::Solid);


                
            }
        }
    }

    FunctionsModel *functionModel = static_cast<FunctionsModel*>(m_functionsFilterProxyModel->sourceModel());


    if (f.isCorrect() && (m_errors.isEmpty()))
    {

        RealInterval::List spaceBounds;
        spaceBounds<< RealInterval(-6,6) << RealInterval(-6,6);
        QRect viewport__;
        viewport__.setLeft(spaceBounds[0].lower());
        viewport__.setRight(spaceBounds[0].upper());
        viewport__.setBottom(spaceBounds[1].lower());
        viewport__.setTop(spaceBounds[1].upper());
        
        
        //TODO
        //f.update(viewport__);

        if (!f.errors().isEmpty())
        {
            m_errors << f.errors();
            return false;

        }
        

        //TODO
//         if (m_isEditing)
//             functionModel->editFunction(m_currentEditFunctionId, f);
//         else
//             functionModel->addFunction(f);

//         functionModel->setSpaceId(m_functionsFilterProxyModel->filterSpaceId());
        
        return true;
    }
    else
    {
        return false;
    }
}

void FunctionEditor::initDomainInfo(const QStringList &arguments, const RealInterval::List &functionDomain)
{

    if (arguments.isEmpty())
        return ; 

    m_editor->functionName->setFilterArguments(arguments);

    m_editor->argument1->setText(arguments.at(0));


    if (!functionDomain.isEmpty() && (arguments.size() == 1))

    {
        m_editor->argument1->setText(arguments.at(0));

        m_editor->argument1MinValue->setExpression(Analitza::Expression(QString::number(functionDomain.at(0).lower()), false));
        m_editor->argument1MaxValue->setExpression(Analitza::Expression(QString::number(functionDomain.at(0).upper()), false));
    }

    if (arguments.size() == 2)
    {
        m_editor->argument2->setText(arguments.at(1));

        m_editor->argument2->show();
        m_editor->argument2MinValue->show();
        m_editor->argument2MaxValue->show();

        if (!functionDomain.isEmpty() && (arguments.size() != functionDomain.size()))
        {
            m_editor->argument2MinValue->setExpression(Analitza::Expression(QString::number(functionDomain.at(1).lower()), false));
            m_editor->argument2MaxValue->setExpression(Analitza::Expression(QString::number(functionDomain.at(1).upper()), false));
        }
    }
    else
    {
        m_editor->argument2->hide();
        m_editor->argument2MinValue->hide();
        m_editor->argument2MaxValue->hide();
    }
}

void FunctionEditor::clearAppearanceAndDomainSettings() 
{





    int ra = KRandom::random() % FNCS_PALETTE_SIZE;
    m_editor->curveColor->setColor(QColor(fncspal[ra][0], fncspal[ra][1], fncspal[ra][2]));

    ra = KRandom::random() % FNCS_PALETTE_SIZE;
    m_editor->surfaceColor->setColor(QColor(fncspal[ra][0], fncspal[ra][1], fncspal[ra][2]));

    m_editor->curveLineWidth->setValue(0.4);

    

    m_editor->functionName->clear();
}

void FunctionEditor::showErrorMessage()
{
    if (m_errors.isEmpty()) return;
    
    QFont errorFont = m_editor->errorMessage->font();
    errorFont.setBold(true);

    m_editor->errorMessage->setFont(errorFont);

    m_editor->errorMessage->setText(m_errors.first()); 

    KColorScheme scheme(QPalette::Normal);

    QPalette p = m_editor->errorMessage->palette();
    p.setColor(foregroundRole(), scheme.foreground(KColorScheme::NegativeText).color());

    m_editor->errorMessage->setPalette(p);

    m_editor->errorIcon->show();
    m_editor->errorMessage->show();
}

bool FunctionEditor::evalDomainExpression(ExpressionEdit *minDomainEdit, ExpressionEdit *maxDomainEdit, qreal &refmin, qreal &refmax)
{
    if (!minDomainEdit->expression().isCorrect() ||
            !maxDomainEdit->expression().isCorrect())
    {
        m_errors << "Verificar el dominio";

        return false;
    }

    Analitza::Analyzer a;
    a.setExpression(minDomainEdit->expression());


    Analitza::Cn ret = a.calculate().toReal();


    if (!a.isCorrect())
    {
        m_errors << "Error al evaluar el dominio" << a.expression().toString();

        return false;
    }

    qreal min = ret.value();

    a.setExpression(maxDomainEdit->expression());

    Analitza::Cn ret1 = a.calculate().toReal();


    if (!a.isCorrect())
    {
        m_errors << "Error al evaluar el dominio" << a.expression().toString();

        return false;
    }

    qreal max = ret1.value();

    if (min > max)
    {
        m_errors << i18n("Incongruencia en el dominio");

        return false;
    }

    refmin = min;
    refmax = max;

    return true;
}













} 
