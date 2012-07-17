/*************************************************************************************
 *  Copyright (C) 2007-2009 by Aleix Pol <aleixpol@kde.org>                          *
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



#include "functionsmodel.h"

#include <KLocale>
#include <KApplication>
#include <KDebug>
#include <QPixmap>
#include <QFont>
#include <qitemselectionmodel.h>
#include <cmath>
// #include "solvers/solver.h"
#include "analitza/expression.h"

#include "analitzaplot/planecurve.h"
#include "analitzaplot/plotsview2d.h"
#include <analitzaplot/plotsmodel.h>

namespace Keomath
{


FunctionsModel::FunctionsModel(QObject * parent, Analitza::Variables *v)
    : PlotsModel(parent, v)

{
    m_selectionModel = new QItemSelectionModel(this);

}


FunctionsModel::~FunctionsModel()
{

}


QString FunctionsModel::spaceId(int row) const
{
//     return m_spaceIds.key(item(row)->id());
return QString();
}

int FunctionsModel::currentItem() const
{
    return m_selectionModel->currentIndex().row();
}

void FunctionsModel::setSpaceId(int row, const QString& spaceId)
{
    m_spaceIds[spaceId] = item(row);
}

void FunctionsModel::removeItemsBySpace(const QString& spaceId)
{
    QList<PlotItem*> its = m_spaceIds.values(spaceId);
    
    for (int row = 0; row < rowCount(); ++row)
        foreach(PlotItem *itemid, its)
            if (itemid == item(row))
                removeItem(row);
}

///

FunctionsFilterProxyModel::FunctionsFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_dimension(-1)
{
    setDynamicSortFilter(true);
}

void FunctionsFilterProxyModel::setFilterSpaceId(const QString &spaceId)
{
    m_spaceId = spaceId;
    invalidateFilter();
}

void FunctionsFilterProxyModel::setFilterDimension(int dimension)
{
    m_dimension = dimension;
    invalidateFilter();
}

bool FunctionsFilterProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    QModelIndex indexName = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex indexLambda = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex indexDimension = sourceModel()->index(sourceRow, 2, sourceParent);

    QString dim;

    if (m_dimension == 2)
        dim = QString("2D");
    else if (m_dimension == 3)
        dim = QString("3D");

    Keomath::FunctionsModel *fm = static_cast<Keomath::FunctionsModel*>(sourceModel());



    if (!m_spaceId.isNull())
        if (fm->spaceId(sourceRow) != m_spaceId)
            return false;


    if (m_dimension != -1) 
        if (sourceModel()->data(indexDimension, Qt::DisplayRole).toString() != dim)
            return false;


    return (sourceModel()->data(indexName).toString().contains(filterRegExp()) ||
            sourceModel()->data(indexLambda).toString().contains(filterRegExp()));
}

} 

