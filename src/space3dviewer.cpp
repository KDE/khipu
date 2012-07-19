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


#include "space3dviewer.h"
#include <QtGui/QLayout>
#include "functionsmodel.h"



#include "KIcon"
#include <QPropertyAnimation>
#include <QGraphicsProxyWidget>
#include <QSlider>
#include <QtOpenGL>
#include <KComboBox>
#include <QStackedWidget>
#include <QImage>




#include "spacesmodel.h"
#include "functioneditor.h"
#include "functionsview.h"
#include <QMouseEvent>
#include <KIconLoader>



#include "spaceitem.h"
#include "ui_space3dviewer.h"


class Viewer3DWidget : public QWidget, public Ui::Viewer3DWidget
{
public:
    Viewer3DWidget(QWidget *parent = 0)
        : QWidget(parent)
    {
        setupUi(this);







//         view->setAxisIsDrawn(true);

        
        

//         view->camera()->setPosition(qglviewer::Vec(0,0,10));
        











        functionEditorDock->close();
        coordSysSettingsDock->close();
        spaceInfoDock->close();
        connect(showAxis, SIGNAL(toggled(bool)), view, SLOT(setAxisIsDrawn(bool)));
        connect(showRefPlane, SIGNAL(toggled(bool)), view, SLOT(setGridIsDrawn(bool)));






    }
};



FunctionEditor *Space3DViewer::functionEditor() const
{
    return qobject_cast<FunctionEditor*>(m_viewer3DWidget->functionEditorDock->widget());
}

Space3DViewer::Space3DViewer(QWidget *parent)
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




void Space3DViewer::setFunctionsModel(FunctionsFilterProxyModel *functionsFilterProxyModel)
{

    m_functionsFilterProxyModel = functionsFilterProxyModel;
    m_functionEditor->setFunctionsModel(functionsFilterProxyModel);
    //TODO
//     m_viewer3DWidget->view->setFunctionsModel(functionsFilterProxyModel);




    connect(m_functionEditor->functionsView(), SIGNAL(functionShown( Function)),
            m_viewer3DWidget->view, SLOT(updateSurface( Function)));

    connect(m_functionEditor->functionsView(), SIGNAL(functionHided(QUuid,QString)),
            m_viewer3DWidget->view, SLOT(removeSurface(QUuid,QString)));



}
void Space3DViewer::setSpacesModel(SpacesModel *spacesModel)
{

    m_spacesModel = spacesModel;
}


void Space3DViewer::setSpace(const SpaceItem &space)
{
/*
    
    
    m_viewer3DWidget->functionEditorDock->close();
    m_viewer3DWidget->coordSysSettingsDock->close();
    m_viewer3DWidget->spaceInfoDock->close();
    
//     m_viewer3DWidget->view->setBackgroundColor(Qt::black);

    m_space = space;
    m_viewer3DWidget->spaceTitle->setText(m_space.title());
    m_viewer3DWidget->spaceDescription->setPlainText(m_space.description());



    //TODO
//     m_viewer3DWidget->view->setSpaceId(space.id());
    


    m_viewer3DWidget->spaceTitle->setText(m_space.title());
    m_viewer3DWidget->spaceDescription->setPlainText(m_space.description());*/


}

void Space3DViewer::toggleShownFunctionEditor()
{
    m_viewer3DWidget->functionEditorDock->setVisible(!m_viewer3DWidget->functionEditorDock->isVisible());
}

void Space3DViewer::toggleShownCoordSysSettings()
{
    m_viewer3DWidget->coordSysSettingsDock->setVisible(!m_viewer3DWidget->coordSysSettingsDock->isVisible());
}

void Space3DViewer::toggleShownSpaceInfo()
{
    m_viewer3DWidget->spaceInfoDock->setVisible(!m_viewer3DWidget->spaceInfoDock->isVisible());
}

void Space3DViewer::hideDocks()
{
    m_viewer3DWidget->functionEditorDock->close();
    m_viewer3DWidget->coordSysSettingsDock->close();
    m_viewer3DWidget->spaceInfoDock->close();
}

void Space3DViewer::resizeScene3D(int v)
{
    qreal si = (qreal)v;

//     m_viewer3DWidget->view->setSceneCenter(qglviewer::Vec(0.f,0.f,0.f));
//     m_viewer3DWidget->view->setSceneRadius(si);

}

void Space3DViewer::toImage(const QString laurl)
{
//     m_viewer3DWidget->view->setSnapshotQuality(100);
//     m_viewer3DWidget->view->setSnapshotFileName(i18n("Surface"));
//     m_viewer3DWidget->view->setSnapshotCounter(0);
// 
//     m_viewer3DWidget->view->saveSnapshot(laurl, false);
}

void Space3DViewer::copyImageToClipboard()
{
//     m_viewer3DWidget->view->snapshotToClipboard();
}


void Space3DViewer::setVariables(Analitza::Variables* v)
{
    m_functionEditor->setVariables(v);
}


Analitza::Variables* Space3DViewer::variables() const
{
    return m_functionEditor->variables();
}


void Space3DViewer::saveSpace()
{
/*
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

    m_spacesModel->editSpace(m_space.id(), m_space);*/

}

