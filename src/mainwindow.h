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

private slots:
    //helpers
    void activateDashboardUi();
    void activateSpaceUi();
    
    //app
    void addSpace2D();
    void addSpace3D();
    void goHome();
    
    
    

    
    
    void updateTittleWhenChangeDocState();
    void updateTittleWhenOpenSaveDoc();

private:
    KAction *createAction(const char *name, const QString &text, const QString &iconName, const QKeySequence & shortcut,const char *slot, bool isCheckable = false);
    void setupActions();
    void setupToolBars();
    void setupDocks();
    bool queryClose();

private:
    Document *m_document;
    Dashboard *m_dashboard;
    
    PlotsEditor *m_spacePlotsDock;
    QDockWidget *m_spaceInfoDock;
    QDockWidget *m_spaceOptionsDock;
    
};

#endif
