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


#ifndef KEOMATH_SPACESVIEW_H
#define KEOMATH_SPACESVIEW_H

#include <KDE/KCategorizedView>


namespace Keomath
{

class SpacesModel;
class Space;
class FunctionsModel;

class SpacesView : public KCategorizedView
{
    Q_OBJECT
public:
    SpacesView(QWidget *parent=0);

    virtual void selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );

    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
    void setMainFunctionsModel(FunctionsModel *fm);

signals:
    void spaceShown(const Keomath::Space &space); 

private:
    FunctionsModel *m_mainFunctionsModel;

};

} 

#endif 
