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

#include "dashboard.h"

#include "spaceitem.h"
#include "spaceinformation.h"
#include "spaceoptions.h"
#include "spacesmodel.h"
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

#include "spacesdelegate.h"

Dashboard::Dashboard(QWidget *parent)
    : QStackedWidget(parent)
{
    m_widget = new  Ui::DashboardWidget;
    m_widget->setupUi(this);
    m_widget->findIcon->setPixmap(KIcon("edit-find").pixmap(16,16));
}

Dashboard::~Dashboard()
{
    delete m_widget;
}

void Dashboard::setDocument(DataStore* doc)
{
    m_document = doc;

//     doc->plotsModel()->setCheckable(false); // en la action view show functions ... ojo esa tendra un preview

    m_widget->spacesView->setModel(doc->spacesModel());
    m_widget->spacesView->setSelectionModel(doc->currentSpaceSelectionModel());

    //BUG qt? memory leak ... no acepta ponerle this as parent
    m_widget->spacesView->setViewMode(QListView::IconMode);
    m_widget->spacesView->setItemDelegate(new SpacesDelegate(m_widget->spacesView));
    
    //este necesita otro proxy del modelo
//     m_widget->plotsView->setModel(m_document->spacePlotsFilterProxyModel());

    //NOTE AQUI cambiamos los models y selects de los plotsview2d/3d
    m_document->currentPlots()->setFilterSpaceDimension(Dim2D);
    m_widget->plotsView2D->setModel(m_document->currentPlots());
    m_widget->plotsView2D->setSelectionModel(m_document->currentSelectionModel());

    m_document->currentPlots()->setFilterSpaceDimension(Dim3D);
    m_widget->plotsView3D->setModel(m_document->currentPlots());
    m_widget->plotsView3D->setSelectionModel(m_document->currentSelectionModel());

    //al insertar nuevos plots que el current sea el ultimo insertado ... esto es necesario
    //para que los plotsview se enteren ...
    connect(m_document->currentPlots(), SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(setCurrentPlot(QModelIndex,int,int)));


//     m_document->spacePlotsFilterProxyModel()->setFilterSpaceDimension(-1); //TODO hacks para evitar los asertos de setmodel... enums?


//     connect(m_widget->spacesView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
//             SLOT(setCurrentSpace(QItemSelection,QItemSelection)));
    
    
    
    connect(m_widget->spacesView, SIGNAL(doubleClicked(QModelIndex)), SLOT(setCurrentSpace(QModelIndex)));
    connect(m_widget->spacesView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setCurrentSpace(QModelIndex,QModelIndex)));

    SpacesModel * m = m_document->spacesModel();

    connect(m, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(setCurrentSpace(QModelIndex,int,int)));

}

QPixmap Dashboard::currentPlotsViewSnapshot() const
{
//     switch (m_document->spacesModel()->item(m_document->currentSpace())->dimension())
//     {
//         case 2:
//             return m_widget->plotsView2D;
//     }

    return QPixmap();
}

PlotsView2D* Dashboard::view2d()
{
    return m_widget->plotsView2D;
}

PlotsView3D* Dashboard::view3d()
{
    return m_widget->plotsView3D;
}

void Dashboard::goHome()
{
    setCurrentIndex(0);
    m_widget->views->setCurrentIndex(0);
    m_widget->plotsViewOptions->setCurrentIndex(0);
}

void Dashboard::showDictionary()
{
    m_widget->views->setCurrentIndex(1);
    m_widget->plotsViewOptions->setCurrentIndex(1);
    m_widget->plotsView->setModel(m_document->plotsDictionaryModel());
}

void Dashboard::showPlotsView2D()
{
    m_widget->plotsViews->setCurrentIndex(0);
}

void Dashboard::showPlotsView3D()
{
//     qDebug() << "3ddd";
    m_widget->plotsViews->setCurrentIndex(1);
}

void Dashboard::removeCurrentSpace()
{

}


void Dashboard::exportSpace2DSnapshot()
{
//     QString path = KFileDialog::getSaveFileName(KUrl(), i18n("*.png|PNG Image File\n*.svg|SVG File"), this);
//     if(!path.isEmpty())
//         m_widget->space2D->toImage(path);
}

void Dashboard::exportSpace3DSnapshot()
{
//     QString path = KFileDialog::getSaveFileName(KUrl(), i18n("*.png|PNG Image File\n*.jpg|JPG Image File"), this);
//     if(!path.isEmpty())
//         m_widget->space3D->toImage(path);
}

void Dashboard::copySpace2DSnapshotToClipboard()
{
//     m_widget->space2D->copyImageToClipboard();

}

void Dashboard::removeSpace(const QModelIndex& index)
{

}


void Dashboard::copySpace3DSnapshotToClipboard()
{
//     m_widget->space3D->copyImageToClipboard();
}

void Dashboard::filterByText(const QString &text)
{
//     switch (m_dashboardWidget->viewMode->currentIndex())
//     {
//     case 0:
//         m_spacesProxyModel->setFilterRegExp(QRegExp(m_dashboardWidget->filterTextSpaces->text(),
//                                             Qt::CaseInsensitive, QRegExp::RegExp));
//         break;
//
//     case 1:
//         m_functionsProxyModel->setFilterRegExp(QRegExp(m_dashboardWidget->filterTextFunctions->text(),
//                                                Qt::CaseInsensitive, QRegExp::RegExp));
//         break;
//     }
}

void Dashboard::filterByDimension(int radioButton)
{
//     switch (m_dashboardWidget->viewMode->currentIndex())
//     {
//     case 0:
//     {
//         switch (radioButton)
//         {
//         case 0:
//             m_spacesProxyModel->setFilterDimension(-1);
//             break;
//         case 1:
//             m_spacesProxyModel->setFilterDimension(2);
//             break;
//         case 2:
//             m_spacesProxyModel->setFilterDimension(3);
//             break;
//         }
//     }
//     break;
//
//     case 1:
//     {
//         switch (radioButton)
//         {
//         case 0:
//             m_functionsProxyModel->setFilterDimension(-1);
//             break;
//         case 1:
//             m_functionsProxyModel->setFilterDimension(2);
//             break;
//         case 2:
//             m_functionsProxyModel->setFilterDimension(3);
//             break;
//         }
//     }
//     break;
//     }
}

void Dashboard::setCurrentSpace(const QModelIndex &index)
{
//     m_document->spacePlotsProxyModel()->setFilterSpaceDimension(m_document->spacesModel()->item(selected.indexes().first().row())->dimension());

    setCurrentIndex(1);

    emit spaceActivated(index.row());

    switch (m_document->spacesModel()->item(index.row())->dimension())
    {
    case 2:
    {
        m_widget->plotsViews->setCurrentIndex(0);

        break;
    }

    case 3:
    {
        m_widget->plotsViews->setCurrentIndex(1);
        break;
    }
    }
}

void Dashboard::setCurrentSpace(const QModelIndex& index, const QModelIndex& old)
{
    m_document->setCurrentSpace(index.row());
}

//luego de agregar un space la vista de espacio debe selecionar el nuevo espacio y hacerlo current
void Dashboard::setCurrentSpace(const QModelIndex& index, int row, int )
{
//     qDebug() << "sadasadadsd ads";
//     m_widget->spacesView->selectionModel()->setCurrentIndex(m_document->spacesModel()->index(row), QItemSelectionModel::Current);

    m_widget->spacesView->setCurrentIndex(m_document->spacesModel()->index(row));

    //solo se cambia el filtro aqui porque es cuando se agrega un space ... luego todo cambio sera en datastore::setCurrentSpace
//     m_document->currentPlots()->setFilterSpaceDimension(m_document->spacesModel()->item(row)->dimension());
//     m_document->currentPlots()->setFilterSpace(m_document->spacesModel()->item(row));

    emit spaceActivated(row);
//     qDebug() << row;
}

void Dashboard::setCurrentPlot(const QModelIndex& parent, int start, int end)
{
//     qDebug() << start << "??";

    m_document->currentSelectionModel()->clear();
    m_document->currentSelectionModel()->setCurrentIndex(m_document->currentPlots()->index(start,0), QItemSelectionModel::SelectCurrent );

}


void Dashboard::setupWidget()
{
//     m_widget = new DashboardWidget(this);

    /*
        m_widget->space2D->setFunctionsModel(m_proxyViewer2D);
        m_widget->space2D->setSpacesModel(m_spacesModel);

        m_widget->space3D->setFunctionsModel(m_proxyViewer3D);
        m_widget->space3D->setSpacesModel(m_spacesModel);


        connect(m_widget->addSpace2D, SIGNAL(clicked()), SLOT(addSpace2D()));
        connect(m_widget->addSpace3D, SIGNAL(clicked()), SLOT(addSpace3D()));
        connect(m_widget->backFromSpace2D, SIGNAL(clicked()), SLOT(showDashboard()));
        connect(m_widget->backFromSpace3D, SIGNAL(clicked()), SLOT(showDashboard()));

        connect(m_widget->showFunctionEditor2D, SIGNAL(clicked()),
                m_widget->space2D, SLOT(toggleShownFunctionEditor()));

        connect(m_widget->showFunctionEditor3D, SIGNAL(clicked()),
                m_widget->space3D, SLOT(toggleShownFunctionEditor()));

        connect(m_widget->showCoordSysSettings2D, SIGNAL(clicked()),
                m_widget->space2D, SLOT(toggleShownCoordSysSettings()));

        connect(m_widget->showCoordSysSettings3D, SIGNAL(clicked()),
                m_widget->space3D, SLOT(toggleShownCoordSysSettings()));

        connect(m_widget->showSpace2DInfo, SIGNAL(clicked()),
                m_widget->space2D, SLOT(toggleShownSpaceInfo()));

        connect(m_widget->showSpace3DInfo, SIGNAL(clicked()),
                m_widget->space3D, SLOT(toggleShownSpaceInfo()));

        connect(m_widget->filterDimensionsSpaces, SIGNAL(clicked(int)), SLOT(setFilterDimension(int)));
        connect(m_widget->filterDimensionsFunctions, SIGNAL(clicked(int)), SLOT(setFilterDimension(int)));

        connect(m_widget->filterTextSpaces, SIGNAL(textChanged(QString)), SLOT(setFilterText(QString)));
        connect(m_widget->filterTextFunctions, SIGNAL(textChanged(QString)), SLOT(setFilterText(QString)));


        connect(m_widget->spaces, SIGNAL(spaceShown( SpaceItem)), SLOT(showSpace( SpaceItem)));

        connect(m_widget->functions, SIGNAL(functionOnSpaceShown(QUuid)), SLOT(showFunctionOnSpace(QUuid)));

        connect(m_widget->saveSpace2DImage, SIGNAL(clicked()), SLOT(saveSpace2DImage()));
        connect(m_widget->saveSpace3DImage, SIGNAL(clicked()), SLOT(saveSpace3DImage()));

        connect(m_widget->copySpace2DImage, SIGNAL(clicked()), SLOT(copySpace2DImage()));
        connect(m_widget->copySpace3DImage, SIGNAL(clicked()), SLOT(copySpace3DImage()));

        connect(m_widget->showAboutAppDialog, SIGNAL(clicked()), SIGNAL(dashemitShowAppInfo()));*/

//     m_widget->viewMode->setCurrentIndex(1);
//     m_widget->viewMode->setCurrentIndex(0);

}

