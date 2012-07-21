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

#ifndef KHIPU_DOCUMENT_H_h_
#define KHIPU_DOCUMENT_H_h_

#include "analitzaplot/plotsproxymodel.h"

class SpaceItem;
class SpacesModel;
class DataStore;

//ademas de filtrar la dimencione sta clase se encarga de filtra por space asociado al plotitem
class SpacePlotsFilterProxyModel : public PlotsProxyModel
{
    Q_OBJECT

    public:
        SpacePlotsFilterProxyModel(DataStore *ds, QObject *parent = 0);
        virtual ~SpacePlotsFilterProxyModel();

        SpaceItem* filterSpace() const { return m_space; }
        void setFilterSpace(SpaceItem *space);

    protected:
        virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

    private:
        SpaceItem *m_space;
        DataStore *m_dataStore; // tiene los maps: space ->items
};


#endif