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

#include "spacesmodel.h"
#include "spaceplotsproxymodel.h"
#include <analitzaplot/plotsmodel.h>
#include <analitzaplot/planecurve.h>
#include <analitzaplot/plotsdictionarymodel.h>
#include <analitza/variables.h>
#include <analitza/expression.h>
#include <qitemselectionmodel.h>

DataStore::DataStore(QObject* parent)
: QObject(parent)
, m_currentSpace(-1)
{
    m_plotsDictionaryModel = new PlotsDictionaryModel(this);

    m_spacesModel = new SpacesModel(this);

    m_variables = new Analitza::Variables;
    
    m_plotsModel = new PlotsModel(this, m_variables);
    // 

    //EL ORDEN DE los  connect IMPORTA

    //primero nuestro datastore debe saber cual es el currentspace
    connect(this, SIGNAL(spaceActivated(int)), SLOT(setCurrentSpace(int)));
    
    //luego nuestro data store debe mapear los items a un space
    connect(m_plotsModel, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(mapPlot(QModelIndex,int,int)));
    connect(m_plotsModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), SLOT(unmapPlot(QModelIndex,int,int)));

    //luego se debe permitir que los connect del proxy actuen cuando se agrega un row al modelo otiginal
    m_spacePlotsFilterProxyModel = new SpacePlotsFilterProxyModel(this);
    m_spacePlotsFilterProxyModel->setSourceModel(m_plotsModel);
    
    m_currentSelectionModel = new QItemSelectionModel(m_spacePlotsFilterProxyModel);
    
//     connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
//         this, SLOT(updateFuncs(QModelIndex,QModelIndex)));
//     connect(m_model, SIGNAL(rowsInserted(QModelIndex,int,int)),
//         this, SLOT(addFuncs(QModelIndex,int,int)));
//     connect(m_model, SIGNAL(rowsRemoved(QModelIndex,int,int)),
//         this, SLOT(removeFuncs(QModelIndex,int,int)));

}

DataStore::~DataStore()
{
    delete m_variables;
}

bool DataStore::isMapped(SpaceItem* space, PlotItem* plot) const
{
//     qDebug() << space << plot << m_maps.values(space).contains(plot);
//     return true;//m_maps.values(space).contains(plot);

//     qDebug() << m_maps;

    return m_maps.values(space).contains(plot);
}

void DataStore::setCurrentSpace(int spaceidx)
{
//TODO aser  limites
    m_currentSpace = spaceidx;
    
    //cambiar el filtro tambien hacer esto en el dashboard pero solo al agregar un nuevo space
    m_spacePlotsFilterProxyModel->setFilterSpaceDimension(m_spacesModel->item(spaceidx)->dimension());
    m_spacePlotsFilterProxyModel->setFilterSpace(m_spacesModel->item(spaceidx));
}
//esto se dispara cuendo se interta un plot al modelo total
void DataStore::mapPlot(const QModelIndex & parent, int start, int end)
{
    //TODO assert si el current forma un buen item
    //aserto si se esta agregando un plot de dim != al space 
    qDebug() << m_maps;

    //NOTE la relacion es un key varios values ... un space contiene varios plots, por eso se usa el insertmulti
    m_maps.insertMulti(m_spacesModel->item(m_currentSpace), m_plotsModel->item(start));
    
//     qDebug() << m_currentSpace << start;
    
    //NOTE marcar como selectionado el ultimo plot que se interta
    
//     m_currentSelectionModel->select(m_spacePlotsFilterProxyModel->mapFromSource(m_plotsModel->index(start)), QItemSelectionModel::SelectCurrent);
//     m_currentSelectionModel->select(m_spacePlotsFilterProxyModel->index(0,0), QItemSelectionModel::SelectCurrent);
//     m_currentSelectionModel->setCurrentIndex(m_spacePlotsFilterProxyModel->index(m_spacePlotsFilterProxyModel->rowCount()-1,0), QItemSelectionModel::SelectCurrent);
}
//asrtos para verificar que no existan un plot asociado a mas de un space
void DataStore::unmapPlot(const QModelIndex& parent, int start, int end)
{
    //TODO assert si el start genera un buen key
    m_maps.remove(m_maps.key(m_plotsModel->item(start)));
}
