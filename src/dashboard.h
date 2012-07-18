/*************************************************************************************
 *  Copyright (C) 2010-2012 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com> *
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


#ifndef GPLACS_DASHBOARD_H
#define GPLACS_DASHBOARD_H


#include <QtGui/QWidget>
#include <QUuid>
class QModelIndex;

namespace Keomath {
class FunctionsModel;
class Space;
class SpacesModel;
class Viewer2D;
class Viewer3D;
class SpacesFilterProxyModel;
class FunctionsFilterProxyModel;
}
class KCategorizedSortFilterProxyModel;
class QSortFilterProxyModel;
namespace GPLACS {

class DashboardWidget;

class Dashboard : public QWidget
{
    Q_OBJECT


    signals:
        void modified();

private:


void updateGPLACSDocument();

        bool m_modified;
        bool m_saved;

QString m_file;
QString m_lastErrorString;

public:
    QString lastErrorString() const {  return m_lastErrorString; }

private:

    QByteArray pixmapToUtf8(const QPixmap &pix) const;
    QPixmap utf8ToPixmap(const QString &pixdata) const;



public:
    bool isSaved() const { return m_saved; }
QString fileName() const { return m_file; }

    void setModified(bool v)  { m_modified = v; }
    void setSaved(bool v) { m_saved = v; }

bool isModified() const {return m_modified; }
signals:

void saveRequest();
void openRequest();

    public:    
        Dashboard(Keomath::FunctionsModel *functionsModel, Keomath::SpacesModel *spacesModel, QWidget *parent = 0, Qt::WindowFlags f = 0);
        virtual ~Dashboard();

    public slots:


        bool save(const QString &file);
        bool load(const QString &file);















        void showDashboard();

        void addSpace2D();
        void addSpace3D();

void showFunctionOnSpace(const QUuid &spaceId);

        void showSpace(const Keomath::Space &space);


        void setFilterDimension(int radioButton);
        void setFilterText(const QString &text);



        void saveSpace2DImage();
        void saveSpace3DImage();
        void copySpace2DImage(); 
        void copySpace3DImage(); 

    signals:
        void dashemitShowAppInfo();

    private:
        void setupWidget();

    private:
        DashboardWidget *m_dashboardWidget;
        Keomath::FunctionsModel *m_functionsModel;
        Keomath::SpacesModel *m_spacesModel;


        Keomath::FunctionsFilterProxyModel *m_functionsProxyModel;
        Keomath::SpacesFilterProxyModel *m_spacesProxyModel;

        Keomath::FunctionsFilterProxyModel *m_proxyViewer2D;
        Keomath::FunctionsFilterProxyModel *m_proxyViewer3D;


};

} 

#endif 
