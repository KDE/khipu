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

//Analitza includes
#include <analitzaplot/plotsdictionarymodel.h>

//Qt includes
#include <QtGui/QWidget>
#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QDockWidget>

//local includes
#include "dashboard.h"

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

    //set the appropirate dashboard widget reference
    void setDashboardWidget(Dashboard *dashboard);

    //set the current document
    void setDocument(DataStore *doc);

    //attach the in-built dictionaries provided by analitza
    void setDefaultDictionaries();

    //map the dictionary path to the dictionary names
    void setDictionaryDataMap();

    //checks whether the dictionary is available in the combobox
    bool conains(const QString &dictionaryname);

    //total number of avalialble dictionaries
    int totalDictionaries();

    //retuns the reference of the dictionary view
    QTreeView* dictionaryPlotsView();

    //returns the index of the dictionary from the combo-box
    int indexOf(const QString &dictionaryname);

signals:

    //this signal is emited when the plot is added in the document
    void mapDataChanged();

public slots:

    //Slot for handling push button's click-event and plots from the dictionaries
    void addPlotClicked();
    void setDictionaryData(int ind);
    void addPlotinSpace(const QModelIndex& ind);

private slots:

    //To import dictionary from the disk
    void importDictionary();

    //Used to add random coloring in the dictionary plots
    QColor randomFunctionColor() { return QColor::fromHsv(qrand()%255, 255, 255); }

private:
    Ui::DictionaryCollectionWidget *m_widget;
    Dashboard* m_dashboard;
    DataStore* m_document;
    Analitza::Dimension m_currentDimension;
    PlotsDictionaryModel* m_dictionaryModel;
    QMap<QString,QString> m_DictionaryPathName;
};

#endif
