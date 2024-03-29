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
#include "ui_dashboard.h"

//Analitza includes
#include <analitza/variables.h>
#include <analitzaplot/plotsmodel.h>

//Qt includes
#include <QAbstractItemView>
#include <QStringListModel>
#include <QMainWindow>
#include <QPainter>
#include <QRadialGradient>
#include <QPaintEvent>
#include <QToolButton>
#include <QMenu>
#include <QDebug>
#include <QClipboard>

//KDE includes
#include <kpushbutton.h>
#include <klineedit.h>
#include <kwidgetitemdelegate.h>
#include <KSelectionProxyModel>
#include <QFileDialog>
#include <KStandardDirs>
#include <KMessageBox>
#include <KNS3/DownloadDialog>

//local includes
#include "spaceinformation.h"
#include "spaceoptions.h"
#include "spacesmodel.h"
#include "datastore.h"
#include "spaceplotsproxymodel.h"
#include "spaceitem.h"
#include "spacesdelegate.h"

using namespace Analitza;

Dashboard::Dashboard(QWidget *parent)
    : QStackedWidget(parent)
    , m_ui(new Ui::DashboardWidget)
{
    m_ui->setupUi(this);
    m_openclicked=false;

    m_filterText << "Dimension-All" << "Dimension-2D" << "Dimension-3D";
    m_ui->comboBox->clear();
    initializeDictionaryNames();
    initializePlotsViews();

    //Init plotters
    m_ui->plotsView2D->setKeepAspectRatio(true);
    m_ui->plotsView2D->setAutoGridStyle(false);
    m_ui->plotsView2D->setViewport(QRectF(QPointF(-5.0, 5.0), QSizeF(10.0, -10.0)));
    m_ui->plotsView2D->setShowMinorTicks(true);
    m_ui->plotsView2D->setShowMinorGrid(true);
    m_ui->plotsView2D->setShowPolarAxis(true);
    m_ui->plotsView2D->setShowPolarAngles(true);
    
    //Init space delegate and proxyfilter
    m_spacesProxyModel = new SpacesFilterProxyModel(this);
    m_delegate = new SpacesDelegate(m_ui->spacesView, this);
    
    m_ui->spacesView->setItemDelegate(m_delegate);
    
    connect(m_ui->spacesView, SIGNAL(doubleClicked(QModelIndex)), SLOT(setCurrentSpace(QModelIndex)));
}

Dashboard::~Dashboard()
{
}

void Dashboard::initializeDictionaryNames()
{
    // QString str=KStandardDirs::installPath("data"); // works for -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix`
    // QDir dir(str.append("libanalitza/plots"));  // path to the dir where .plots files are installed by analitza

    KStandardDirs KStdDirs;
    QStringList dirList = KStdDirs.findDirs("data","libanalitza");

    QStringList filters("*.plots");

    foreach(const QString &dirPath, dirList) {
        QDir dir=QString(dirPath).append("plots");
        dir.setNameFilters(filters);
        QString key=dir.path().append("/"); // dir path , goes into key section of the map
        QStringList fileList=dir.entryList();
        foreach(const QString &file, fileList) {
            m_DictionaryPathName.insertMulti(key,file);
            m_ui->comboBox->addItem(QFileInfo(file).baseName());
        }
    }

    connect(m_ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(setDictionaryData(int)));

    if(m_DictionaryPathName.empty() || m_DictionaryPathName.keys().empty()) {
        QString error =i18n("Please make sure that the dictionary (.plots) files are correctly installed");
        KMessageBox::error(this,error,i18n("No Dictionary found!"));
        return;
    }
}

void Dashboard::initializePlotsViews()
{
    m_ui->plotsView2D->setShowGrid(false);
}

void Dashboard::setDocument(DataStore* doc)
{
    m_delegate->hideOperatorBar();
    m_delegate->setDocument(doc);
    
    //NOTE avoid re-connection when the doc is updated (see mainapp::loadfile)
    if (doc != m_document)
        connect(m_delegate, SIGNAL(saveDictionary(QModelIndex)), doc,SLOT(saveSpaceAsDictionary(QModelIndex)));

    m_document = doc;

    m_spacesProxyModel->setSourceModel(doc->spacesModel());

    m_ui->spacesView->setModel(m_spacesProxyModel);
    
    m_document->currentPlots()->setFilterSpaceDimension(Dim2D);
    m_ui->plotsView2D->setModel(m_document->currentPlots());
    m_ui->plotsView2D->setSelectionModel(m_document->currentSelectionModel());

    m_document->currentPlots()->setFilterSpaceDimension(Dim3D);
    m_ui->plotsView3D->setModel(m_document->currentPlots());
    m_ui->plotsView3D->setSelectionModel(m_document->currentSelectionModel());
    m_ui->plotsView3D->setUseSimpleRotation(true);

    connect(m_document->currentPlots(), SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(setCurrentPlot(QModelIndex,int)));
    connect(m_document->spacesModel(), SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(setCurrentSpace(QModelIndex,int)));
}

QPixmap Dashboard::thumbnail(Analitza::Dimension dim) const
{
    QPixmap thumbnail;
    switch (dim)
    {
    case Analitza::Dim2D:
        thumbnail = m_ui->plotsView2D->grab();
        break;
    case Analitza::Dim3D: {
        m_ui->plotsView3D->update();
        thumbnail = QPixmap::fromImage(m_ui->plotsView3D->grabFramebuffer(), Qt::ColorOnly);
        break;
    }
    }
    return thumbnail;
}

PlotsView2D* Dashboard::view2d()
{
    return m_ui->plotsView2D;
}

PlotsView3DES* Dashboard::view3d()
{
    return m_ui->plotsView3D;
}

void Dashboard::goHome()
{
    setCurrentIndex(0);
    m_ui->views->setCurrentIndex(0);
    emit showFilter(true);
}

void Dashboard::showDictionary()
{
    m_ui->views->setCurrentIndex(1);
    emit showFilter(false);
}

void Dashboard::setDictionaryData(int ind)
{
    PlotsDictionaryModel *model = m_document->plotsDictionaryModel();
    m_plotdictionarymodel=model;
    model->clear();
    QString fileName=m_ui->comboBox->itemText(ind).append(".plots");
    QString dirPath=m_DictionaryPathName.key(fileName);
    dirPath.append(fileName);
    model->createDictionary(dirPath);
    m_ui->plotsView->setModel(model);
    m_ui->page->setModel(model->plotModel());
    m_ui->page_2->setModel(model->plotModel());
    connect(m_ui->plotsView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(setModelIndex(QModelIndex)));
}

void Dashboard::setModelIndex(const QModelIndex& ind)
{
    m_plotdictionarymodel->setCurrentIndex(ind);
    setPlotsView(m_plotdictionarymodel->dimension());
}

void Dashboard::setPlotsView(Analitza::Dimension dim)
{
    if(dim==Dim2D){
        m_ui->stackedWidget->setCurrentIndex(0);

    } else if (dim==Dim3D){
        m_ui->stackedWidget->setCurrentIndex(1);
    }
}

void Dashboard::importDictionaryClicked()
{
    const QString path = QFileDialog::getOpenFileName(this, i18n( "Open" ), {},
                     i18n( "Dictionary Files (*.plots);;All Files (*)" ) );

    if(!path.isEmpty()) {
        int currentIndex=m_ui->comboBox->count();
        m_ui->comboBox->addItem(QFileInfo(path).baseName());
        m_ui->comboBox->setCurrentIndex(currentIndex);
        m_DictionaryPathName.insertMulti(QFileInfo(path).path().append("/"),QFileInfo(path).fileName());
        setDictionaryData(currentIndex);
    }
}

void Dashboard::showPlotsView2D()
{
    m_ui->plotsViews->setCurrentIndex(0);
}

void Dashboard::showPlotsView3D()
{
    m_ui->plotsViews->setCurrentIndex(1);
}

void Dashboard::exportSpaceSnapshot(Dimension dim)
{
    QString path = QFileDialog::getSaveFileName(this, i18n("Where to export"), {}, i18n("PNG Image File (*.png)"));
    if(!path.isEmpty()){
        if(dim==Dim2D){
            view2d()->toImage(path,PlotsView2D::PNG);
        } else if(dim==Dim3D){
            view3d()->save(QUrl::fromLocalFile(path));
        }
    }
}

void Dashboard::copySpace2DSnapshotToClipboard()
{
    view2d()->snapshotToClipboard();
}

void Dashboard::copySpace3DSnapshotToClipboard()
{
    QClipboard *cb = QApplication::clipboard();
    cb->setImage(view3d()->grabFramebuffer());
}

void Dashboard::filterByText(const QString &text)
{
    m_spacesProxyModel->setFilterText(text);
}

void Dashboard::filterByDimension(Dimensions dim)
{
    static_cast<SpacesDelegate*>(m_ui->spacesView->itemDelegate())->filterEvent();
    m_spacesProxyModel->setFilterDimension(dim);
}

void Dashboard::setCurrentSpace(const QModelIndex &index)
{
    setCurrentIndex(1);

    QModelIndex ind;
    if(index.model()==m_spacesProxyModel)
        ind=m_spacesProxyModel->mapToSource(index);
    else
        ind=index;

    emit spaceActivated(ind.row());

    switch (m_document->spacesModel()->space(ind.row())->dimension())
    {
    case Dim2D:
        m_ui->plotsViews->setCurrentIndex(0);
        //set dialog visible
        emit restoreDictionaryData(Dim2D);
        break;

    case Dim3D:
        m_ui->plotsViews->setCurrentIndex(1);
        //set dialog invisible
        emit restoreDictionaryData(Dim3D);
        break;

    default: break;
    }

    // identification of the space
    if(!(m_spaceindexList.contains(ind)) && m_openclicked){
        m_spaceindexList.append(ind);
        emit plotRequested(ind);
    }

 emit showFilter(false);
}

//FIXME: 3 methods called setCurrentSpace is too much
void Dashboard::setCurrentSpace(const QModelIndex& index, const QModelIndex& old)
{
	Q_UNUSED(old);
    m_document->setCurrentSpace(index.row());
}

void Dashboard::setCurrentSpace(const QModelIndex& index, int row)
{
	Q_UNUSED(index);
//     qDebug() << "sadasadadsd ads";
//     m_widget->spacesView->selectionModel()->setCurrentIndex(m_document->spacesModel()->index(row), QItemSelectionModel::Current);

    m_ui->spacesView->setCurrentIndex(m_document->spacesModel()->index(row));

//     m_document->currentPlots()->setFilterSpaceDimension(m_document->spacesModel()->item(row)->dimension());
//     m_document->currentPlots()->setFilterSpace(m_document->spacesModel()->item(row));

    emit spaceActivated(row);
    
    //BUG qt el filtermodel no respeta iconviewmode al agregar los items ...
//     if (m_document->spacesModel()->rowCount() < 2) // si esta vacio por primera actiualizo el view 
//         m_widget->spacesView->update();
    
    static_cast<SpacesDelegate*>(m_ui->spacesView->itemDelegate())->filterEvent();
}

void Dashboard::setCurrentPlot(const QModelIndex& parent, int start)
{
    Q_ASSERT(!parent.isValid());
    m_document->currentSelectionModel()->clear();
    m_document->currentSelectionModel()->setCurrentIndex(m_document->currentPlots()->index(start,0), QItemSelectionModel::SelectCurrent );
}

void Dashboard::setGridColor(const QColor &color) // used for making the axis visible/invisible
{
    if (m_document->spacesModel()->space(m_document->currentSpace())->dimension()==Dim2D) // 2d space
        m_ui->plotsView2D->setGridColor(color);
    else 
        if (m_document->spacesModel()->space(m_document->currentSpace())->dimension()==Dim3D) // 3d space
        { 
            auto plotter3d = dynamic_cast<Analitza::Plotter3DES*>(view3d());
            plotter3d->setReferencePlaneColor(color);
        }
}

void Dashboard::setGridStyle(int i)
{
    if (m_document->spacesModel()->space(m_document->currentSpace())->dimension()==Dim2D) // 2d space
    {
        switch (i)
        {
            case 0: 
                m_ui->plotsView2D->setShowGrid(false);
                break;
            case 1: 
                m_ui->plotsView2D->setShowGrid(true);
                m_ui->plotsView2D->setGridStyleHint(Analitza::Squares);
                break;
            case 2: 
                m_ui->plotsView2D->setShowGrid(true);
                m_ui->plotsView2D->setGridStyleHint(Analitza::Circles);
                break;
        }
    }
    else 
        if (m_document->spacesModel()->space(m_document->currentSpace())->dimension()==Dim3D) // 3d space
        {
            switch (i)
            {
                case 0: 
                    m_ui->plotsView2D->setShowGrid(false);
                    break;
                case 1: 
                    m_ui->plotsView2D->setShowGrid(true);
                    break;
                case 2: 
                    m_ui->plotsView2D->setShowGrid(true);
                    break;
            }
        }
}

void Dashboard::setPlotsViewGridColor(const QColor &color)
{
    if(m_document->spacesModel()->space(m_document->currentSpace())->dimension()==Dim2D){ // 2d space
        m_ui->plotsView2D->setGridColor(color);
    } else if(m_document->spacesModel()->space(m_document->currentSpace())->dimension()==Dim3D){ // 3d space
        auto plotter3d = dynamic_cast<Analitza::Plotter3DES*>(view3d());
        plotter3d->setReferencePlaneColor(color);
        showPlotsView3D();
    }
}

void Dashboard::getDictionaryClicked()
{
    KNS3::DownloadDialog newStuffDialog(this);
    newStuffDialog.exec();
    KNS3::Entry::List installedentries = newStuffDialog.installedEntries();
}

void Dashboard::activateDictionaryData() {
    setDictionaryData(m_ui->comboBox->currentIndex());
}

#include "moc_dashboard.cpp"
