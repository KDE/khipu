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


#ifndef KEOMATH_VIEWER3D_H
#define KEOMATH_VIEWER3D_H

#include <QtGui/QWidget>

#include "spaceitem.h"
namespace Analitza {
class Variables;
}


class View3D;
class Viewer3DWidget;
class FunctionEditor;
class FunctionsFilterProxyModel;
class SpacesModel;
class Viewer3D : public QWidget
{
    Q_OBJECT
    public:
        Viewer3D(QWidget *parent = 0);
        FunctionEditor *functionEditor() const;
        void setSpace(const SpaceItem &space);




        void setFunctionsModel(FunctionsFilterProxyModel *functionsFilterProxyModel);
        void setSpacesModel(SpacesModel *spacesModel);
        void toImage(const QString laurl);
        void copyImageToClipboard();


    
        void setVariables(Analitza::Variables* v);
        Analitza::Variables* variables() const;

    public slots:
        void toggleShownFunctionEditor();
        void toggleShownCoordSysSettings();
        void toggleShownSpaceInfo();
        void hideDocks();
        void resizeScene3D(int v);
        void saveSpace();




    private slots:


    private:
        FunctionsFilterProxyModel *m_functionsFilterProxyModel;

        SpaceItem m_space;
        Viewer3DWidget *m_viewer3DWidget;
        FunctionEditor *m_functionEditor;
        SpacesModel *m_spacesModel;



};

#endif
