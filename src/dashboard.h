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

//Analitza includes
#include <analitzaplot/plotsdictionarymodel.h>
#include <analitzaplot/plottingenums.h>

//Qt includes
#include <QtGui/QWidget>
#include <QtGui/QListView>
#include <QtGui/QSortFilterProxyModel>
#include <QStackedWidget>
#include <QModelIndex>
#include <QMap>

//local includes
#include "plotseditor.h"

class DictionariesViewer;
class KWidgetItemDelegate;
class SpacesDelegate;
class SpacesFilterProxyModel;
class QTreeView;

namespace Ui
{
    class DashboardWidget;
}
namespace Analitza
{
    class PlotsView3D;
    class PlotsView2D;
    class PlotsDictionaryModel;
}


// Dashboard is a main widget for the user. It allows user to save his work on the same UI.
// User can add/remove spaces in khipu using the tools provided by Dashboard class.
class Dashboard : public QStackedWidget
{
    Q_OBJECT

public:

    explicit Dashboard(QWidget *parent = 0);
    ~Dashboard();

    void setDocument(DataStore *doc);
    QMap<QString,QString> dictionaryDataMap() const { return  m_DictionaryPathName; }

    //views
    Analitza::PlotsView2D *view2d();
    Analitza::PlotsView3D *view3d();
    bool m_openclicked;

public slots:
    
    //Slots to control the filter feature
    void filterByText(const QString &text); // any text on the title or description
    void filterByDimension(Analitza::Dimensions dim);
    
    //shows the dashboard
    void goHome();

    //shows dictionary view
    void showDictionary();

    //shows appropriate plotsview
    void showPlotsView2D();
    void showPlotsView3D();

    void exportSpaceSnapshot(Analitza::Dimension dim);
    void copySpace2DSnapshotToClipboard();
    void copySpace3DSnapshotToClipboard();
    void setGridColor(const QColor &color);
    void setPlotsViewGridColor(const QColor& color);

    void setCurrentSpace(const QModelIndex &index);

    void importDictionaryClicked();
    void getDictionaryClicked();
    void activateDictionaryData();

private slots:
    void setDictionaryData(int ind);
    void setCurrentSpace(const QModelIndex &index, const QModelIndex &old);
    void setCurrentSpace(const QModelIndex& index, int row);
    
    void setCurrentPlot(const QModelIndex& parent, int start);
    void setModelIndex(const QModelIndex & ind);

signals:
    void spaceActivated(int spaceidx);
    void plotRequested(const QModelIndex &ind); //  for requesting the plots when double-clicked on the plot-space thumbnail.
    void showFilter(bool b);
    void setDialogSettingsVisible(bool b);
    void restoreDictionaryData(Analitza::Dimension dim);

private:
    void initializeDictionaryNames();
    void setPlotsView(Analitza::Dimension dim);

    DataStore * m_document;
    QScopedPointer<Ui::DashboardWidget> m_ui;
    QModelIndexList m_spaceindexList;
    SpacesFilterProxyModel *m_spacesProxyModel;
    QStringList m_filterText;
    Analitza::PlotsDictionaryModel* m_plotdictionarymodel;
    bool m_isDictionaryFound;
    QMap<QString,QString> m_DictionaryPathName;
    SpacesDelegate *m_delegate;
};

#endif
