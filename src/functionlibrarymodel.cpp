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


#include "functionlibrarymodel.h"

#include <KLocale>
#include <KStandardDirs>
#include <QDomElement>
#include <QFile>
#include <QDebug>


FunctionLibraryItem::FunctionLibraryItem()
{

}

FunctionLibraryItem::FunctionLibraryItem(const QString& name, const QString& lambda, Analitza::Dimensions dimension, const QStringList& arguments)
    : m_name(name)
    , m_lambda(lambda)
    , m_dimension(dimension)
    , m_arguments(arguments)
{

}

FunctionLibraryItem::FunctionLibraryItem(const FunctionLibraryItem &functionLibraryItem)
    : m_name(functionLibraryItem.m_name)
    , m_lambda(functionLibraryItem.m_lambda)
    , m_dimension(functionLibraryItem.m_dimension)
    , m_arguments(functionLibraryItem.m_arguments)
{

}

FunctionLibraryItem::~FunctionLibraryItem()
{

}




FunctionLibraryModel::FunctionLibraryModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

bool FunctionLibraryModel::loadData()
{
    QString kflpath = KStandardDirs::locate("data", QString("gplacs/data/functionlibrary.kfl"));



    QFile device(kflpath);

    if (!device.open(QFile::ReadOnly | QFile::Text))
        return false; 



    QDomDocument domDocument;

    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDocument.setContent(&device, true, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "Parse error" << errorStr << errorLine << errorColumn;
        return false;
    }

    QDomElement root = domDocument.documentElement();



    QDomElement functionElement = root.firstChildElement("function");

    QString lang = KGlobal::locale()->language();

    QString name;
    QString lambda;
    Analitza::Dimensions dimension = Analitza::DimAll;
    QStringList arguments;

    
    
    while (!functionElement.isNull())
    {
        QDomNodeList functionDataElements = functionElement.childNodes();

        

        name = functionDataElements.at(0).toElement().text();





        lambda = functionDataElements.at(1).toElement().text();
        dimension = Analitza::Dimension(functionDataElements.at(2).toElement().text().toInt());
        arguments.clear();

        for (int i = 0; i < functionDataElements.at(3).toElement().childNodes().size(); i +=1)
            arguments << functionDataElements.at(3).toElement().childNodes().at(i).toElement().text();

        beginInsertRows (QModelIndex(), rowCount(), rowCount());


        m_items.append(FunctionLibraryItem(name, lambda, dimension, arguments));

        endInsertRows();



        functionElement = functionElement.nextSiblingElement("function");
    }

    return  true;


}

QVariant FunctionLibraryModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.count())
        return QVariant();





    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0:
            return m_items.at(index.row()).name();

        case 1:
            return m_items.at(index.row()).lambda();

        case 2:
            return int(m_items.at(index.row()).dimension());

        case 3:
            return m_items.at(index.row()).arguments();
        }
    }
    else if (role == Qt::ToolTipRole)
    {
        if (m_items.at(index.row()).dimension() == 2)
        {
            if (m_items.at(index.row()).arguments().size() == 2)
            {
                return QString::fromUtf8("Curva implícita");
            }
            else
            {
                if (m_items.at(index.row()).arguments().at(0) == QString("x"))
                {
                    return QString::fromUtf8("Curva cartesiana y=f(x)");
                }
                if (m_items.at(index.row()).arguments().at(0) == QString("y"))
                {
                    return QString::fromUtf8("Curva cartesiana x=f(y)");
                }
                if (m_items.at(index.row()).arguments().at(0) == QString("q"))
                {
                    return QString::fromUtf8("Curva polar radio=f(q), q=theta");
                }
                if (m_items.at(index.row()).arguments().at(0) == QString("t"))
                {
                    return QString::fromUtf8("Curva paramétrica 2D");
                }
            }
        }
        else
        {




            if (m_items.at(index.row()).arguments().size() == 1)
            {
                return QString::fromUtf8("Curva Paramétrica");
            }
            else
            {
                if (m_items.at(index.row()).arguments().at(0) == QString("x"))
                {
                    return QString::fromUtf8("Superficie Cartesiana");
                }
                if (m_items.at(index.row()).arguments().at(0) == QString("r"))
                {
                    return QString::fromUtf8("Superficie en Coordenadas Cilíndricas");
                }
                if (m_items.at(index.row()).arguments().at(0) == QString("s"))
                {
                    return QString::fromUtf8("Superficie en Coordenadas Esféricas");
                }
                if (m_items.at(index.row()).arguments().at(0) == QString("u"))
                {
                    return QString::fromUtf8("Superficie Paramétrica");
                }
            }



        }


        return m_items.at(index.row()).arguments();
    }

    return QVariant();
}

QVariant FunctionLibraryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role==Qt::DisplayRole && orientation==Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return i18nc("@title:column", "Name");
            break;

        case 1:
            return i18nc("@title:column", "Function");
            break;

        case 2:
            return i18nc("@title:column", "Dimension");
            break;

        case 3:
            return i18nc("@title:column", "Arguments");
            break;
        }
    }

    return QVariant();
}

int FunctionLibraryModel::rowCount(const QModelIndex &idx) const
{
    if(idx.isValid())
        return 0;
    else
        return m_items.count();
}





FunctionLibraryFilterProxyModel::FunctionLibraryFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_dimension(-1)
{
    setDynamicSortFilter(true);
}

void FunctionLibraryFilterProxyModel::setFilterArguments(const QStringList &args)
{
    m_args = args;
    invalidateFilter();
}

void FunctionLibraryFilterProxyModel::setFilterDimension(Analitza::Dimensions dimension)
{
    m_dimension = dimension;
    invalidateFilter();
}

bool FunctionLibraryFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex indexName = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex indexLambda = sourceModel()->index(sourceRow, 1, sourceParent);

    QModelIndex indexDimension = sourceModel()->index(sourceRow, 2, sourceParent);
    QModelIndex indexArguments = sourceModel()->index(sourceRow, 3, sourceParent);


    if (sourceModel()->data(indexDimension, Qt::DisplayRole).toInt() != m_dimension)
        return false;

    if (!m_args.isEmpty()) 
        if (indexArguments.data().toStringList() != m_args)
            return false;

    return (sourceModel()->data(indexName).toString().contains(filterRegExp()) ||
            sourceModel()->data(indexLambda).toString().contains(filterRegExp()));
}
