/*************************************************************************************
 *  Copyright (C) 2010 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>      *
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


#include "viewer3d.h"
#include <QtGui/QLayout>
#include "function/functionsmodel.h"
#include "function/function.h"



#include "KIcon"
#include <QPropertyAnimation>
#include <QGraphicsProxyWidget>
#include <QSlider>
#include <QtOpenGL>
#include <KComboBox>
#include <QStackedWidget>
#include <QImage>




#include "space/spacesmodel.h"
#include "function/functioneditor.h"
#include "function/functionsview.h"
#include <QMouseEvent>
#include <KIconLoader>



#include "space/space.h"
#include "ui_viewer3dwidget.h"

namespace Keomath
{



class Viewer3DWidget : public QWidget, public Ui::Viewer3DWidget
{
public:
    Viewer3DWidget(QWidget *parent = 0)
        : QWidget(parent)
    {
        setupUi(this);







        view->setAxisIsDrawn(true);

        
        

        view->camera()->setPosition(qglviewer::Vec(0,0,10));
        











        functionEditorDock->close();
        coordSysSettingsDock->close();
        spaceInfoDock->close();
        connect(showAxis, SIGNAL(toggled(bool)), view, SLOT(setAxisIsDrawn(bool)));
        connect(showRefPlane, SIGNAL(toggled(bool)), view, SLOT(setGridIsDrawn(bool)));






    }
};



FunctionEditor *Viewer3D::functionEditor() const
{
    return qobject_cast<FunctionEditor*>(m_viewer3DWidget->functionEditorDock->widget());
}

Viewer3D::Viewer3D(QWidget *parent)
    : QWidget(parent)
{
    m_viewer3DWidget = new Viewer3DWidget(this);

    m_functionEditor = new FunctionEditor(m_viewer3DWidget->functionEditorDock);
    m_functionEditor->setupEditorBehaviour(3);

    m_viewer3DWidget->functionEditorDock->setWidget(m_functionEditor);


    connect(m_viewer3DWidget->space3DSize, SIGNAL(valueChanged(int)), SLOT(resizeScene3D(int)));

















    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_viewer3DWidget);






    connect(m_viewer3DWidget->view, SIGNAL(viewClicked()), SLOT(hideDocks()));


    resizeScene3D(5);

}




void Viewer3D::setFunctionsModel(FunctionsFilterProxyModel *functionsFilterProxyModel)
{

    m_functionsFilterProxyModel = functionsFilterProxyModel;
    m_functionEditor->setFunctionsModel(functionsFilterProxyModel);
    m_viewer3DWidget->view->setFunctionsModel(functionsFilterProxyModel);




    connect(m_functionEditor->functionsView(), SIGNAL(functionShown(Keomath::Function)),
            m_viewer3DWidget->view, SLOT(updateSurface(Keomath::Function)));

    connect(m_functionEditor->functionsView(), SIGNAL(functionHided(QUuid,QString)),
            m_viewer3DWidget->view, SLOT(removeSurface(QUuid,QString)));



}
void Viewer3D::setSpacesModel(SpacesModel *spacesModel)
{

    m_spacesModel = spacesModel;
}


void Viewer3D::setSpace(const Space &space)
{

    m_viewer3DWidget->functionEditorDock->close();
    m_viewer3DWidget->coordSysSettingsDock->close();
    m_viewer3DWidget->spaceInfoDock->close();
    
    m_viewer3DWidget->view->setBackgroundColor(Qt::black);

    m_space = space;
    m_viewer3DWidget->spaceTitle->setText(m_space.name());
    m_viewer3DWidget->spaceDescription->setPlainText(m_space.description());



    m_viewer3DWidget->view->setSpaceId(space.id());
    


    m_viewer3DWidget->spaceTitle->setText(m_space.name());
    m_viewer3DWidget->spaceDescription->setPlainText(m_space.description());


















    
    
}


void Viewer3D::toggleShownFunctionEditor()
{
    m_viewer3DWidget->functionEditorDock->setVisible(!m_viewer3DWidget->functionEditorDock->isVisible());
}

void Viewer3D::toggleShownCoordSysSettings()
{
    m_viewer3DWidget->coordSysSettingsDock->setVisible(!m_viewer3DWidget->coordSysSettingsDock->isVisible());
}

void Viewer3D::toggleShownSpaceInfo()
{
    m_viewer3DWidget->spaceInfoDock->setVisible(!m_viewer3DWidget->spaceInfoDock->isVisible());
}

void Viewer3D::hideDocks()
{
    m_viewer3DWidget->functionEditorDock->close();
    m_viewer3DWidget->coordSysSettingsDock->close();
    m_viewer3DWidget->spaceInfoDock->close();

    
    
    
}

void Viewer3D::resizeScene3D(int v)
{
    qreal si = (qreal)v;



    m_viewer3DWidget->view->setSceneCenter(qglviewer::Vec(0.f,0.f,0.f));
    m_viewer3DWidget->view->setSceneRadius(si);

    m_viewer3DWidget->view->updateGL();
}

void Viewer3D::toImage(const QString laurl)
{
    m_viewer3DWidget->view->setSnapshotQuality(100);
    m_viewer3DWidget->view->setSnapshotFileName(i18n("Surface"));
    m_viewer3DWidget->view->setSnapshotCounter(0);

    m_viewer3DWidget->view->saveSnapshot(laurl, false);
}

void Viewer3D::copyImageToClipboard()
{
    m_viewer3DWidget->view->snapshotToClipboard();
}


void Viewer3D::setVariables(Analitza::Variables* v)
{
    m_functionEditor->setVariables(v);
}


Analitza::Variables* Viewer3D::variables() const
{
    return m_functionEditor->variables();
}


void Viewer3D::saveSpace()
{














    hideDocks();

    m_viewer3DWidget->view->updateGL();
    m_viewer3DWidget->view->setFocus();
    m_viewer3DWidget->view->makeCurrent();
    m_viewer3DWidget->view->raise();
    QImage image(m_viewer3DWidget->view->grabFrameBuffer(true));

    QPixmap thumbnail = QPixmap::fromImage(image, Qt::ColorOnly);
    thumbnail = thumbnail.scaled(QSize(240, 240), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);






    





    m_space.setName(m_viewer3DWidget->spaceTitle->text());
    m_space.setDescription(m_viewer3DWidget->spaceDescription->toPlainText());
    m_space.setThumbnail(thumbnail);

    m_spacesModel->editSpace(m_space.id(), m_space);

}



}

