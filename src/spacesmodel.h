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

#ifndef DICTIONARIESMODEL_H
#define DICTIONARIESMODEL_H

//Analitza includes
#include <analitzaplot/plottingenums.h>

//Qt includes
#include <QtCore/QAbstractListModel>
#include <QIcon>

namespace Analitza { class Expression; }
class SpaceItem;

class SpacesModel : public QAbstractListModel
{

friend class SpaceItem;
Q_OBJECT

public:
    explicit SpacesModel(QObject *parent=nullptr);

    Qt::ItemFlags flags ( const QModelIndex & index ) const override;
    
    QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const override;
    QVariant data( const QModelIndex &index, int role=Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    int columnCount(const QModelIndex& parent) const override;
    bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() ) override;

    SpaceItem * addSpace(Analitza::Dimension dim, const QString & title = QString(), const QString &description = QString(),
                         const QPixmap &thumbnail=QIcon::fromTheme("khipu").pixmap(QSize(256,256)));

    SpaceItem * space(int row) const;

    //TODO temp solution for bug ... we need better API for this model
    QList<SpaceItem *> items() const { return m_items; }
    
protected:
    void emitChanged(SpaceItem* it);

private:
    QList<SpaceItem *> m_items;
    bool m_itemCanCallModelRemoveItem; // just a lock para evitar que el item llame recursivamente a removeItem
};

#endif 
