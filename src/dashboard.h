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

#ifndef GPLACS_DASHBOARD_H
#define GPLACS_DASHBOARD_H

#include <QtGui/QWidget>
#include <QStackedWidget>

#include <QModelIndex>
#include "plotseditor.h"
#include "document.h"

class QListView;
class QTreeView;
class Document;

class QSortFilterProxyModel;
class QItemSelection;
class DashboardWidget;

namespace Ui
{
    class DashboardWidget;
}

//dashboard solo se encarga de cambiar el space actual al momento de que el main window
//agrega un space (osea al momento de rowsinsertedelspacemodel)
//luego de eso envia una signal activatespace que llegua a document y a mainaplication

class Dashboard : public QStackedWidget
{
    Q_OBJECT

public:

    Dashboard(QWidget *parent = 0);
    ~Dashboard();

    void setDocument(DataStore *doc);
    
public slots:
    void goHome();
    void showPlotsView2D();
    void showPlotsView3D();
    void addSpace2D();
    void addSpace3D();
    void removeCurrentSpace();

    void exportSpace2DSnapshot();
    void exportSpace3DSnapshot();
    void copySpace2DSnapshotToClipboard();
    void copySpace3DSnapshotToClipboard();
    
    

private slots:
    void removeSpace(const QModelIndex & index);
    
    void filterByText(const QString &text); // any text on the title or description
    void filterByDimension(int radioButtonid);

    void setCurrentSpace(const QItemSelection & selected, const QItemSelection & deselected); // al selcionar de la vista
    void setCurrentSpace(const QModelIndex &index ,int,int); // al agregar un nuevo space
    
signals:
    void spaceActivated(int spaceidx); // hacia el mainwind para que prepare los toolbars etc

private:
    void setupWidget();

    DataStore * m_document;
    Ui::DashboardWidget *m_widget;
};

#endif
