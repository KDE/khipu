/*************************************************************************************
 *  Copyright (C) 2012 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>      *
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

#ifndef KEOMATH_VIEWER2D_H
#define KEOMATH_VIEWER2D_H

#include <QtGui/QWidget>
#include <QDockWidget>
#include <QModelIndex>
#include "dashboard.h"

namespace Ui
{
    class SpaceInformationWidget;
}

class SpaceInformation : public QDockWidget
{
    Q_OBJECT
public:
    SpaceInformation(QWidget *parent);
    ~ SpaceInformation();
    
    QString title() const;
    QString description() const;
    
    void setInformation(const QString &title, const QString &description);

public slots:
    void clear();
    
private:
    Ui::SpaceInformationWidget *m_widget;
};

#endif


