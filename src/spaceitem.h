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

#ifndef KEOMATH_SPACE_H
#define KEOMATH_SPACE_H

#include <QtGui/QPixmap>
#include <KDE/KDateTime>

class SpacesModel;

class SpaceItem
{
    
friend class SpacesModel;    
    
public:
    SpaceItem(int dimension);
    virtual ~SpaceItem();

    int dimension() const { return m_dimension; }
    
    QString title() const { return m_name; }
    void setTitle(const QString &name) { m_name = name; }

    QString description() const { return m_description; }
    void setDescription(const QString &description) { m_description = description; }

    QPixmap thumbnail() const { return m_thumbnail; }
    void setThumbnail(const QPixmap thumbnail) { m_thumbnail = thumbnail; }

    KDateTime  timestamp() const { return m_dateTime; }

    void setTimestamp(const KDateTime &date) { m_dateTime = date; }

private:
    SpaceItem() {}
    SpaceItem(const SpaceItem &other) {}

    void setModel(SpacesModel *m);
    
    void emitDataChanged();

    int m_dimension;
    QPixmap m_thumbnail;
    KDateTime  m_dateTime;

    QString m_name;
    QString m_description;
    
    SpacesModel *m_model;
    bool m_inDestructorSoDontDeleteMe;
};

typedef QList<SpaceItem *> SpaceItemList;

#endif
