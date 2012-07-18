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






class KUrl;
class KToggleAction;

namespace Keomath {
class FunctionsModel;
class SpacesModel;
}

namespace GPLACS {

class Dashboard;


class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

    public:
        
        MainWindow(QWidget *parent = 0);

        
        virtual ~MainWindow();

    public slots:
        void openFile();
        void showAboutAppDialog();
        
        void hide3dtb();

    private slots:
        void newFile();
        void saveFile();
        void saveFileAs(const QString &filename);

    void optionsPreferences();
    void updateInputTypePreviewImage(bool text_input);




        void updateTittleWhenChangeDocState();
        void updateTittleWhenOpenSaveDoc();



private:
    void setupActions();
        bool queryClose();

private:
    Keomath::FunctionsModel *m_functionsModel;
    Keomath::SpacesModel *m_spacesModel;

    Dashboard *m_gplacsWidget;

    KToggleAction *m_toolbarAction;
    KToggleAction *m_statusbarAction;


};

} 

#endif 
