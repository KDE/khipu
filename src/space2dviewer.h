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
// #ifndef KEOMATH_VIEWER2D_H
// #define KEOMATH_VIEWER2D_H
// 
// #include <QtGui/QWidget>
// #include <QModelIndex>
// 
// class PlotsModel;
// class SpacesModel;
// 
// class SpaceViewerWidget;
// 
// class SpaceViewer : public QWidget
// {
//     Q_OBJECT
// 
// public:
//     SpaceViewer(QWidget *parent = 0);
// 
//     void setPlotsModel(PlotsModel *pm);
//     void setSpacesModel(SpacesModel *spacesModel);
// 
// public slots:
//     void activateSpace(const QModelIndex & index);
// 
// 
// 
// public:
// 
// 
//     void toImage(const QString laurl);
//     void copyImageToClipboard();
// 
// 
// 
// 
// 
//     
//     void setVariables(Analitza::Variables* v);
// 
//     Analitza::Variables* variables() const;
// 
//     PlotsView2D *view() const;
// 
// public slots:
//     void toggleShownFunctionEditor();
//     void toggleShownCoordSysSettings();
//     void toggleShownSpaceInfo();
//     void toggleShownAnalysisTools();
// 
//     void hideDocks();
//     void saveSpace();
// 
// 
// 
// 
// 
// 
// 
// 
// private slots:
// 
// 
// 
//     void updateTick(int);
//     void updateGridColor(int);
// 
// private:
//     FunctionsFilterProxyModel *m_functionsFilterProxyModel;
//     SpacesModel *m_spacesModel;
//     FunctionEditor *m_functionEditor;
// 
// //     SpaceItem m_space;
//     Viewer2DWidget *m_viewer2DWidget;
// 
//     
// 
// 
// private:
//     void addTickEntry(QString tick, QString tickScaleSymbol, qreal tickScaleSymbolValue,
//                       int tickScaleNumerator,
//                       int tickScaleDenominator);
// 
// };
// 
// #endif
// 
// 
