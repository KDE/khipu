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


#include <QtGui/QLayout>
#include <QtGui/QStackedWidget>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QDebug>
#include <KDE/KPushButton>
#include <KDE/KLineEdit>
#include <KDE/KComboBox>
#include <KFileDialog>
#include "functionsmodel.h"
#include "spaceitem.h"
#include "space2dviewer.h"
#include "space3dviewer.h"
#include "spacesmodel.h"
#include "analitza/variables.h"
#include "ui_dashboard.h"
#include <QDomDocument>
#include <analitza/analyzer.h>
#include <analitzaplot/plotsview2d.h>
#include <QTextCodec>
#include <QBuffer>
 
// class DashboardWidget : public QStackedWidget, public Ui::DashboardWidget
// {
// public:
//     DashboardWidget(QWidget *parent = 0)
//         : QStackedWidget(parent)
//     {
//         setupUi(this);
// 
//         
// /*
//         openFile->setIcon(KIcon("document-open"));
//         saveFile->setIcon(KIcon("document-save"));
// 
// 
//         addSpace2D->setIcon(KIcon("add-space2d"));
//         addSpace3D->setIcon(KIcon("add-space3d"));
//         backFromSpace2D->setIcon(KIcon("go-previous"));
//         backFromSpace3D->setIcon(KIcon("go-previous"));
// 
//         filterAllSpaces->setIcon(KIcon("format-justify-fill"));
// 
//         filter2DSpaces->setIcon(KIcon("office-chart-line"));
//         filter3DSpaces->setIcon(KIcon("office-chart-ring"));
// 
//         filterAllFunctions->setIcon(KIcon("format-justify-fill"));
//         filter2DFunctions->setIcon(KIcon("office-chart-line"));
//         filter3DFunctions->setIcon(KIcon("office-chart-ring"));
// 
//         showCoordSysSettings2D->setIcon(KIcon("configure"));
//         showCoordSysSettings3D->setIcon(KIcon("configure"));
// 
//         showFunctionEditor2D->setIcon(KIcon("address-book-new"));
//         showFunctionEditor3D->setIcon(KIcon("address-book-new"));
// 
// 
// 
//         showSpace2DInfo->setIcon(KIcon("document-edit"));
//         showSpace3DInfo->setIcon(KIcon("document-edit"));
// 
//         saveSpace2DImage->setIcon(KIcon("view-preview"));
//         saveSpace3DImage->setIcon(KIcon("view-preview"));
// 
//         copySpace2DImage->setIcon(KIcon("application-x-mswrite"));
//         copySpace3DImage->setIcon(KIcon("application-x-mswrite"));
// 
//         showAboutAppDialog->setIcon(KIcon("help-about"));*/
// 
// 
// 
//         filterTextFunctions->setClearButtonShown(true);
//         filterTextSpaces->setClearButtonShown(true);
//     }
// };






Dashboard::Dashboard(QWidget *parent)
    : QStackedWidget(parent)
{
    Ui::DashboardWidget a;
    a.setupUi(this);
    
    
    ///
//     setupWidget();

//     m_dashboardWidget->spaces->setModel(m_spacesProxyModel);
//     m_widget->functions->setModel(m_functionsProxyModel);

//     m_widget->functions->setIsMainFunctionsView(true);


    

//     QHBoxLayout *mainLayout = new QHBoxLayout(this);
//     mainLayout->addWidget(m_widget);
}

Dashboard::~Dashboard()
{
}

void Dashboard::goHome()
{
    
//     if (m_widget->currentIndex() == 2)
//     {
// 
// 
//         m_widget->space3D->saveSpace();
//     }
//     else if (m_widget->currentIndex() == 1) 
//         m_widget->space2D->saveSpace();
// 
// 
// 
//     m_widget->setCurrentIndex(0);
}


void Dashboard::addSpace2D()
{
//      SpaceItem space(2);
// 
// 
//     m_dashboardWidget->setCurrentIndex(1);
//     m_spacesModel->addSpace(space);
// 
//     m_dashboardWidget->space2D->setSpace(space);
//     m_proxyViewer2D->setFilterSpaceId(space.id());
// 
//     updateGPLACSDocument();
}

void Dashboard::addSpace3D()
{
//      SpaceItem space(3);
// 
//     m_dashboardWidget->setCurrentIndex(2);
//     m_spacesModel->addSpace(space);
// 
//     m_dashboardWidget->space3D->setSpace(space);
// 
//     m_proxyViewer3D->setFilterSpaceId(space.id());
// 
// 
// 
// 
// 
//     updateGPLACSDocument();

    

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

