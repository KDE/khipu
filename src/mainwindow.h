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

#ifndef GPLACS_MAINWINDOW_H
#define GPLACS_MAINWINDOW_H

#include <KDE/KXmlGuiWindow>
#include <QModelIndex>
#include <analitzaplot/plotitem.h>

class Filter;
class SpaceOptions;
class SpaceInformation;
class PlotsBuilder;
class DataStore;
class SpacePlotsFilterProxyModel;
class PlotsEditor;
class KIcon;
class KAction;
class Document;
class KUrl;
class KToggleAction;

class FunctionsModel;
class SpacesModel;

class Dashboard;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

public slots:
    void newFile();
    void openFile();
    void saveClicked();
    void saveAsClicked();
    void activateSpace(int);
    void createPlot(const QModelIndex &ind);
    void savePlot();

private slots:
    //helpers - space
    void activateDashboardUi();
    void activateSpaceUi();
    void copySnapshot();
    void exportSnapShot();
    
    //app
    void setVisibleDictionary(); // este llama a dashboard void setVisibleDictionary(bool t);
    void addSpace2D();
    void addSpace3D();
    void removeCurrentSpace();
    void goHome();

    //builder dock actions: shortcuts ... first add the space then show the editor
    void buildCartesianGraphCurve();
    void buildCartesianImplicitCurve();
    void buildCartesianParametricCurve2D();
    void buildPolarGraphCurve();
    //3D
    void buildCartesianParametricCurve3D();
    void buildCartesianGraphSurface();
    void buildCartesianImplicitSurface();
    void buildCartesianParametricSurface();
    void buildCylindricalGraphSurface();
    void buildSphericalGraphSurface();
    
    void updateTittleWhenChangeDocState();
    void updateTittleWhenOpenSaveDoc();

signals:
    void plotMapped(Analitza::PlotItem* item); //used to map the plot when user opens a file from the disk.

private:
    KAction *createAction(const char *name, const QString &text, const QString &iconName, const QKeySequence & shortcut, const QObject *recvr, const char *slot, bool isCheckable = false, bool checked = true);
    void setupDocks();
    void setupActions();
    void setupToolBars();
    bool queryClose();
    void saveFile(const QString& path);
    QByteArray thumbnailtoByteArray(const QPixmap &thumbnail);
    QPixmap toPixmap(const QByteArray &bytearray);

private:
    DataStore *m_document;
    Dashboard *m_dashboard;
    Filter *m_filter;
    int m_currentSpaceDim;
    int m_totalSpaces; // total hits to addspace2d + addspace3D
    int m_savedSpaces; // total hits to home

    QDockWidget *m_plotsBuilderDock;
    PlotsEditor *m_spacePlotsDock;
    SpaceInformation *m_spaceInfoDock;
    SpaceOptions *m_spaceOptionsDock;
    QList<QByteArray> imageList;
    QVariantList m_parsedSpaceDetails;
    QString m_fileLocation; // location of the current running file which is saved by the user.
    //remove this
    
private slots:
    void fooSlot(bool t = false);
};

#endif
