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

#include "ui_space2dviewer.h"
#include "ui_space3dviewer.h"


MainWindow::MainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
{
    m_document = new Document(this);
    m_dashboard = new Dashboard(this);

    setupActions();
    setupGUI(Keys | StatusBar | Save | Create, "khipu.rc");
    setCentralWidget(m_dashboard);

    QDockWidget *dock = new QDockWidget(this);
    
    Ui::spaceItemWidget uispaceItemWidget;
    uispaceItemWidget.setupUi(dock);
    
        QDockWidget *dock1 = new QDockWidget(this);
    
    Ui::coordSysSettingsDock_2 uispaceItemWidget1;
    uispaceItemWidget1.setupUi(dock1);
    
    addDockWidget(Qt::RightDockWidgetArea, dock);
    addDockWidget(Qt::RightDockWidgetArea, dock1);

    updateTittleWhenOpenSaveDoc();

    
    
    
    
    

    connect(m_dashboard, SIGNAL( modified() ), SLOT( updateTittleWhenChangeDocState() ));

    connect(m_dashboard, SIGNAL(dashemitShowAppInfo()), SLOT(showAboutAppDialog()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupActions()
{
    KStandardAction::quit   (this, SLOT( close()      ), actionCollection());

    KAction* clearAction = new KAction(this);
    clearAction->setText(i18n("&Clear"));
    clearAction->setIcon(KIcon("document-new"));
    clearAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("zoom_operations", clearAction);


    KAction* clearAction1 = new KAction(this);
    clearAction1->setText(i18n("&Clear"));
    clearAction1->setIcon(KIcon("kde"));
    clearAction1->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("find_operations", clearAction1);

    KAction* clearAction11 = new KAction(this);
    clearAction11->setText(i18n("&Clear"));
    clearAction11->setIcon(KIcon("list-add"));
    clearAction11->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("new_space2d", clearAction11);


//   KStandardAction::open(this, SLOT(configureToolbars()), actionCollection());
    KStandardAction::open(this, SLOT(hide3dtb()), actionCollection());
//   unplugActionList("");
//     toolBar("view2DToolbar")->setVisible(true);


    connect(m_dashboard, SIGNAL(saveRequest()), SLOT(saveFile()));
    connect(m_dashboard, SIGNAL(openRequest()), SLOT(openFile()));
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

void MainWindow::openFile()
{
 
}

void MainWindow::showAboutAppDialog()
{
    const KAboutData *aboutData = KGlobal::mainComponent().aboutData();
    KAboutApplicationDialog a(aboutData, KAboutApplicationDialog::HideTranslators, this);
    a.exec();
}

void MainWindow::newFile()
{
    MainWindow * newWnd = new MainWindow();
    newWnd->show();
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

