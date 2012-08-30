/*************************************************************************************
 *  Copyright (C) 2012 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>      *
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

#ifndef GPLACS_DASHBOARD_H_DV
#define GPLACS_DASHBOARD_H_DV

#include <QtGui/QWidget>
#include <QStackedWidget>

#include <QModelIndex>
#include "plotseditor.h"
#include "document.h"
#include <analitzaplot/private/utils/mathutils.h>

class PlotsDictionariesModel;
class KWidgetItemDelegate;
class SpacesDelegate;
class SpacesDetailsViewDelegate;
class PlotsView3D;
class PlotsView2D;
class QTreeView;
class Document;

class QSortFilterProxyModel;
class QItemSelection;
class DictionariesViewerWidget;

namespace Ui
{
    class DictionariesViewer;
}

#include <QtGui/QListView>
#include <QSortFilterProxyModel>

class DictionariesViewer : public QWidget
{
    Q_OBJECT

public:

    DictionariesViewer(QWidget *parent = 0);
    ~DictionariesViewer();

    void setModel(PlotsDictionariesModel *model);
    
private:
    Ui::DictionariesViewer *m_widget;
};

#endif
