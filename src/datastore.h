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

//Qt includes
#include <QObject>
#include <QMap>
#include <QModelIndex>

class QItemSelectionModel;
class SpaceItem;

namespace Analitza {
class PlotsDictionaryModel;
class Variables;
class PlotsModel;
class PlotItem;
}

class SpacesModel;
class SpacePlotsFilterProxyModel;

class DataStore : public QObject
{
    Q_OBJECT

public:
    explicit DataStore(QObject *parent = nullptr);
    ~DataStore() override;

    int currentSpace() const { return m_currentSpace; }

    Analitza::PlotsDictionaryModel *plotsDictionaryModel() const { return m_plotsDictionaryModel; }
    SpacesModel *spacesModel() const { return m_spacesModel; }
    Analitza::PlotsModel *plotsModel() const { return m_plotsModel; }

    SpacePlotsFilterProxyModel * currentPlots() const { return m_spacePlotsFilterProxyModel; }
    QItemSelectionModel *currentSelectionModel() const { return m_currentSelectionModel; }
    QItemSelectionModel *currentSpaceSelectionModel() const { return m_currentSpaceSelectionModel; }
    QMap<SpaceItem*, Analitza::PlotItem*> currentDataMap() const { return m_maps; }

    bool isMapped(SpaceItem *space, Analitza::PlotItem *plot) const;
    void removeSpace(int row);

private slots:
    void mapPlot(const QModelIndex& parent, int start);
    void selectCurrentPlot(const QModelIndex & curr);
    void plotDataChanged ( const QModelIndex& topLeft );

public slots: 
    void setCurrentSpace(int spaceidx);
    void removeCurrentSpace();
    void unmapPlot(const QModelIndex & proxyindex);
    void saveSpaceAsDictionary(QModelIndex ind);
    void clearAllData();

signals:
    void spaceActivated(int spaceidx);
    void gridStyleChanged(int i); // 1 cartesian 2 polar
    void mapDataChanged();

private:
    Analitza::PlotsDictionaryModel *m_plotsDictionaryModel;
    SpacesModel *m_spacesModel;
    Analitza::PlotsModel *m_plotsModel;
    
    SpacePlotsFilterProxyModel * m_spacePlotsFilterProxyModel;
    QItemSelectionModel *m_currentSelectionModel;
    QItemSelectionModel *m_currentSpaceSelectionModel;
    
    Analitza::Variables *m_variables;

    //one to many -- space index -> many plots index
    int m_currentSpace; // curr space index 
    QMap<SpaceItem*, Analitza::PlotItem *> m_maps;
};


#endif
