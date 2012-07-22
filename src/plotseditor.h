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
#include <QModelIndex>
#include "plotsbuilder.h"

class DataStore;
class Document;
class PlotsModel;
class QStackedWidget;
class QTreeView;
class PlotsModel;
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
    

private slots:
    void showList();
    void showTypes();
    void showEditor();
    
    void addPlots(); // test method
    
    void buildCartesianGraphCurve();
    void buildCartesianImplicitCurve();
    void buildCartesianParametricCurve2D();
    void buildPolarGraphCurve();
    //3D
    void buildCartesianParametricCurve3D();
    void buildCartesianGraphSurface();
    void buildCartesianImplicitSurface();
    void buildCartesianParametricSurface();
    void buildCylindricalGraphSurface();
    void buildSphericalGraphSurface();

    void savePlot();
    
    void removePlot();
    
signals:
    void plotAdded(const QModelIndex &index);  // emit when item != 0
    void plotRemoved(const QModelIndex &index);  // emit when item != 0

private:
    Ui::PlotsEditorWidget *m_widget;
    PlotsModel *m_localModel; // usado solo para los previews
    DataStore *m_document;
    
    PlotsBuilder::PlotType m_currentType; // tipo actual que se esta editando o creando
};

#endif 
