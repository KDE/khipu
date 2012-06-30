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


#include "space.h"
#include <quuid.h>

namespace Keomath
{

Space::Space()
{

}

Space::Space(int dimension)
    : m_dimension(dimension)
{
    m_id = QUuid::createUuid().toString();
    m_dateTime = KDateTime::currentLocalDateTime(); 
}

Space::Space(const Space &space)
    : m_dimension(space.dimension())
    , m_name(space.m_name)
    , m_description(space.m_description)
    , m_id(space.id())
    , m_thumbnail(space.thumbnail())
    , m_dateTime(space.dateTime())
{
}

Space::~Space()
{

}

QPixmap Space::thumbnail() const
{
    return m_thumbnail;
}

void Space::setThumbnail(const QPixmap thumbnail)
{
    m_thumbnail = thumbnail;
}

int Space::dimension() const
{
    return m_dimension;
}

} 




