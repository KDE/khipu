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

//Analitza includes
#include <analitzaplot/plotitem.h>

//Qt includes
#include <QAction>
#include <QModelIndex>

//KDE includes
#include <KXmlGuiWindow>
#include <KRecentFilesAction>

class DictionaryCollection;
class Filter;
class SpaceOptions;
class SpaceInformation;
class PlotsBuilder;
class DataStore;
class SpacePlotsFilterProxyModel;
class PlotsEditor;
class KToggleAction;

class FunctionsModel;
class SpacesModel;

class Dashboard;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    DataStore* getDocument() { return m_document; }

    //used for checking the default autosave file (.khipu.autosave)
    void checkforAutoSavedFile();

signals:
    //used to know the type of the dictionary to be shown in the dictionarydock
    void Spaceinserted(Analitza::Dimension dim);

public slots:

    //New file slot
    void newFile();

    //open file slot
    bool openFileClicked();

    //opening a file spcified by the url
    bool openFile(const QUrl &url);

    //recent file list
    void openRecentClicked(const QUrl&  name);
    void saveClicked();
    bool saveAsClicked();
    bool saveFile(const QUrl &url);
    void activateSpace(int);

    //create the plots for the appropriate space which are clicked by the user
    void createPlot(const QModelIndex &ind);

    //saves the plot
    void savePlot();

    //set the current space's title and description
    void setCurrentSpaceTitle(const QString& str);
    void setCurrentSpaceDesc(const QString& desc);

private slots:
    //helpers - space
    void activateDashboardUi();
    void activateSpaceUi();
    void copySnapshot();
    bool closeClicked();
    void autoSaveFile();

    //used to set visible/invisible dictionary-view
    void setVisibleDictionary();

    //slots for addSpace2D/3D buttons
    void addSpace2D();
    void addSpace3D();

    //removes the current space
    void removeCurrentSpace();

    void goHome();
    void fullScreenView (bool isFull);
    void setMenuBarVisibility (bool isShow);
    void clearRecentFileList();

    //Actions related to space's navigation
    void firstPageActClicked();
    void priorActClicked();
    void nextActClicked();
    void lastPageActClicked();

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

private:
    QAction *createAction(const char *name, const QString &text, const QString &iconName, const QKeySequence & shortcut, const QObject *recvr, const char *slot, bool isCheckable = false, bool checked = true);

    //setting up the dock-widgets
    void setupDocks();
    void setupActions();

    //changes the title bar when file-name is available
    void changeTitleBar(const QString& path);

    //current opened file's name (i.e. path)
    void setCurrentFile(const QString &fileName);
    void updateRecentFileList();

    // used for parsing file path from the url provided by openrecent
    QString pathFromUrl(const QUrl &url);

    //converts a thubnail from bytearray to pixmap and vice-versa
    QPixmap toPixmap(const QByteArray &bytearray);
    QByteArray thumbnailtoByteArray(const QPixmap &thumbnail);

    void closeEvent(QCloseEvent * event);
    void updateThumbnail();

    //returns the current autosave file's path (i.e.currentdirecotrypath/.filename.khipu.autosave)
    QString getCurrentAutoSavepath(const QString& path);

    //returns the default autosave file's path (i.e.homedirectotypath/.khipu.autosave)
    QString getDefaultAutoSavepath();

private:
    DataStore *m_document;
    Dashboard *m_dashboard;
    Filter *m_filter;

    QDockWidget *m_plotsBuilderDock;
    PlotsEditor *m_spacePlotsDock;
    SpaceInformation *m_spaceInfoDock;
    SpaceOptions *m_spaceOptionsDock;
    DictionaryCollection *m_dictionaryDock;
    QVariantList m_parsedSpaceDetails;
    QString m_fileLocation; // location of the current running file which is saved by the user.
    KRecentFilesAction * m_openrecent;
    QAction *m_priorAct;
    QAction *m_nextAct;
    QAction *m_importdictionaryAct;
    QAction *m_getdictionaryAct;
    QUrl m_currentFileUrl;
    
private: // consts
    static const unsigned short MaxRecentFiles = 7;
    
    // time interval to show the message on statusbar in milliseconds
    static const unsigned short MessageDuration = 3000;
};

#endif
