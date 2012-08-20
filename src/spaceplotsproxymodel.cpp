/*************************************************************************************
 *  Copyright (C) 2012 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>      *
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

#include "spaceplotsproxymodel.h"
#include "datastore.h"
#include <analitzaplot/plotsmodel.h>

SpacePlotsFilterProxyModel::SpacePlotsFilterProxyModel(DataStore *ds, QObject* parent): PlotsProxyModel(parent)
, m_dataStore(ds)
{
    
}

SpacePlotsFilterProxyModel::~SpacePlotsFilterProxyModel()
{

}

void SpacePlotsFilterProxyModel::setFilterSpace(DictionaryItem* space)
{
    if (!space) return;
    
    m_space = space;
    
    invalidateFilter();
}

bool SpacePlotsFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    return m_dataStore->isMapped(m_space, m_dataStore->plotsModel()->plot(sourceRow));
}
