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

#include "datastore.h"

//Analitza includes
#include <analitza/variables.h>
#include <analitza/expression.h>
#include <analitzaplot/plotsmodel.h>
#include <analitzaplot/planecurve.h>
#include <analitzaplot/plotsdictionarymodel.h>

//Qt includes
#include <qitemselectionmodel.h>
#include <QFile>
#include <QString>

//KDE includes
#include <KUrl>
#include <KFileDialog>
#include <KMessageBox>
#include <KDE/KLocalizedString>

//local includes
#include "dictionariesmodel.h"
#include "spaceplotsproxymodel.h"
#include "dictionaryitem.h"
#include "plotseditor.h"

using namespace Analitza;

Q_DECLARE_METATYPE(PlotItem*);

DataStore::DataStore(QObject* parent)
    : QObject(parent)
    , m_currentSpace(-1)
{
    m_spacesModel = new DictionariesModel(this);
    m_variables = new Analitza::Variables;
    m_plotsModel = new PlotsModel(this);
    
    m_plotsDictionaryModel = new PlotsDictionaryModel(this);
    
    connect(this, SIGNAL(spaceActivated(int)), SLOT(setCurrentSpace(int)));

    connect(m_plotsModel, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(mapPlot(QModelIndex,int,int)));

    m_spacePlotsFilterProxyModel = new SpacePlotsFilterProxyModel(this);
    m_spacePlotsFilterProxyModel->setSourceModel(m_plotsModel);

    m_currentSelectionModel = new QItemSelectionModel(m_spacePlotsFilterProxyModel);
    connect(m_currentSelectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(selectCurrentPlot(QModelIndex,QModelIndex)));

    m_currentSpaceSelectionModel = new QItemSelectionModel(m_spacesModel);
    

    connect(m_spacePlotsFilterProxyModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(plotDataChanged(QModelIndex,QModelIndex)));
}

DataStore::~DataStore()
{
    delete m_variables;
}

bool DataStore::isMapped(DictionaryItem* space, PlotItem* plot) const
{
    return m_maps.values(space).contains(plot);
}

void DataStore::setCurrentSpace(int spaceidx)
{
    if (m_spacesModel->index(spaceidx).isValid())
    {
        m_currentSpace = spaceidx;
        m_spacePlotsFilterProxyModel->setFilterSpaceDimension(m_spacesModel->space(spaceidx)->dimension());
        m_spacePlotsFilterProxyModel->setFilterSpace(m_spacesModel->space(spaceidx));
    }
}

void DataStore::mapPlot(const QModelIndex & parent, int start, int end)
{
    PlotItem* item = m_plotsModel->index(start, 0, parent).data(PlotsModel::PlotRole).value<PlotItem*>();

    m_maps.insertMulti(m_spacesModel->space(m_currentSpace), item);

    int i = 0;

    switch (item->coordinateSystem())
    {
        case Cartesian: i = 1; break;
        case Polar: i = 2; break;
        default : i=0; break;
    }
    //emit gridStyleChanged(i);
}

void DataStore::selectCurrentPlot(const QModelIndex& curr, const QModelIndex& prev)
{
    if (!curr.isValid())
        return;
    
    int i = 0;
    switch (curr.data(PlotsModel::PlotRole).value<PlotItem*>()->coordinateSystem())
    {
        //TODO for 3d
        case Cartesian: i = 1; break;
        case Polar: i = 2; break;
        default : i=0; break;
    }
    emit gridStyleChanged(i);
}

void DataStore::plotDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    int i = 0;

    switch (topLeft.data(PlotsModel::PlotRole).value<PlotItem*>()->coordinateSystem())
    {
        case Cartesian: i = 1; break;
        case Polar: i = 2; break;
        default : i=0; break;
    }
    emit gridStyleChanged(i);
}

void DataStore::removeCurrentSpace()
{
    if (m_currentSpaceSelectionModel->hasSelection())
    {

        m_maps.remove(m_spacesModel->space(m_currentSpace));

        m_spacesModel->removeRow(m_currentSpace);

        m_currentSpaceSelectionModel->clear();
    }
}

void DataStore::unmapPlot(const QModelIndex & proxyindex )
{
    QMap<DictionaryItem*, PlotItem*>::iterator i = m_maps.begin();

    while (i != m_maps.end())
    {
        if (i.value() == proxyindex.data(PlotsModel::PlotRole).value<PlotItem*>())
        {
            m_maps.erase(i);
            break;
        }
        ++i;
    }
    m_spacePlotsFilterProxyModel->removeRow(proxyindex.row());
}

void DataStore::removeSpace(int row)
{
    m_maps.remove(m_spacesModel->space(row));
    m_spacesModel->removeRow(row);
}

void DataStore::saveSpaceAsDictionary(QModelIndex ind)
{
    KUrl url = KFileDialog::getSaveUrl(QDir::homePath(),i18n( "*.plots|Plot-Dictionary Files (*.plots)\n*|All Files" ),0, i18n( "Export the space as a Dictionary " ));
    QString path=url.path();
    QFile *file = new QFile(path,this);

    if(!file->open(QFile::WriteOnly | QFile::Text)){
        qDebug() << "Error in writing";
        return;
    }

    QTextStream out(file);
    if(m_maps.empty()) {
        KMessageBox::error(0,i18n("Error in saving file,no plots available for this space"),i18n("Error in saving a dictionary"));
        return;
    }

    QList<Analitza::PlotItem*> itemList= m_maps.values(m_spacesModel->space(ind.row()));

    for(int i=0;i<itemList.size();i++) {
        QString name = itemList.at(i)->name();
        // we dont want the plots in the dictionary which dont have names !
        if(!name.isEmpty())
            out << name << " := " << itemList.at(i)->expression().toString() << "\n";
    }
    file->close();
}
