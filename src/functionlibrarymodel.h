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


#ifndef KEOMATH_FUNCTIONLIBRARYMODEL_H
#define KEOMATH_FUNCTIONLIBRARYMODEL_H

#include <QtCore/QAbstractTableModel>
#include <analitzaplot/plottingenums.h>
#include <QSortFilterProxyModel>

#include <QStringList>

class FunctionLibraryItem
{
public:
    FunctionLibraryItem();
    FunctionLibraryItem(const QString &name, const QString &lambda, Analitza::Dimensions dimension, const QStringList &arguments);
    FunctionLibraryItem(const FunctionLibraryItem &functionLibraryItem);
    virtual ~FunctionLibraryItem();

    QString name() const
    {
        return m_name;
    }
    QString lambda() const
    {
        return m_lambda;
    }
    Analitza::Dimensions dimension() const
    {
        return m_dimension;
    }
    QStringList arguments() const
    {
        return m_arguments;
    }

private:
    QString m_name;
    QString m_lambda;
    Analitza::Dimensions m_dimension;
    QStringList m_arguments;
};

class QIODevice;


class FunctionLibraryModel : public QAbstractTableModel
{
public:
    
    explicit FunctionLibraryModel(QObject *parent = 0);

    bool loadData(); 

    QVariant data( const QModelIndex &index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex & =QModelIndex()) const
    {
        return 4;
    }

private:


    QList<FunctionLibraryItem> m_items;
};

class FunctionLibraryFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    FunctionLibraryFilterProxyModel(QObject *parent = 0);

    QStringList filterArguments() const
    {
        return m_args;
    }
    void setFilterArguments(const QStringList &args);

    Analitza::Dimensions filterDimension() const
    {
        return m_dimension;
    }
    void setFilterDimension(Analitza::Dimensions dimension);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    QStringList m_args;
    Analitza::Dimensions m_dimension; 
};


#endif 

