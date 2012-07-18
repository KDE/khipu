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

 
MainWindow::MainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
{



    m_functionsModel = new  FunctionsModel(this);
    
    m_functionsModel->addPlaneCurve(Analitza::Expression("x*x+y*y=4"), "asdas", Qt::lightGray);
    
    m_spacesModel = new  SpacesModel(this);

    m_gplacsWidget = new Dashboard(m_functionsModel, m_spacesModel, this);

    
//     statusBar()->hide();
    setupActions();



// toolBar("fooToolBar")->setWindowTitle("edqweqewe");

   setCentralWidget(m_gplacsWidget); 
//     QTabWidget *tabs = new QTabWidget(this);
//     tabs->addTab(m_gplacsWidget, "ad");
//     
//     VisualItemsModel *model = m_functionsModel;
//     KCategorizedSortFilterProxyModel *proxyModel = new KCategorizedSortFilterProxyModel(this);
//     proxyModel->setCategorizedModel( true );
//     proxyModel->setSortCategoriesByNaturalComparison(true);
//     proxyModel->setSourceModel( model );
//     proxyModel->sort( 0 );
//     
// 
//         
//     KCategorizedView *categoryView = new KCategorizedView(this);
//     KCategoryDrawerV2 *categoryDrawer = new KCategoryDrawerV2();
//     categoryView->setSelectionMode( QAbstractItemView::SingleSelection );
// //     categoryView->setSpacing( KDialog::spacingHint() );
//     categoryView->setCategoryDrawer( categoryDrawer );
// //     categoryView->setViewMode( QListView::IconMode );
// //     categoryView->setMouseTracking( true );
// //     categoryView->viewport()->setAttribute( Qt::WA_Hover );
// //     categoryView->setFrameShape( QFrame::NoFrame );
//     categoryView->setModel( proxyModel );
//     categoryView->setGridSize(QSize(200,32));
// 
//     proxyModel->setCategorizedModel(false); //BEGIN NOTE IMPORTANT WARNING PRIMERO HACER UN LOCK
// 
//     
//     PlaneCurve *item = model->addPlaneCurve(Analitza::Expression("x->x*x"), "para", Qt::cyan);
//     model->addPlaneCurve(Analitza::Expression("q->q+2"), "polar simple", Qt::green);
//     model->addPlaneCurve(Analitza::Expression("t->vector{t*t+1, t+2}"), "vec", Qt::yellow);
//     PlaneCurve *item2 = model->addPlaneCurve(Analitza::Expression("5*(x**2+y**2)**3=15*(x*y*72)**2"), "impl", Qt::red);
//     model->addPlaneCurve(Analitza::Expression("x->2+x*x"), "otra simple", Qt::blue);
//     model->addPlaneCurve(Analitza::Expression("t->vector{t*t+1, 2*t+2}"), "vec", Qt::yellow);
//     model->addPlaneCurve(Analitza::Expression("t->vector{t*t+1, 3*t+2}"), "vec", Qt::yellow);
//     model->addPlaneCurve(Analitza::Expression("t->vector{t*t+1, 4*t+2}"), "vec", Qt::yellow);
//     model->addPlaneCurve(Analitza::Expression("t->vector{t*t+1, 4*t+2}"), "vec", Qt::yellow);
//     model->addPlaneCurve(Analitza::Expression("t->vector{t*t+1, 4*t+2}"), "vec", Qt::yellow);
//     model->addPlaneCurve(Analitza::Expression("t->vector{t*t+1, 4*t+2}"), "vec", Qt::yellow);
//     proxyModel->setCategorizedModel(true); //END NOTE IMPORTANT WARNING PRIMERO HACER UN LOCK
// 
//     tabs->addTab(categoryView, "lis");
//     
//     tabs->setCurrentIndex(1);
//     setCentralWidget(tabs);
    


    updateTittleWhenOpenSaveDoc();


    connect(m_gplacsWidget, SIGNAL( modified() ), SLOT( updateTittleWhenChangeDocState() ));

    connect(m_gplacsWidget, SIGNAL(dashemitShowAppInfo()), SLOT(showAboutAppDialog()));

//     menuBar()->hide();

    





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


    
    setupGUI(Keys | StatusBar | Save | Create, "khipu.rc");
    

    
//     toolBar("view2DToolbar")->setVisible(true);
    

    connect(m_gplacsWidget, SIGNAL(saveRequest()), SLOT(saveFile()));
    connect(m_gplacsWidget, SIGNAL(openRequest()), SLOT(openFile()));
    
    
    //NOTE registeredCurves servira mas adelante para construir mejores mensajes al usario sobre que tipo de curva se dibujara
    // y para construir un buen functioneditor
    for (int i = 0; i < PlaneCurve::registeredCurves().size(); ++i)
        qDebug() << PlaneCurve::registeredCurves().keys()[i] << PlaneCurve::registeredCurves().values()[i].first << PlaneCurve::registeredCurves().values()[i].second.toString();

}

void MainWindow::hide3dtb()
{

//     toolBar("view2DToolbar")->setVisible(!toolBar("view2DToolbar")->isVisible());

//     bool f = toolBar("view2DToolbar")->isVisible();
    
//     toolBar("view2DToolBar")->setVisible(f);
 

    //los index del xml en los toolbars corresponden a toolbars
    
//     toolBars()[0]->setVisible(false);
}

 
bool MainWindow::queryClose()
{
    if (m_gplacsWidget->isModified())
    {
        QString paletteFileName = m_gplacsWidget->fileName();

        if (paletteFileName.isEmpty())
            paletteFileName = i18n("Untitled");

        switch (KMessageBox::warningYesNoCancel(this,
                                                i18n( "The document \"%1\" has been modified.\n"
                                                        "Do you want to save your changes or discard them?", paletteFileName),
                                                i18n( "Close Document" ), KStandardGuiItem::save(), KStandardGuiItem::discard()))
        {
        case KMessageBox::Yes:
        {
            
            
            
            
            
            m_gplacsWidget->showDashboard();

            saveFile();

            return m_gplacsWidget->isSaved();
        }
        case KMessageBox::No :
            return true;
            
        default :
            return false;
        }
    }

    return true;
}

void MainWindow::openFile()
{
    QString filter = QString("*.gplacs|") + i18n("All Supported Files") +
                     QString("\n*.gplacs|") + i18n("GPLACS standard file format") + QString(" (*.gplacs)");

    KUrl paletteUrl = KFileDialog::getOpenUrl(KUrl(), filter);



    QString paletteFileName;

    if (!paletteUrl.isEmpty())
    {
        if (KIO::NetAccess::download(paletteUrl, paletteFileName, this))
        {
            if (m_gplacsWidget->load(paletteFileName))
            {
                updateTittleWhenOpenSaveDoc();

                m_gplacsWidget->setModified(false);
                m_gplacsWidget->setSaved(true);
            }



            KIO::NetAccess::removeTempFile(paletteFileName);
        }
        else
            KMessageBox::error(this, KIO::NetAccess::lastErrorString());
    }
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

void MainWindow::saveFile()
{
    

    if (!m_gplacsWidget->fileName().isEmpty())
    {
        if (m_gplacsWidget->save(m_gplacsWidget->fileName()))
        {
            updateTittleWhenOpenSaveDoc();
        }

    }
    else
    {
        QString filter = QString("*.gplacs|") + i18n("All Supported Files") +
                         QString("\n*.gplacs|") + i18n("GPLACS standard file format") + QString(" (*.gplacs)");

        QString paletteFileName = KFileDialog::getSaveFileName(KUrl(QDir::homePath()), filter);

        if (KIO::NetAccess::exists(KUrl(paletteFileName), KIO::NetAccess::DestinationSide, widget()))
            if (KMessageBox::warningContinueCancel(widget(), i18n("A file named \"%1\" already exists. Are you sure you want to overwrite it?", paletteFileName), QString(), KGuiItem(i18n("Overwrite"))) != KMessageBox::Continue)
                return;


        if (m_gplacsWidget->save(paletteFileName))
        {
            updateTittleWhenOpenSaveDoc();

        }

    }
}

void MainWindow::saveFileAs(const QString &filename)
{






    
}


void MainWindow::updateInputTypePreviewImage(bool text_input)
{






}

void MainWindow::updateTittleWhenChangeDocState()
{
    QString paletteFileName = m_gplacsWidget->fileName();

    if (paletteFileName.isEmpty())
        paletteFileName = i18n("Untitled");

    setWindowTitle(QString("%1 - GPLACS " + i18n("[modificado]")).arg(paletteFileName));
}

void MainWindow::updateTittleWhenOpenSaveDoc()
{
    QString paletteFileName = m_gplacsWidget->fileName();

    if (paletteFileName.isEmpty())
        paletteFileName = i18n("Untitled");

    setWindowTitle(QString("%1 - GPLACS").arg(paletteFileName));
}

void MainWindow::optionsPreferences()
{

} 
