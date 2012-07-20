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

#include <QDockWidget>
#include <QModelIndex>

class PlotsModel;
class PlotsView;

class PlotsEditor : public QDockWidget
{
    Q_OBJECT
public:
    PlotsEditor(QWidget *parent);
    ~ PlotsEditor();
    
    void setModel(PlotsModel *m);

public slots:
    void showList();
    void showCreateByName();
    void showCreateByExpression();
    void showEditor();
    
signals:
    void plotAdded(const QModelIndex &index);  // emit when item != 0
    void plotRemoved(const QModelIndex &index);  // emit when item != 0

private:
    PlotsView *m_plotsView;
};

#endif 
