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
#include <analitza/variables.h>
#include <analitza/expression.h>

DataStore::DataStore(QObject* parent)
: QObject(parent)
, m_currentSpace(-1)
{
    m_spacesModel = new SpacesModel(this);

//     m_spacesModel->addSpace(2, "adasd", "333", KIcon("kde").pixmap(QSize(256,256)));
//     m_spacesModel->addSpace(3, "123 2 234 424 23424 adaadad ad a ada adad sadassd", "333", KIcon("list-add").pixmap(QSize(256,256)));
//     m_spacesModel->addSpace(2, "adasd", "333", KIcon("roll").pixmap(QSize(256,256)));
//     m_spacesModel->addSpace(3, "adasd", "333", KIcon("list-remove").pixmap(QSize(256,256)));
//     m_spacesModel->addSpace(3, "adasd", "333", KIcon("oxygen").pixmap(QSize(256,256)));
//     m_spacesModel->addSpace(3, "adasd", "333", KIcon("okular").pixmap(QSize(256,256)));
//     m_spacesModel->addSpace(2, "adasd", "333", KIcon("dolphin").pixmap(QSize(256,256)));

    m_variables = new Analitza::Variables;
    
    m_plotsModel = new PlotsModel(this, m_variables);
    // 
    m_spacePlotsFilterProxyModel = new SpacePlotsFilterProxyModel(this);
    m_spacePlotsFilterProxyModel->setSourceModel(m_plotsModel);
    
    connect(this, SIGNAL(spaceActivated(int)), SLOT(setCurrentSpace(int)));
    
    connect(m_plotsModel, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(mapPlot(QModelIndex,int,int)));
    connect(m_plotsModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), SLOT(unmapPlot(QModelIndex,int,int)));

//     connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
//         this, SLOT(updateFuncs(QModelIndex,QModelIndex)));
//     connect(m_model, SIGNAL(rowsInserted(QModelIndex,int,int)),
//         this, SLOT(addFuncs(QModelIndex,int,int)));
//     connect(m_model, SIGNAL(rowsRemoved(QModelIndex,int,int)),
//         this, SLOT(removeFuncs(QModelIndex,int,int)));
        
//     //test code
//     PlotsModel *model = m_plotsModel;
// 
//     model->addPlaneCurve(Analitza::Expression("x->x*x"), "para", Qt::cyan);
//     model->addPlaneCurve(Analitza::Expression("q->q+2"), "polar simple", Qt::green);
//     model->addPlaneCurve(Analitza::Expression("t->vector{t*t+1, t+2}"), "vec", Qt::yellow);
//     model->addPlaneCurve(Analitza::Expression("5*(x**2+y**2)**3=15*(x*y*72)**2"), "impl", Qt::red);
//     model->addPlaneCurve(Analitza::Expression("x->2+x*x"), "otra simple", Qt::blue);
//     model->addPlaneCurve(Analitza::Expression("(x**2+y**2)**3=4*(x**2)*(y**2)"), "otra simple", Qt::lightGray);
//     model->addPlaneCurve(Analitza::Expression("(y-x**2)**2=x*y**3"), "otra simple", Qt::lightGray);
//     model->addPlaneCurve(Analitza::Expression("sin(x)*sin(y)=1/2"), "otra simple", Qt::yellow);    
//     model->addPlaneCurve(Analitza::Expression("x->x*x+2"), "asdads", Qt::yellow);
    

}

DataStore::~DataStore()
{
    delete m_variables;
}

void DataStore::setCurrentSpace(int spaceidx)
{
//TODO aser  limites
    m_currentSpace = spaceidx;
}

void DataStore::mapPlot(const QModelIndex & parent, int start, int end)
{
    
    //aserto si se esta agregando un plot de dim != al space 
    m_maps[m_currentSpace] = start;
    
//     qDebug() << m_currentSpace << start;
}
//asrtos para verificar que no existan un plot asociado a mas de un space
void DataStore::unmapPlot(const QModelIndex& parent, int start, int end)
{
    for (int i = 0; i < m_maps.values().size(); ++i)
        if (m_maps.values().at(i) == start)
        {
            m_maps.remove(m_maps.key(start));
            
            break;
        }
}


///
