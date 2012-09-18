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

#ifndef SPACEOPTIONS_H
#define SPACEOPTIONS_H

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
    void setGridStyleIndex(int i);
    void setDimension(int d); // d muestra opciones para el view2d 3 para el 3d
    void updateTicks();
    void updateAxes();

signals: //TODO better names for signals
    void updateGridStyle(int i);
    void updateGridColor(const QColor &col);
    void setXAxisLabel(const QString &label);
    void setYAxisLabel(const QString &label);
    void updateTickScale(QString m_tickScaleSymbol, qreal m_tickScaleSymbolValue, int m_tickScaleNumerator, int m_tickScaleDenominator);
    void ticksShown(QFlags<Qt::Orientation> o);
    void axesShown(QFlags<Qt::Orientation> o);
    
    //3d
    void gridIsDrawn(bool);
    void axisIsDrawn(bool);
    void sceneResized(int);

private slots:
    void updateScale();
    void setGridStyle(int i);
    
private:
    void addTickEntry(QString tick, QString tickScaleSymbol, qreal tickScaleSymbolValue, int tickScaleNumerator, int tickScaleDenominator);
    
    Ui::SpaceOptionsWidget *m_widget;
};

#endif


