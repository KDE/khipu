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

#include "dictionariesviewer.h"

#include "spaceinformation.h"
#include "spaceoptions.h"
#include "analitzaplot/dictionariesmodel.h"
#include "datastore.h"
#include "spaceplotsproxymodel.h"
#include "analitza/variables.h"
#include <analitzaplot/plotsmodel.h>
#include <analitzaplot/plotsdictionarymodel.h>
#include "ui_dashboard.h"
#include <QDebug>

///
#include <QtGui/QAbstractItemView>
#include <QtGui/QListView>
#include <QtGui/QStringListModel>
#include <QtGui/QMainWindow>
#include <QtGui/QPainter>
#include <QtGui/QRadialGradient>
#include <QtGui/QPaintEvent>
#include <QtCore/QDebug>
#include <QtGui/QToolButton>
#include <QtGui/QMenu>

#include <kpushbutton.h>
#include <klineedit.h>
#include <kicon.h>
#include <kwidgetitemdelegate.h>
#include <KSelectionProxyModel>

#include "spacesdelegate.h"

#include "ui_dictionariesviewer.h"

DictionariesViewer::DictionariesViewer(QWidget *parent)
    : QWidget(parent)
{
    m_widget = new Ui::DictionariesViewer;
    
    m_widget->setupUi(this);
}

DictionariesViewer::~DictionariesViewer()
{
    delete m_widget;
}

void DictionariesViewer::setModel(PlotsDictionariesModel* model)
{
    //TODO move QSortFilterProxyModel  as member
    QSortFilterProxyModel *f=new QSortFilterProxyModel(this);
    f->setSourceModel(model);
    m_widget->kfilterproxysearchline->setProxy(f);
    m_widget->plotsView->setModel(f);
}

