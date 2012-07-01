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


#ifndef KEOMATH_VIEWER2D_H
#define KEOMATH_VIEWER2D_H

#include <QtGui/QWidget>

#include "space.h"
// #include "keomath/viewers/view2d.h"

class Graph2D;

namespace Analitza
{
class Variables;
}


namespace Keomath
{

class Viewer2DWidget;
class FunctionsFilterProxyModel;
class FunctionEditor;
class SpacesModel;
// class Function;
class FunctionsView;
// class View2D;

class Viewer2D : public QWidget
{
    Q_OBJECT

public:



public:
    Viewer2D(QWidget *parent = 0);
    FunctionEditor *functionEditor();
    void setSpace(const Space &space);
    void setFunctionsModel(FunctionsFilterProxyModel *functionsFilterProxyModel);
    void setSpacesModel(SpacesModel *spacesModel);


    void toImage(const QString laurl);
    void copyImageToClipboard();





    
    void setVariables(Analitza::Variables* v);

    Analitza::Variables* variables() const;

    Graph2D *view() const;

public slots:
    void toggleShownFunctionEditor();
    void toggleShownCoordSysSettings();
    void toggleShownSpaceInfo();
    void toggleShownAnalysisTools();

    void hideDocks();
    void saveSpace();








private slots:



    void updateTick(int);
    void updateGridColor(int);

private:
    FunctionsFilterProxyModel *m_functionsFilterProxyModel;
    SpacesModel *m_spacesModel;
    FunctionEditor *m_functionEditor;

    Space m_space;
    Viewer2DWidget *m_viewer2DWidget;

    


private:
    void addTickEntry(QString tick, QString tickScaleSymbol, qreal tickScaleSymbolValue,
                      int tickScaleNumerator,
                      int tickScaleDenominator);

};

} 

#endif


