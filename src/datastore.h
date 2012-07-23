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

#ifndef KHIPU_DOCUMENT_H_ds
#define KHIPU_DOCUMENT_H_ds

#include <QObject>
#include <QMap>

class PlotsDictionaryModel;
class QItemSelectionModel;
class SpaceItem;
class QModelIndex;
namespace Analitza {
class Variables;
}

class PlotItem;
class SpacesModel;
class PlotsModel;

class SpacePlotsFilterProxyModel;

class DataStore : public QObject
{
    Q_OBJECT

public:
    DataStore(QObject *parent = 0);
    ~DataStore();

    //es necesario para casos en donde no se esta dentro de un slot que tiene el space actua: ejemplo mainwnd::go_slot
    //o dashboard currentsnapshot
    int currentSpace() const { return m_currentSpace; }

    PlotsDictionaryModel *plotsDictionaryModel() const { return m_plotsDictionaryModel; }
    SpacesModel *spacesModel() const { return m_spacesModel; }
    PlotsModel *plotsModel() const { return m_plotsModel; }
    
    // este proxy se usara en el editor y en el dashboard cuando se este editando un space y se neceite filtrar sus plots
    SpacePlotsFilterProxyModel * currentPlots() const { return m_spacePlotsFilterProxyModel; }
    QItemSelectionModel *currentSelectionModel() const { return m_currentSelectionModel; }

    bool isMapped(SpaceItem *space, PlotItem *plot) const;

private slots:
    void setCurrentSpace(int spaceidx);
    void mapPlot(const QModelIndex & parent, int start, int end); // mapea el plot con el spacio actual start == end
    void unmapPlot(const QModelIndex & parent, int start, int end); // cuando se borra un plot del modelo 
    
signals:
//     void modified(); ... TODO to document???

    void spaceActivated(int spaceidx);

private:
    PlotsDictionaryModel *m_plotsDictionaryModel;
    SpacesModel *m_spacesModel;
    PlotsModel *m_plotsModel;
    
    SpacePlotsFilterProxyModel * m_spacePlotsFilterProxyModel;
    QItemSelectionModel *m_currentSelectionModel;
    
    Analitza::Variables *m_variables;

    //one to many -- space index -> many plots index
    int m_currentSpace; // curr space index 
    QMap<SpaceItem*, PlotItem*> m_maps;
};


#endif
