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

#include "spaceinformation.h"

#include "ui_spaceinformation.h"

SpaceInformation::SpaceInformation(QWidget* parent): QDockWidget(parent)
{
    m_widget = new Ui::SpaceInformationWidget;
    m_widget->setupUi(this);
    
    setObjectName("asdasdds222");
}

SpaceInformation::~SpaceInformation()
{
    delete m_widget;
}

QString SpaceInformation::title() const
{
    return m_widget->spaceTitle->text();
}

QString SpaceInformation::description() const
{
    return m_widget->spaceDescription->toPlainText();
}

void SpaceInformation::setInformation(const QString& title, const QString& description)
{
    m_widget->spaceTitle->setText(title);
    m_widget->spaceDescription->setPlainText(description);
}

void SpaceInformation::clear()
{
    m_widget->spaceTitle->clear();
    m_widget->spaceDescription->clear();
}
