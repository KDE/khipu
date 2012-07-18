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


#ifndef KEOMATH_SPACE_H
#define KEOMATH_SPACE_H

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtGui/QPixmap>
#include <KDE/KDateTime>

class SpaceItem
{
public:
    typedef QList<SpaceItem> List;

    SpaceItem(); 
    SpaceItem(int dimension);
    SpaceItem(const SpaceItem &space);
    virtual ~SpaceItem();

    
    QString id() const
    {
        return m_id;
    }

    void setId(const QString &newid) 
    {
        m_id = newid; 
    }

    QString name() const
    {
        return m_name;
    }

    void setName(const QString &name)
    {
        m_name = name;
    }

    QString description() const
    {
        return m_description;
    }

    void setDescription(const QString &description)
    {
        m_description = description;
    }

    QPixmap thumbnail() const;
    void setThumbnail(const QPixmap thumbnail);

    int dimension() const;

    void setDate(const QString date)
    {
        m_dateTime = KDateTime::fromString(date);
    }

    
    KDateTime  dateTime() const
    {
        return m_dateTime;
    }

protected:
    QString m_id;
    int m_dimension;
    QPixmap m_thumbnail;
    KDateTime  m_dateTime;

    QString m_name;
    QString m_description;
};

#endif


