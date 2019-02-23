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
#include <analitzagui/plotsview3d_es.h>
#include <analitzaplot/plotsdictionarymodel.h>
#include <analitzaplot/planecurve.h>
#include <analitzaplot/functiongraph.h>
#include <analitzaplot/plotter3d_es.h>
#include <analitzaplot/plotsmodel.h>
#include <analitzaplot/plotsfactory.h>

//Qt includes
#include <QBuffer>
#include <QDockWidget>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QDebug>
#include <QFileDialog>
#include <QClipboard>
#include <QSettings>
#include <QJsonDocument>
#include <QApplication>

//KDE includes
#include <KLocalizedString>
#include <KStandardDirs>
#include <KActionCollection>
#include <KStandardAction>
#include <KStatusBar>
#include <KMessageBox>
#include <KToolInvocation>
#include <KIO/NetAccess>
#include <KTemporaryFile>
#include <KToolBar>
#include <KMenuBar>
#include <KConfigGroup>

//local includes
#include "spaceitem.h"
#include "spacesmodel.h"
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

    //From Main Dashboard to the activated space UI
    connect(m_dashboard, SIGNAL(spaceActivated(int)), SLOT(activateSpace(int)));

    connect(m_dashboard, SIGNAL(spaceActivated(int)), m_document , SIGNAL(spaceActivated(int)));

    setupDocks();
    setupActions();
    setupGUI(Keys | StatusBar | Save | Create, "khipu.rc");

    m_filter = new Filter(this);
    m_filter->setFilterDashboard(m_dashboard);

    connect(m_filter, SIGNAL(filterByText(QString)), m_dashboard, SLOT(filterByText(QString)));
    connect(m_dashboard, SIGNAL(plotRequested(QModelIndex)), this, SLOT(createPlot(QModelIndex)));
    connect(m_dashboard, SIGNAL(showFilter(bool)), m_filter, SLOT(setFilterVisible(bool)));
    connect(m_dashboard, SIGNAL(restoreDictionaryData(Analitza::Dimension)), m_dictionaryDock, SLOT(setSpaceDimension(Analitza::Dimension)));

    //Main tool bar for Khipu
    toolBar("mainToolBar")->addWidget(m_filter);
    setCentralWidget(m_dashboard);
    activateDashboardUi();

    //Used to update the recent file list
    updateRecentFileList();
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    // Do not want to close then just ignore the event
    if (!closeClicked())
        event->ignore();
}

MainWindow::~MainWindow()
{
}

QAction* MainWindow::createAction(const char* name, const QString& text, const QString& iconName, const QKeySequence& shortcut, const QObject* recvr, const char* slot, bool isCheckable, bool checked)
{
    auto act = new QAction(this);
    act->setText(text);
    act->setIcon(QIcon::fromTheme(iconName));
    act->setShortcut(shortcut);
    act->setCheckable(isCheckable);

    if (isCheckable)
    {
        act->setChecked(checked);
        QObject::connect(act, SIGNAL(toggled(bool)), recvr, slot);
    } else
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
    m_plotsBuilderDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_plotsBuilderDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    m_plotsBuilderDock->hide();

    //Connecting the buttons to create a specific plot
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

    //Plot-Editing dock
    m_spacePlotsDock = new PlotsEditor(this);
    m_spacePlotsDock->setDocument(m_document);
    m_spacePlotsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_spacePlotsDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    connect(m_spacePlotsDock, SIGNAL(goHome()), SLOT(goHome()));
    connect(m_spacePlotsDock, SIGNAL(sendStatus(QString, int)), statusBar(), SLOT(showMessage(QString, int)));
    connect(m_spacePlotsDock, SIGNAL(updateGridcolor(QColor)), m_dashboard, SLOT(setGridColor(QColor)));
    connect(m_dashboard, SIGNAL(spaceActivated(int)), m_spacePlotsDock, SLOT(setCurrentSpace(int)));
    connect(m_spacePlotsDock, SIGNAL(mapDataChanged()), this, SLOT(autoSaveFile()));
    connect(m_document, SIGNAL(mapDataChanged()), this, SLOT(autoSaveFile()));

    //Space information dock
    m_spaceInfoDock = new SpaceInformation(this);
    m_spaceOptionsDock = new SpaceOptions(this);

    //Plot-Dictionary Dock
    m_dictionaryDock = new DictionaryCollection(this);
    m_dictionaryDock->setDashboardWidget(m_dashboard);
    m_dictionaryDock->setDocument(m_document);
    m_dictionaryDock->setDictionaryDataMap();
    m_dictionaryDock->setDefaultDictionaries();

    connect(this, SIGNAL(Spaceinserted(Analitza::Dimension)), m_dictionaryDock, SLOT(setSpaceDimension(Analitza::Dimension)));
    connect(m_dictionaryDock, SIGNAL(mapDataChanged()), this, SLOT(autoSaveFile()));
    connect(m_document, SIGNAL(gridStyleChanged(int)), m_spaceOptionsDock, SLOT(setGridStyleIndex(int)));

    //Signal/slots for 2d Space
    connect(m_spaceOptionsDock, SIGNAL(updateGridStyle(int)), m_dashboard, SLOT(setGridStyle(int)));
    connect(m_spaceOptionsDock, SIGNAL(updateGridColor(QColor)), m_dashboard, SLOT(setPlotsViewGridColor(QColor)));
    connect(m_spaceOptionsDock, SIGNAL(setXAxisLabel(QString)), m_dashboard->view2d(), SLOT(setXAxisLabel(QString)));
    connect(m_spaceOptionsDock, SIGNAL(setYAxisLabel(QString)), m_dashboard->view2d(), SLOT(setYAxisLabel(QString)));
    connect(m_spaceOptionsDock, SIGNAL(ticksFormatChanged(Analitza::TicksFormat)), m_dashboard->view2d(), SLOT(setTicksFormat(Analitza::TicksFormat)));
    connect(m_spaceOptionsDock, SIGNAL(ticksShown(QFlags<Qt::Orientation>)), m_dashboard->view2d(), SLOT(setTicksShown(QFlags<Qt::Orientation>)));
    connect(m_spaceOptionsDock, SIGNAL(axesShown(QFlags<Qt::Orientation>)), m_dashboard->view2d(), SLOT(setAxesShown(QFlags<Qt::Orientation>)));

    //Signal/slots for 3d Space
   // connect(m_spaceOptionsDock, SIGNAL(axisIsDrawn(bool)), m_dashboard->view3d(), SLOT(setAxisIsDrawn(bool)));
   // connect(m_spaceOptionsDock, SIGNAL(gridIsDrawn(bool)), m_dashboard->view3d(), SLOT(setGridIsDrawn(bool)));
   // connect(m_spaceOptionsDock, SIGNAL(sceneResized(int)), m_dashboard->view3d(), SLOT(resizeScene(int)));

    addDockWidget(Qt::LeftDockWidgetArea, m_plotsBuilderDock);
    addDockWidget(Qt::LeftDockWidgetArea, m_spacePlotsDock);
    addDockWidget(Qt::LeftDockWidgetArea, m_spaceOptionsDock);
    addDockWidget(Qt::LeftDockWidgetArea, m_spaceInfoDock);
    addDockWidget(Qt::LeftDockWidgetArea, m_dictionaryDock);

    tabifyDockWidget(m_plotsBuilderDock, m_spacePlotsDock);
    tabifyDockWidget(m_spacePlotsDock, m_spaceOptionsDock);
    tabifyDockWidget(m_spaceOptionsDock, m_spaceInfoDock);
    tabifyDockWidget(m_spaceInfoDock, m_dictionaryDock);
}

void MainWindow::setupActions()
{
    //file
    KStandardAction::openNew(this, SLOT(newFile()), actionCollection());
    KStandardAction::open(this, SLOT(openFileClicked()), actionCollection());
    m_openrecent=KStandardAction::openRecent(this, SLOT(openRecentClicked(const QUrl& )), actionCollection());
    connect(m_openrecent, SIGNAL(recentListCleared()), this, SLOT(clearRecentFileList()));

    KStandardAction::save(this, SLOT(saveClicked()), actionCollection());
    KStandardAction::saveAs(this, SLOT(saveAsClicked()), actionCollection());
    KStandardAction::quit(this, SLOT(close()), actionCollection());
    createAction("save_plotImage", i18n("&Save Plot as PNG"), QString(), Qt::CTRL + Qt::Key_P, this, SLOT(savePlot()));

    //edit - dashboard
    createAction("add_space2d", i18n("Add Space &2D"), "add-space2d", Qt::CTRL + Qt::Key_2, this, SLOT(addSpace2D()));
    createAction("add_space3d", i18n("Add Space &3D"), "add-space3d", Qt::CTRL + Qt::Key_3, this, SLOT(addSpace3D()));

    //view - dashboard
    m_plotsBuilderDock->toggleViewAction()->setIcon(QIcon::fromTheme("formula"));
    m_plotsBuilderDock->toggleViewAction()->setShortcut(Qt::Key_S);
    m_plotsBuilderDock->toggleViewAction()->setToolTip(i18n("Create a plot in a new space"));
    actionCollection()->addAction("show_plotsbuilder", m_plotsBuilderDock->toggleViewAction());

    createAction("show_plotsdictionary", i18n("Plot &Dictionaries"), "functionhelp", Qt::CTRL + Qt::Key_D, this,
                 SLOT(setVisibleDictionary()));

    //view - space
//     createAction("show_plots_editor", i18n("S&how Space Plots"), "address-book-new", Qt::CTRL + Qt::Key_W, this, SLOT(fooSlot()), true);
    m_spacePlotsDock->toggleViewAction()->setIcon(QIcon::fromTheme("editplots"));
    m_spacePlotsDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_A);
    actionCollection()->addAction("show_plots_editor", m_spacePlotsDock->toggleViewAction());

//     createAction("show_space_info", i18n("&Show Space Information"), "document-properties", Qt::CTRL + Qt::Key_W, this, SLOT(fooSlot()), true);
    m_spaceInfoDock->toggleViewAction()->setIcon(QIcon::fromTheme("dialog-information"));
    m_spaceInfoDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_I);
    actionCollection()->addAction("show_space_info", m_spaceInfoDock->toggleViewAction());

//     createAction("show_plotter_options", i18n("&Show Space Options"), "configure", Qt::CTRL + Qt::Key_W, this, SLOT(fooSlot()), true);
    m_spaceOptionsDock->toggleViewAction()->setIcon(QIcon::fromTheme("diagram-options"));
    m_spaceOptionsDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_O);
    actionCollection()->addAction("show_plotter_options", m_spaceOptionsDock->toggleViewAction());

    m_dictionaryDock->toggleViewAction()->setIcon(QIcon::fromTheme("list-add"));
    m_dictionaryDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_D);
    actionCollection()->addAction("show_dictionary_collection", m_dictionaryDock->toggleViewAction());

    //go
    QAction *firstPageAct = KStandardAction::firstPage(this, SLOT(firstPageActClicked()), actionCollection());
    firstPageAct->setText(i18n("&Go First Space"));
    firstPageAct->setIcon(QIcon::fromTheme("go-first-view"));
    firstPageAct->setEnabled(true);

    m_priorAct = KStandardAction::prior(this, SLOT(priorActClicked()), actionCollection());
    m_priorAct->setText(i18n("&Go Previous Space"));
    m_priorAct->setIcon(QIcon::fromTheme("go-previous-view"));
    m_priorAct->setEnabled(true);

    m_nextAct = KStandardAction::next(this, SLOT(nextActClicked()), actionCollection());
    m_nextAct->setText(i18n("&Go Next Space"));
    m_nextAct->setIcon(QIcon::fromTheme("go-next-view"));
    m_nextAct->setEnabled(true);

    QAction *lastPageAct = KStandardAction::lastPage(this, SLOT(lastPageActClicked()), actionCollection());
    lastPageAct->setText(i18n("&Go Last Space"));
    lastPageAct->setIcon(QIcon::fromTheme("go-last-view"));
    lastPageAct->setEnabled(true);

    KStandardAction::home(this, SLOT(goHome()), actionCollection());
    //tools dashboard
    createAction("delete_currentspace", i18n("&Remove Current Space"), "list-remove", Qt::CTRL + Qt::Key_W, this, SLOT(removeCurrentSpace()))->setVisible(false);;
    //tools space
    createAction("copy_snapshot", i18n("&Take Snapshot"), "take-snapshot", Qt::CTRL + Qt::SHIFT + Qt::Key_C, this, SLOT(copySnapshot()));
    m_getdictionaryAct=createAction("get_dictionary", i18n("&Get Plot-Dictionary files"), "get-hot-new-stuff", Qt::CTRL + Qt::SHIFT + Qt::Key_G, m_dashboard, SLOT(getDictionaryClicked()));
    m_importdictionaryAct=createAction("import_dictionary", i18n("&Import Plot-Dictionary file"), "document-import", Qt::CTRL + Qt::SHIFT + Qt::Key_I, m_dashboard, SLOT(importDictionaryClicked()));

    //     createAction("export_snapshot", i18n("&Export Space Snapshot"), "view-preview", Qt::CTRL + Qt::Key_W, this, SLOT(fooSlot()));
    //settings
    KStandardAction::showMenubar(this, SLOT(setMenuBarVisibility(bool)), actionCollection());
    KStandardAction::fullScreen(this, SLOT(fullScreenView(bool)), this , actionCollection());
}

//Saving a space title
void MainWindow::setCurrentSpaceTitle(const QString& str)
{
    m_document->spacesModel()->space(m_document->currentSpace())->setName(str);
}

//Saving a space description
void MainWindow::setCurrentSpaceDesc(const QString& desc)
{
    m_document->spacesModel()->space(m_document->currentSpace())->setDescription(desc);
}

void MainWindow::fullScreenView (bool isFull)
{
    if (isFull) {
        showFullScreen();
        statusBar()->hide();
        m_spaceOptionsDock->hide();
        m_spaceInfoDock->hide();
        m_spacePlotsDock->hide();
        m_plotsBuilderDock->hide();
        m_dictionaryDock->hide();
    } else {
        showNormal();
        statusBar()->show();
    }
    toolBar("mainToolBar")->setVisible(!isFull);
}

void MainWindow::firstPageActClicked()
{
    m_getdictionaryAct->setVisible(false);
    m_importdictionaryAct->setVisible(false);

    int currentSpaceRow=m_document->currentSpace();

    if (m_document->spacesModel()->rowCount()==0) { //For empty space model
        statusBar()->showMessage(i18n("There is not any space available to show!"), MessageDuration);
    } else if (currentSpaceRow==0) { //Already on the first space
        QModelIndex firstInd=m_document->spacesModel()->index(0);
        m_dashboard->setCurrentSpace(firstInd);
        statusBar()->showMessage(i18n("Currently you are on the first space!"), MessageDuration);
    } else if (m_document->spacesModel()->rowCount()>0) {
        QModelIndex firstInd=m_document->spacesModel()->index(0);
        m_dashboard->setCurrentSpace(firstInd);
    }
}

void MainWindow::priorActClicked()
{
    int currentSpaceRow=m_document->currentSpace();
    if (currentSpaceRow==0) {
        statusBar()->showMessage(i18n("You are already on the first space!"), MessageDuration);
    } else if (currentSpaceRow>0) {
        QModelIndex prevInd=m_document->spacesModel()->index(currentSpaceRow-1);
        m_dashboard->setCurrentSpace(prevInd);
    }
}

void MainWindow::nextActClicked()
{
    int currentSpaceRow=m_document->currentSpace();
    int size=m_document->spacesModel()->rowCount();
    if (currentSpaceRow==size-1) {
        statusBar()->showMessage(i18n("You are already on the last space!"), MessageDuration);
    } else if (currentSpaceRow<size-1) {
        QModelIndex nextInd=m_document->spacesModel()->index(currentSpaceRow+1);
        m_dashboard->setCurrentSpace(nextInd);
    }
}

void MainWindow::lastPageActClicked()
{
    m_getdictionaryAct->setVisible(false);
    m_importdictionaryAct->setVisible(false);

    int currentSpaceRow=m_document->currentSpace();
    int size=m_document->spacesModel()->rowCount();

    if (m_document->spacesModel()->rowCount()==0) { //For empty space model
        statusBar()->showMessage(i18n("There is not any space available to show!"), MessageDuration);
    } else if (currentSpaceRow==size-1) { //Already on the last space
        QModelIndex lastInd=m_document->spacesModel()->index(size-1);
        m_dashboard->setCurrentSpace(lastInd);
        statusBar()->showMessage(i18n("Currently you are on the last space!"), MessageDuration);
    } else if (m_document->spacesModel()->rowCount() >0) {
        int size=m_document->spacesModel()->rowCount();
        QModelIndex lastInd=m_document->spacesModel()->index(size-1);
        m_dashboard->setCurrentSpace(lastInd);
    }
}

void MainWindow::clearRecentFileList()
{
    KConfig config;
    KConfigGroup recentFiles(&config, "file_Recent");
    recentFiles.deleteEntry("recentFileList");
}

void MainWindow::setMenuBarVisibility(bool isShow)
{
    if (isShow) {
        menuBar()->show();
    } else {
        KMessageBox::information(this, i18n("Press Ctrl + M to make menubar visible again"), i18n("Menubar Visibility"));
        menuBar()->hide();
    }
}

void MainWindow::autoSaveFile()
{
    updateThumbnail();

    //no filename available, need to save it as temporary , filename used here is ".khipu.autosave"
    if (m_fileLocation.isEmpty()) {
        saveFile(getDefaultAutoSavepath());
    } else {
        QString path=QFileInfo(m_fileLocation).dir().path().append("/.").append(QFileInfo(m_fileLocation).baseName().append(".khipu.autosave"));
        saveFile(QUrl::fromLocalFile(path));
    }
}

//Update thumbnail whenever the plots are changed.
void MainWindow::updateThumbnail()
{

    if (m_document->spacesModel()->rowCount()==0)
        return;

    SpaceItem *space = m_document->spacesModel()->space(m_document->currentSpace());

    if (space==0)
        return;

    if (space->dimension()!=Analitza::Dim2D && space->dimension()!=Analitza::Dim3D)
        return;

    const QPixmap thumbnail = m_dashboard->thumbnail(space->dimension()).scaled(QSize(PreviewWidth, PreviewHeight), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    space->setThumbnail(thumbnail);
}

void MainWindow::checkforAutoSavedFile()
{
    openFile(getDefaultAutoSavepath());
}

void MainWindow::newFile()
{
    // if there are not any plots added (i.e. file is completely clean)
    if (m_document->plotsModel()->rowCount()==0) {
        QMessageBox::information(this, i18n("No need to create New Window"), i18n("There are not any plots available. So, you do not need to create a new Plot Window"));
        return;
    }

    // creates a new proccess.
    // The commandline arguments is used to create a completely new instance of Khipu(i.e. it does not reload the autosaved file)
    QStringList args;
    args << "ignoreautosavedfile"; // we dont want to reload the autosave file for the new action's slot.
    KToolInvocation::kdeinitExec("khipu", args);
}

void MainWindow::openRecentClicked(const QUrl&  name)
{
    // no plots are available , can open in the same proccess.
    if (m_document->plotsModel()->rowCount()==0) {
        openFile(name);
        return;
    }
    QStringList args;
    args << name.toLocalFile();
    KToolInvocation::kdeinitExec("khipu", args);
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    if (fileName.isEmpty())
        return;

    KConfig config;
    KConfigGroup recentFiles(&config, "file_Recent");

    QStringList files =recentFiles.readPathEntry("recentFileList", QStringList());
    files.removeAll(fileName);
    files.prepend(fileName);

    while (files.size() > MaxRecentFiles)
        files.removeLast();

    recentFiles.deleteEntry("recentFileList");
    recentFiles.writePathEntry("recentFileList", files);
    recentFiles.sync();
    updateRecentFileList();
}

void MainWindow::updateRecentFileList()
{
    KConfig config;
    KConfigGroup recentFiles(&config, "file_Recent");
    QStringList files =  recentFiles.readPathEntry("recentFileList", QStringList());
    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    // Traversing in reverse manner will lead to the correct Recently opened file-list
    for (int i = numRecentFiles-1; i >=0 ; i--) {
        QString text=files[i];
        m_openrecent->addUrl(QUrl(text), QFileInfo(text).fileName());
    }
}

bool MainWindow::openFileClicked()
{
    const auto url = QFileDialog::getOpenFileUrl(this, i18n("Open"), {},
                     i18n("Khipu Files (*.khipu);;All Files (*)"));

    if (url.path().isEmpty() || m_currentFileUrl == url)
        return false;

    openFile(url);

    return true;
}

bool MainWindow::openFile(const QUrl &url)
{
    if (url.path().isEmpty())
        return false;

    QFile file;

    if (!url.isLocalFile())
    {
        if (!KIO::NetAccess::exists(url, KIO::NetAccess::SourceSide, this))
        {
            KMessageBox::sorry(this, i18n("The file does not exist."));
            return false;
        }

        QString tmpfile;

        if (!KIO::NetAccess::download(url, tmpfile, this))
        {
            KMessageBox::sorry(this, i18n("An error appeared when opening this file (%1)", KIO::NetAccess::lastErrorString()));
            return false;
        }
        file.setFileName(tmpfile);
    }
    else
        file.setFileName(url.toLocalFile());

    // for the default autosaved file (.khipu.autosave)
    if (url == getDefaultAutoSavepath())
    {
        // ask for reloading the autosave file
        if (!file.exists())
            return false;
        int answer=KMessageBox::questionYesNo(this, i18n("Do you want to recover the file you have not saved last time?"), i18n("Autosaved .khipu file"));

        if (answer!=KMessageBox::Yes)
        {
            QFile file(url.toLocalFile());
            if (file.remove())
                return false;
        }
    } 
    else 
    {
        QString path=url.toLocalFile();
        
        // not , current autosave !
        if (!path.contains(".khipu.autosave"))
            setCurrentFile(path);
        
        m_fileLocation = path; // this allows user to save the other work in the file which he/she has just opened.
        
        changeTitleBar(path);

        //check for available autosave file
        QString currentautosavepath=getCurrentAutoSavepath(path);
        
        if (QFile::exists(currentautosavepath)) 
        {

            // ask for reloading the autosave file
            int answer=KMessageBox::questionYesNo(this, i18n("There are some unsaved changes in the file %1. Do you want to recover them?", QFileInfo(path).baseName()), i18n("Autosaved .khipu file"));
            
            if (answer==KMessageBox::Yes) 
            {
                // user wants to open the autosave file.
                file.setFileName(currentautosavepath);
            }
        }
    }
    if (!file.open(QFile::ReadOnly)) 
    {
        KMessageBox::error(this, i18n("Error while reading file, maybe path is not found."), i18n("Error while reading"));
        
        if (file.fileName() != getDefaultAutoSavepath().toLocalFile())
            KMessageBox::sorry(this, i18n("%1 could not be opened", file.fileName()));
       
        return false;
    }

    m_parsedSpaceDetails = QJsonDocument::fromJson(file.readAll()).toVariant().toList();
    file.close();

    if (!url.isLocalFile())
        KIO::NetAccess::removeTempFile( file.fileName() );

    // if a wrong file is hit
    if (m_parsedSpaceDetails.isEmpty())
    { 
        KMessageBox::error(this, i18n("Problem in parsing the contents of the file, maybe a wrong khipu file."), i18n("Error while reading"));
        
        return false;
    }

    m_dashboard->m_openclicked = true;

    //NOTE we need to remove all current data before load new data from file
    bool reloadviews = false;
    
    if (m_document->spacesModel()->rowCount() > 0)
    {
        m_document->clearAllData();
        reloadviews = true;
    }
    
    foreach (QVariant record, m_parsedSpaceDetails) 
    {
        QVariantMap map = record.toMap();

        QString spacename=map.value("name").toString();
        QString spaceDescription=map.value("description").toString();        
        QByteArray image= map.value("image").toByteArray();

        Analitza::Dimension dim=static_cast<Analitza::Dimension> (map.value("dimension").toInt());
        QPixmap thumbnail= toPixmap(image);

        m_document->spacesModel()->addSpace(dim, spacename, spaceDescription, thumbnail);
    }
    
    //NOTE we need to realod views after load a new file data (see datastore::clearalldata)
    if (reloadviews)
    {
        m_dashboard->setDocument(m_document);
        m_spacePlotsDock->setDocument(m_document);
        m_dictionaryDock->setDocument(m_document);
    }
    
    m_dashboard->goHome();
    activateDashboardUi();
    
    m_currentFileUrl = url;
    
    return true;
}

QPixmap MainWindow::toPixmap(const QByteArray &bytearray)
{
    QByteArray imageArray(QByteArray::fromBase64(bytearray));
    QBuffer imagebuffer(&imageArray);
    imagebuffer.open(QIODevice::ReadOnly);
    QImage img;
    img.loadFromData(imagebuffer.data(), "PNG");
    return QPixmap::fromImage(img);
}

void MainWindow::saveClicked()
{
    // Intially when the data is not saved. We would not have the actual file path.
    if (m_fileLocation.isEmpty())
    {
        QUrl url = QFileDialog::getSaveFileUrl(this, i18n("Save As"), {}, i18n("Khipu Files (*.khipu);;All Files (*)") );
        if (url.isEmpty())
            return;

        //overwriting warning
        if (QFileInfo::exists(url.toLocalFile()))
        {
            int answer=KMessageBox::questionYesNo(this,
            i18n("The file which you want to save, already exists. Do you want to overwrite this file?"),
            i18n("Warning: File already exists"));
                if (answer==KMessageBox::No)
                    return;
        }
            m_fileLocation =url.toLocalFile();
    }
    saveFile(QUrl::fromLocalFile(m_fileLocation));
}

bool MainWindow::closeClicked()
{
    QFile autosaveFile(getDefaultAutoSavepath().toLocalFile());

    const QFileInfo fi(m_fileLocation);
    QString path = fi.dir().path().append("/.").append(fi.baseName().append(".khipu.autosave"));
    QFile currentautosaveFile(path);

    if (autosaveFile.exists() || currentautosaveFile.exists()) {
        int answer=KMessageBox::questionYesNoCancel(this,
                   i18n("The current file contains some unsaved work. Do you want to save it?"),
                   i18n("Warning: Unsaved changes"));
        if (answer==KMessageBox::Yes) {
            saveClicked();
            return true;
        } else if (answer==KMessageBox::Cancel) {
            return false;
        } else if (answer==KMessageBox::No) {
            // if user selects No , then he/she can restore the work using autosaving feature. But this can be removed !
            return true;
        }
    }
    return true; // just close the application.
}

bool MainWindow::saveAsClicked()
{
    const auto path = QFileDialog::getSaveFileUrl( this, i18n("Save As"), {}, i18n("Khipu Files (*.khipu);;All Files (*)"));
 
    if (path.isEmpty())
        return false;
    
    if (saveFile(path))
    {
        m_fileLocation = path.toLocalFile();
        
        return true;
    }
    
    return false;
}

bool MainWindow::saveFile(const QUrl &url)
{
    QMap<SpaceItem*, Analitza::PlotItem*> map=m_document->currentDataMap();

    // just starting, no plots and no spaces are available, so no need to save
    if (map.empty() && m_document->spacesModel()->items().isEmpty())
    {
        // no plots are there. so, we dont need autosave file
        QFile tempautosaveFile(getDefaultAutoSavepath().toLocalFile());
        tempautosaveFile.remove();

        QString path=QFileInfo(m_fileLocation).dir().path().append("/.").append(QFileInfo(m_fileLocation).baseName().append(".khipu.autosave"));
        QFile currentautosaveFile(path);
        currentautosaveFile.remove();
        
        return false;
    }

    //NOTE We need to read all space items, not just only the mapped ones (see bug: 328252)
    QList<SpaceItem*> spaceList=m_document->spacesModel()->items();

    if (spaceList.empty()) 
    {
        KMessageBox::error(this, i18n("Error while reading file, file may be empty"), i18n("Error while reading"));
    
        return false;
    }

    QVariantList plotspace_list;

    foreach(SpaceItem* space, spaceList) {
        QString spaceName = space->name();
        QString spaceDescription = space->description();        
        QPixmap thumbnail = space->thumbnail();
        int dim = space->dimension();

        QVariantList subplot_list;
        subplot_list.clear();

        QVariantMap plotspace;
        plotspace.insert("name", spaceName);
        plotspace.insert("description", spaceDescription);
        plotspace.insert("dimension", dim);
        plotspace.insert("image", thumbnailtoByteArray(thumbnail));

        QList<Analitza::PlotItem*> plotList=map.values(space);

        foreach(Analitza::PlotItem* plotitem, plotList) {
            QString plotName = plotitem->name();
            QString plotExpression = plotitem->expression().toString();
            QColor plotcolor = plotitem->color();

            QVariantMap plot;
            plot.insert("name", plotName);
            plot.insert("expression", plotExpression);
            plot.insert("color", plotcolor);

            if (dim==2) {
                Analitza::FunctionGraph* functiongraph = static_cast<Analitza::FunctionGraph*>(plotitem);

                if (functiongraph->hasIntervals())
                {
                    QString arg1min = functiongraph->interval(functiongraph->parameters().at(0), false).first.toString();
                    QString arg1max = functiongraph->interval(functiongraph->parameters().at(0), false).second.toString();

                    plot.insert("arg1min", arg1min);
                    plot.insert("arg1max", arg1max);
                }
            }

            //writehere
            subplot_list << plot;

        }

        QByteArray subjson = QJsonDocument::fromVariant(subplot_list).toJson();
        plotspace.insert("plots", subjson);
        plotspace_list << plotspace;
    }

    QByteArray json = QJsonDocument::fromVariant(plotspace_list).toJson();

    if (!url.isLocalFile())
    {
        KTemporaryFile tmpfile;
       
        if (!tmpfile.open())
        {
            KMessageBox::error(this, i18n("Could not open %1 for writing", QUrl(tmpfile.fileName()).toLocalFile()), i18n("Error while writing"));
            
            return false;
        }
        
        QTextStream out(&tmpfile);
        out << json;
        out.flush();
        
        if (!KIO::NetAccess::upload(tmpfile.fileName(), url, this))
        {
            KMessageBox::error(this, i18n("Could not open %1 for writing %2", url.toDisplayString(), KIO::NetAccess::lastErrorString()), i18n("Error while writing"));

            return false;
        }
    }
    else
    {
        if (!url.isLocalFile())
        {
            KMessageBox::error(this, i18n("Error while saving file. Maybe path is not found"), i18n("Error while saving"));
        
            return false;
        }

        QFile file(url.toLocalFile());

        // saved action clicked by the user , this is not the autosave case
        QString currentautosavepath = getCurrentAutoSavepath(m_fileLocation);
        
        if (url != getDefaultAutoSavepath() && url.toLocalFile()!=currentautosavepath)
        {
            if (!file.open(QFile::WriteOnly | QFile::Text)) 
                return false;

            // remove the auto save file (can be improved later) -> better to use function (TODO)
            QFile tempautosaveFile(getDefaultAutoSavepath().toLocalFile());
            tempautosaveFile.remove();
            QFile currentautosaveFile(currentautosavepath);
            currentautosaveFile.remove();

            setCurrentFile(url.toLocalFile());
            statusBar()->showMessage(i18n("File %1 [%2] is saved successfully",
                                          QFileInfo(url.toLocalFile()).fileName(), url.toLocalFile()), MessageDuration);
            changeTitleBar(url.toLocalFile());
        }
        else // autosave case
        {
            if (!file.open(QFile::WriteOnly | QFile::Text)) 
                return false;
        }

        QTextStream out(&file);
        out << json;
        file.close();
    }
    
    m_currentFileUrl = url;
    
    return true;
}

void MainWindow::changeTitleBar(const QString& path)
{
    window()->setWindowTitle(QFileInfo(path).fileName().append(" - Khipu"));
}

QString MainWindow::getCurrentAutoSavepath(const QString &path)
{
    if (!path.isEmpty())
        return QFileInfo(path).dir().path().append("/.").append(QFileInfo(path).baseName().append(".autosave"));
    
    return QString();
}

QUrl MainWindow::getDefaultAutoSavepath() const
{
    return QUrl::fromLocalFile(QDir::homePath().append("/.khipu.autosave"));
}

void MainWindow::savePlot()
{
    if (m_document->currentSpace()==-1)
        return;
    Dimension dim = m_document->spacesModel()->space(m_document->currentSpace())->dimension();
    m_dashboard->exportSpaceSnapshot(dim);
}

void MainWindow::activateSpace(int spaceidx)
{
    activateSpaceUi();

    m_spacePlotsDock->reset(true);

    SpaceItem *space = m_document->spacesModel()->space(spaceidx);
    m_spaceInfoDock->setInformation(space->name(), space->description());

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

    //docks
    // primero oculto los widgets sino el size de los que voy a ocultar interfieren y la mainwnd se muestra muy grande
    m_spacePlotsDock->hide();
    m_spaceInfoDock->hide();
    m_spaceOptionsDock->hide();
    m_plotsBuilderDock->hide();
    m_dictionaryDock->hide();
    m_priorAct->setVisible(false);
    m_nextAct->setVisible(false);
    //actions should be invisible
    m_getdictionaryAct->setVisible(false);
    m_importdictionaryAct->setVisible(false);
}

void MainWindow::activateSpaceUi()
{
    m_dashboard->setCurrentIndex(1);

    //menu
    
    //tools
    m_filter->setFilterVisible(false);
    
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
    SpaceItem *space = m_document->spacesModel()->space(m_document->currentSpace());
    switch (space->dimension())
    {
    case Analitza::Dim2D:
        m_dashboard->copySpace2DSnapshotToClipboard();
        break;
    case Analitza::Dim3D:
        m_dashboard->copySpace3DSnapshotToClipboard();
        break;
    default:
        break;
    }
    statusBar()->showMessage(i18n("The diagram was copied to clipboard"), MessageDuration);
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

    //docks on the dashboard
    m_plotsBuilderDock->hide();
    m_spacePlotsDock->hide();
    m_spaceInfoDock->hide();
    m_spaceOptionsDock->hide();
    m_dictionaryDock->hide();
    m_dashboard->showDictionary();
    m_priorAct->setVisible(false);
    m_nextAct->setVisible(false);

    //actions should be visible
    m_getdictionaryAct->setVisible(true);
    m_importdictionaryAct->setVisible(true);
    m_dashboard->activateDictionaryData();
}

void MainWindow::addSpace2D()
{
    activateSpaceUi();

    m_dashboard->showPlotsView2D();
    int rows=m_document->spacesModel()->rowCount();
    m_document->spacesModel()->addSpace(Analitza::Dim2D, i18n("Untitled %1", rows+1));

    emit Spaceinserted(Dim2D);
}

void MainWindow::addSpace3D()
{
    activateSpaceUi();

    m_dashboard->showPlotsView3D();
    int rows=m_document->spacesModel()->rowCount();
    m_document->spacesModel()->addSpace(Analitza::Dim3D, i18n("Untitled %1", rows+1));

    emit Spaceinserted(Dim3D);
}

void MainWindow::removeCurrentSpace()
{
    m_document->removeCurrentSpace();

    if (m_document->spacesModel()->rowCount() == 0)
        action("delete_currentspace")->setVisible(false);
}

// Allows the user to go Home which will save the current space and take the user to the main view
void MainWindow::goHome()
{
    if (m_dashboard->currentIndex() != 0)
    {
        //Current Space from where go home is pressed
        SpaceItem *space = m_document->spacesModel()->space(m_document->currentSpace());

        space->stamp(); // marcamos la fecha y hora de ingreso al space
        space->setName(m_spaceInfoDock->title());
        space->setDescription(m_spaceInfoDock->description());

        // Thumbnail of the current space which is used to identify the space
        const QPixmap thumbnail = m_dashboard->thumbnail(space->dimension()).scaled(QSize(PreviewWidth, PreviewHeight), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        space->setThumbnail(thumbnail);
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
    if (ind.row() < 0)
        return;

    //Parsing the data from the qjson file
    QVariantMap map = m_parsedSpaceDetails.at(ind.row()).toMap(); // corresponding space entry

    Analitza::Dimension dim=static_cast<Analitza::Dimension> (map.value("dimension").toInt());
    QVariantList plotList= QJsonDocument::fromJson(map.value("plots").toByteArray()).toVariant().toList();

    foreach(QVariant plot, plotList) {

        QVariantMap plotmap = plot.toMap();
        QString plotname=plotmap.value("name").toString();
        QString ploteqn=plotmap.value("expression").toString();
        QColor plotcolor=plotmap.value("color").toString();

        double arg1min=plotmap.value("arg1min").toDouble();
        double arg1max=plotmap.value("arg1max").toDouble();

        QStringList errors;

        PlotBuilder req = PlotsFactory::self()->requestPlot(Analitza::Expression(QString(ploteqn)), dim);

        if (req.canDraw()) 
        {

            FunctionGraph *item = 0;
            item = req.create(plotcolor, plotname);

            if (dim==Dim2D)
            {
                //Used to deal with infinite intervals.
                if (arg1min!=0 || arg1max!=0)
                    item->setInterval(item->parameters().first(), arg1min, arg1max);
            }
            
            m_document->plotsModel()->addPlot(item);
        } 
        else
            errors = req.errors();
    }
}
