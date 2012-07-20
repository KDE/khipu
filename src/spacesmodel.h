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


#ifndef KEOMATH_SPACESMODEL_H
#define KEOMATH_SPACESMODEL_H

#include <QtCore/QAbstractListModel>

#include "spaceitem.h"

namespace Analitza
{
class Expression;
}

class SpacesView;

class SpacesModel : public QAbstractListModel 
{

friend class SpaceItem;   
Q_OBJECT

public:
    enum SpacesModelRoles { Selection=Qt::UserRole+1 };
    
    SpacesModel(QObject *parent=0);

    Qt::ItemFlags flags ( const QModelIndex & index ) const;

    QVariant data( const QModelIndex &index, int role=Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent=QModelIndex()) const;

    SpaceItem * addSpace(int dim, const QString & title = QString(), const QString &description = QString(), 
                         const QPixmap &thumbnail=QPixmap());

    SpaceItem * item(int row) const;
    void removeItem(int row);
    
private:
    SpaceItemList m_items;
    bool m_itemCanCallModelRemoveItem; // just a lock para evitar que el item llame recursivamente a removeItem
};

/*
class SpacesFilterProxyModel : public KCategorizedSortFilterProxyModel
{
    Q_OBJECT

public:
    SpacesFilterProxyModel(QObject *parent = 0);

    int filterDimension() const
    {
        return m_dimension;
    }
    void setFilterDimension(int dimension);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;


private:
    int m_dimension; 
};*/

#endif 
