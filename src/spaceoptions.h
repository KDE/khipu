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

#ifndef KEOMATH_VIEWER2D_H_OPTS
#define KEOMATH_VIEWER2D_H_OPTS

#include <QtGui/QWidget>
#include <QDockWidget>
#include <QModelIndex>
#include "dashboard.h"

namespace Ui
{
    class SpaceOptionsWidget;
}

class SpaceOptions : public QDockWidget
{
    Q_OBJECT
public:
    SpaceOptions(QWidget *parent);
    ~ SpaceOptions();

public slots:
    void reset();
    
private:
    void addTickEntry(QString tick, QString tickScaleSymbol, qreal tickScaleSymbolValue,
        /*bool tickScaleUseSymbols, */int tickScaleNumerator,
        int tickScaleDenominator);
    
    Ui::SpaceOptionsWidget *m_widget;
};

#endif


