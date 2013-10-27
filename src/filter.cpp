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
#include "ui_filter.h"

//KDE includes
#include <KIcon>

Filter::Filter(QWidget* parent)
    : QWidget(parent)
    , m_ui(new Ui::FilterWidget)
    , m_dashboard(0)
{
    m_ui->setupUi(this);

    m_ui->filterOptions->addItem(KIcon("all-space-filter"), i18n("Dimension All"));
    m_ui->filterOptions->addItem(KIcon("2d-space-filter"), i18n("Dimension 2D"));
    m_ui->filterOptions->addItem(KIcon("3d-space-filter"), i18n("Dimension 3D"));

    connect(m_ui->filterText, SIGNAL(textChanged(QString)), SIGNAL(filterByText(QString)));
    connect(m_ui->filterOptions, SIGNAL(currentIndexChanged(int)), SLOT(getDimIndex(int)));
}

Filter::~Filter()
{
}

void Filter::getDimIndex(int index)
{
    switch (index)
    {
        case 0: m_dashboard->filterByDimension(Analitza::DimAll); break;
        case 1: m_dashboard->filterByDimension(Analitza::Dim2D); break;
        case 2: m_dashboard->filterByDimension(Analitza::Dim3D); break;
    }
}

void Filter::setFilterDashboard(Dashboard *sourceDashboard)
{
    m_dashboard = sourceDashboard;
}

void Filter::setFilterVisible(bool b)
{
    m_ui->filterText->setVisible(b);
    m_ui->filterOptions->setVisible(b);
}
