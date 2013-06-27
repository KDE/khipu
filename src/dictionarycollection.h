/*************************************************************************************
 *  Copyright (C) 2013 by Punit Mehta <punit9462@gmail.com>                          *
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

#ifndef DICTIONARY_COLLECTION_H
#define DICTIONARY_COLLECTION_H

//Qt includes
#include <QtGui/QWidget>
#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QDockWidget>

//analitza includes
#include <dashboard.h>
#include <analitzaplot/plotsdictionarymodel.h>

using namespace Analitza;

namespace Ui
{
    class DictionaryCollectionWidget;
}

class DictionaryCollection : public QDockWidget
{
    Q_OBJECT

public:
    DictionaryCollection(QWidget *parent);
    ~ DictionaryCollection();

    void setDashboardWidget(Dashboard *dashboard);
    void setDocument(DataStore *doc);
    void setDefaultDictionaries();
    void setDictionaryfilePath();
    void setDictionaryTitles();

signals:
    void mapDataChanged();

private slots:
    void setDictionaryData(int ind);
    void addPlotInSpace();
    void setModelIndex(const QModelIndex& ind);
    void importDictionary();

private:
    Ui::DictionaryCollectionWidget *m_widget;
    Dashboard* m_dashboard;
    DataStore* m_document;
    Analitza::Dimension m_currentDimension;
    PlotsDictionaryModel* m_dictionaryModel;
    QStringList m_dictionaryTitles;
    QStringList m_dictionaryList;
    int m_totalinternalDict;
};

#endif
