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
#include <analitzaplot/private/utils/mathutils.h>

class KWidgetItemDelegate;
class SpacesDelegate;
class SpacesDetailsViewDelegate;
class PlotsView3D;
class PlotsView2D;
class QTreeView;
class Document;

class QSortFilterProxyModel;
class QItemSelection;
class DashboardWidget;

namespace Ui
{
    class DashboardWidget;
    class DictionariesViewer;
}

#include <QtGui/QListView>
#include <QSortFilterProxyModel>

class SpacesFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    public:
        SpacesFilterProxyModel(QObject *parent = 0);

        int filterDimension() const { return m_dimension; }
        void setFilterDimension(int dimension);

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
////        bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

    private:
        int m_dimension; // 2 => 2D, 3 => 3D, -1 => ALL
};


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
    
    QPixmap currentPlotsViewSnapshot() const;
    
    //views
    
    PlotsView2D *view2d();
    PlotsView3D *view3d();

public slots:
    
    void filterByText(const QString &text); // any text on the title or description
    void filterByDimension(Dimension dim);
    
    
    void goHome();
    void showDictionary();
    void showPlotsView2D();
    void showPlotsView3D();

    void exportSpace2DSnapshot();
    void exportSpace3DSnapshot();
    void copySpace2DSnapshotToClipboard();
    void copySpace3DSnapshotToClipboard();
    
    

private slots:
    void removeSpace(const QModelIndex & index);
    


//     void setCurrentSpace(const QItemSelection & selected, const QItemSelection & deselected); // al selcionar de la vista
    void setCurrentSpace(const QModelIndex &index);
    void setCurrentSpace(const QModelIndex &index, const QModelIndex &old); // este solo emite el spaceactivate para que se entere el datasotre ... no hace cambios en la ui
    void setCurrentSpace(const QModelIndex &index ,int,int); // al agregar un nuevo space
    
    //al agregar un plot notificar que es el ultimo seleccionado a los view2d3}
    //los index son del proxy 
    void setCurrentPlot(const QModelIndex & parent, int start, int end);

signals:
    void spaceActivated(int spaceidx); // hacia el mainwind para que prepare los toolbars etc

private:
    void setupWidget();

    DataStore * m_document;
    Ui::DashboardWidget *m_widget;
    
    SpacesFilterProxyModel *m_spacesProxyModel;
    
    Ui::DictionariesViewer *m_dviewer;
};

#endif
