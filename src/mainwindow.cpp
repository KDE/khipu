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

#include "mainwindow.h"

//Analitza includes
#include <analitza/expression.h>
#include <analitzagui/plotsview2d.h>
#include <analitzagui/plotsview3d.h>
#include <analitzaplot/plotsdictionarymodel.h>
#include <analitzaplot/planecurve.h>
#include <analitzaplot/functiongraph.h>
#include <analitzaplot/plotter3d.h>
#include <analitzaplot/plotsmodel.h>
#include <analitzaplot/plotsfactory.h>

//Qt includes
#include <QBuffer>
#include <QtGui/QDockWidget>
#include <QtGui/QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QDebug>
#include <QFileDialog>
#include <QClipboard>
#include <QSettings>

//KDE includes
#include <KDE/KApplication>
#include <KDE/KLocale>
#include <KDE/KLocalizedString>
#include <KDE/KStandardDirs>
#include <KDE/KAction>
#include <KDE/KActionCollection>
#include <KDE/KStandardAction>
#include <KDE/KStatusBar>
#include <KDE/KFileDialog>
#include <KDE/KMessageBox>
#include <KDE/KToolInvocation>
#include <KIO/NetAccess>
#include <KTemporaryFile>
#include <KToolBar>
#include <KMenuBar>

//QJson includes
#include <qjson/serializer.h>
#include <qjson/parser.h>

//local includes
#include "dictionaryitem.h"
#include "dictionariesmodel.h"
#include "dashboard.h"
#include "plotseditor.h"
#include "datastore.h"
#include "plotsbuilder.h"
#include "spaceinformation.h"
#include "spaceoptions.h"
#include "filter.h"
#include "dictionarycollection.h"

using namespace Analitza;

MainWindow::MainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
{
    m_document = new DataStore(this);

    m_dashboard = new Dashboard(this);
    m_dashboard->setDocument(m_document);

    //para main de dashboard
    connect(m_dashboard, SIGNAL(spaceActivated(int)), SLOT(activateSpace(int)));
    
    //para document de dashboard
    connect(m_dashboard, SIGNAL(spaceActivated(int)), m_document , SIGNAL(spaceActivated(int)));

    setupDocks();
    setupActions();
    setupGUI(Keys | StatusBar | Save | Create, "khipu.rc");

    m_filter = new Filter(this);
    m_filter->setFilterDashboard(m_dashboard);
    
    connect(m_filter, SIGNAL(filterByDimension(Dimensions)), m_dashboard, SLOT(filterByDimension(Dimensions)));
    connect(m_filter, SIGNAL(filterByText(QString)), m_dashboard, SLOT(filterByText(QString)));
    connect(m_dashboard,SIGNAL(plotRequested(QModelIndex)),this,SLOT(createPlot(QModelIndex)));
    connect(m_dashboard,SIGNAL(showFilter(bool)),m_filter,SLOT(setFilterVisible(bool)));

    toolBar("mainToolBar")->addWidget(m_filter);
    setCentralWidget(m_dashboard);
    activateDashboardUi();
    updateRecentFileList();
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    // Do not want to close then just ignore the event
    if(!closeClicked())
        event->ignore();
}

MainWindow::~MainWindow()
{
}

KAction* MainWindow::createAction(const char* name, const QString& text, const QString& iconName, const QKeySequence& shortcut, const QObject* recvr, const char* slot, bool isCheckable, bool checked)
{
    KAction* act = new KAction(this);
    act->setText(text);
    act->setIcon(KIcon(iconName));
    act->setShortcut(shortcut);
    act->setCheckable(isCheckable);
    
    if (isCheckable) 
    {
        act->setChecked(checked);
        
        QObject::connect(act, SIGNAL(toggled(bool)), recvr, slot);
    }
    else
        QObject::connect(act, SIGNAL(triggered()), recvr, slot);

        
    actionCollection()->addAction(name, act);

    return act;
}

void MainWindow::setupDocks()
{
    PlotsBuilder *plotsBuilder = new PlotsBuilder(this);
    m_plotsBuilderDock = new QDockWidget(i18n("&Shortcuts"), this);
    m_plotsBuilderDock->setWidget(plotsBuilder); // plotsbuilder debe ser miembro
    m_plotsBuilderDock->setObjectName("dsfs");
//     m_plotsBuilderDock->setFloating(false);
    m_plotsBuilderDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_plotsBuilderDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    m_plotsBuilderDock->hide();

    plotsBuilder->mapConnection(PlotsBuilder::CartesianGraphCurve, this, SLOT(buildCartesianGraphCurve()));
    plotsBuilder->mapConnection(PlotsBuilder::CartesianImplicitCurve, this, SLOT(buildCartesianImplicitCurve()));
    plotsBuilder->mapConnection(PlotsBuilder::CartesianParametricCurve2D, this, SLOT(buildCartesianParametricCurve2D()));
    plotsBuilder->mapConnection(PlotsBuilder::PolarGraphCurve, this, SLOT(buildPolarGraphCurve()));
    plotsBuilder->mapConnection(PlotsBuilder::CartesianParametricCurve3D, this, SLOT(buildCartesianParametricCurve3D()));
    plotsBuilder->mapConnection(PlotsBuilder::CartesianGraphSurface, this, SLOT(buildCartesianGraphSurface()));
    plotsBuilder->mapConnection(PlotsBuilder::CartesianImplicitSurface, this, SLOT(buildCartesianImplicitSurface()));
    plotsBuilder->mapConnection(PlotsBuilder::CartesianParametricSurface, this, SLOT(buildCartesianParametricSurface()));
    plotsBuilder->mapConnection(PlotsBuilder::CylindricalGraphSurface, this, SLOT(buildCylindricalGraphSurface()));
    plotsBuilder->mapConnection(PlotsBuilder::SphericalGraphSurface, this, SLOT(buildSphericalGraphSurface()));
    
    ///
    
    m_spacePlotsDock = new PlotsEditor(this);
    m_spacePlotsDock->setDocument(m_document);
//     m_spacePlotsDock->setFloating(false);
    m_spacePlotsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_spacePlotsDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    connect(m_spacePlotsDock, SIGNAL(goHome()), SLOT(goHome()));
    connect(m_spacePlotsDock, SIGNAL(sendStatus(QString,int)), statusBar(),SLOT(showMessage(QString,int)));
    connect(m_spacePlotsDock,SIGNAL(updateGridcolor(QColor)),m_dashboard,SLOT(setGridColor(QColor)));
    connect(m_dashboard, SIGNAL(spaceActivated(int)), m_spacePlotsDock, SLOT(setCurrentSpace(int)));
    connect(m_spacePlotsDock,SIGNAL(mapDataChanged()),this,SLOT(autoSaveFile()));
    connect(m_document,SIGNAL(mapDataChanged()),this,SLOT(autoSaveFile()));

    m_spaceInfoDock = new SpaceInformation(this);
    m_spaceOptionsDock = new SpaceOptions(this);

    m_dictionaryDock = new DictionaryCollection(this);
    m_dictionaryDock->setDashboardWidget(m_dashboard);
    m_dictionaryDock->setDocument(m_document);
    m_dictionaryDock->setDictionaryDataMap();
    m_dictionaryDock->setDefaultDictionaries();

    connect(m_dictionaryDock,SIGNAL(mapDataChanged()),this,SLOT(autoSaveFile()));
    connect(m_document, SIGNAL(gridStyleChanged(int)), m_spaceOptionsDock, SLOT(setGridStyleIndex(int)));
    //2d view
    connect(m_spaceOptionsDock, SIGNAL(updateGridStyle(int)), m_dashboard->view2d(), SLOT(useCoorSys(int)));
    connect(m_spaceOptionsDock, SIGNAL(updateGridColor(QColor)), m_dashboard, SLOT(setPlotsViewGridColor(QColor)));
    connect(m_spaceOptionsDock, SIGNAL(setXAxisLabel(QString)), m_dashboard->view2d(), SLOT(setXAxisLabel(QString)));
    connect(m_spaceOptionsDock, SIGNAL(setYAxisLabel(QString)), m_dashboard->view2d(), SLOT(setYAxisLabel(QString)));
    connect(m_spaceOptionsDock, SIGNAL(updateTickScale(QString,qreal,int,int)), m_dashboard->view2d(), SLOT(updateTickScale(QString,qreal,int,int)));
    connect(m_spaceOptionsDock, SIGNAL(ticksShown(QFlags<Qt::Orientation>)), m_dashboard->view2d(), SLOT(setTicksShown(QFlags<Qt::Orientation>)));
    connect(m_spaceOptionsDock, SIGNAL(axesShown(QFlags<Qt::Orientation>)), m_dashboard->view2d(), SLOT(setAxesShown(QFlags<Qt::Orientation>)));
    //3d view
    connect(m_spaceOptionsDock, SIGNAL(axisIsDrawn(bool)), m_dashboard->view3d(), SLOT(setAxisIsDrawn(bool)));
    connect(m_spaceOptionsDock, SIGNAL(gridIsDrawn(bool)), m_dashboard->view3d(), SLOT(setGridIsDrawn(bool)));
    connect(m_spaceOptionsDock, SIGNAL(sceneResized(int)), m_dashboard->view3d(), SLOT(resizeScene(int)));

    connect(m_dashboard, SIGNAL(setDialogSettingsVisible(bool)),m_spaceOptionsDock, SLOT(setWidgetsVisible(bool)));

    //connect(m_spaceOptionsDock, SIGNAL(colorindexChanged(int)),
    addDockWidget(Qt::LeftDockWidgetArea, m_plotsBuilderDock);
    addDockWidget(Qt::LeftDockWidgetArea, m_spacePlotsDock);
    addDockWidget(Qt::LeftDockWidgetArea, m_spaceOptionsDock);
    addDockWidget(Qt::LeftDockWidgetArea, m_spaceInfoDock);
    addDockWidget(Qt::LeftDockWidgetArea, m_dictionaryDock);

    tabifyDockWidget(m_plotsBuilderDock, m_spacePlotsDock);
    tabifyDockWidget(m_spacePlotsDock, m_spaceOptionsDock);
    tabifyDockWidget(m_spaceOptionsDock, m_spaceInfoDock);
    tabifyDockWidget(m_spaceInfoDock,m_dictionaryDock);
}

void MainWindow::setupActions()
{
    //file
    KStandardAction::openNew(this, SLOT(newFile()), actionCollection());
    KStandardAction::open(this, SLOT(openFileClicked()), actionCollection());
    m_openrecent=KStandardAction::openRecent(this, SLOT(openRecentClicked(const KUrl& )), actionCollection());
    connect(m_openrecent,SIGNAL(recentListCleared()),this,SLOT(clearRecentFileList()));

    KStandardAction::save(this, SLOT(saveClicked()), actionCollection());
    KStandardAction::saveAs(this, SLOT(saveAsClicked()), actionCollection());
    KStandardAction::quit(this, SLOT(close()), actionCollection());
    createAction("save_plotImage", i18n("&Save Plot as PNG"),QString(),Qt::CTRL + Qt::Key_P, this, SLOT(savePlot()));
    //TODO
//     KStandardAction::showMenubar(menuBar(), SLOT(setVisible(bool)), actionCollection());

    //edit - dashboard
    createAction("add_space2d", i18n("Add Space &2D"), "add-space2d", Qt::CTRL + Qt::Key_2, this, SLOT(addSpace2D()));
    createAction("add_space3d", i18n("Add Space &3D"), "add-space3d", Qt::CTRL + Qt::Key_3, this, SLOT(addSpace3D()));

    //view - dashboard //TODO Show Plots Dictionary
    m_plotsBuilderDock->toggleViewAction()->setIcon(KIcon("formula"));
    m_plotsBuilderDock->toggleViewAction()->setShortcut(Qt::Key_S);
    m_plotsBuilderDock->toggleViewAction()->setToolTip(i18n("Create a plot in a new space"));
    actionCollection()->addAction("show_plotsbuilder", m_plotsBuilderDock->toggleViewAction());

    createAction("show_plotsdictionary", i18n("Plot &Dictionaries"), "functionhelp", Qt::CTRL + Qt::Key_D, this,
                 SLOT(setVisibleDictionary()));

    //view - space
//     createAction("show_plots_editor", i18n("S&how Space Plots"), "address-book-new", Qt::CTRL + Qt::Key_W, this, SLOT(fooSlot()), true);
    m_spacePlotsDock->toggleViewAction()->setIcon(KIcon("editplots"));
    m_spacePlotsDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_A);
    actionCollection()->addAction("show_plots_editor", m_spacePlotsDock->toggleViewAction());    
    
//     createAction("show_space_info", i18n("&Show Space Information"), "document-properties", Qt::CTRL + Qt::Key_W, this, SLOT(fooSlot()), true);
    m_spaceInfoDock->toggleViewAction()->setIcon(KIcon("dialog-information"));
    m_spaceInfoDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_I);
    actionCollection()->addAction("show_space_info", m_spaceInfoDock->toggleViewAction());       
    
//     createAction("show_plotter_options", i18n("&Show Space Options"), "configure", Qt::CTRL + Qt::Key_W, this, SLOT(fooSlot()), true);
    m_spaceOptionsDock->toggleViewAction()->setIcon(KIcon("diagram-options"));
    m_spaceOptionsDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_O);
    actionCollection()->addAction("show_plotter_options", m_spaceOptionsDock->toggleViewAction());
    
    m_dictionaryDock->toggleViewAction()->setIcon(KIcon("list-add"));
    m_dictionaryDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_D);
    actionCollection()->addAction("show_dictionary_collection", m_dictionaryDock->toggleViewAction());

    //go
    KAction *firstPageAct = KStandardAction::firstPage(this, SLOT(firstPageActClicked()), actionCollection());
    firstPageAct->setText(i18n("&Go First Space"));
    firstPageAct->setIcon(KIcon("go-first-view"));
    firstPageAct->setEnabled(true);

    m_priorAct = KStandardAction::prior(this, SLOT(priorActClicked()), actionCollection());
    m_priorAct->setText(i18n("&Go Previous Space"));
    m_priorAct->setIcon(KIcon("go-previous-view"));
    m_priorAct->setEnabled(true);

    m_nextAct = KStandardAction::next(this, SLOT(nextActClicked()), actionCollection());
    m_nextAct->setText(i18n("&Go Next Space"));
    m_nextAct->setIcon(KIcon("go-next-view"));
    m_nextAct->setEnabled(true);
    
    KAction *lastPageAct = KStandardAction::lastPage(this, SLOT(lastPageActClicked()), actionCollection());
    lastPageAct->setText(i18n("&Go Last Space"));
    lastPageAct->setIcon(KIcon("go-last-view"));
    lastPageAct->setEnabled(true);

    KStandardAction::home(this, SLOT(goHome()), actionCollection());
    //tools dashboard
    createAction("delete_currentspace", i18n("&Remove Current Space"), "list-remove", Qt::CTRL + Qt::Key_W, this, SLOT(removeCurrentSpace()))->setVisible(false);;
    //tools space
    createAction("copy_snapshot", i18n("&Take Snapshot"),"take-snapshot", Qt::CTRL + Qt::SHIFT + Qt::Key_C, this, SLOT(copySnapshot()));
//     createAction("export_snapshot", i18n("&Export Space Snapshot"), "view-preview", Qt::CTRL + Qt::Key_W, this, SLOT(fooSlot()));
    //settings
    KStandardAction::showMenubar(this, SLOT(setMenuBarVisibility(bool)), actionCollection());
    KToggleFullScreenAction *fullScreenAction = KStandardAction::fullScreen(this, SLOT(fullScreenView(bool)), this ,actionCollection());


//     connect(m_dashboard, SIGNAL(saveRequest()), SLOT(saveFile()));
//     connect(m_dashboard, SIGNAL(openRequest()), SLOT(openFile()));

}

void MainWindow::setCurrentSpaceTitle(const QString& str) {
    m_document->spacesModel()->space(m_document->currentSpace())->setTitle(str);
}
void MainWindow::setCurrentSpaceDesc(const QString& desc) {
    m_document->spacesModel()->space(m_document->currentSpace())->setDescription(desc);
}

void MainWindow::fullScreenView (bool isFull)
{
    if(isFull) {
        showFullScreen();
        statusBar()->hide();
        m_spaceOptionsDock->hide();
        m_spaceInfoDock->hide();
        m_spacePlotsDock->hide();
        m_plotsBuilderDock->hide();
        m_dictionaryDock->hide();
        toolBar("mainToolBar")->hide();
    }
    else {
        showNormal();
        statusBar()->show();
        toolBar("mainToolBar")->show();
    }
}

void MainWindow::fooSlot(bool t)
{
    qDebug() << "test slot" << t;
}

void MainWindow::firstPageActClicked()
{
    int currentSpaceRow=m_document->currentSpace();
    if(m_document->spacesModel()->rowCount()==0) {
            KMessageBox::information(this,i18n("There is not any space available to show!"),i18n("No space available"));
    }
    else if(currentSpaceRow==0) {
        QModelIndex firstInd=m_document->spacesModel()->index(0);
        m_dashboard->setCurrentSpace(firstInd);
        KMessageBox::information(this,i18n("Currently,you are on the first space!"),i18n("Information"));
    }
    else if(m_document->spacesModel()->rowCount()>0) {
        QModelIndex firstInd=m_document->spacesModel()->index(0);
        m_dashboard->setCurrentSpace(firstInd);
    }
}

void MainWindow::priorActClicked()
{
    int currentSpaceRow=m_document->currentSpace();
    if(currentSpaceRow==0){
        KMessageBox::information(this,i18n("You are already on the first space!"),i18n("Already on the first space"));
    }
    else if(currentSpaceRow>0) {
        QModelIndex prevInd=m_document->spacesModel()->index(currentSpaceRow-1);
        m_dashboard->setCurrentSpace(prevInd);
    }
}

void MainWindow::nextActClicked()
{
    int currentSpaceRow=m_document->currentSpace();
    int size=m_document->spacesModel()->rowCount();
    if(currentSpaceRow==size-1){
        KMessageBox::information(this,i18n("You are already on the last space!"),i18n("Already on the last space"));
    }
    else if(currentSpaceRow<size-1) {
        QModelIndex nextInd=m_document->spacesModel()->index(currentSpaceRow+1);
        m_dashboard->setCurrentSpace(nextInd);
    }  
}

void MainWindow::lastPageActClicked()
{
    int currentSpaceRow=m_document->currentSpace();
    int size=m_document->spacesModel()->rowCount();
    if(m_document->spacesModel()->rowCount()==0){
        KMessageBox::information(this,i18n("There is not any space available to show!"),i18n("No space available"));
    }
    else if(currentSpaceRow==size-1) {
        QModelIndex lastInd=m_document->spacesModel()->index(size-1);
        m_dashboard->setCurrentSpace(lastInd);
        KMessageBox::information(this,i18n("Currently,you are on the last space!"),i18n("Information"));
    }
    else if(m_document->spacesModel()->rowCount() >0) {
        int size=m_document->spacesModel()->rowCount();
        QModelIndex lastInd=m_document->spacesModel()->index(size-1);
        m_dashboard->setCurrentSpace(lastInd);
    }  
}

void MainWindow::clearRecentFileList()
{
    KConfig config;
    KConfigGroup recentFiles(&config,"file_Recent");
    recentFiles.deleteEntry("recentFileList");
}

void MainWindow::setMenuBarVisibility(bool isShow)
{
    if(isShow) {
        menuBar()->show();
    }
    else {
        KMessageBox::information(this,i18n("Press ctrl + M to make Menubar Visible again"),i18n("Menubar Visibility"));
      menuBar()->hide();
    }
}

void MainWindow::autoSaveFile()
{
    updateThumbnail();

    //no filename available, need to save it as temp
    if(m_fileLocation.isEmpty())
        saveFile(getDefaultAutoSavepath());
    else {
        QString path=QFileInfo(m_fileLocation).dir().path().append("/.").append(QFileInfo(m_fileLocation).baseName().append(".khipu.autosave"));
        saveFile(path);
    }
}

void MainWindow::updateThumbnail()
{

    if (m_document->spacesModel()->rowCount()==0)
        return;

    DictionaryItem *space = m_document->spacesModel()->space(m_document->currentSpace());

    if(space==0)
        return;

    if(space->dimension()!=Analitza::Dim2D && space->dimension()!=Analitza::Dim3D)
        return;

    QPixmap thumbnail;

    switch (space->dimension())
    {
        case Analitza::Dim2D:
            thumbnail = QPixmap::grabWidget(m_dashboard->view2d());
            break;
        case Analitza::Dim3D:
        {
            m_dashboard->view3d()->updateGL();
            m_dashboard->view3d()->setFocus();
            m_dashboard->view3d()->makeCurrent();
            m_dashboard->view3d()->raise();

            QImage image(m_dashboard->view3d()->grabFrameBuffer(true));

            thumbnail = QPixmap::fromImage(image, Qt::ColorOnly);

            break;
        }
        default: break;
    }

    thumbnail = thumbnail.scaled(QSize(PreviewWidth, PreviewHeight), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    space->setThumbnail(thumbnail);
}

bool MainWindow::queryClose()
{
//     if (m_dashboard->isModified())
//     {
//         QString paletteFileName = m_dashboard->fileName();
// 
//         if (paletteFileName.isEmpty())
//             paletteFileName = i18n("Untitled");
// 
//         switch (KMessageBox::warningYesNoCancel(this,
//                                                 i18n( "The document \"%1\" has been modified.\n"
//                                                         "Do you want to save your changes or discard them?", paletteFileName),
//                                                 i18n( "Close Document" ), KStandardGuiItem::save(), KStandardGuiItem::discard()))
//         {
//         case KMessageBox::Yes:
//         {
// 
// 
// 
// 
// 
//             m_dashboard->showDashboard();
// 
//             saveFile();
// 
//             return m_dashboard->isSaved();
//         }
//         case KMessageBox::No :
//             return true;
// 
//         default :
//             return false;
//         }
//     }

    return true;
}

void MainWindow::checkforAutoSavedFile()
{
    QString path = getDefaultAutoSavepath();
    if(openFile(path)) {
        qDebug() << "file is autosaved and it is opening";
    }
}

void MainWindow::newFile()
{
    // if there are not any plots added (i.e. file is completely clean)
    if(m_document->plotsModel()->rowCount()==0){
        QMessageBox::information(this,i18n("No need to create New Window"),i18n("There are not any plots available. So, you do not need to create a new Plot Window"));
        return;
    }

    // creates a new proccess.
    // The commandline arguments is used to create a completely new instance of Khipu(i.e. it does not reload the autosaved file)
    QStringList args;
    args << "ignoreautosavedfile";
    KToolInvocation::kdeinitExec("khipu",args);
}

void MainWindow::openRecentClicked(const KUrl&  name)
{
    // no plots are available , can open in the same proccess.
    if(m_document->plotsModel()->rowCount()==0)
        openFile(name.path());
    else {
        QStringList args;
        args << name.toLocalFile();
        KToolInvocation::kdeinitExec("khipu",args);
    }
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    m_curFile = fileName;
    if (m_curFile.isEmpty())
        return;

    KConfig config;
    KConfigGroup recentFiles(&config,"file_Recent");

    QStringList files =recentFiles.readPathEntry("recentFileList",QStringList());
    files.removeAll(fileName);
    files.prepend(fileName);

    while (files.size() > MaxRecentFiles)
        files.removeLast();

    recentFiles.deleteEntry("recentFileList");
    recentFiles.writePathEntry("recentFileList",files);
    recentFiles.sync();
    updateRecentFileList();
}

void MainWindow::updateRecentFileList()
{
    KConfig config;
    KConfigGroup recentFiles(&config,"file_Recent");
    QStringList files =  recentFiles.readPathEntry("recentFileList",QStringList());
    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    // Traversing in reverse manner will lead to the correct Recently opened file-list
    for (int i = numRecentFiles-1; i >=0 ; i--) {
        QString text=files[i];
        m_openrecent->addUrl(KUrl(text),QFileInfo(text).fileName());
    }
}

bool MainWindow::openFileClicked()
{
    KUrl const url = KFileDialog::getOpenUrl( QDir::currentPath(),
                     i18n( "*.khipu|Khipu Files (*.khipu)\n*|All Files" ), this, i18n( "Open" ) );

    if(url.path().isEmpty()){
        qDebug() << "error in opening file...may be path not found." ;
        return false;
    }

    if(m_document->plotsModel()->rowCount()!=0) {
        QStringList args;
        args << url.toLocalFile();
        KToolInvocation::kdeinitExec("khipu",args);
        return false;
    }
    else
        return openFile(url);
}

bool MainWindow::openFile(const KUrl &url)
{
    if(url.path().isEmpty())
    return false;

    QFile file;
    if (!url.isLocalFile())
    {
        if(!KIO::NetAccess::exists(url,KIO::NetAccess::SourceSide,this))
        {
            KMessageBox::sorry(this,i18n("The file does not exist."));
            return false;
        }
        QString tmpfile;
        if(!KIO::NetAccess::download(url,tmpfile,this))
        {
            KMessageBox::sorry(this,i18n("An error appeared when opening this file (%1)", KIO::NetAccess::lastErrorString()));
            return false;
        }
        file.setFileName(tmpfile);
    }
    else
        file.setFileName(url.toLocalFile());

    if(url.toLocalFile()==getDefaultAutoSavepath())
    {
        // ask for reloading the autosave file
        if(!file.exists())
            return false;
        int answer=KMessageBox::questionYesNo(this,i18n("Do you want to recover the file you have not saved, last time ?"),i18n("Autosaved .khipu file"));

        if(answer!=KMessageBox::Yes)
        {
            QFile file(url.toLocalFile());
            if(file.remove())
                return false;
        }
    }
    else
    {
          QString path=url.toLocalFile();
          // not , current autosave !
          // if(!path.contains(".khipu.autosave")) {
               setCurrentFile(path);
               m_fileLocation=path; // this allows user to save the other work in the file which he/she has just opened.
               changeTitleBar(path);
           //}

           //check for available autosave file
           QString currentautosavepath=getCurrentAutoSavepath(path);
           if(QFile::exists(currentautosavepath)) {
               // ask for reloading the autosave file
               int answer=KMessageBox::questionYesNo(this,i18n("There are some unsaved chanes in the file %1.Do you want to recover them?",QFileInfo(path).baseName()),i18n("Autosaved .khipu file"));

               if(answer==KMessageBox::Yes) {
                   // user wants to open the autosave file.
                   file.setFileName(currentautosavepath);
               }
           }
    }

    if(!file.open(QFile::ReadOnly)) {
        qDebug() << "error in reading";
        if(file.fileName()!=getDefaultAutoSavepath())
                   KMessageBox::sorry(this,i18n("%1 could not be opened", file.fileName()));
        return false;
    }

    qDebug() << "parsing....";

    QJson::Parser parser;
    m_parsedSpaceDetails = parser.parse(&file).toList();
    file.close();

    if(!url.isLocalFile())
            KIO::NetAccess::removeTempFile( file.fileName() );

    if(m_parsedSpaceDetails.isEmpty()) { // if a wrong file is hit
        qDebug() << "problem in parsing ..may be a wrong file";
        return false;
    }

    m_dashboard->m_openclicked=true;

    foreach(QVariant record, m_parsedSpaceDetails) {

        QVariantMap map = record.toMap();

        QString spacename=map.value("name").toString();
        QByteArray image= map.value("image").toByteArray();

        Analitza::Dimension dim=static_cast<Analitza::Dimension> (map.value("dimension").toInt());
        QPixmap thumbnail= toPixmap(image);

        m_document->spacesModel()->addSpace(dim,spacename,QString(),thumbnail);
    }
    m_dashboard->goHome();
    activateDashboardUi();
    return true;
}

QPixmap MainWindow::toPixmap(const QByteArray &bytearray)
{
    QByteArray *imageArray = new QByteArray(QByteArray::fromBase64(bytearray));
    QBuffer imagebuffer(imageArray);
    imagebuffer.open(QIODevice::ReadOnly);
    QImage img;
    img.loadFromData(imagebuffer.data(), "PNG");
    return QPixmap::fromImage(img);
}

void MainWindow::saveClicked()
{
    // Intially when the data is not saved. We would not have the actual file path.
    if(m_fileLocation=="") {
        KUrl url = KFileDialog::getSaveUrl( QDir::homePath(), i18n( "*.khipu|Khipu Files (*.khipu)\n*|All Files" ),this, i18n( "Save As" ) );
        m_fileLocation =url.toLocalFile();
    }

    saveFile(m_fileLocation);
}

bool MainWindow::closeClicked()
{
    QFile autosaveFile(getDefaultAutoSavepath());
    QString path=QFileInfo(m_fileLocation).dir().path().append("/.").append(QFileInfo(m_fileLocation).baseName().append(".khipu.autosave"));
    QFile currentautosaveFile(path);

    if(autosaveFile.exists() || currentautosaveFile.exists()) {
        int answer=KMessageBox::questionYesNoCancel(this,
                                                    i18n("The current file contains some unsaved work.Do you want to save it?"),
                                                    i18n("Warining: Unsaved changes"));
        if(answer==KMessageBox::Yes) {
            saveClicked();
            return true;
        }
        else if(answer==KMessageBox::Cancel) {
            return false;
        }
        else if(answer==KMessageBox::No) {
            // if user selects No , then he/she can restore the work using autosaving feature. But this can be removed !
            return true;
        }
    }
    return true; // just close the application.
}

bool MainWindow::saveAsClicked()
{
    KUrl url = KFileDialog::getSaveUrl( QDir::homePath(), i18n( "*.khipu|Khipu Files (*.khipu)\n*|All Files" ),this, i18n( "Save As" ) );
    if(saveFile(url.toLocalFile())) {
        m_fileLocation =url.toLocalFile();
        return true;
    }
    return false;
}

bool MainWindow::saveFile(const KUrl &url)
{
    QMap<DictionaryItem*, Analitza::PlotItem*> map=m_document->currentDataMap();

    // just starting #no plot is available so no need to save
    if(map.empty())
    {
        qDebug() << "map is empty";

        // no plots are there. so, we dont need autosave file
        QFile tempautosaveFile(getDefaultAutoSavepath());
        tempautosaveFile.remove();

        QString path=QFileInfo(m_fileLocation).dir().path().append("/.").append(QFileInfo(m_fileLocation).baseName().append(".khipu.autosave"));
        QFile currentautosaveFile(path);
        currentautosaveFile.remove();
        return false;
    }

    QList<DictionaryItem*> spaceList=map.uniqueKeys();
    if(spaceList.empty()){
            qDebug() << "list is empty";
            return false;
    }

    QVariantList plotspace_list;

    foreach(DictionaryItem* space, spaceList) {
             QString spaceName = space->title();
             QPixmap thumbnail = space->thumbnail();
             int dim = space->dimension();

             QVariantList subplot_list;
             subplot_list.clear();

             QVariantMap plotspace;
             plotspace.insert("name",spaceName);
             plotspace.insert("dimension",dim);
             plotspace.insert("image",thumbnailtoByteArray(thumbnail));

             QList<Analitza::PlotItem*> plotList=map.values(space);

             foreach(Analitza::PlotItem* plotitem, plotList) {
                   QString plotName=plotitem->name();
                   QString plotExpression=plotitem->expression().toString();
                   QColor plotcolor=plotitem->color();

                   QVariantMap plot;
                   plot.insert("name",plotName);
                   plot.insert("expression",plotExpression);
                   plot.insert("color",plotcolor);

                   if(dim==2) {

                          Analitza::FunctionGraph*functiongraph=static_cast<Analitza::FunctionGraph*> (plotitem);
                          //need to fix this in analitza
                          /* double arg1min=functiongraph->interval(functiongraph->parameters().at(0)).first;
                          double arg1max=functiongraph->interval(functiongraph->parameters().at(0)).second;
                         */
                          double arg1min=-5;
                          double arg1max=5;
                          plot.insert("arg1min",arg1min);
                          plot.insert("arg1max",arg1max);
                   }

                    //writehere
                    subplot_list << plot;

                }

             QJson::Serializer subserializer;
             QByteArray subjson = subserializer.serialize(subplot_list);
             plotspace.insert("plots",subjson);
             plotspace_list << plotspace;
        }

QJson::Serializer serializer;
QByteArray json = serializer.serialize(plotspace_list);


    if(!url.isLocalFile())
    {
        KTemporaryFile tmpfile;
        if(!tmpfile.open())
        {
            qDebug() << "Could not open " << KUrl(tmpfile.fileName()).toLocalFile() << " for writing.\n";
            return false;
        }
        QTextStream out(&tmpfile);
        out << json;
        out.flush();
        if (!KIO::NetAccess::upload(tmpfile.fileName(),url,this))
        {
            qDebug() << "Could not open " << url.prettyUrl() << " for writing ("<<KIO::NetAccess::lastErrorString()<<").\n";
            return false;
        }
    }
    else
    {
        if(!url.hasPath()){
            qDebug() << "error in saving file...may be path not found." ;
            return false;
        }

        QFile file(url.toLocalFile());
        qDebug() << "path: " << url.toLocalFile();

        // saved action clicked by the user , this is not the autosave case
        QString currentautosavepath=getCurrentAutoSavepath(m_fileLocation);
        if(url.toLocalFile()!=getDefaultAutoSavepath() && url.toLocalFile()!=currentautosavepath) {
            if(!file.open(QFile::WriteOnly | QFile::Text)){
                qDebug() << "Error in writing";
                return false;
            }

            // remove the auto save file (can be improved later) -> better to use function (TODO)
            QFile tempautosaveFile(getDefaultAutoSavepath());
            tempautosaveFile.remove();
            QFile currentautosaveFile(currentautosavepath);
            currentautosaveFile.remove();

            setCurrentFile(url.toLocalFile());
            statusBar()->showMessage(i18n("File : %1 [%2] is saved successfully",
                                          QFileInfo(url.toLocalFile()).fileName(),url.toLocalFile(),10000));
            changeTitleBar(url.toLocalFile());
        }

        // autosave case
        else {
            if(!file.open(QFile::WriteOnly | QFile::Text)){
                qDebug() << "Error in writing";
                return false;
            }
        }

    QTextStream out(&file);
    out << json;
    file.close();
    }
    return true;
}


void MainWindow::changeTitleBar(const QString& path)
{
    window()->setWindowTitle(QFileInfo(path).fileName().append(" - Khipu"));
}

QString MainWindow::getCurrentAutoSavepath(const QString &path)
{
    if(!path.isEmpty())
        return QFileInfo(path).dir().path().append("/.").append(QFileInfo(path).baseName().append(".autosave"));
    return QString();
}

QString MainWindow::getDefaultAutoSavepath()
{
    return QDir::homePath().append("/.Temp.khipu.autosave");
}

void MainWindow::savePlot()
{
    if(m_document->currentSpace()==-1)
        return;
    Dimension dim = m_document->spacesModel()->space(m_document->currentSpace())->dimension();
    m_dashboard->exportSpaceSnapshot(dim);
}

void MainWindow::activateSpace(int spaceidx)
{
    activateSpaceUi();
    
    m_spacePlotsDock->reset(true);
    
    //clear space infor widget 
//     m_spaceInfoDock->clear();
    DictionaryItem *space = m_document->spacesModel()->space(spaceidx);
    m_spaceInfoDock->setInformation(space->title(), space->description());
    
    m_spaceOptionsDock->setDimension(space->dimension());
}

void MainWindow::activateDashboardUi()
{
    //menubar
    //edit
    action("add_space2d")->setVisible(true);
    action("add_space3d")->setVisible(true);
    
    if (m_document->spacesModel()->rowCount()>0)
        action("delete_currentspace")->setVisible(true);        
    
    //view
    action("show_plotsbuilder")->setVisible(true);
    action("show_plotsdictionary")->setVisible(true);
    action("show_plots_editor")->setVisible(false);
    action("show_space_info")->setVisible(false);
    action("show_plotter_options")->setVisible(false);
    action("show_dictionary_collection")->setVisible(false);

    //go
    action("go_home")->setVisible(false);
    //tools
    action("copy_snapshot")->setVisible(false);
    action("save_plotImage")->setVisible(false);
//     action("export_snapshot")->setVisible(false);
    
    //toolbars
//     toolBar("mainToolBar")->show();
//     toolBar("spaceToolBar")->hide();

    //docks
    // primero oculto los widgets sino el size de los que voy a ocultar interfieren y la mainwnd se muestra muy grande
    m_spacePlotsDock->hide();
    m_spaceInfoDock->hide();
    m_spaceOptionsDock->hide();
    m_plotsBuilderDock->hide(); 
    m_dictionaryDock->hide();
    m_priorAct->setVisible(false);
    m_nextAct->setVisible(false);
}

void MainWindow::activateSpaceUi()
{
    m_dashboard->setCurrentIndex(1);

    //menu
    //edit
    action("add_space2d")->setVisible(false);
    action("add_space3d")->setVisible(false);
    action("delete_currentspace")->setVisible(false);        
    //view
    action("show_plotsbuilder")->setVisible(false);
    action("show_plotsdictionary")->setVisible(false);
    action("show_plots_editor")->setVisible(true);
    action("show_space_info")->setVisible(true);
    action("show_plotter_options")->setVisible(true);
    action("show_dictionary_collection")->setVisible(true);

    //go
    action("go_home")->setVisible(true);    
    //tools
    action("copy_snapshot")->setVisible(true);
    action("save_plotImage")->setVisible(true);
//     action("export_snapshot")->setVisible(true);
    
    //toolbars
//     toolBar("mainToolBar")->hide();
//     toolBar("spaceToolBar")->show();

    //docks
    //lo mismo ... primero hides luego show
    m_plotsBuilderDock->hide();
    m_spacePlotsDock->show();
    m_spaceInfoDock->show();
    m_spaceOptionsDock->show();
    m_dictionaryDock->show();
    m_priorAct->setVisible(true);
    m_nextAct->setVisible(true);
}

void MainWindow::copySnapshot()
{
    DictionaryItem *space = m_document->spacesModel()->space(m_document->currentSpace());
    switch (space->dimension())
    {
        case Analitza::Dim2D: m_dashboard->copySpace2DSnapshotToClipboard(); break;
        case Analitza::Dim3D: m_dashboard->copySpace3DSnapshotToClipboard(); break;
        default: break;
    }
    statusBar()->showMessage(i18n("The diagram was copied to clipboard"), 2500);
}

void MainWindow::setVisibleDictionary()
{
    
        //menu
        //edit 
        action("add_space2d")->setVisible(false);
        action("add_space3d")->setVisible(false);
        action("delete_currentspace")->setVisible(false);        
        //view
        action("show_plotsbuilder")->setVisible(false);
        action("show_plots_editor")->setVisible(false);
        action("show_space_info")->setVisible(false);
        action("show_plotter_options")->setVisible(false);
        action("show_dictionary_collection")->setVisible(false);
        //go
        action("show_plotsdictionary")->setVisible(false); 
        action("go_home")->setVisible(true);  
        
        //tools
        action("copy_snapshot")->setVisible(false);
        action("save_plotImage")->setVisible(false);
//         action("export_snapshot")->setVisible(false);
        
        //toolbars
//         toolBar("mainToolBar")->show();
//         toolBar("spaceToolBar")->show();

        //docks
        //lo mismo ... primero hides luego show
        m_plotsBuilderDock->hide();
        m_spacePlotsDock->hide();
        m_spaceInfoDock->hide();
        m_spaceOptionsDock->hide();
    m_dictionaryDock->hide();
    m_dashboard->showDictionary();
    m_priorAct->setVisible(false);
    m_nextAct->setVisible(false);;
}

void MainWindow::addSpace2D()
{
    m_currentSpaceDim=2;
    //m_totalSpaces++;

    activateSpaceUi();

    m_dashboard->showPlotsView2D();
    m_document->spacesModel()->addSpace(Analitza::Dim2D, i18n("Untitled %1", m_document->spacesModel()->rowCount()+1));
    m_filter->setFilterVisible(false);
    m_spaceOptionsDock->setWidgetsVisible(true);
}

void MainWindow::addSpace3D()
{
    m_currentSpaceDim=3;
    //m_totalSpaces++;

    activateSpaceUi();
    
    m_dashboard->showPlotsView3D();

    m_document->spacesModel()->addSpace(Analitza::Dim3D, i18n("Untitled %1", m_document->spacesModel()->rowCount()+1));
    m_filter->setFilterVisible(false);
    m_spaceOptionsDock->setWidgetsVisible(false);
}

void MainWindow::removeCurrentSpace()
{
    m_document->removeCurrentSpace();
    
    if (m_document->spacesModel()->rowCount() == 0)
        action("delete_currentspace")->setVisible(false);
}


//NOTE se emite cuando se regresa de un space ... aqui se debe guardar la imforacion del space
void MainWindow::goHome()
{
    if (m_dashboard->currentIndex() != 0) // si no esta en modo dashboard
    {
        ///guardando space info
        
        DictionaryItem *space = m_document->spacesModel()->space(m_document->currentSpace());

        space->stamp(); // marcamos la fecha y hora de ingreso al space
        space->setTitle(m_spaceInfoDock->title());
        space->setDescription(m_spaceInfoDock->description());
        
        QPixmap thumbnail; 

        switch (space->dimension())
        {
            case Analitza::Dim2D:
                thumbnail = QPixmap::grabWidget(m_dashboard->view2d());
                break;
            case Analitza::Dim3D:
            {
                m_dashboard->view3d()->updateGL();
                m_dashboard->view3d()->setFocus();
                m_dashboard->view3d()->makeCurrent();
                m_dashboard->view3d()->raise();
        
                QImage image(m_dashboard->view3d()->grabFrameBuffer(true));

                thumbnail = QPixmap::fromImage(image, Qt::ColorOnly);

                break;
            }
            default: break;
        }

        thumbnail = thumbnail.scaled(QSize(PreviewWidth, PreviewHeight), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);   
        space->setThumbnail(thumbnail);

        //QByteArray imageByteArray=thumbnailtoByteArray(thumbnail);

        if(!m_spacenameList.contains(space->title())){
        //    m_imageList.append(imageByteArray);
            m_spacenameList.append(space->title());
           // m_savedSpaces++;
        }
        else {
          //  int ind=m_spacenameList.indexOf(space->title());
           //  m_imageList.replace(ind,imageByteArray);
        }
    }
    m_dashboard->goHome();
    activateDashboardUi();
}

QByteArray MainWindow::thumbnailtoByteArray(const QPixmap &thumbnail)
{
    QImage image = thumbnail.toImage();
    QByteArray imageArray;
    QBuffer buffer(&imageArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QByteArray encodedImage=buffer.data().toBase64();
    return encodedImage;
}

void MainWindow::buildCartesianGraphCurve()
{
    addSpace2D();
    m_spacePlotsDock->buildCartesianGraphCurve(true);
}

void MainWindow::buildCartesianImplicitCurve()
{
    addSpace2D();
    m_spacePlotsDock->buildCartesianImplicitCurve(true); 
}

void MainWindow::buildCartesianParametricCurve2D()
{
    addSpace2D();
    m_spacePlotsDock->buildCartesianParametricCurve2D(true);    
}

void MainWindow::buildPolarGraphCurve()
{
    addSpace2D();
    m_spacePlotsDock->buildPolarGraphCurve(true);
}


void MainWindow::buildCartesianParametricCurve3D()
{
    addSpace3D();
    m_spacePlotsDock->buildCartesianParametricCurve3D(true);
}

void MainWindow::buildCartesianGraphSurface()
{
    addSpace3D();
    m_spacePlotsDock->buildCartesianGraphSurface(true);    
}

void MainWindow::buildCartesianImplicitSurface()
{
    addSpace3D();
    m_spacePlotsDock->buildCartesianImplicitSurface(true);   
}

void MainWindow::buildCartesianParametricSurface()
{
    addSpace3D();
    m_spacePlotsDock->buildCartesianParametricSurface(true);   
}

void MainWindow::buildCylindricalGraphSurface()
{
    addSpace3D();
    m_spacePlotsDock->buildCylindricalGraphSurface(true);    
}

void MainWindow::buildSphericalGraphSurface()
{
    addSpace3D();   
    m_spacePlotsDock->buildSphericalGraphSurface(true);
}

void MainWindow::createPlot(const QModelIndex &ind)
{
    QJson::Parser parser;
    QVariantMap map = m_parsedSpaceDetails.at(ind.row()).toMap(); // corresponding space entry

    Analitza::Dimension dim=static_cast<Analitza::Dimension> (map.value("dimension").toInt());
    QVariantList plotList= parser.parse(map.value("plots").toByteArray()).toList();

    qDebug() << "plots data";

    foreach(QVariant plot, plotList) {

        QVariantMap plotmap = plot.toMap();
        QString plotname=plotmap.value("name").toString();
        QString ploteqn=plotmap.value("expression").toString();
        QColor plotcolor=plotmap.value("color").toString();

        double arg1min=plotmap.value("arg1min").toDouble();
        double arg1max=plotmap.value("arg1max").toDouble();

        QStringList errors;

        PlotBuilder req = PlotsFactory::self()->requestPlot(Analitza::Expression(QString(ploteqn)), dim);

        if (req.canDraw()) {

            FunctionGraph *item = 0;
            item = req.create(plotcolor, plotname);

            if(dim==Dim2D)
                item->setInterval(item->parameters().first(), arg1min, arg1max);

            m_document->plotsModel()->addPlot(item);
        }

        else
            errors = req.errors();
    }
}
