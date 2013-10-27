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

#include <dictionarycollectiontest.h>

//Qt includes
#include <qtest_kde.h>
#include <QTreeView>
#include <QItemSelectionModel>

//Analitza includes
#include <analitzaplot/plotitem.h>
#include <analitzaplot/plotsmodel.h>

//libkhipu includes
#include <dictionarycollection.h>
#include <dashboard.h>
#include <datastore.h>
#include <spacesmodel.h>

using namespace Analitza;

QTEST_KDEMAIN( DictionaryCollectionTest, GUI)

Q_DECLARE_METATYPE (Analitza::Dimension);

Q_DECLARE_METATYPE (PlotItem*);

DictionaryCollectionTest::DictionaryCollectionTest(QObject *parent)
    : QObject(parent)
{
}

DictionaryCollectionTest::~DictionaryCollectionTest()
{
}

void DictionaryCollectionTest::initTestCase()
{}

void DictionaryCollectionTest::cleanupTestCase()
{}

void DictionaryCollectionTest::testCorrect_data()
{
    QTest::addColumn<QString>("dictionaryname");
    QTest::newRow("1") << "3Ds";
    QTest::newRow("2") << "basic_curves";
    QTest::newRow("3") << "conics";
    QTest::newRow("4") << "polar";
}

void DictionaryCollectionTest::testCorrect()
{
    QFETCH(QString,dictionaryname);

    Dashboard *dashboard = new Dashboard(0);
    DictionaryCollection *testWidget = new DictionaryCollection(0);
    testWidget->setDashboardWidget(dashboard);
    testWidget->setDictionaryDataMap();
    testWidget->setDefaultDictionaries();

    QList<QString> values=dashboard->dictionaryDataMap().values();
    QCOMPARE(values.size(),4);
    QVERIFY(testWidget->conains(dictionaryname));
    QCOMPARE(testWidget->totalDictionaries(),4);
}

void DictionaryCollectionTest::testIncorrect_data()
{
    QTest::addColumn<QString>("dictionaryname");
    QTest::newRow("1") << "dictionary1";
    QTest::newRow("2") << "test";
    QTest::newRow("3") << "wrong_name";
    QTest::newRow("4") << "";
    QTest::newRow("5") << "incorrect";
}

void DictionaryCollectionTest::testIncorrect()
{
    QFETCH(QString,dictionaryname);

    Dashboard *dashboard = new Dashboard(0);
    DictionaryCollection *testWidget = new DictionaryCollection(0);
    testWidget->setDashboardWidget(dashboard);
    testWidget->setDictionaryDataMap();
    testWidget->setDefaultDictionaries();

    QList<QString> values=dashboard->dictionaryDataMap().values();

    // This will always 4.Since the correct dictionaries are always there.
    QCOMPARE(values.size(),4);
    QCOMPARE(testWidget->totalDictionaries(),4);

    QVERIFY(!testWidget->conains(dictionaryname));
}

void DictionaryCollectionTest::testAddPlot_data()
{
    QTest::addColumn<QString>("dictionaryname");
    QTest::addColumn<QString>("plotname1");
    QTest::addColumn<QString>("ploteqn1");

    QTest::newRow("1") << "3Ds";
}

void DictionaryCollectionTest::testAddPlot()
{
    QFETCH(QString,dictionaryname);

    Dashboard *dashboard = new Dashboard(0);
    DataStore *document = new DataStore(0);
    document->spacesModel()->addSpace(Dim3D);
    document->setCurrentSpace(0);
    DictionaryCollection *testWidget = new DictionaryCollection(0);

    testWidget->setDashboardWidget(dashboard);
    testWidget->setDictionaryDataMap();
    testWidget->setDefaultDictionaries();
    testWidget->setDocument(document);

    //setting the plot-dictionary model based on the name of the dictionary.
    testWidget->setDictionaryData(testWidget->indexOf(dictionaryname));

    //test index
    QModelIndex ind=testWidget->dictionaryPlotsView()->model()->index(0,0);

    //selecting the plot on the view.
    testWidget->dictionaryPlotsView()->selectionModel()->setCurrentIndex(ind,QItemSelectionModel::Select);

    //add plot in the space
    testWidget->addPlotClicked();

    //check for the correctness of the added plot
    QCOMPARE(document->plotsModel()->rowCount(),1);

    PlotItem* plotitem=document->plotsModel()->data(document->plotsModel()->index(0),PlotsModel::PlotRole).value<PlotItem*>();
    QVERIFY(plotitem!=0);

    QCOMPARE(plotitem->name(),QString("plane2"));
    QCOMPARE(plotitem->expression(),Analitza::Expression("(x, y, z)->(x+y+z)-5"));
}

#include "dictionarycollectiontest.moc"
