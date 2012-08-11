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

#include "analitzaplot/plotsdictionarymodel.h"
#include <analitzaplot/planecurve.h>
#include <analitzaplot/plotsview2d.h>
#include <analitzaplot/plotsview3d.h>
#include <analitza/expression.h>

#include <KDE/KApplication>

#include <QtGui/QDockWidget>
#include <QtGui/QLayout>
#include <QDebug>
#include <KDE/KLocale>
#include <KDE/KLocalizedString>
#include <KDE/KStandardDirs>
#include <KDE/KAction>
#include <KDE/KActionCollection>
#include <KDE/KStandardAction>
#include <KDE/KStatusBar>
#include <KDE/KFileDialog>
#include <KDE/KMessageBox>
#include <KIO/NetAccess>
#include <KDE/KMessageBox>
#include <KDE/KStandardDirs>
#include <KToolBar>
#include "spacesmodel.h"
#include <KMenuBar>
#include "dashboard.h"
#include "document.h"
#include "plotseditor.h"
#include "datastore.h"
#include "plotsbuilder.h"
#include "spaceinformation.h"
#include "spaceoptions.h"

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
    setCentralWidget(m_dashboard);
    setupToolBars();
    activateDashboardUi();
    
    updateTittleWhenOpenSaveDoc();
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
    m_plotsBuilderDock = new QDockWidget(i18n("Shortcuts"), this);
    m_plotsBuilderDock->setWidget(plotsBuilder); // plotsbuilder debe ser miembro
    m_plotsBuilderDock->setObjectName("dsfs");
    m_plotsBuilderDock->setFloating(false);
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

    connect(m_spacePlotsDock, SIGNAL(goHome()), SLOT(goHome()));
    connect(m_spacePlotsDock, SIGNAL(sendStatus(QString,int)), statusBar(),SLOT(showMessage(QString,int)));
    
    connect(m_dashboard, SIGNAL(spaceActivated(int)), m_spacePlotsDock, SLOT(setCurrentSpace(int)));
    
    m_spaceInfoDock = new SpaceInformation(this);
    
    m_spaceOptionsDock = new SpaceOptions(this);
    connect(m_document, SIGNAL(gridStyleChanged(int)), m_spaceOptionsDock, SLOT(setGridStyleIndex(int)));
    //2d view
    connect(m_spaceOptionsDock, SIGNAL(updateGridStyle(int)), m_dashboard->view2d(), SLOT(useCoorSys(int)));
    connect(m_spaceOptionsDock, SIGNAL(updateGridColor(QColor)), m_dashboard->view2d(), SLOT(updateGridColor(QColor)));
    connect(m_spaceOptionsDock, SIGNAL(setXAxisLabel(QString)), m_dashboard->view2d(), SLOT(setXAxisLabel(QString)));
    connect(m_spaceOptionsDock, SIGNAL(setYAxisLabel(QString)), m_dashboard->view2d(), SLOT(setYAxisLabel(QString)));
    connect(m_spaceOptionsDock, SIGNAL(updateTickScale(QString,qreal,int,int)), m_dashboard->view2d(), SLOT(updateTickScale(QString,qreal,int,int)));
    connect(m_spaceOptionsDock, SIGNAL(setUseTickSymbols(bool)), m_dashboard->view2d(), SLOT(setUseTickSymbols(bool)));
    connect(m_spaceOptionsDock, SIGNAL(showHTicks(bool)), m_dashboard->view2d(), SLOT(showHTicks(bool)));
    connect(m_spaceOptionsDock, SIGNAL(showVTicks(bool)), m_dashboard->view2d(), SLOT(showVTicks(bool)));
    connect(m_spaceOptionsDock, SIGNAL(showHAxes(bool)), m_dashboard->view2d(), SLOT(showHAxes(bool)));
    connect(m_spaceOptionsDock, SIGNAL(showVAxes(bool)), m_dashboard->view2d(), SLOT(showVAxes(bool)));
    //3d view
    connect(m_spaceOptionsDock, SIGNAL(axisIsDrawn(bool)), m_dashboard->view3d(), SLOT(setAxisIsDrawn(bool)));
    connect(m_spaceOptionsDock, SIGNAL(gridIsDrawn(bool)), m_dashboard->view3d(), SLOT(setGridIsDrawn(bool)));
    connect(m_spaceOptionsDock, SIGNAL(sceneResized(int)), m_dashboard->view3d(), SLOT(resizeScene(int)));

    addDockWidget(Qt::LeftDockWidgetArea, m_plotsBuilderDock);
    addDockWidget(Qt::LeftDockWidgetArea, m_spacePlotsDock);
    addDockWidget(Qt::RightDockWidgetArea, m_spaceInfoDock);
    addDockWidget(Qt::RightDockWidgetArea, m_spaceOptionsDock);
}

void MainWindow::setupActions()
{
    //file
    KStandardAction::openNew(this, SLOT(newFile()), actionCollection());
    KStandardAction::open(this, SLOT(fooSlot()), actionCollection());
    KStandardAction::openRecent(this, SLOT(fooSlot()), actionCollection());    
    KStandardAction::save(this, SLOT(fooSlot()), actionCollection());
    KStandardAction::saveAs(this, SLOT(fooSlot()), actionCollection());
    KStandardAction::close(this, SLOT(close()), actionCollection());
    KStandardAction::quit(this, SLOT(close()), actionCollection());
    
    //edit - dashboard
    createAction("add_space2d", i18n("&Add Space 2D"), "add-space2d", Qt::CTRL + Qt::Key_W, this, SLOT(addSpace2D()));
    createAction("add_space3d", i18n("&Add Space 3D"), "add-space3d", Qt::CTRL + Qt::Key_W, this, SLOT(addSpace3D()));
    createAction("add_random_plot", i18n("&Add Random Plot"), "roll", Qt::CTRL + Qt::Key_W, this, SLOT(fooSlot()));
    //view - dashboard //TODO Show Plots Dictionary
    m_plotsBuilderDock->toggleViewAction()->setIcon(KIcon("formula"));
    m_plotsBuilderDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_W);
    m_plotsBuilderDock->toggleViewAction()->setToolTip(i18n("Create a plot in a new space"));
    actionCollection()->addAction("show_plotsbuilder", m_plotsBuilderDock->toggleViewAction());

    createAction("show_plots", i18n("&Show Plots"), "view-list-details", Qt::CTRL + Qt::Key_W, this, SLOT(fooSlot()));
    createAction("show_spaces", i18n("&Show Spaces"), "view-list-icons", Qt::CTRL + Qt::Key_W, this, SLOT(fooSlot()));
    createAction("show_plotsdictionary", i18n("&Mathematical Objects"), "accessories-dictionary", Qt::CTRL + Qt::Key_W, this, 
                 SLOT(setVisibleDictionary()));

    //view - space
//     createAction("show_plots_editor", i18n("S&how Space Plots"), "address-book-new", Qt::CTRL + Qt::Key_W, this, SLOT(fooSlot()), true);
    m_spacePlotsDock->toggleViewAction()->setIcon(KIcon("address-book-new"));
    m_spacePlotsDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("show_plots_editor", m_spacePlotsDock->toggleViewAction());    
    
//     createAction("show_space_info", i18n("&Show Space Information"), "document-properties", Qt::CTRL + Qt::Key_W, this, SLOT(fooSlot()), true);
    m_spaceInfoDock->toggleViewAction()->setIcon(KIcon("document-properties"));
    m_spaceInfoDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("show_space_info", m_spaceInfoDock->toggleViewAction());       
    
//     createAction("show_plotter_options", i18n("&Show Space Options"), "configure", Qt::CTRL + Qt::Key_W, this, SLOT(fooSlot()), true);
    m_spaceOptionsDock->toggleViewAction()->setIcon(KIcon("coord-settings"));
    m_spaceOptionsDock->toggleViewAction()->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("show_plotter_options", m_spaceOptionsDock->toggleViewAction());       
    
    
    //go
    KAction *act = KStandardAction::firstPage(this, SLOT(fooSlot()), actionCollection());
    act->setText(i18n("&Go First Space"));
    act->setIcon(KIcon("go-first-view"));
    act->setEnabled(false);

    act = KStandardAction::prior(this, SLOT(fooSlot()), actionCollection());
    act->setText(i18n("&Go Previous Space"));
    act->setIcon(KIcon("go-previous-view"));
    act->setEnabled(false);

    act = KStandardAction::next(this, SLOT(fooSlot()), actionCollection());
    act->setText(i18n("&Go Next Space"));
    act->setIcon(KIcon("go-next-view"));
    act->setEnabled(false);
    
    act = KStandardAction::lastPage(this, SLOT(fooSlot()), actionCollection());
    act->setText(i18n("&Go Last Space"));
    act->setIcon(KIcon("go-last-view"));
    act->setEnabled(false);

    KStandardAction::home(this, SLOT(goHome()), actionCollection());
    //tools dashboard
    createAction("delete_currentspace", i18n("&Remove Current Space"), "list-remove", Qt::CTRL + Qt::Key_W, this, SLOT(removeCurrentSpace()))->setVisible(false);;
    //tools space
    createAction("copy_snapshot", i18n("&Copy Space Snapshot"), "edit-copy", Qt::CTRL + Qt::Key_W, this, SLOT(copySnapshot()));
//     createAction("export_snapshot", i18n("&Export Space Snapshot"), "view-preview", Qt::CTRL + Qt::Key_W, this, SLOT(fooSlot()));
    //settings
    KStandardAction::showMenubar(this, SLOT(fooSlot()), actionCollection());
    KStandardAction::fullScreen(this, SLOT(fooSlot()), this ,actionCollection());

//     connect(m_dashboard, SIGNAL(saveRequest()), SLOT(saveFile()));
//     connect(m_dashboard, SIGNAL(openRequest()), SLOT(openFile()));
}

void MainWindow::fooSlot(bool t)
{
//     qDebug() << "test slot" << t;
}


void MainWindow::setupToolBars()
{
//     hideSpaceToolBar();
    
//     qDebug() << action("add_space2d")->isCheckable();
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
void MainWindow::newFile()
{
//     MainWindow * newWnd = new MainWindow();
//     newWnd->show();
}
void MainWindow::openFile()
{
 
}

void MainWindow::activateSpace(int spaceidx)
{
    activateSpaceUi();
    
    m_spacePlotsDock->reset(true);
    
    //clear space infor widget 
//     m_spaceInfoDock->clear();
    SpaceItem *space = m_document->spacesModel()->item(spaceidx);
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
    
    action("add_random_plot")->setVisible(false);
    //view
    action("show_plotsbuilder")->setVisible(true);
    action("show_plots")->setVisible(true);
    action("show_spaces")->setVisible(true);
    action("show_plotsdictionary")->setVisible(true);    
    action("show_plots_editor")->setVisible(false);
    action("show_space_info")->setVisible(false);
    action("show_plotter_options")->setVisible(false);
    //go
    action("go_home")->setVisible(false);
    //tools
    action("copy_snapshot")->setVisible(false);
//     action("export_snapshot")->setVisible(false);
    
    //toolbars
    toolBar("mainToolBar")->show();
    toolBar("spaceToolBar")->hide();

    //docks
    // primero oculto los widgets sino el size de los que voy a ocultar interfieren y la mainwnd se muestra muy grande
    m_spacePlotsDock->hide();
    m_spaceInfoDock->hide();
    m_spaceOptionsDock->hide();
    m_plotsBuilderDock->hide(); 
}

void MainWindow::activateSpaceUi()
{
    m_dashboard->setCurrentIndex(1);

    //menu
    //edit
    action("add_space2d")->setVisible(false);
    action("add_space3d")->setVisible(false);
    action("delete_currentspace")->setVisible(false);        
    action("add_random_plot")->setVisible(true);
    //view
    action("show_plotsbuilder")->setVisible(false);
    action("show_plots")->setVisible(false);
    action("show_spaces")->setVisible(false);
    action("show_plotsdictionary")->setVisible(false); 
    action("show_plots_editor")->setVisible(true);
    action("show_space_info")->setVisible(true);
    action("show_plotter_options")->setVisible(true);
    //go
    action("go_home")->setVisible(true);    
    //tools
    action("copy_snapshot")->setVisible(true);
//     action("export_snapshot")->setVisible(true);
    
    //toolbars
    toolBar("mainToolBar")->hide();
    toolBar("spaceToolBar")->show();

    //docks
    //lo mismo ... primero hides luego show
    m_plotsBuilderDock->hide();
    m_spacePlotsDock->show();
    m_spaceInfoDock->show();
    m_spaceOptionsDock->show();
    
    ///
}

void MainWindow::copySnapshot()
{
    SpaceItem *space = m_document->spacesModel()->item(m_document->currentSpace());

    switch (space->dimension())
    {
        case 2: m_dashboard->view2d()->snapshotToClipboard(); break;
        case 3: m_dashboard->view3d()->snapshotToClipboard(); break;
    }
    
    statusBar()->showMessage(i18n("The diagram was copied to clipboard"), 2500);
}

void MainWindow::exportSnapShot()
{

}

void MainWindow::setVisibleDictionary()
{
    
        //menu
        //edit 
        action("add_space2d")->setVisible(false);
        action("add_space3d")->setVisible(false);
        action("delete_currentspace")->setVisible(false);        
        action("add_random_plot")->setVisible(false);
        //view
        action("show_plotsbuilder")->setVisible(false);
        action("show_plots")->setVisible(false);
        action("show_spaces")->setVisible(false);
        action("show_plots_editor")->setVisible(false);
        action("show_space_info")->setVisible(false);
        action("show_plotter_options")->setVisible(false);
        //go
        action("show_plotsdictionary")->setVisible(false); 
        action("go_home")->setVisible(true);  
        
        //tools
        action("copy_snapshot")->setVisible(false);
//         action("export_snapshot")->setVisible(false);
        
        //toolbars
        toolBar("mainToolBar")->show();
        toolBar("spaceToolBar")->show();

        //docks
        //lo mismo ... primero hides luego show
        m_plotsBuilderDock->hide();
        m_spacePlotsDock->hide();
        m_spaceInfoDock->hide();
        m_spaceOptionsDock->hide();
    
    m_dashboard->showDictionary();
}

void MainWindow::addSpace2D()
{
    activateSpaceUi();
    
    m_dashboard->showPlotsView2D();
    m_document->spacesModel()->addSpace(Dim2D, i18n("Untitled %1").arg(m_document->spacesModel()->rowCount()+1));
}

void MainWindow::addSpace3D()
{
    activateSpaceUi();
    
    m_dashboard->showPlotsView3D();
    m_document->spacesModel()->addSpace(Dim3D, i18n("Untitled %1").arg(m_document->spacesModel()->rowCount()+1));
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
        
        SpaceItem *space = m_document->spacesModel()->item(m_document->currentSpace());

        space->stamp(); // marcamos la fecha y hora de ingreso al space
        space->setTitle(m_spaceInfoDock->title());
        space->setDescription(m_spaceInfoDock->description());
        
        QPixmap thumbnail; 

        switch (space->dimension())
        {
            case 2: thumbnail = QPixmap::grabWidget(m_dashboard->view2d()); break;
            case 3:
            {
                m_dashboard->view3d()->updateGL();
                m_dashboard->view3d()->setFocus();
                m_dashboard->view3d()->makeCurrent();
                m_dashboard->view3d()->raise();
        
                QImage image(m_dashboard->view3d()->grabFrameBuffer(true));

                thumbnail = QPixmap::fromImage(image, Qt::ColorOnly);

                break;
            }
        }

        thumbnail = thumbnail.scaled(QSize(220, 220), Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);   
        space->setThumbnail(thumbnail);
    }

    m_dashboard->goHome();
    activateDashboardUi();
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


void MainWindow::updateTittleWhenChangeDocState()
{
//     QString paletteFileName = m_dashboard->fileName();
// 
//     if (paletteFileName.isEmpty())
//         paletteFileName = i18n("Untitled");
// 
//     setWindowTitle(QString("%1 - GPLACS " + i18n("[modificado]")).arg(paletteFileName));
}

void MainWindow::updateTittleWhenOpenSaveDoc()
{
//     QString paletteFileName = m_dashboard->fileName();
// 
//     if (paletteFileName.isEmpty())
//         paletteFileName = i18n("Untitled");
// 
//     setWindowTitle(QString("%1 - GPLACS").arg(paletteFileName));
}

