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


#ifndef FUNCTIONSMODEL_H
#define FUNCTIONSMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtGui/QSortFilterProxyModel>
#include <QtCore/QPointF>
#include <QtGui/QVector3D>
#include <QVector>
#include <QLineF>


#include <KDE/KLocalizedString>


#include "analitzaplot/private/functiongraphsmodel.h"

class QItemSelectionModel;

namespace Keomath
{


class FunctionsModel : public FunctionGraphsModel
{
    Q_OBJECT
public:
    explicit FunctionsModel(QObject *parent=0);
    FunctionsModel(Analitza::Variables *v, QObject * parent = 0);

    virtual ~FunctionsModel();
    
    QString spaceId(int row) const;
    int currentItem() const;

public slots:
    void setSpaceId(int row, const QString& spaceId); //str sid -> str fky 
    void removeItemsBySpace(const QString& spaceId);
    
private:
    QMap<QString, QString> m_spaceIds;
    QItemSelectionModel *m_selectionModel;
};

class FunctionsFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    FunctionsFilterProxyModel(QObject *parent = 0);

    QString filterSpaceId() const
    {
        return m_spaceId;
    }
    void setFilterSpaceId(const QString &spaceId);


    int filterDimension() const
    {
        return m_dimension;
    }
    void setFilterDimension(int dimension);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    int m_dimension; 
    QString m_spaceId;
};


} 

#endif
