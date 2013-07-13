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

#include "filter.h"
#include <QCompleter>
#include "ui_filter.h"
#include <KIcon>

Filter::Filter(QWidget* parent): QWidget(parent)
{
    m_widget = new Ui::FilterWidget;
    m_widget->setupUi(this);

    m_widget->filterOptions->insertItem(0,KIcon("all-space-filter"),"Dimension-All");
    m_widget->filterOptions->insertItem(1,KIcon("2d-space-filter"),"Dimension-2D");
    m_widget->filterOptions->insertItem(2,KIcon("3d-space-filter"),"Dimension-3D");


    setObjectName("a33sdasddsaaa1");

    connect(m_widget->filterText, SIGNAL(textChanged(QString)), SIGNAL(filterByText(QString)));
    connect(m_widget->filterOptions, SIGNAL(currentIndexChanged(int)), SLOT(getDimIndex(int)));
}

Filter::~Filter()
{
    delete m_widget;
}

void Filter::getDimIndex(int index)
{
    switch (index)
    {
    case 0: m_filterDashboard->filterByDimension(Analitza::DimAll); break;
    case 1: m_filterDashboard->filterByDimension(Analitza::Dim2D); break;
    case 2: m_filterDashboard->filterByDimension(Analitza::Dim3D); break;
    }
}

void Filter::setFilterDashboard(Dashboard *sourceDashboard){
    m_filterDashboard= new Dashboard(sourceDashboard);
    m_filterDashboard->setVisible(false);
    m_filterDashboard->setDashboardData(sourceDashboard);
}

void Filter::setFilterVisible(bool b) {
    m_widget->filterText->setVisible(b);
    m_widget->filterOptions->setVisible(b);
}
