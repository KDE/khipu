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

#ifndef KEOMATH_VIEWER2D_H_OPTS_filter
#define KEOMATH_VIEWER2D_H_OPTS_filter

#include <QtGui/QWidget>
#include <QDockWidget>
#include <QModelIndex>
#include "dashboard.h"
#include <analitzaplot/plottingenums.h>

namespace Ui
{
    class FilterWidget;
}
//NOTE TODO CADA WIDGET DE KHIPU QUE BUSCARA/FILTRARA COSAS DEBE IMPLEMENTAR UNA IFACE: editor(funclist), dashboard, dicsview, etc
class Filter : public QWidget
{
    Q_OBJECT
public:
    Filter(QWidget *parent);
    ~ Filter();

// public slots:

signals: //TODO better names for signals adn slots
    void filterByText(const QString &text); // any text on the title or description
    void filterByDimension(Analitza::Dimensions radioButtonid);

private slots:
    void getDimIndex(int index);
    
    
private:
    
    Ui::FilterWidget *m_widget;
};

#endif


