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
    : m_dimension(dimension), m_model(0), m_inDestructorSoDontDeleteMe(false), m_callingCtrFromMode(true)
{
    m_dateTime = KDateTime::currentLocalDateTime(); 
    
    m_id = QUuid::createUuid();
}

void SpaceItem::setTitle(const QString& name)
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

SpaceItem::~SpaceItem()
{
    if (m_model && m_model->m_itemCanCallModelRemoveItem)
    {
        m_inDestructorSoDontDeleteMe = true;
        m_model->removeRow(m_model->m_items.indexOf(this));
        m_inDestructorSoDontDeleteMe = false;
    }
}

void SpaceItem::setModel(SpacesModel * m)
{
    Q_ASSERT(m);
    Q_ASSERT(m != m_model);
    
    m_model = m;
    
    m_callingCtrFromMode = false;
}

int SpaceItem::row()
{
    return m_model ? -1 : m_model->spaceIndex(this).row();
}

void SpaceItem::emitDataChanged()
{
    if (m_callingCtrFromMode)
        return; // no emitir la signal datachange cuando se esta agregando un item desde el model

    int itemRow = row();
    if (itemRow>=0)
    {
        QModelIndex idx = m_model->index(itemRow);
        m_model->dataChanged(idx, idx);
    }
}
