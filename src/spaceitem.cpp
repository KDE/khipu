/*************************************************************************************
 *  Copyright (C) 2010-2012 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com> *
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

#include "spaceitem.h"

//local includes
#include "spacesmodel.h"

SpaceItem::SpaceItem(Analitza::Dimension dimension)
    : m_dimension(dimension)
    , m_model(0)
{
    m_dateTime = KDateTime::currentLocalDateTime(); 
    
    m_id = QUuid::createUuid();
}

SpaceItem::~SpaceItem()
{
}

void SpaceItem::setName(const QString& name)
{
    m_name = name;
    
    emitDataChanged();
}

void SpaceItem::setDescription(const QString& description)
{
    m_description = description; 
    
    emitDataChanged();
}

void SpaceItem::setThumbnail(const QPixmap& thumbnail)
{
    m_thumbnail = thumbnail;
    
    emitDataChanged();
}

void SpaceItem::stamp()
{
    m_dateTime = KDateTime::currentLocalDateTime(); 
    
    emitDataChanged(); // actualimos las vistas itemviews
}


void SpaceItem::setModel(SpacesModel * m)
{
    Q_ASSERT(m);
    Q_ASSERT(m != m_model);
    
    m_model = m;
}

void SpaceItem::emitDataChanged()
{
    if (m_model) {
        m_model->emitChanged(this);
    }
}
