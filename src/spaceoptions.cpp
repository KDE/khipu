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

//C++ includes
#include <cmath>

SpaceOptions::SpaceOptions(QWidget* parent): QDockWidget(parent)
{
    m_widget = new Ui::SpaceOptionsWidget;
    m_widget->setupUi(this);
    m_widget->moreOptions->setChecked(false);
    setObjectName("a33sdasdds");

    QString sym = QString((QChar(0x03C0)));
    addTickEntry(sym+"/2", sym, M_PI, 1, 2);
    addTickEntry(sym, sym, M_PI, 1, 1);
    sym = QString("e");
    addTickEntry(sym, sym, M_E, 1, 1);
    sym = QString((QChar(0x221A)))+"2";
    qreal symval = sqrt(2);
    addTickEntry(sym, sym, symval, 1, 1);
    sym = QString((QChar(0x221A)))+"3";
    symval = sqrt(3);
    addTickEntry(sym, sym, symval, 1, 1);
    addTickEntry("1", QString(), 1, 1, 1);
    m_widget->scale->setCurrentIndex(m_widget->scale->count()-1);

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
    QVariantMap vmap = m_widget->scale->itemData(i, Qt::UserRole).toMap();

    emit updateTickScale(vmap["symbol"].toString(),
    vmap["symbolValue"].toReal(),
    vmap["numerator"].toInt(),
    vmap["denominator"].toInt());
}

void SpaceOptions::setGridStyle(int i)
{
    emit updateGridStyle(i);
    m_widget->backgroundColor->setVisible(i!=0);
}

void SpaceOptions::addTickEntry(QString tick, QString tickScaleSymbol, qreal tickScaleSymbolValue,
        int tickScaleNumerator,int tickScaleDenominator)
{
    QVariantMap vmap;

    vmap.insert("tick", tick);
    vmap.insert("symbol", tickScaleSymbol);
    vmap.insert("symbolValue", tickScaleSymbolValue);
    vmap.insert("numerator", tickScaleNumerator);
    vmap.insert("denominator", tickScaleDenominator);

    m_widget->scale->addItem(tick, vmap);
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
    if(m_widget->marksVisible->checkState()==Qt::Checked){
        o|=Qt::Horizontal;
        o|=Qt::Vertical;
    }
    emit ticksShown(o);
}

void SpaceOptions::setWidgetsVisible(bool isWidgetVisible)
{
    m_widget->gridstylecontainer->setVisible(isWidgetVisible);
    m_widget->moreOptions->setVisible(isWidgetVisible);
    if(!isWidgetVisible) {
        m_widget->gridStyle->clear();
        m_widget->gridStyle->addItem("Lines");
    } else {
        m_widget->gridStyle->clear();
        m_widget->gridStyle->addItem("None");
        m_widget->gridStyle->addItem("Lines");
        m_widget->gridStyle->addItem("Polar");
    }
    m_widget->backgroundColor->show();
}
