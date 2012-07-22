// /*************************************************************************************
//  *  Copyright (C) 2010 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>      *
//  *                                                                                   *
//  *  This program is free software; you can redistribute it and/or                    *
//  *  modify it under the terms of the GNU General Public License                      *
//  *  as published by the Free Software Foundation; either version 2                   *
//  *  of the License, or (at your option) any later version.                           *
//  *                                                                                   *
//  *  This program is distributed in the hope that it will be useful,                  *
//  *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
//  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
//  *  GNU General Public License for more details.                                     *
//  *                                                                                   *
//  *  You should have received a copy of the GNU General Public License                *
//  *  along with this program; if not, write to the Free Software                      *
//  *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
//  *************************************************************************************/
// 
// 
// #include "space2dviewer.h"
// 
// #include <QtGui/QLayout>
// #include "functionsmodel.h"
// #include "KIcon"
// #include <QPropertyAnimation>
// #include <QGraphicsProxyWidget>
// #include <QSlider>
// #include <QtOpenGL>
// #include <KComboBox>
// #include <QStackedWidget>
// 
// 
// 
// 
// #include <math.h>
// #include <cmath>
// 
// #include <QMouseEvent>
// #include <KIconLoader>
// 
// #include <KColorUtils>
// 
// 
// #include "analitzaplot/plotsview2d.h"
// #include "functioneditor.h"
// #include "spaceitem.h"
// #include "spacesmodel.h"
// #include "ui_space2dviewer.h"
// #include "functionsview.h"
// 
// 
// class Viewer2DWidget : public QWidget, public Ui::Viewer2DWidget
// {
// public:
//     Viewer2DWidget(QWidget *parent = 0)
//         : QWidget(parent)
//     {
//         setupUi(this);
// 
// 
// 
// 
//         functionEditorDock->close();
//         coordSysSettingsDock->close();
//         spaceInfoDock->close();
// 
// 
//     }
// };
// 
// 
// 
// 
// SpaceViewer::SpaceViewer(QWidget *parent)
//     : QWidget(parent)
// {
// 
// 
//     m_viewer2DWidget = new Viewer2DWidget(this);
// 
//     m_functionEditor = new FunctionEditor(m_viewer2DWidget->functionEditorDock);
// 
//     m_functionEditor->setupEditorBehaviour(2);
//     m_viewer2DWidget->functionEditorDock->setWidget(m_functionEditor);
// 
// 
//     QVBoxLayout *layout = new QVBoxLayout(this);
//     layout->addWidget(m_viewer2DWidget);
// 
// 
// 
// 
// 
//     connect(m_viewer2DWidget->view, SIGNAL(viewClicked()), SLOT(hideDocks()));
// 
//     
//     connect(m_viewer2DWidget->xAxisLabel, SIGNAL(textChanged(QString)), m_viewer2DWidget->view, SLOT(setXAxisLabel(QString)));
//     connect(m_viewer2DWidget->yAxisLabel, SIGNAL(textChanged(QString)), m_viewer2DWidget->view, SLOT(setYAxisLabel(QString)));
// 
// 
//     QString sym = QString((QChar(0x03C0)));
// 
//     addTickEntry(sym+"/6", sym, M_PI, 1, 6);
//     addTickEntry(sym+"/5", sym, M_PI, 1, 5);
//     addTickEntry(sym+"/4", sym, M_PI, 1, 4);
//     addTickEntry(sym+"/3", sym, M_PI, 1, 3);
//     addTickEntry(sym+"/2", sym, M_PI, 1, 2);
//     addTickEntry(sym, sym, M_PI, 1, 1);
//     addTickEntry("2"+sym, sym, M_PI, 2, 1);
//     addTickEntry("3"+sym, sym, M_PI, 3, 1);
//     addTickEntry("4"+sym, sym, M_PI, 4, 1);
//     addTickEntry("5"+sym, sym, M_PI, 5, 1);
//     addTickEntry("6"+sym, sym, M_PI, 6, 1);
// 
// 
//     sym = QString("e");
// 
//     addTickEntry(sym+"/2", sym, M_E, 1, 2);
//     addTickEntry(sym, sym, M_E, 1, 1);
//     addTickEntry("2"+sym, sym, M_E, 2, 1);
// 
// 
// 
// 
//     sym = QString((QChar(0x221A)))+"2";
//     qreal symval = sqrt(2);
//     addTickEntry(sym+"/2", sym, symval, 1, 2);
//     addTickEntry(sym, sym, symval, 1, 1);
//     addTickEntry("2"+sym, sym, symval, 2, 1);
// 
//     sym = QString((QChar(0x221A)))+"3";
//     symval = sqrt(3);
//     addTickEntry(sym+"/3", sym, symval, 1, 3);
//     addTickEntry(sym+"/2", sym, symval, 1, 2);
//     addTickEntry(sym, sym, symval, 1, 1);
//     addTickEntry("2"+sym, sym, symval, 2, 1);
//     addTickEntry("3"+sym, sym, symval, 3, 1);
// 
// 
//     addTickEntry("1", QString(), 1, 1, 1);
//     addTickEntry("2", QString(), 1, 2, 1);
//     addTickEntry("3", QString(), 1, 3, 1);
//     addTickEntry("4", QString(), 1, 4, 1);
//     addTickEntry("5", QString(), 1, 5, 1);
//     addTickEntry("6", QString(), 1, 6, 1);
//     addTickEntry("7", QString(), 1, 7, 1);
//     addTickEntry("8", QString(), 1, 8, 1);
//     addTickEntry("9", QString(), 1, 9, 1);
// 
//     m_viewer2DWidget->tickScale->setCurrentIndex(22);
// 
// 
// 
//     connect(m_viewer2DWidget->tickScale, SIGNAL(currentIndexChanged(int)), SLOT(updateTick(int)));
//     connect(m_viewer2DWidget->useSymbols, SIGNAL(toggled(bool)), m_viewer2DWidget->view,
//             SLOT(setUseTickSymbols(bool)));
// 
// 
//     connect(m_viewer2DWidget->showHTicks, SIGNAL(toggled(bool)), m_viewer2DWidget->view,
//             SLOT(showHTicks(bool)));
//     connect(m_viewer2DWidget->showVTicks, SIGNAL(toggled(bool)), m_viewer2DWidget->view,
//             SLOT(showVTicks(bool)));
// 
// 
// 
// 
//     connect(m_viewer2DWidget->gridColor, SIGNAL(activated(int)), SLOT(updateGridColor(int)));
// 
// 
//     connect(m_viewer2DWidget->showMeshGrid, SIGNAL(toggled(bool)), m_viewer2DWidget->view, SLOT(setMeshGridShown(bool)));
// 
//     connect(m_viewer2DWidget->showXAxis, SIGNAL(toggled(bool)), m_viewer2DWidget->view,
//             SLOT(showHAxes(bool)));
//     connect(m_viewer2DWidget->showYAxis, SIGNAL(toggled(bool)), m_viewer2DWidget->view,
//             SLOT(showVAxes(bool)));
//     connect(m_viewer2DWidget->showRTang, SIGNAL(clicked(bool)),m_viewer2DWidget->view,SLOT(showRTang(bool)));
//     connect(m_viewer2DWidget->showRNorm, SIGNAL(clicked(bool)),m_viewer2DWidget->view,SLOT(showRNorm(bool)));
// 
// 
// 
// 
//     connect(m_viewer2DWidget->coordinateSystems, SIGNAL(activated(int)), m_viewer2DWidget->view,
//             SLOT(useCoorSys(int)));
// 
// 
//     
//     connect(m_viewer2DWidget->view, SIGNAL(coordSysUsed(int)), m_viewer2DWidget->coordinateSystems,
//             SLOT(setCurrentIndex(int)));
// 
// 
//     
// 
// }
// 
// void SpaceViewer::hideDocks()
// {
//     m_viewer2DWidget->functionEditorDock->close();
//     m_viewer2DWidget->coordSysSettingsDock->close();
//     m_viewer2DWidget->spaceInfoDock->close();
// 
// }
// 
// 
// FunctionEditor *SpaceViewer::functionEditor()
// {
//     return qobject_cast<FunctionEditor*>(m_viewer2DWidget->functionEditorDock->widget());
// }
// 
// 
// 
// 
// 
// void SpaceViewer::setSpace(const SpaceItem &space)
// {
// //     m_viewer2DWidget->functionEditorDock->close();
// //     m_viewer2DWidget->coordSysSettingsDock->close();
// //     m_viewer2DWidget->spaceInfoDock->close();
// // 
// // 
// // 
// // //     m_space = space;
// // 
// //     m_viewer2DWidget->spaceTitle->setText(m_space.title());
// //     m_viewer2DWidget->spaceDescription->setPlainText(m_space.description());
// // 
// // 
// // 
// //     if (!m_functionsFilterProxyModel) return;
// // 
// //     FunctionsModel *functionModel = static_cast<FunctionsModel*>(m_functionsFilterProxyModel->sourceModel());
// // 
// //     if (!functionModel) return;
// // 
// // 
// //     if ((m_functionsFilterProxyModel->rowCount() > 0))
// //     {
// // 
// // //TODO
// // //         functionModel->setSelected(functionModel->editFunction(m_functionsFilterProxyModel->mapToSource(m_functionsFilterProxyModel->index(0,0)).row())->id());
// //     }
// 
// }
// 
// void SpaceViewer::setFunctionsModel(FunctionsFilterProxyModel *functionsFilterProxyModel)
// {
//     m_functionsFilterProxyModel = functionsFilterProxyModel;
//     m_functionEditor->setFunctionsModel(m_functionsFilterProxyModel);
//     //TODO
// //     m_viewer2DWidget->view->setFunctionsModel(m_functionsFilterProxyModel);
// 
// 
// 
// 
// }
// 
// void SpaceViewer::toggleShownFunctionEditor()
// {
//     m_viewer2DWidget->functionEditorDock->setVisible(!m_viewer2DWidget->functionEditorDock->isVisible());
// }
// 
// void SpaceViewer::toggleShownCoordSysSettings()
// {
//     m_viewer2DWidget->coordSysSettingsDock->setVisible(!m_viewer2DWidget->coordSysSettingsDock->isVisible());
// }
// 
// void SpaceViewer::toggleShownSpaceInfo()
// {
//     m_viewer2DWidget->spaceInfoDock->setVisible(!m_viewer2DWidget->spaceInfoDock->isVisible());
// }
// 
// void SpaceViewer::toggleShownAnalysisTools()
// {
// 
// }
// 
// void SpaceViewer::setSpacesModel(SpacesModel *spacesModel)
// {
//     m_spacesModel = spacesModel;
// }
// 
// void SpaceViewer::toImage(const QString laurl)
// {
//     //TODO
//     //m_viewer2DWidget->view->toImage(laurl);
// }
// void SpaceViewer::copyImageToClipboard()
// {
// 
// 
// //TODO
//     //m_viewer2DWidget->view->snapshotToClipboard();
// 
// }
// 
// 
// 
// void SpaceViewer::setVariables(Analitza::Variables* v)
// {
//     m_functionEditor->setVariables(v);
// }
// 
// 
// Analitza::Variables* SpaceViewer::variables() const
// {
//     return m_functionEditor->variables();
// }
// 
// 
// PlotsView2D * SpaceViewer::view() const
// {
//     return m_viewer2DWidget->view;
// }
// 
// 
// void SpaceViewer::saveSpace()
// {
// //     QPixmap thumbnail = QPixmap::grabWidget(m_viewer2DWidget->view);
// //     thumbnail = thumbnail.scaled(QSize(240, 240), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
// // 
// //     m_space.setName(m_viewer2DWidget->spaceTitle->text());
// //     m_space.setDescription(m_viewer2DWidget->spaceDescription->toPlainText());
// //     m_space.setThumbnail(thumbnail);
// 
// // // // // // //     m_spacesModel->editSpace(m_space.id(), m_space);
// 
// 
// 
// }
// 
// 
// 
// 
// void SpaceViewer::updateTick(int i)
// {
//     QVariantMap vmap = m_viewer2DWidget->tickScale->itemData(i, Qt::UserRole).toMap();
// 
// 
//     //TODO
// //     m_viewer2DWidget->view->updateTickScale(vmap["symbol"].toString(),
// //                                             vmap["symbolValue"].toReal(),
// // 
// //                                             vmap["numerator"].toInt(),
// //                                             vmap["denominator"].toInt());
// }
// 
// void SpaceViewer::addTickEntry(QString tick, QString tickScaleSymbol, qreal tickScaleSymbolValue,
//                             int tickScaleNumerator,
//                             int tickScaleDenominator)
// {
//     QVariantMap vmap;
// 
//     vmap.insert("tick", tick);
//     vmap.insert("symbol", tickScaleSymbol);
//     vmap.insert("symbolValue", tickScaleSymbolValue);
// 
//     vmap.insert("numerator", tickScaleNumerator);
//     vmap.insert("denominator", tickScaleDenominator);
// 
//     m_viewer2DWidget->tickScale->addItem(tick, vmap);
// }
// 
// void SpaceViewer::updateGridColor(int i)
// {
//     
// //TODO
// //     QColor col = KColorUtils::lighten(m_viewer2DWidget->gridColor->color(), .35);
// //     m_viewer2DWidget->view->setGridColor(col);
// }
// 
