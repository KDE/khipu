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

#include "dictionarycollection.h"
#include "ui_dictionarycollection.h"

//KDE includes
#include <KStandardDirs>

//Qt includes
#include <QDir>
#include <QFileDialog>

//Analitza includes
#include <analitzaplot/plotsdictionarymodel.h>
#include <datastore.h>
#include <dictionariesmodel.h>
#include <dictionaryitem.h>
#include <plotsbuilder.h>
#include <analitzaplot/plotsfactory.h>
#include <analitzaplot/plotsmodel.h>
#include <analitzagui/plotsview2d.h>
#include <analitzagui/plotsview3d.h>
#include <analitzaplot/functiongraph.h>
#include <analitzaplot/plotitem.h>

using namespace Analitza;

DictionaryCollection::DictionaryCollection(QWidget* parent): QDockWidget(parent)
  ,m_dictionaryModel(0)
{
    m_widget = new Ui::DictionaryCollectionWidget;
    m_widget->setupUi(this);
    connect(m_widget->dictionaryNames,SIGNAL(currentIndexChanged(int)),this,SLOT(setDictionaryData(int)));
    connect(m_widget->AddButton,SIGNAL(pressed()),this,SLOT(addPlotInSpace()));
    connect(m_widget->importDictionary,SIGNAL(pressed()),this,SLOT(importDictionary()));
}

DictionaryCollection::~DictionaryCollection()
{
    delete m_widget;
}

void DictionaryCollection::setDashboardWidget(Dashboard *dashboard)
{
    m_dashboard=dashboard;
}

void DictionaryCollection::setDocument(DataStore *doc)
{
    m_document=doc;
}

void DictionaryCollection::setDictionaryfilePath()
{
    m_dictionaryList=m_dashboard->dictionaryFileList();
}

void DictionaryCollection::setDictionaryTitles()
{
    m_dictionaryTitles=m_dashboard->dictionaryTitles();
}

void DictionaryCollection::setDefaultDictionaries()
{
    m_widget->dictionaryNames->addItems(m_dictionaryTitles);
    m_totalinternalDict=m_dictionaryTitles.size();
}

void DictionaryCollection::setDictionaryData(int ind)
{
    PlotsDictionaryModel *model=new PlotsDictionaryModel;
    model->clear();
    m_dictionaryModel=model;

    if(ind < m_totalinternalDict) { // total internal available dictionaries
        QString str=KStandardDirs::installPath("data"); // works for -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix`
        QDir dir(str.append("libanalitza/plots"));
        model->createDictionary(dir.path().append("/").append(m_dictionaryList.at(ind)));
    }

    else {
        model->createDictionary(m_dictionaryList.at(ind));
    }

    if(m_document->currentSpace()==-1)
        return;

    // decide the space dimension
    m_currentDimension=m_document->spacesModel()->space(m_document->currentSpace())->dimension();

    for(int i=0;i<model->rowCount();i++) {

        for(int j=0;j<model->columnCount();j++) {

            QString str =model->data(model->index(i,j),PlotsDictionaryModel::ExpressionRole).toString();
            QString exp=str.right(str.length()-str.indexOf(":=")-2); //parsing of expression from the line
            PlotBuilder req = PlotsFactory::self()->requestPlot(Analitza::Expression(exp), m_currentDimension);

            //remove un-wanted plots (unmatched with the current dimnesion) from the model
            if (!req.canDraw()) {
                qDebug() << "can't draw";
                model->removeRow(i);
                i--;// to check on the position where the current data is replaced with the next row
            }
        }
    }

    m_widget->plotsView->setModel(model);
    connect(m_widget->plotsView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(setModelIndex(QModelIndex)));
}

void DictionaryCollection::setModelIndex(const QModelIndex& ind)
{

}

void DictionaryCollection::addPlotInSpace()
{
    QModelIndex ind=m_widget->plotsView->selectionModel()->currentIndex();

    QString str =m_dictionaryModel->data(ind,PlotsDictionaryModel::ExpressionRole).toString();

    QString exp=str.right(str.length()-str.indexOf(":=")-2); //parsing of expression from the line

    PlotBuilder req = PlotsFactory::self()->requestPlot(Analitza::Expression(exp), m_currentDimension);

    FunctionGraph *item = 0;
    item = req.create(Qt::blue,m_dictionaryModel->data(ind).toString());

    m_document->plotsModel()->addPlot(item);

    emit mapDataChanged();
}

void DictionaryCollection::importDictionary()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open a .plots file"),"/",tr("Text files (*.plots)"));
    if(path==0){
        qDebug() << "error in opening file...may be path not found." ;
        return;
    }

    m_widget->dictionaryNames->addItem("ImportedDictionary");
    m_dictionaryTitles << "ImportedDictionary";
    m_dictionaryList << path;
}
