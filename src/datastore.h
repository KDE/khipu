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

class QItemSelectionModel;
class DictionaryItem;
class QModelIndex;
namespace Analitza {
class PlotsDictionaryModel;
class Variables;
class PlotsModel;
class PlotItem;
}

class DictionariesModel;
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


    Analitza::PlotsDictionaryModel *plotsDictionaryModel() const { return m_plotsDictionaryModel; }
    DictionariesModel *spacesModel() const { return m_spacesModel; }
    Analitza::PlotsModel *plotsModel() const { return m_plotsModel; }
    
    // este proxy se usara en el editor y en el dashboard cuando se este editando un space y se neceite filtrar sus plots
    SpacePlotsFilterProxyModel * currentPlots() const { return m_spacePlotsFilterProxyModel; }
    QItemSelectionModel *currentSelectionModel() const { return m_currentSelectionModel; }
    QItemSelectionModel *currentSpaceSelectionModel() const { return m_currentSpaceSelectionModel; }


    bool isMapped(DictionaryItem *space, Analitza::PlotItem *plot) const;

private slots:
    void mapPlot(const QModelIndex & parent, int start, int end); // mapea el plot con el spacio actual start == end
    void selectCurrentPlot(const QModelIndex & curr, const QModelIndex & prev );
    void plotDataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight ); // actualiza el combo de coords
    
    
public slots: 
    void setCurrentSpace(int spaceidx);
    void removeCurrentSpace();
    //NOTA esta es la foma de borrar un plot
    void unmapPlot(const QModelIndex & proxyindex /*or viewindex*/); // cuando se borra un plot del modelo ... el viewindex es el index del view es decir del proxy

signals:
//     void modified(); ... TODO to document???

    void spaceActivated(int spaceidx);
    void gridStyleChanged(int i); // 1 cartesian 2 polar
private:
    Analitza::PlotsDictionaryModel *m_plotsDictionaryModel;
    DictionariesModel *m_spacesModel;
    Analitza::PlotsModel *m_plotsModel;
    
    SpacePlotsFilterProxyModel * m_spacePlotsFilterProxyModel;
    QItemSelectionModel *m_currentSelectionModel;
    QItemSelectionModel *m_currentSpaceSelectionModel;
    
    Analitza::Variables *m_variables;

    //one to many -- space index -> many plots index
    int m_currentSpace; // curr space index 
    QMap<DictionaryItem*, Analitza::PlotItem*> m_maps;
};


#endif
