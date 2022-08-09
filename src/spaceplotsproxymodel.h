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

//Analitza includes
#include <analitzaplot/plottingenums.h>

//Qt includes
#include <QSortFilterProxyModel>

class SpaceItem;
class SpacesModel;
class DataStore;

class SpacesFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    public:
        explicit SpacesFilterProxyModel(QObject *parent = nullptr);

        void setFilterDimension(Analitza::Dimensions dimension);
        void setFilterText(const QString& text);
protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

    private:
        Analitza::Dimensions m_dimension;
        QString m_filterText;
};

//si no se establece ningun filter2d entonces no hay filtro y muetra todos los items sin importar que dimension
//estoy es util para un vista (itemview) que quiera mostrar todo el modelo por ejemplo 
class PlotsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    public:

        explicit PlotsProxyModel(QObject *parent = nullptr);
        ~PlotsProxyModel() override;

        int filterSpaceDimension() const { return m_dimension; }
        void setFilterSpaceDimension(Analitza::Dimensions dimension);

        //functiontype ... if the item is a functiongraph TODO

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
        bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;

    private:
        Analitza::Dimensions m_dimension;
};

//ademas de filtrar la dimencione sta clase se encarga de filtra por space asociado al plotitem
class SpacePlotsFilterProxyModel : public PlotsProxyModel
{
    Q_OBJECT

    public:
        explicit SpacePlotsFilterProxyModel(DataStore *ds, QObject *parent = nullptr);
        ~SpacePlotsFilterProxyModel() override;

        SpaceItem* filterSpace() const { return m_space; }
        void setFilterSpace(SpaceItem *space);

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

    private:
        SpaceItem *m_space;
        DataStore *m_dataStore; // tiene los maps: space ->items
};

#endif
