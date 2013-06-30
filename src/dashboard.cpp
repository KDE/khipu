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

#include "spaceinformation.h"
#include "spaceoptions.h"
#include "dictionariesmodel.h"
#include "datastore.h"
#include "spaceplotsproxymodel.h"
#include "analitza/variables.h"
#include <analitzaplot/plotsmodel.h>
#include <analitzaplot/plotsdictionarymodel.h>
#include <dictionaryitem.h>
#include "ui_dashboard.h"
#include <QDebug>

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
#include <QFileDialog>

#include <kpushbutton.h>
#include <klineedit.h>
#include <kicon.h>
#include <kwidgetitemdelegate.h>
#include <KSelectionProxyModel>
#include <KFileDialog>
#include <KStandardDirs>
#include "spacesdelegate.h"
#include <KMessageBox>

using namespace Analitza;

SpacesFilterProxyModel::SpacesFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    m_dimension = DimAll;
    setDynamicSortFilter(true);
}

void SpacesFilterProxyModel::setFilterDimension(Dimensions dimension)
{
    m_dimension = dimension;
    invalidateFilter();
}

bool SpacesFilterProxyModel::filterAcceptsRow(int sourceRow,
                                               const QModelIndex &sourceParent) const
{
    if (!sourceModel()) return false;

    DictionaryItem *spaceItem = static_cast<DictionariesModel*>(sourceModel())->space(sourceRow);
    
    if (!spaceItem) return false;
    
    if (m_dimension != DimAll && spaceItem->dimension() != m_dimension)
        return false;

    return spaceItem->title().contains(filterRegExp()) || spaceItem->description().contains(filterRegExp());
}

Dashboard::Dashboard(QWidget *parent)
    : QStackedWidget(parent)
    , m_isDictionaryFound(false)
{
    m_widget = new  Ui::DashboardWidget;
    m_widget->setupUi(this);
    m_spaceindexList=new QModelIndexList();
    m_openclicked=false;
    m_totalinternalDict=0;

    m_filterText << "Dimension-All" << "Dimension-2D" << "Dimension-3D";
    connect(m_widget->importDictionarybutton,SIGNAL(clicked()),this,
            SLOT(importDictionary()));
    m_widget->comboBox->clear();
    checkforDictionary();
    setDictionaryNames();

}

void Dashboard::setDashboardData(Dashboard* source){
    m_document=source->m_document;
    m_widget=source->m_widget;
    m_spacesProxyModel=source->m_spacesProxyModel;
}

Dashboard::~Dashboard()
{
//     QAbstractItemDelegate *d = m_widget->spacesView->itemDelegate();
//     m_widget->spacesView->setItemDelegate(0);
// 
//     delete d;
//     
    m_widget=0;
    delete m_widget;
}

void Dashboard::setDictionaryNames()
{
    if(!m_isDictionaryFound)
        return;

    QString str=KStandardDirs::installPath("data"); // works for -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix`
    QDir dir(str.append("libanalitza/plots"));  // path to the dir where .plots files are installed by analitza
    QStringList filters;
    filters << "*.plots";
    dir.setNameFilters(filters);
    m_fileList=dir.entryList();
    m_dictionaryTitles << "3Ds" << "Dictionary1" << "conics" << "polar"; // we know this , so we can set this ;)
    m_widget->comboBox->addItems(m_dictionaryTitles);
    m_totalinternalDict=m_dictionaryTitles.size();

    connect(m_widget->comboBox,SIGNAL(activated(int)),this,SLOT(setDictionaryData(int)));
}

void Dashboard::setDocument(DataStore* doc)
{
    m_document = doc;

//     doc->plotsModel()->setCheckable(false); // en la action view show functions ... ojo esa tendra un preview

    m_spacesProxyModel = new SpacesFilterProxyModel(this);
    m_spacesProxyModel->setSourceModel(doc->spacesModel());

    m_widget->spacesView->setModel(m_spacesProxyModel);
    
    ///
    QItemSelectionModel *selection = new QItemSelectionModel(m_spacesProxyModel);
    
    m_widget->spacesView->setSelectionModel(selection);
    ///


    m_widget->spacesView->setMouseTracking(true);
    m_widget->spacesView->setAlternatingRowColors(true);
    m_widget->spacesView->setViewMode(QListView::IconMode);
//     m_widget->spacesView->setFlow(QListView::LeftToRight);
    SpacesDelegate *delegate = new SpacesDelegate(m_widget->spacesView, this);

    delegate->setDocument(doc);
    m_widget->spacesView->setItemDelegate(delegate);

    connect(delegate,SIGNAL(saveDictionary(QModelIndex)),m_document,SLOT(saveSpaceAsDictionary(QModelIndex)));
//     delegate->setIconMode(true);
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

    DictionariesModel * m = m_document->spacesModel();

    connect(m, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(setCurrentSpace(QModelIndex,int,int)));
}

QPixmap Dashboard::currentPlotsViewSnapshot() const
{
//     switch (m_document->spacesModel()->item(m_document->currentSpace())->dimension())
//     {
//         case Dim2D:
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

    emit showFilter(true);
}

void Dashboard::showDictionary()
{
    m_widget->views->setCurrentIndex(1);
    m_widget->plotsViewOptions->setCurrentIndex(1);

    emit showFilter(false);
}

void Dashboard::setDictionaryData(int ind)
{
    if(!m_isDictionaryFound)
        return;

    PlotsDictionaryModel *model = m_document->plotsDictionaryModel();
    m_plotdictionarymodel=model;
    model->clear();

    if(ind < m_totalinternalDict) { // total internal available dictionaries
        QString str=KStandardDirs::installPath("data"); // works for -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix`
        QDir dir(str.append("libanalitza/plots"));
        model->createDictionary(dir.path().append("/").append(m_fileList.at(ind)));
    }
    else {
        model->createDictionary(m_fileList.at(ind));
    }

    m_widget->plotsView->setModel(model);
    m_widget->page->setModel(model->plotModel());
    m_widget->page_2->setModel(model->plotModel());
    connect(m_widget->plotsView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(setModelIndex(QModelIndex)));
}

void Dashboard::setModelIndex(const QModelIndex& ind)
{
    m_plotdictionarymodel->setCurrentIndex(ind);
    setPlotsView(m_plotdictionarymodel->dimension());
}

void Dashboard::setPlotsView(Analitza::Dimension dim)
{
    if(dim==Dim2D){
        m_widget->page_2->setVisible(false);
        m_widget->page->setVisible(true);

    }
    else if (dim==Dim3D){
        m_widget->page_2->setVisible(true);
        m_widget->page->setVisible(false);
    }

}

void Dashboard::importDictionary()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open a .plots file"),"/",tr("Text files (*.plots)"));
    if(path==0){
        qDebug() << "error in opening file...may be path not found." ;
        return;
    }

    m_widget->comboBox->addItem("ImportedDictionary");
    m_dictionaryTitles << "ImportedDictionary";
    m_fileList << path;
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

void Dashboard::exportSpaceSnapshot(Dimension dim)
{
    QString path = KFileDialog::getSaveFileName(KUrl(), i18n("*.png|PNG Image File"), this);
    if(!path.isEmpty()){

        if(dim==Dim2D){
            view2d()->toImage(path,PlotsView2D::PNG);
        }

        else if(dim==Dim3D){
            view3d()->updateGL();
            view3d()->setFocus();
            view3d()->makeCurrent();
            view3d()->raise();

            QImage image(view3d()->grabFrameBuffer(true));
            QPixmap::fromImage(image, Qt::ColorOnly).save(path,"PNG");
        }
    }
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
    switch (m_filterText.indexOf(text))
    {
        case 0: filterByDimension(Analitza::DimAll); break;
        case 1: filterByDimension(Analitza::Dim2D); break;
        case 2: filterByDimension(Analitza::Dim3D); break;
    }
}

void Dashboard::filterByDimension(Dimensions dim)
{
    //desaparecemos los botones y editores del delegate
    static_cast<SpacesDelegate*>(m_widget->spacesView->itemDelegate())->filterEvent();
    m_spacesProxyModel->setFilterDimension(dim);
}


void Dashboard::setCurrentSpace(const QModelIndex &index)
{
//     m_document->spacePlotsProxyModel()->setFilterSpaceDimension(m_document->spacesModel()->item(selected.indexes().first().row())->dimension());

    setCurrentIndex(1);

    emit spaceActivated(index.row());

    switch (m_document->spacesModel()->space(index.row())->dimension())
    {
    case Dim2D:
        m_widget->plotsViews->setCurrentIndex(0);
        break;

    case Dim3D:
        m_widget->plotsViews->setCurrentIndex(1);
        break;
    }

    // identification of the space
    if(!(m_spaceindexList->contains(index)) && m_openclicked){
        m_spaceindexList->append(index);
        emit plotRequested(index);
    }

 emit showFilter(false);
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
    
    //BUG qt el filtermodel no respeta iconviewmode al agregar los items ...
//     if (m_document->spacesModel()->rowCount() < 2) // si esta vacio por primera actiualizo el view 
//         m_widget->spacesView->update();
    
    static_cast<SpacesDelegate*>(m_widget->spacesView->itemDelegate())->filterEvent();


}

void Dashboard::setCurrentPlot(const QModelIndex& parent, int start, int end)
{
//     qDebug() << start << "??";

    m_document->currentSelectionModel()->clear();
    m_document->currentSelectionModel()->setCurrentIndex(m_document->currentPlots()->index(start,0), QItemSelectionModel::SelectCurrent );

}

void Dashboard::setGridColor(const QColor &color) // used for making the axis visible/invisible
{
    if(m_document->spacesModel()->space(m_document->currentSpace())->dimension()==Dim2D){ // 2d space
        m_widget->plotsView2D->updateGridColor(color);
    }

    else if(m_document->spacesModel()->space(m_document->currentSpace())->dimension()==Dim3D){ // 3d space
        Analitza::Plotter3D* plotter3d = dynamic_cast<Analitza::Plotter3D*>(view3d());
        plotter3d->setReferencePlaneColor(color);
    }
}

void Dashboard::setPlotsViewGridColor(const QColor &color){
    if(m_document->spacesModel()->space(m_document->currentSpace())->dimension()==Dim2D){ // 2d space
        m_widget->plotsView2D->updateGridColor(color);
    }
    else if(m_document->spacesModel()->space(m_document->currentSpace())->dimension()==Dim3D){ // 3d space
        Analitza::Plotter3D* plotter3d = dynamic_cast<Analitza::Plotter3D*>(view3d());
        plotter3d->setReferencePlaneColor(color);
        showPlotsView3D();
    }
}

void Dashboard::setupWidget()
{
//     m_widget = new DashboardWidget(this);

    /*
        m_widget->space2D->setFunctionsModel(m_proxyViewer2D);
        m_widget->space2D->setDictionariesModel(m_spacesModel);

        m_widget->space3D->setFunctionsModel(m_proxyViewer3D);
        m_widget->space3D->setDictionariesModel(m_spacesModel);


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


        connect(m_widget->spaces, SIGNAL(spaceShown( DictionaryItem)), SLOT(showSpace( DictionaryItem)));

        connect(m_widget->functions, SIGNAL(functionOnSpaceShown(QUuid)), SLOT(showFunctionOnSpace(QUuid)));

        connect(m_widget->saveSpace2DImage, SIGNAL(clicked()), SLOT(saveSpace2DImage()));
        connect(m_widget->saveSpace3DImage, SIGNAL(clicked()), SLOT(saveSpace3DImage()));

        connect(m_widget->copySpace2DImage, SIGNAL(clicked()), SLOT(copySpace2DImage()));
        connect(m_widget->copySpace3DImage, SIGNAL(clicked()), SLOT(copySpace3DImage()));

        connect(m_widget->showAboutAppDialog, SIGNAL(clicked()), SIGNAL(dashemitShowAppInfo()));*/

//     m_widget->viewMode->setCurrentIndex(1);
//     m_widget->viewMode->setCurrentIndex(0);

}
void Dashboard::checkforDictionary() {
    QString str=KStandardDirs::installPath("data"); // works for -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix`
    QDir dir(str.append("libanalitza/plots"));
    QStringList filters;
    filters << "*.plots";
    dir.setNameFilters(filters);

    if(dir.count() >= 4) {
        qDebug() << "found";
        m_isDictionaryFound=true;
        return;
    }

    QString error =" Please make sure that the dictionary(.plots) files are correctly installed in the path: ";
    KMessageBox::error(this,error.append(dir.path()).append("  If not, use -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix` with cmake !"),"Error in Dictionary Path:");
    qDebug() << "not found..";
return;
}
