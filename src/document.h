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

#ifndef KHIPU_DOCUMENT_H
#define KHIPU_DOCUMENT_H

#include <QObject>
#include <QMap>
#include <QPixmap>
#include <qitemselectionmodel.h>
#include <kurl.h>

class QModelIndex;
namespace Analitza {
class Variables;
}

class PlotItem;
class SpaceItem;
class SpacesModel;
class PlotsModel;

#include "analitzaplot/plotsfilterproxymodel.h"

class DataStore : public QObject
{
    
};


//ademas de filtrar la dimencione sta clase se encarga de filtra por space asociado al plotitem
class SpacePlotsFilterProxyModel : public PlotsFilterProxyModel
{
    Q_OBJECT

    public:
        SpacePlotsFilterProxyModel(QObject *parent = 0);
        virtual ~SpacePlotsFilterProxyModel();

        const SpaceItem* filterSpace() const { return m_space; }
        void setFilterSpace(const SpaceItem *space);

    protected:
        virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

    private:
        const SpaceItem *m_space;
        SpacesModel * m_spacesModel;
};

//NOTE one app <-> one doc ... kiss ;)
//contiene los modelos y las funciones de guardar load etc
class Document : public QObject
{
    Q_OBJECT

public:
    Document(QObject *parent = 0);
    virtual ~Document();
    
    KUrl fileUrl() const { return m_fileUrl; }
    bool isModified() const { return m_modified; }
    
    //este metodo publico era necesario para el editor ... pero el main window ya le avisa al edito cuando se activa un space
//     int currentSpace() const { return m_currentSpace; }

    SpacesModel *spacesModel() const { return m_spacesModel; }
    PlotsModel *plotsModel() const { return m_plotsModel; }
    
    // este proxy se usara en el editor y en el dashboard cuando se este editando un space y se neceite filtrar sus plots
    SpacePlotsFilterProxyModel * spacePlotsFilterProxyModel() const { return m_spacePlotsFilterProxyModel; }

public slots:
    void load(const KUrl& fileUrl);
    void save();
    void saveAs(const KUrl& fileUrl);
    void setModified(bool mod = true) { m_modified = mod; }
    
private slots:
    void setCurrentSpace(int spaceidx);
    void mapPlot(const QModelIndex & parent, int start, int end); // mapea el plot con el spacio actual start == end
    void unmapPlot(const QModelIndex & parent, int start, int end); // cuando se borra un plot del modelo 
    
signals:
    void loaded(bool isok);
    void saved(bool isok);
    void modified();

    void spaceActivated(int spaceidx);
    
private:
    QByteArray pixmapToUtf8(const QPixmap &pix) const;
    QPixmap utf8ToPixmap(const QString &pixdata) const;
    
    KUrl m_fileUrl;
    bool m_modified;
    
    SpacesModel *m_spacesModel;
    PlotsModel *m_plotsModel;
    SpacePlotsFilterProxyModel * m_spacePlotsFilterProxyModel;
    
    Analitza::Variables *m_variables;

    //one to many -- space index -> many plots index
    int m_currentSpace; // curr space index 
    QMap<int, int> m_maps;
};











#endif // KHIPU_DOCUMENT_H
