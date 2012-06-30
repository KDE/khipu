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


#ifndef FUNCTIONEDITOR_H
#define FUNCTIONEDITOR_H

#include <QWidget>
#include <QAbstractItemDelegate>
#include "space/space.h"


#include "mathhelper.h"

namespace Analitza
{
class Variables;
}


class ExpressionEdit;

namespace Keomath
{

class FunctionEditorWidget;
class FunctionsFilterProxyModel;
class FunctionsView;

class FunctionEditor : public QWidget
{
    Q_OBJECT

public:
    enum State
    {
        
        EditingCartesianCurve = 0,
        EditingPolarCurve,
        EditingParametricCurve2D,
        
        EditingCartesianSurface,
        EditingCylindricalSurface,
        EditingSphericalSurface,
        EditingParametricSurface,
        EditingParametricCurve3D,
        EditingImplicitSurface,
    };

public:
    FunctionEditor(QWidget *parent);
    virtual ~ FunctionEditor();

    FunctionsView *functionsView() const;

    void setFunctionsModel(FunctionsFilterProxyModel * functionsFilterProxyModel);


    void setupEditorBehaviour(int dimension);

    FunctionEditor::State state() const
    {
        return m_state;
    }

    
    void setVariables(Analitza::Variables* v);

    Analitza::Variables* variables() const
    {
        return m_vars;
    }
    int currentImplicitIndex;

private:





    

    double liArea;
    double lsArea;

    





public slots:
    void initCartesianCurve();
    void initPolarCurve(const RealInterval::List &functionDomain = RealInterval::List());
    void initParametricCurve2D(const RealInterval::List &functionDomain = RealInterval::List());

    void initCartesianSurface(const RealInterval::List &functionDomain = RealInterval::List());
    void initCylindricalSurface(const RealInterval::List &functionDomain = RealInterval::List());
    void initSphericalSurface(const RealInterval::List &functionDomain = RealInterval::List());
    void initParametricSurface(const RealInterval::List &functionDomain = RealInterval::List());
    void initParametricCurve3D(const RealInterval::List &functionDomain = RealInterval::List());
    void initImplicitSurface(const RealInterval::List &functionDomain = RealInterval::List());

    void createFunction();
    
    void changeCurrentImplicitIndex(int index);
    void changeCurrentAxis(int index);
    void chkboxCurvasOnSelect(int index);
    
    
    void resetAll();
    void editFunction(const QModelIndex &index);

    void showCartesianInfo();
    void showParametricInfo();
    void showPolarInfo();

    void showCartesianSurfaceInfo();
    void showCylindricalSurfaceInfo();
    void showSphericalSurfaceInfo();
    void showParametricSurfaceInfo();
    void showParametric3DInfo();

    void clearFunctions2DInfo();

private slots:
    void setCartesianCurveDomainInfo(int index);
    void setPolarCurveDomainInfo();
    void setParametricCurveDomainInfo();

    void setCartesianSurfaceDomainInfo();
    void setCylindricalSurfaceDomainInfo();
    void setSphericalSurfaceDomainInfo();
    void setParametricSurfaceDomainInfo();
    void setParametricCurve3DDomainInfo();





    
//     void setModAnActivo();
//     void setAnimActiva();
//     void setDirAnim();
//     void reiniciarAnimacion();
//     void llenarFuncionesR();
//     void calcularAreaR();
//     void calcularAreaE();
//     void cambiarAreaF();

    





    
    void varsContextMenu(const QPoint& p);

    
    void saveFromQuick(const QString & newExp, int dimension,
                       const QString &name, const QStringList &bvars);

    
    void completeFunctionInfoFromQuickName(const QString & newExp, int dimension,
                                           const QString &name, const QStringList &bvars);


private:
    void initFunction();





    

    double absF(double n1, double n2);
    double abs2(double n);

    int fi;

    





    bool createCartesianCurve();
    bool createPolarCurve();
    bool createParametricCurve2D();

    void clearCartesianCurve();
    void clearPolarCurve();
    void clearParametricCurve2D();
    
    bool createCartesianSurface();
    bool createCylindricalSurface();
    bool createSphericalSurface();
    bool createParametricSurface();
    bool createParametricCurve3D();
//     bool createImplicitSurface(int index);
//     QList<double> implicitparameters;
    void clearCartesianSurface();
    void clearCylindricalSurface();
    void clearSphericalSurface();
    void clearParametricSurface();
    void clearParametricCurve3D();
    void clearImplicitSurface();
    bool areImplicitParametersOK(int index);
    int implicitfunctionsId;
    
    
    bool saveFunction(const Analitza::Expression& newExp, int dimension,
                      const RealInterval::List &domain, bool useDefaults = false, const QString &quickname = QString(),
                      const QStringList &bvars = QStringList());

    void initDomainInfo(const QStringList &arguments, const RealInterval::List &functionDomain = RealInterval::List());
    void clearAppearanceAndDomainSettings();

    void showErrorMessage();




    

    void cambiarMArea();
    double devFuncion(int tipo, double val);
    QVector<QLineF> buildPathsArea(QVector<QPointF> &points);

    




    bool evalDomainExpression(ExpressionEdit *minDomainEdit, ExpressionEdit *maxDomainEdit, qreal &min, qreal &max);

private:
    FunctionEditorWidget * m_editor;
    FunctionsFilterProxyModel *m_functionsFilterProxyModel;
    FunctionEditor::State m_state;

    bool m_isEditing; 
    QString m_currentEditFunctionId; 
    int m_dimensionBehaviour; 

    

    
    
    
    QStringList m_errors;

    
    
    Analitza::Variables *m_vars;




};

} 

#endif 
