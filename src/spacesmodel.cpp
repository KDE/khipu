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

//KDE includes
#include <KLocalizedString>

//local includes
#include "spaceitem.h"

SpacesModel::SpacesModel(QObject *parent)
    : QAbstractListModel(parent),  m_itemCanCallModelRemoveItem(true)
{}

Qt::ItemFlags SpacesModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant SpacesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal) {
        switch(section) 
        {
            case 0: return i18nc("@title:column", "Name");
            case 1: return i18nc("@title:column", "Description");
        }
    }
    return QVariant();
}

QVariant SpacesModel::data( const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
        case Qt::StatusTipRole:  return m_items.at(index.row())->timestamp().toString("%A %l:%M %p %B %Y");
        case Qt::DecorationRole: if (index.column() == 0)  return m_items.at(index.row())->thumbnail(); break;
        case Qt::EditRole: 
        case Qt::DisplayRole: 
            if (index.column() == 0)
                return m_items.at(index.row())->name();
            else if (index.column() == 1)
                return m_items.at(index.row())->description();
            
//         case Qt::StatusTipRole: return m_items.at(index.row())->description(); //TODO GSOC agregar un prefix algo como space descrp: txttt
    }

    return QVariant();
}

bool SpacesModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    //TODO edit also description (column/section #2)
    if (index.isValid() && role == Qt::EditRole) 
    {
        m_items[index.row()]->setName(value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

int SpacesModel::rowCount(const QModelIndex &idx) const
{
    return idx.isValid() ? 0 : m_items.count();
}

int SpacesModel::columnCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;
    else
        return 2;
}

bool SpacesModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if(parent.isValid())
        return false;

    Q_ASSERT(row+count<=m_items.size());
    beginRemoveRows(QModelIndex(), row, row+count-1);

    for (int i = 0; i < count; ++i) 
    {
        SpaceItem *tmpcurve = m_items[row];

        m_itemCanCallModelRemoveItem = false;

        if (!tmpcurve->m_inDestructorSoDontDeleteMe)
        {
            delete tmpcurve;
            tmpcurve = nullptr;
        }

        m_itemCanCallModelRemoveItem = true;

        m_items.removeAt(row);
    }

    endRemoveRows();
    
    return true;
}

SpaceItem* SpacesModel::addSpace(Analitza::Dimension dim, const QString & title, const QString &description, const QPixmap &thumbnail)
{
    SpaceItem* ret = new SpaceItem(dim);
    ret->setName(title);
    ret->setDescription(description);
    ret->setThumbnail(thumbnail);

    beginInsertRows (QModelIndex(), m_items.count(), m_items.count());
    ret->setModel(this);
    m_items.append(ret);
    endInsertRows();

    return ret;
}

SpaceItem* SpacesModel::space(int row) const
{
    Q_ASSERT(row >=0 && row<rowCount());
    return m_items[row];
}

void SpacesModel::emitChanged(SpaceItem* it)
{
    int row = m_items.indexOf(it);
    QModelIndex idx = index(row);
    emit dataChanged(idx, idx);
}

#include "moc_spacesmodel.cpp"
