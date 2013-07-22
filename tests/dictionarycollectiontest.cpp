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

//Analitza includes
#include <analitzaplot/plotitem.h>

//khipuLib includes
#include <dictionarycollection.h>
#include <dashboard.h>

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

#include "dictionarycollectiontest.moc"
