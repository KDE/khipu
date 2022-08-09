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

#ifndef FUNCTIONEDITOR_H
#define FUNCTIONEDITOR_H

//Analitza includes
#include <analitzaplot/plotitem.h>

//Qt includes
#include <QDockWidget>
#include <qcombobox.h>
#include <QModelIndex>

//local includes
#include "plotsbuilder.h"

namespace Analitza
{
class FunctionGraph;
class PlotsModel;
class PlotsModel;
}

class DataStore;
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
    explicit PlotsEditor(QWidget *parent);
    ~ PlotsEditor() override;
    
    void setDocument(DataStore *doc);

public slots:
    void setCurrentSpace(int spaceidx);
    void reset(bool clearBuilder = false); // clear fields and reset the widgets like view3d /view2d (centrandolos etc))

    // used to buiild the UI for the plots based on its category
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
    void showTypes();
    void showEditor();
    
    void cancelEditor();
    void addPlots();

    // slot gets activated while editing any plot.
    void editPlot(const QModelIndex &index = QModelIndex());

    //adds the plot in the cuurent document
    void savePlot();

    //showing/hiding the axis
    void showAxis(int state);

    //slot to remove a plot from document
    void removePlot();

    //to connect the combo-box
    void setCurrentFunctionGraphs(const QString &txt);

    //adds the default plot names (plot 1 , plot 2 and so on)
    void plotnamecheckClicked(bool b);

signals:
    //for main-window
    void goHome();
    void sendStatus(const QString &stat, int mstimeout = 0);
    void updateGridcolor(const QColor &color);
    void mapDataChanged(); // emit when the map data is changed ( used for autosaving the  file )

private:
    QColor randomFunctionColor() { return QColor::fromHsv(qrand()%255, 255, 255); }
    bool eventFilter(QObject *object, QEvent *event) override;

    // helpers to set the widgets of interval
    void setupVarName(int var, const QString &vvalue); //var: 1 x 2 y 3 z .. vvalue var value ...z,p,t,...
    void setupFuncName(int var, const QString &vvalue, const QStringList &vars, bool withparenthesis = true); //var: fname,gname ...
    void setupExpressionType(const QStringList &funvalues, const QStringList &varsvalues, bool isimplicit = false, bool isvectorValued = false);

    Ui::PlotsEditorWidget *m_widget;
    Analitza::PlotsModel *m_localModel;
    DataStore *m_document;
    PlotsBuilder::PlotType m_currentType; // get the current plots's type
    QStringList m_currentFunctionGraphs;
    
    //utils for mainwnd
    bool m_cancelIsGoHome;
    
    //track current status when edit/create a plotsbuilder
    QStringList m_currentVars;
    bool m_currentIsImplicit;
    bool m_currentIsVectorValued;
    int m_currentVectorSize;
    QModelIndex m_indexEdited;

    bool isEditing;
    int plotnumber;
};

#endif 
