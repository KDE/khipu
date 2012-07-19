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

#include "spacesmodel.h"

SpacesModel::SpacesModel(QObject *parent)
    : QAbstractListModel(parent),  m_itemCanCallModelRemoveItem(true)
{}


Qt::ItemFlags SpacesModel::flags(const QModelIndex &idx) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant SpacesModel::data( const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
        case Qt::DecorationRole: return m_items.at(index.row())->thumbnail();
        case Qt::ToolTipRole:  return m_items.at(index.row())->timestamp().toString("%A %l:%M %p %B %Y");
        case Qt::DisplayRole: return m_items.at(index.row())->title();
        case Qt::StatusTipRole: return m_items.at(index.row())->description(); //TODO GSOC agregar un prefix algo como space descrp: txttt
    }

    return QVariant();
}

int SpacesModel::rowCount(const QModelIndex &idx) const
{
    return m_items.count();
}

SpaceItem* SpacesModel::addSpace(int dim)
{
    Q_ASSERT(dim == 2 || dim == 3);
    
    SpaceItem * ret = 0;

    {
        beginInsertRows (QModelIndex(), m_items.count(), m_items.count());

        ret = new SpaceItem(dim);
        ret->setModel(this);
        m_items.append(ret);

        endInsertRows();

        return ret;
    }

    return ret;
}

SpaceItem* SpacesModel::item(int row) const
{
    Q_ASSERT(row<m_items.count());

    return m_items[row];
}

void SpacesModel::removeItem(int row)
{
    Q_ASSERT(row<m_items.size());

    beginRemoveRows(QModelIndex(), row, row);

    SpaceItem *tmpcurve = m_items[row];

    m_itemCanCallModelRemoveItem = false;

    if (!tmpcurve->m_inDestructorSoDontDeleteMe)
    {
        delete tmpcurve;
        tmpcurve = 0;
    }

    m_itemCanCallModelRemoveItem = true;

    m_items.removeAt(row);

    endRemoveRows();
}
/*
SpacesFilterProxyModel::SpacesFilterProxyModel(QObject *parent)
    : KCategorizedSortFilterProxyModel(parent)
    , m_dimension(-1)
{
    setDynamicSortFilter(true);

}

void SpacesFilterProxyModel::setFilterDimension(int dimension)
{
    m_dimension = dimension;
    invalidateFilter();
}

bool SpacesFilterProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    QModelIndex indexName = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex indexDescription = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex indexDimension = sourceModel()->index(sourceRow, 2, sourceParent);

    QString dim;

    if (m_dimension == 2)
        dim = QString("2D");
    else if (m_dimension == 3)
        dim = QString("3D");

    if (m_dimension != -1) 
        if (sourceModel()->data(indexDimension, Qt::DisplayRole).toString() != dim)
            return false;

    return (sourceModel()->data(indexName).toString().contains(filterRegExp()) ||
            sourceModel()->data(indexDescription).toString().contains(filterRegExp()));
}*/
