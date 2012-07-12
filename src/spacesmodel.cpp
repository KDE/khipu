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


#include "spacesmodel.h"

#include <KLocale>
#include <KApplication>
#include <KDebug>
#include <QPixmap>
#include <QFont>
#include <cmath>
#include <kcategorizedsortfilterproxymodel.h>
#include "analitza/expression.h"
#include <QModelIndex>
#include <kicon.h>

namespace Keomath
{

SpacesModel::SpacesModel(QObject *parent)
    : QAbstractTableModel(parent)


{}

QVariant SpacesModel::data( const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case KCategorizedSortFilterProxyModel::CategoryDisplayRole:
    {




        return m_spaceList.at(index.row()).dimension();
//         return QString();
//        return index.row()/4;        
    }
    case KCategorizedSortFilterProxyModel::CategorySortRole:
    {

        
        return index.row()/4;
    }
    case Qt::DecorationRole:


        return m_spaceList.at(index.row()).thumbnail();
        break;






    case Qt::ToolTipRole:
        return m_spaceList.at(index.row()).dateTime().toString("%A %l:%M %p %B %Y");

        break;


    case Qt::DisplayRole:





        switch(index.column())
        {
        case 0:
            return m_spaceList.at(index.row()).name();
            break;
        case 1:
            return m_spaceList.at(index.row()).description();
            break;
        case 2:

            if (m_spaceList.at(index.row()).dimension() == 2)
                return i18n("2D");
            else if (m_spaceList.at(index.row()).dimension() == 3)
                return i18n("3D");
            break;
        case 3:
            
            

            
            return m_spaceList.at(index.row()).dateTime().toString( KDateTime::QtTextDate);

            break;

        }




        break;


    }

    return QVariant();


































}

QModelIndex SpacesModel::spaceIndex(const Space& space) const
{
    int row = 0;

    foreach (const Space &s, m_spaceList)
    {
        if (s.id() == space.id())
            return index(row, 0);

        ++row;
    }

    return QModelIndex();
}

const Space & SpacesModel::spaceFromId(const QString &id) const
{
    for (int i = 0; i < m_spaceList.size(); i+=1)
    {
        if (m_spaceList.at(i).id() == id)
            return m_spaceList.at(i);
    }

    return Space();
}

const Space & SpacesModel::spaceFromIndex(int index) const
{
    
    return m_spaceList.at(index);
}



















int SpacesModel::rowCount(const QModelIndex &idx) const
{




    return m_spaceList.count();
}

bool SpacesModel::addSpace(const Space& space)
{
    bool exists = false;





    {
        beginInsertRows (QModelIndex(), rowCount(), rowCount());
        m_spaceList.append(space);







        endInsertRows();



    }

    return exists;
}

bool SpacesModel::removeRows(int row, int count, const QModelIndex & parent)
{
    Q_ASSERT(row+count-1<m_spaceList.count());
    if(parent.isValid())
        return false;
    beginRemoveRows(parent, row, row+count-1);




    Space::List::iterator it=m_spaceList.begin()+row;
    for(int i=count-1; i>=0; i--)
    {

        it=m_spaceList.erase(it);

    }
    endRemoveRows();



    return true;
}






































void SpacesModel::clear()
{
    if(!m_spaceList.isEmpty())
    {
        beginRemoveRows (QModelIndex(), 0, rowCount());
        m_spaceList.clear();
        endRemoveRows ();
        reset();
    }
}













const Space* SpacesModel::getSpace(int num) const
{
    Q_ASSERT(num<m_spaceList.count());
    return &m_spaceList[num];
}






















bool SpacesModel::editSpace(const QString& toChange, const Space& func)
{
    bool exist=false;

    int i=0;
    for (Space::List::iterator it = m_spaceList.begin(); !exist && it != m_spaceList.end(); ++it, ++i )
    {
        if(it->id() == toChange)
        {
            exist = true;

            it->setName(func.name());
            it->setDescription(func.description());
            it->setThumbnail(func.thumbnail());


            QModelIndex idx=index(i, 0), idxEnd=index(i, columnCount()-1);
            emit dataChanged(idx, idxEnd);
            emit spaceModified(func);
        }
    }

    return exist;
}

bool SpacesModel::setData(const QModelIndex & idx, const QVariant &value, int role)
{


    if(role==Qt::EditRole)
    {

        m_spaceList[idx.row()].setThumbnail(value.value<QPixmap>());



        emit dataChanged(idx, idx);
        return true;
    }
    return false;
}













Qt::ItemFlags SpacesModel::flags(const QModelIndex &idx) const
{

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;


}


































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
}

}



