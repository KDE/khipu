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


#include "gplacswidget.h"

#include <QtGui/QLayout>

#include <KLocalizedString>
#include <KDE/KLocale>
#include <KDE/KPushButton>

#include "widgets/dashboard.h"

namespace GPLACS
{

MainWidget::MainWidget(Keomath::FunctionsModel *functionsModel, Keomath::SpacesModel *spacesModel, QWidget * parent)
    : QWidget(parent)
    , m_functionsModel(functionsModel)
    , m_spacesModel(spacesModel)
{
    m_dashboard = new Dashboard(m_functionsModel, spacesModel, this);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_dashboard);

    connect(m_dashboard, SIGNAL(dashemitShowAppInfo()), SIGNAL(emitShowAppInfo()));


}

MainWidget::~MainWidget()
{
}

void MainWidget::updateSettings(QString page)
{
    Q_UNUSED(page);


}

} 
