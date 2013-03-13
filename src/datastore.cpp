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

#include "dictionariesmodel.h"
#include "spaceplotsproxymodel.h"
#include <analitzaplot/plotsmodel.h>
#include <analitzaplot/planecurve.h>
#include <analitzaplot/plotsdictionarymodel.h>
#include <dictionaryitem.h>
#include <analitza/variables.h>
#include <analitza/expression.h>
#include <qitemselectionmodel.h>

using namespace Analitza;

Q_DECLARE_METATYPE(Analitza::PlotItem*);

DataStore::DataStore(QObject* parent)
    : QObject(parent)
    , m_currentSpace(-1)
{
//     m_plotsDictionaryModel = new PlotsDictionaryModel(this);
//     m_plotsDictionaryModel = getDictionary(this); //load with a thread

    m_spacesModel = new DictionariesModel(this);
    m_variables = new Analitza::Variables;
    m_plotsModel = new PlotsModel(this);
    
//     PlaneCurve *c = new PlaneCurve(Analitza::Expression("x->x*x"), "ee");
// //     c->setSpace(spacesModel()->addSpace(Dim2D, "adsda", "eeeee"));
//     m_plotsModel->addPlot(c);

    m_plotsDictionaryModel = new PlotsDictionaryModel(this);
    
    //EL ORDEN DE los  connect IMPORTA

    //primero nuestro datastore debe saber cual es el currentspace
    connect(this, SIGNAL(spaceActivated(int)), SLOT(setCurrentSpace(int)));

    //luego nuestro data store debe mapear los items a un space
    connect(m_plotsModel, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(mapPlot(QModelIndex,int,int)));
    // no sirve mapear una vez eliminado el item no es posible consultar
    /// see unmap para eliminar un item/plot
//     connect(m_plotsModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), SLOT(unmapPlot(QModelIndex,int,int)));

    //luego se debe permitir que los connect del proxy actuen cuando se agrega un row al modelo otiginal
    m_spacePlotsFilterProxyModel = new SpacePlotsFilterProxyModel(this);
    m_spacePlotsFilterProxyModel->setSourceModel(m_plotsModel);

    m_currentSelectionModel = new QItemSelectionModel(m_spacePlotsFilterProxyModel);
    connect(m_currentSelectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(selectCurrentPlot(QModelIndex,QModelIndex)));

    m_currentSpaceSelectionModel = new QItemSelectionModel(m_spacesModel);
    
    connect(m_spacePlotsFilterProxyModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(plotDataChanged(QModelIndex,QModelIndex)));
//     connect(m_model, SIGNAL(rowsInserted(QModelIndex,int,int)),
//         this, SLOT(addFuncs(QModelIndex,int,int)));
//     connect(m_model, SIGNAL(rowsRemoved(QModelIndex,int,int)),
//         this, SLOT(removeFuncs(QModelIndex,int,int)));

}

DataStore::~DataStore()
{
    delete m_variables;
}

bool DataStore::isMapped(DictionaryItem* space, PlotItem* plot) const
{
//     qDebug() << space << plot << m_maps.values(space).contains(plot);
//     return true;//m_maps.values(space).contains(plot);

//     qDebug() << m_maps;

    return m_maps.values(space).contains(plot);
}

void DataStore::setCurrentSpace(int spaceidx)
{
    if (m_spacesModel->index(spaceidx).isValid())
    {
//TODO aser  limites
        m_currentSpace = spaceidx;

//     qDebug() << m_currentSpace;

        //cambiar el filtro tambien hacer esto en el dashboard pero solo al agregar un nuevo space
        m_spacePlotsFilterProxyModel->setFilterSpaceDimension(m_spacesModel->space(spaceidx)->dimension());
        m_spacePlotsFilterProxyModel->setFilterSpace(m_spacesModel->space(spaceidx));
    }
}
//esto se dispara cuendo se interta un plot al modelo total
void DataStore::mapPlot(const QModelIndex & parent, int start, int end)
{
    //TODO assert si el current forma un buen item

    //NOTE la relacion es un key varios values ... un space contiene varios plots, por eso se usa el insertmulti
    PlotItem* item = m_plotsModel->index(start, 0, parent).data(PlotsModel::PlotRole).value<PlotItem*>();
    m_maps.insertMulti(m_spacesModel->space(m_currentSpace), item);

    int i = 0;
    
   /* switch (item->coordinateSystem())
    {
        case Cartesian: i = 1; break;
        case Polar: i = 2; break;
    }*/
    i=1;
    
   // emit gridStyleChanged(i);
}

void DataStore::selectCurrentPlot(const QModelIndex& curr, const QModelIndex& prev)
{
    if (!curr.isValid())
        return;
    
    int i = 0;
    /*switch (curr.data(PlotsModel::PlotRole).value<PlotItem*>()->coordinateSystem())
    {
        //TODO for 3d
        case Cartesian: i = 1; break;
        case Polar: i = 2; break;
    }*/
    i=1;
    //emit gridStyleChanged(i);
}

void DataStore::plotDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    int i = 0;
    
    switch (topLeft.data(PlotsModel::PlotRole).value<PlotItem*>()->coordinateSystem())
    {
        case Cartesian: i = 1; break;
        case Polar: i = 2; break;
    }
    
  //  emit gridStyleChanged(i);

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

//asrtos para verificar que no existan un plot asociado a mas de un space
void DataStore::unmapPlot(const QModelIndex & proxyindex )
{
    //TODO assert si el start genera un buen key
//     m_maps.remove(m_maps.key(m_plotsModel->item(start)));
// como es multimap se debe hacer una busqueda lineal

//     qDebug() << realrow << proxyindex;
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
