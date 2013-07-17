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


#include <qtest_kde.h>

#include <datastoretest.h>
#include <datastore.h>
#include <analitzaplot/plotsmodel.h>
#include <analitzaplot/plotsfactory.h>
#include <analitzaplot/plottingenums.h>
#include <analitzaplot/functiongraph.h>
#include <analitzaplot/plotitem.h>

using namespace Analitza;

QTEST_KDEMAIN_CORE( DataStoreTest )

Q_DECLARE_METATYPE(Analitza::Dimension);

Q_DECLARE_METATYPE(Qt::GlobalColor);

DataStoreTest::DataStoreTest(QObject *parent)
    : QObject(parent)
{
    m_document = new DataStore();
}

DataStoreTest::~DataStoreTest()
{
}

void DataStoreTest::initTestCase()
{}

void DataStoreTest::cleanupTestCase()
{}

void DataStoreTest::testCorrect_data()
{
    QTest::addColumn<QString>("expression");
    QTest::addColumn<Analitza::Dimension>("dimension");
    QTest::addColumn<Qt::GlobalColor>("color");
    QTest::addColumn<QString>("name");

    QTest::newRow("1") << "x->sin(x)" << Analitza::Dim2D << Qt::black << "sin";
    QTest::newRow("2") << "x->cos(x)" << Analitza::Dim2D << Qt::blue  << "cos";
    QTest::newRow("3") << "x->tan(x)"<<  Analitza::Dim2D << Qt::green  << "tan";
    QTest::newRow("4") << "t->vector{t,t}" << Analitza::Dim2D << Qt::red  << "line-2d";
    QTest::newRow("5") << "t->vector{t, t, t}" << Analitza::Dim3D << Qt::yellow  << "line-3d";
    QTest::newRow("6") << "t->vector{t,sin(t),cos(t)}" << Analitza::Dim3D << Qt::white  << "para-3d";
    QTest::newRow("7") << "x+y+z=1" << Analitza::Dim3D << Qt::darkGray  << "plane";
    QTest::newRow("8") << "(t,p)->6" << Analitza::Dim3D << Qt::lightGray  << "test-surface";
}

void DataStoreTest::testCorrect()
{
    FunctionGraph* item=0;

    QFETCH(QString,expression);
    QFETCH(Analitza::Dimension,dimension);
    QFETCH(Qt::GlobalColor,color);
    QFETCH(QString,name);

    PlotBuilder req = PlotsFactory::self()->requestPlot(Analitza::Expression(expression),dimension);

    QVERIFY(req.canDraw());
    item = req.create(QColor(color),name);
    m_document->plotsModel()->addPlot(item);

    QList<Analitza::PlotItem*> plotList=m_document->currentDataMap().values();

    QVERIFY(!plotList.isEmpty());

    Analitza::PlotItem* testitem=plotList.at(0);
    QCOMPARE(testitem->name(),QString(name));

    // need to solve in analitza
    if(dimension!=Dim3D)
        QCOMPARE(testitem->expression(),Analitza::Expression(expression));
    QCOMPARE(testitem->color(),QColor(color));
    QCOMPARE(plotList.removeAll(testitem),1);
}

#include "datastoretest.moc"
