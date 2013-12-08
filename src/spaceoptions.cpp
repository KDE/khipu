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

#include "spaceoptions.h"
#include "ui_spaceoptions.h"

//KDE includes
#include <kcolorutils.h>
#include <KLocalizedString>

//C++ includes
#include <cmath>

SpaceOptions::SpaceOptions(QWidget* parent): QDockWidget(parent)
{
    m_widget = new Ui::SpaceOptionsWidget;
    m_widget->setupUi(this);
    m_widget->moreOptions->setChecked(false);
    setObjectName("khipu_diagram_spaceoptions_dock");
    
    //BEGIN default grid style: NONE (see too Dashboard::initializePlotsViews)
    m_widget->gridStyle->setCurrentIndex(0);
    m_widget->backgroundColor->hide();
    //END

    //BEGIN setup tick marks
    //NOTE always from lower to greater : from 1 to pi
    
    QString sqrtsym = QString((QChar(0x221A)));
    
    m_widget->scale->addItem("1");
    m_widget->scale->addItem(sqrtsym+'2');
    m_widget->scale->addItem(sqrtsym+'3');
    m_widget->scale->addItem("e");
    m_widget->scale->addItem(QString((QChar(0x03C0))));
    
    m_widget->scale->setCurrentIndex(0);
    //END

    //signals
    connect(m_widget->gridStyle, SIGNAL(currentIndexChanged(int)), SLOT(setGridStyle(int)));
    connect(m_widget->backgroundColor, SIGNAL(activated(QColor)), SIGNAL(updateGridColor(QColor)));
    connect(m_widget->horizontalLabel, SIGNAL(textChanged(QString)), SIGNAL(setXAxisLabel(QString)));
    connect(m_widget->verticalLabel, SIGNAL(textChanged(QString)), SIGNAL(setYAxisLabel(QString)));
    connect(m_widget->scale, SIGNAL(currentIndexChanged(int)), SLOT(updateScale()));

    connect(m_widget->marksVisible, SIGNAL(toggled(bool)), SLOT(updateTicks()));
    connect(m_widget->showXAxis, SIGNAL(toggled(bool)), SLOT(updateAxes()));
    connect(m_widget->showYAxis, SIGNAL(toggled(bool)), SLOT(updateAxes()));
    
    //3d
    connect(m_widget->showAxis, SIGNAL(toggled(bool)), SIGNAL(axisIsDrawn(bool)));
    connect(m_widget->showGrid, SIGNAL(toggled(bool)), SIGNAL(gridIsDrawn(bool)));
    connect(m_widget->sceneSize, SIGNAL(valueChanged(int)), SIGNAL(sceneResized(int)));
    
    //TODO too much code : set default grid color (same of plotsview2d)
    m_widget->backgroundColor->setColor(palette().color(QPalette::Active, QPalette::Base));
}

SpaceOptions::~SpaceOptions()
{
    delete m_widget;
}

void SpaceOptions::reset()
{}

void SpaceOptions::setGridStyleIndex(int i)
{
    m_widget->gridStyle->setCurrentIndex(i);
}

void SpaceOptions::setDimension(int d)
{
    m_widget->options->setCurrentIndex(d-2); //-2 porque si dim es 2 se muestra las optiones cero (para el view2d)
}

void SpaceOptions::updateScale()
{
    int i = m_widget->scale->currentIndex();
    
    Analitza::TicksFormat ticksfmt = Analitza::Number;
    
    switch (i)
    {
        case 2: ticksfmt = Analitza::SymbolSqrt2; break;
        case 3: ticksfmt = Analitza::SymbolSqrt3; break;
        case 4: ticksfmt = Analitza::SymbolE; break;
        case 5: ticksfmt = Analitza::SymbolPi; break;
    }

    emit ticksFormatChanged(ticksfmt);
}

void SpaceOptions::setGridStyle(int i)
{
    emit updateGridStyle(i);
    
    m_widget->backgroundColor->setVisible(i!=0);
}

void SpaceOptions::updateAxes()
{
    QFlags<Qt::Orientation> o;
    if(m_widget->showXAxis->checkState()==Qt::Checked)
        o|=Qt::Horizontal;
    if(m_widget->showYAxis->checkState()==Qt::Checked)
        o|=Qt::Vertical;
    emit axesShown(o);
}

void SpaceOptions::updateTicks()
{
    QFlags<Qt::Orientation> o;
    
    if (m_widget->marksVisible->checkState()==Qt::Checked){
        o|=Qt::Horizontal;
        o|=Qt::Vertical;
    }
    
    emit ticksShown(o);
}
