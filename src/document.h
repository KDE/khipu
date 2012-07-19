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
#include <kurl.h>

namespace Analitza {
class Variables;
}

class PlotItem;
class SpaceItem;
class PlotsModel;
class SpacesModel;

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

    PlotsModel *plotsModel() const { return m_plotsModel; }
    SpacesModel *spacesModel() const { return m_spacesModel; }
    
public slots:
    void load(const KUrl& fileUrl);
    void save();
    void saveAs(const KUrl& fileUrl);
    void setModified(bool mod = true) { m_modified = mod; }

signals:
    void loaded(bool isok);
    void saved(bool isok);
    void modified();

private:
    QByteArray pixmapToUtf8(const QPixmap &pix) const;
    QPixmap utf8ToPixmap(const QString &pixdata) const;
    
    
    KUrl m_fileUrl;
    bool m_modified;
    
    PlotsModel *m_plotsModel;
    SpacesModel *m_spacesModel;
    Analitza::Variables *m_variables;

    //one to many
    QMap<SpaceItem *, PlotItem *> m_maps;
};

#endif // KHIPU_DOCUMENT_H
