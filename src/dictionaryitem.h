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

#ifndef DICTIONARYITEM_H
#define DICTIONARYITEM_H

//Analitza includes
#include <analitzaplot/plottingenums.h>

//Qt includes
#include <QtGui/QPixmap>
#include <QUuid>

//KDE includes
#include <KDE/KDateTime>

static const int PreviewWidth = 200;
static const int PreviewHeight = 200;

class DictionariesModel;

class DictionaryItem
{
    
friend class DictionariesModel;    
    
public:
    explicit DictionaryItem(Analitza::Dimension dimension);
    virtual ~DictionaryItem();

    QUuid id() const { return m_id; }

    //space's dimension
    Analitza::Dimension dimension() const { return m_dimension; }
    
    //space's title
    QString title() const { return m_name; }
    void setTitle(const QString &name);
    
    //space's description
    QString description() const { return m_description; }
    void setDescription(const QString &description);

    //space's thumbnail
    QPixmap thumbnail() const { return m_thumbnail; }
    void setThumbnail(const QPixmap &thumbnail);

    KDateTime timestamp() const { return m_dateTime; }
    ///marks current time stamp
    void stamp();

private:
    DictionaryItem() {}
    DictionaryItem(const DictionaryItem &other);

    void setModel(DictionariesModel *m);
    
    void emitDataChanged();

    Analitza::Dimension m_dimension;
    QPixmap m_thumbnail;
    KDateTime  m_dateTime;

    QString m_name;
    QString m_description;
    
    DictionariesModel *m_model;
    bool m_inDestructorSoDontDeleteMe;
    bool m_callingCtrFromMode; // true en ctor y false en setmodel
    
    //persistence
    QUuid m_id;
};

#endif
