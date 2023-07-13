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

#include <persistancefiletest.h>

//Qt includes
#include <qtest_kde.h>

//Analitza includes
#include <analitzaplot/plotsfactory.h>
#include <analitzaplot/plotitem.h>
#include <analitzaplot/functiongraph.h>
#include <analitzaplot/plotsmodel.h>

//libkhipu includes
#include <datastore.h>
#include <spacesmodel.h>
#include <spaceitem.h>
#include <mainwindow.h>

using namespace Analitza;

QTEST_KDEMAIN( PersistanceFileTest , GUI)

Q_DECLARE_METATYPE (Analitza::Dimension);

PersistanceFileTest::PersistanceFileTest(QObject *parent)
    : QObject(parent)
{
}

PersistanceFileTest::~PersistanceFileTest()
{
}

void PersistanceFileTest::initTestCase()
{}

void PersistanceFileTest::cleanupTestCase()
{}

void PersistanceFileTest::testCorrect_data()
{
    QTest::addColumn<QString>("spacename");
    QTest::addColumn<Analitza::Dimension>("spacedimension");

    QTest::addColumn<QString>("plotname");
    QTest::addColumn<QString>("plotequation");
    QTest::addColumn<QColor>("plotcolor");

    QTest::newRow("1") << "space-1" << Analitza::Dim2D << "sin curve" << "x->sin(x)" << QColor(Qt::black);
    QTest::newRow("2") << "space-2" << Analitza::Dim2D << "cos curve" << "x->cos(x)" << QColor(Qt::blue);
    QTest::newRow("3") << "space-3" << Analitza::Dim2D << "tan curve" << "x->tan(x)" << QColor(Qt::green);
    QTest::newRow("4") << "space-4" << Analitza::Dim2D <<  "line-2d" << "t->vector{t,t}" << QColor(Qt::red);
    QTest::newRow("5") << "space-5" << Analitza::Dim3D << "line-3d" << "t->vector{t, t, t}"  << QColor(Qt::yellow);
    QTest::newRow("6") << "space-6" << Analitza::Dim3D << "para-3d" << "t->vector{t,sin(t),cos(t)}"  << QColor(Qt::white);
}

void PersistanceFileTest::testCorrect()
{
    QFETCH(QString,spacename);
    QFETCH(Analitza::Dimension,spacedimension);

    QFETCH(QString,plotname);
    QFETCH(QString,plotequation);
    QFETCH(QColor,plotcolor);

    MainWindow window;
    DataStore* document=window.getDocument();

    document->spacesModel()->addSpace(spacedimension,spacename,QString());

    FunctionGraph *item=nullptr;
    Analitza::PlotBuilder req = Analitza::PlotsFactory::self()->requestPlot(Analitza::Expression(plotequation),spacedimension);
    QVERIFY(req.canDraw());
    item = req.create(QColor(plotcolor),plotname);
    document->plotsModel()->addPlot(item);

    //save data in a file
    const QUrl testFile = QUrl::fromLocalFile(QDir::tempPath().append("/.test.khipu"));

    QVERIFY(window.saveFile(testFile));

    // clear the models and verify them
    QVERIFY(document->spacesModel()->removeRow(0));
    QVERIFY(document->plotsModel()->removeRow(0));

    // open file
    QVERIFY(window.openFile(testFile));

    SpaceItem *space=document->spacesModel()->space(0);
    QVERIFY(space!=nullptr);

    QModelIndex ind = document->spacesModel()->index(0);
    window.createPlot(ind);

    PlotItem* plotitem=document->plotsModel()->data(document->plotsModel()->index(0),PlotsModel::PlotRole).value<PlotItem*>();
    QVERIFY(plotitem!=nullptr);

    //testing the data
    QCOMPARE(space->name(),spacename);
    QCOMPARE(space->dimension(),spacedimension);

    QCOMPARE(plotitem->name(),plotname);
    QCOMPARE(plotitem->expression(),Analitza::Expression(plotequation));
    QCOMPARE(plotitem->color(),plotcolor);

    //clearing the model and verify again
    QVERIFY(document->spacesModel()->removeRow(0));
    QVERIFY(document->plotsModel()->removeRow(0));

    delete document;
}

#include "moc_persistancefiletest.cpp"
