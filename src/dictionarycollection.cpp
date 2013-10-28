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

//Analitza includes
#include <analitzagui/plotsview2d.h>
#include <analitzagui/plotsview3d.h>
#include <analitzaplot/plotsfactory.h>
#include <analitzaplot/plotsmodel.h>
#include <analitzaplot/functiongraph.h>
#include <analitzaplot/plotitem.h>

//Qt includes
#include <QDir>

//KDE includes
#include <KStandardDirs>
#include <KFileDialog>
#include <KMessageBox>

//local includes
#include "datastore.h"
#include "spacesmodel.h"
#include "spaceitem.h"
#include "plotsbuilder.h"

using namespace Analitza;

DictionaryCollection::DictionaryCollection(QWidget* parent): QDockWidget(parent)
  ,m_dictionaryModel(0)
{
    m_widget = new Ui::DictionaryCollectionWidget;
    m_widget->setupUi(this);
    connect(m_widget->AddButton,SIGNAL(pressed()),this,SLOT(addPlotClicked()));
    connect(m_widget->importDictionary,SIGNAL(pressed()),this,SLOT(importDictionary()));
    connect(m_widget->plotsView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(addPlotinSpace(QModelIndex)));
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

void DictionaryCollection::setDictionaryDataMap()
{
    m_DictionaryPathName=m_dashboard->dictionaryDataMap();
}

void DictionaryCollection::setDefaultDictionaries()
{
    QList<QString> dictionaryFileNames=m_DictionaryPathName.values();
    foreach(const QString &file,dictionaryFileNames) {
        m_widget->dictionaryNames->addItem(QFileInfo(file).baseName());
    }
    connect(m_widget->dictionaryNames,SIGNAL(currentIndexChanged(int)),this,SLOT(setDictionaryData(int)));
}

void DictionaryCollection::setDictionaryData(int ind)
{
    if(ind==-1 || m_document==0 || m_document->currentSpace()==-1) {
        return;
    }

    PlotsDictionaryModel *model=new PlotsDictionaryModel;
    model->clear();
    m_dictionaryModel=model;

    QString fileName=m_widget->dictionaryNames->itemText(ind).append(".plots");
    QString dirPath=m_DictionaryPathName.key(fileName);
    dirPath.append(fileName);
    model->createDictionary(dirPath);

    // decide the space dimension
    m_currentDimension=m_document->spacesModel()->space(m_document->currentSpace())->dimension();

    for(int i=0;i<model->rowCount();i++) {
        for(int j=0;j<model->columnCount();j++) {
            QString str =model->data(model->index(i,j),PlotsDictionaryModel::ExpressionRole).toString();
            QString exp=str.right(str.length()-str.indexOf(":=")-2); //parsing of expression from the line
            PlotBuilder req = PlotsFactory::self()->requestPlot(Analitza::Expression(exp), m_currentDimension);

            //remove un-wanted plots (mismatched with the current dimnesion) from the model
            if (!req.canDraw()) {
                model->removeRow(i);
                i--;// to check on which position current data is replaced with the next row
            }
        }
    }
    m_widget->plotsView->setModel(model);
}

void DictionaryCollection::addPlotClicked()
{
    if(m_widget->plotsView->selectionModel()==0)
        return;

    QModelIndex ind=m_widget->plotsView->selectionModel()->currentIndex();

    if(!ind.isValid()) {
        return;
    }
    addPlotinSpace(ind);
}

void DictionaryCollection::addPlotinSpace(const QModelIndex& ind)
{
    if(!ind.isValid()) {
        return;
    }

    QString str=m_dictionaryModel->data(ind,PlotsDictionaryModel::ExpressionRole).toString();
    QString exp=str.right(str.length()-str.indexOf(":=")-2); //parsing of expression from the line
    PlotBuilder req = PlotsFactory::self()->requestPlot(Analitza::Expression(exp), m_currentDimension);

    FunctionGraph *item = 0;
    item = req.create(randomFunctionColor(),m_dictionaryModel->data(ind).toString());
    m_document->plotsModel()->addPlot(item);

    emit mapDataChanged();
}

void DictionaryCollection::importDictionary()
{
    KUrl const url = KFileDialog::getOpenUrl( QDir::currentPath(),
                     i18n( "*.plots|Dictionary Files (*.plots)\n*|All Files" ), this, i18n( "Open" ) );

    QString path=url.toLocalFile();
    if(path.isEmpty())
        return;
    int currentIndex=m_widget->dictionaryNames->count();
    m_widget->dictionaryNames->addItem(QFileInfo(path).baseName());
    m_widget->dictionaryNames->setCurrentIndex(currentIndex);
    m_DictionaryPathName.insertMulti(QFileInfo(path).path().append("/"),QFileInfo(path).fileName());
    setDictionaryData(currentIndex);
}

bool DictionaryCollection::conains(const QString &dictionaryname)
{
    if(m_widget->dictionaryNames->findText(dictionaryname)==-1) {
        return false;
    }
    return true;
}

int DictionaryCollection::indexOf(const QString &dictionaryname)
{
    return m_widget->dictionaryNames->findText(dictionaryname);
}

int DictionaryCollection::totalDictionaries()
{
   return m_widget->dictionaryNames->count();
}

QTreeView* DictionaryCollection::dictionaryPlotsView()
{
    return m_widget->plotsView;
}
