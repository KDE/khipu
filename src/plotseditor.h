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

#ifndef FUNCTIONEDITOR_H
#define FUNCTIONEDITOR_H

#include <QDockWidget>
#include <qcombobox.h>
#include <QModelIndex>
#include "plotsbuilder.h"

namespace Analitza {
class FunctionGraph;
class PlotsModel;
class PlotsModel;
}

class DataStore;
class Document;
class QStackedWidget;
class QTreeView;
class PlotsView;

namespace Ui
{
    class PlotsEditorWidget;
}

class PlotsEditor : public QDockWidget
{
    Q_OBJECT
public:
    PlotsEditor(QWidget *parent);
    ~ PlotsEditor();
    
    void setDocument(DataStore *doc);

public slots:
    void setCurrentSpace(int spaceidx);
    
    //el caso de clearbuilder true es si es llamado desde afuera de este widget ... por ejemplo desde el mainwnd es necesario resetar lon links del builer antes de entrar al editor
    void reset(bool clearBuilder = false); // clear fields and reset the widgets like view3d /view2d (centrandolos etc))

    //NOTE para que el dock plotbuilder del mainwnd se conecte con estos slots
    // si es false se queda en el editor, ... se queda editando el space acutaul y no regresa a home
    //if cancelisgohome true then al presionar cancel se regresara al dashboard
    void buildCartesianGraphCurve(bool cancelIsGoHome = false); 
    void buildCartesianImplicitCurve(bool cancelIsGoHome = false);
    void buildCartesianParametricCurve2D(bool cancelIsGoHome = false);
    void buildPolarGraphCurve(bool cancelIsGoHome = false);
    //3D
    void buildCartesianParametricCurve3D(bool cancelIsGoHome = false);
    void buildCartesianGraphSurface(bool cancelIsGoHome = false);
    void buildCartesianImplicitSurface(bool cancelIsGoHome = false);
    void buildCartesianParametricSurface(bool cancelIsGoHome = false);
    void buildCylindricalGraphSurface(bool cancelIsGoHome = false);
    void buildSphericalGraphSurface(bool cancelIsGoHome = false);

private slots:
    void showList();
    void showTypes(); /// cambiar de nombre pues ahora se usa ademas m_cancelIsGoHome
    void showEditor();
    
    void cancelEditor();
    void addPlots();
    void editPlot(const QModelIndex &index = QModelIndex()); // si el usuario elije editar un plot de la lista

    void savePlot();
    
    void removePlot();
    
    //este slot se coneta con el combo
    void setCurrentFunctionGraphs(const QString &txt); /// see m_currentFunctionGraphs
    
signals:
    void plotAdded(const QModelIndex &index);  // emit when item != 0
    void plotRemoved(const QModelIndex &index);  // emit when item != 0

    //for mainwnd
    void goHome();
    void sendStatus(const QString &stat, int mstimeout = 0);

private:
    Analitza::FunctionGraph* editCurrentFunction(const Analitza::Expression& exp);
    
    //helpers para configurar los widgets f,g,h y x,y,...
    void setupVarName(int var, const QString &vvalue); //var: 1 x 2 y 3 z .. vvalue var value ...z,p,t,...
    void setupFuncName(int var, const QString &vvalue, const QStringList &vars, bool withparenthesis = true); //var: fname,gname ... NO USAR CON fnameForGraphs
    void setupExpressionType(const QStringList &funvalues, const QStringList &varsvalues, bool isimplicit = false, bool isvectorValued = false);

    Ui::PlotsEditorWidget *m_widget;
    Analitza::PlotsModel *m_localModel; // usado solo para los previews
    DataStore *m_document;
    
    PlotsBuilder::PlotType m_currentType; //NOTE  tipo actual que se esta editando o creando
    QStringList m_currentFunctionGraphs; // NOTE en el setupexptyp los graph no usaran funvalues, sino esta variable, pues esta se actualiza con el combobox
    
    //utils for mainwnd
    bool m_cancelIsGoHome;
    
    //track current status when edit/create a plotsbuilder
    QStringList m_currentVars;
    bool m_currentIsImplicit;
    bool m_currentIsVectorValued;
    int m_currentVectorSize;
    
    //flag para indicar que se encuentra editando, por ende no se agrega nada sino se modifica 
    //ademas el cancel va a la lista no al builder
    bool isEditing;
};

class ComboBox : public QComboBox
{
Q_OBJECT

public:
    explicit ComboBox(QWidget* parent = 0);
    
    QSize sizeHint() const;
    void paintEvent(QPaintEvent* e);
private slots:
    void setupCache(const QString &currtext);
    
private:
    QString m_cacheText; // paint llama a sizehint y se necesita el texto actual para sugerir el size
    
    
};


#endif 
