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


#ifndef GPLACS_MAINWIDGET_H
#define GPLACS_MAINWIDGET_H

#include <QtGui/QWidget>

namespace Keomath {
class FunctionsModel;
class SpacesModel;
}

namespace GPLACS {

class Dashboard;

class MainWidget : public QWidget
{
    Q_OBJECT

    public:
        MainWidget(Keomath::FunctionsModel *functionsModel, Keomath::SpacesModel *spacesModel, QWidget * parent = 0);
        virtual ~MainWidget();

    public slots:
        void updateSettings(QString);

    signals:
        void emitShowAppInfo();

    private slots:

    private:
        Keomath::FunctionsModel *m_functionsModel;
        Keomath::SpacesModel *m_spacesModel;

        Dashboard *m_dashboard;
};

} 

#endif 
