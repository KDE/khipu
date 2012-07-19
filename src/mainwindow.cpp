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
#include <analitza/expression.h>

#include <KDE/KApplication>

#include <QtGui/QDropEvent>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>
#include <QtGui/QDockWidget>
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsProxyWidget>
#include <QLineEdit>
#include <qstringlistmodel.h>
#include <qlistview.h>
#include <qtreeview.h>
#include <QDebug>
#include <QDomDocument>
#include <KDE/KLocale>
#include <KDE/KLocalizedString>
#include <KDE/KStandardDirs>
#include <KDE/KAction>
#include <KDE/KActionCollection>
#include <KDE/KStandardAction>
#include <KDE/KStatusBar>
#include <KDE/KConfigDialog>
#include <KDE/KPageWidget>
#include <KDE/KPageModel>
#include <KDE/KPushButton>
#include <KDE/KFileDialog>
#include <KDE/KMessageBox>
#include <KIO/NetAccess>
#include <KDE/KMessageBox>
#include <KDE/KRecentFilesAction>
#include <KDE/KConfigDialog>
#include <KDE/KStandardDirs>
#include <KToolBar>
#include "functionsmodel.h"
#include "spacesmodel.h"
#include <KAboutApplicationDialog>
#include <KMenuBar>
#include <kcategorizedview.h>
#include <kcategorydrawer.h>
#include "dashboard.h"
#include "document.h"
#include "functioneditor.h"

#include "ui_space2dviewer.h"
#include "ui_space3dviewer.h"
#include "ui_functioneditorwidget.h"


MainWindow::MainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
{
    m_document = new Document(this);

    m_dashboard = new Dashboard(this);
    m_dashboard->setDocument(m_document);

    setupActions();
    setupGUI(Keys | StatusBar | Save | Create, "khipu.rc");
    setCentralWidget(m_dashboard);
    setupToolBars();
    setupDocks();
    activateDashboardUi();

    updateTittleWhenOpenSaveDoc();

    connect(m_dashboard, SIGNAL( modified() ), SLOT( updateTittleWhenChangeDocState() ));
    connect(m_dashboard, SIGNAL(dashemitShowAppInfo()), SLOT(showAboutAppDialog()));
    
    
}

MainWindow::~MainWindow()
{
}

KAction* MainWindow::createAction(const char* name, const QString& text, const QString& iconName, const QKeySequence& shortcut, const char* slot, bool isCheckable)
{
    KAction* act = new KAction(this);
    act->setText(text);
    act->setIcon(KIcon(iconName));
    act->setShortcut(shortcut);
    act->setCheckable(isCheckable);
    
    if (isCheckable) 
        act->setChecked(true);
        
    actionCollection()->addAction(name, act);
    
    connect(act, SIGNAL(triggered()), slot);

    return act;
}

void MainWindow::setupActions()
{
    //file
    KStandardAction::openNew(this, SLOT(newFile()), actionCollection());
    KStandardAction::open(this, SLOT(hide3dtb()), actionCollection());
    KStandardAction::save(this, SLOT(hide3dtb()), actionCollection());
    KStandardAction::saveAs(this, SLOT(hide3dtb()), actionCollection());
    KStandardAction::close(this, SLOT(hide3dtb()), actionCollection());
    KStandardAction::quit(this, SLOT(close()), actionCollection());
    //edit - dashboard
    createAction("add_space2d", i18n("&Add Space 2D"), "list-add", Qt::CTRL + Qt::Key_W, SLOT(addSpace2D()));
    createAction("add_space3d", i18n("&Add Space 3D"), "list-add", Qt::CTRL + Qt::Key_W, SLOT(addSpace2D()));
    //view - dashboard //TODO Show Plots Dictionary
    createAction("show_plots", i18n("&Show Plots"), "view-list-details", Qt::CTRL + Qt::Key_W, SLOT(addSpace2D()));
    createAction("show_spaces", i18n("&Show Spaces"), "view-list-icons", Qt::CTRL + Qt::Key_W, SLOT(addSpace2D()));
    //view - space
    createAction("show_plots_editor", i18n("S&how Space Plots"), "address-book-new", Qt::CTRL + Qt::Key_W, SLOT(addSpace2D()), true);
    createAction("show_space_info", i18n("&Show Space Information"), "document-edit", Qt::CTRL + Qt::Key_W, SLOT(addSpace2D()), true);
    createAction("show_plotter_options", i18n("&Show Space Options"), "configure", Qt::CTRL + Qt::Key_W, SLOT(addSpace2D()), true);
    //go
    KStandardAction::home(this, SLOT(goHome()), actionCollection());
    //tools dashboard
    createAction("delete_currentspace", i18n("&Remove Current Space"), "list-remove", Qt::CTRL + Qt::Key_W, SLOT(addSpace2D()));
    //tools space
    createAction("copy_snapshot", i18n("&Copy Space Snapshot"), "edit-copy", Qt::CTRL + Qt::Key_W, SLOT(addSpace2D()));
    createAction("export_snapshot", i18n("&Export Space Snapshot"), "view-preview", Qt::CTRL + Qt::Key_W, SLOT(addSpace2D()));
    //settings
    
    
//     connect(m_dashboard, SIGNAL(saveRequest()), SLOT(saveFile()));
//     connect(m_dashboard, SIGNAL(openRequest()), SLOT(openFile()));
}

void MainWindow::setupToolBars()
{
//     hideSpaceToolBar();
    
//     qDebug() << action("add_space2d")->isCheckable();
}

void MainWindow::setupDocks()
{
    m_spacePlotsDock = new PlotsEditor(this);
    Ui::DockWidget uispaceItemWidget;
    uispaceItemWidget.setupUi(m_spacePlotsDock);
    m_spacePlotsDock->setObjectName("asdasdds");

    m_spaceInfoDock = new QDockWidget(this);
    Ui::spaceItemWidget uispaceItemWidget1;
    uispaceItemWidget1.setupUi(m_spaceInfoDock);
    m_spacePlotsDock->setObjectName("asdasdds222");
    
    m_spaceOptionsDock = new QDockWidget(this);
    Ui::coordSysSettingsDock_2 uispaceItemWidget11;
    uispaceItemWidget11.setupUi(m_spaceOptionsDock);
    m_spacePlotsDock->setObjectName("a33sdasdds");

        
    addDockWidget(Qt::LeftDockWidgetArea, m_spacePlotsDock);
    addDockWidget(Qt::RightDockWidgetArea, m_spaceInfoDock);
    addDockWidget(Qt::RightDockWidgetArea, m_spaceOptionsDock);
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
    MainWindow * newWnd = new MainWindow();
    newWnd->show();
}
void MainWindow::openFile()
{
 
}

void MainWindow::activateDashboardUi()
{
    //menubar
    //edit
    action("add_space2d")->setVisible(true);
    action("add_space3d")->setVisible(true);
    //view
    action("show_plots")->setVisible(true);
    action("show_spaces")->setVisible(true);
    action("show_plots_editor")->setVisible(false);
    action("show_space_info")->setVisible(false);
    action("show_plotter_options")->setVisible(false);
    //tools
    action("copy_snapshot")->setVisible(false);
    action("export_snapshot")->setVisible(false);
    
    //toolbars
    toolBar("spaceToolBar")->hide();
    toolBar("mainToolBar")->show();

    //docks
    m_spacePlotsDock->hide();
    m_spaceInfoDock->hide();
    m_spaceOptionsDock->hide();
}

void MainWindow::activateSpaceUi()
{
    //menu
    //edit
    action("add_space2d")->setVisible(false);
    action("add_space3d")->setVisible(false);
    //view
    action("show_plots")->setVisible(false);
    action("show_spaces")->setVisible(false);
    action("show_plots_editor")->setVisible(true);
    action("show_space_info")->setVisible(true);
    action("show_plotter_options")->setVisible(true);
    //tools
    action("copy_snapshot")->setVisible(true);
    action("export_snapshot")->setVisible(true);
    
    
    //toolbars
    toolBar("mainToolBar")->hide();
    toolBar("spaceToolBar")->show();

    //docks
    m_spacePlotsDock->show();
    m_spaceInfoDock->show();
    m_spaceOptionsDock->show();
}


void MainWindow::addSpace2D()
{
    m_dashboard->setCurrentIndex(1);
    activateSpaceUi();
}

void MainWindow::addSpace3D()
{
    m_dashboard->setCurrentIndex(2);
    activateSpaceUi();
}

void MainWindow::goHome()
{
    m_dashboard->setCurrentIndex(0);
    activateDashboardUi();
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

