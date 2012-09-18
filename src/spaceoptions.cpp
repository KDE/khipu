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

#include "spaceoptions.h"

#include "src/ui_spaceoptions.h"

#include <cmath>
#include <kcolorutils.h>

SpaceOptions::SpaceOptions(QWidget* parent): QDockWidget(parent)
{
    m_widget = new Ui::SpaceOptionsWidget;
    m_widget->setupUi(this);
    m_widget->moreOptions->setChecked(false);
    
    setObjectName("a33sdasdds");

//symbolValue es el valor del simbolo ... no del texto del combobox ...
//
//si el texto del como es pi/2 entonces symval es el valor del simbolo pi ... no es pi/2



//            //pi
//            PiDividedBy5,
//            PiDividedBy4,
//            PiDividedBy3,
//            PiDividedBy2,
//            Pi,
//            PiTimes2,
//            PiTimes3,
//            PiTimes4,

    QString sym = QString((QChar(0x03C0)));

    addTickEntry(sym+"/2", sym, M_PI, 1, 2);
    addTickEntry(sym, sym, M_PI, 1, 1);


//            //e
//            EDividedBy2,
//            E,
//            ETimes2,

    sym = QString("e");

    addTickEntry(sym, sym, M_E, 1, 1);

//            //sqrt constants

//            Sqrt2DividedBy2
//            Sqrt2,
//            Sqrt2Times2,

//            Sqrt3DividedBy3
//            Sqrt3DividedBy2
//            Sqrt3,
//            Sqrt3Times2,
//            Sqrt3Times3,

//            Sqrt5DividedBy5
//            Sqrt5DividedBy2
//            Sqrt5,
//            Sqrt5Times2,
//            Sqrt5Times5,

//            Sqrt6DividedBy6
//            Sqrt6DividedBy3
//            Sqrt6DividedBy2
//            Sqrt6,
//            Sqrt6Times2,
//            Sqrt6Times3,
//            Sqrt6Times6,

//            Sqrt7DividedBy5
//            Sqrt7DividedBy2
//            Sqrt7,
//            Sqrt7Times2,
//            Sqrt7Times5,

//            Sqrt8DividedBy8
//            Sqrt8DividedBy4
//            Sqrt8DividedBy2
//            Sqrt8,
//            Sqrt8Times2,
//            Sqrt8Times4,
//            Sqrt8Times8,

//            Sqrt10DividedBy10
//            Sqrt10DividedBy5
//            Sqrt10DividedBy2
//            Sqrt10,
//            Sqrt10Times2,
//            Sqrt10Times5,
//            Sqrt10Times10,

    sym = QString((QChar(0x221A)))+"2";
    qreal symval = sqrt(2);
    addTickEntry(sym, sym, symval, 1, 1);

    sym = QString((QChar(0x221A)))+"3";
    symval = sqrt(3);
    addTickEntry(sym, sym, symval, 1, 1);


//            // decimals 0 - 1

//            Decimal01,
//            Decimal02,
//            Decimal03,
//            Decimal04,
//            Decimal05,
//            Decimal06,
//            Decimal07,
//            Decimal08,
//            Decimal09,
// el symval por defecto cuando no hay simbo sera 1

    addTickEntry("1", QString(), 1, 1, 1);

    m_widget->scale->setCurrentIndex(m_widget->scale->count()-1);
    m_widget->useSymbols->hide(); // como el valor por defecto es 1 se esconde el chekcbos para usar simbolos

//            // number and decimals from 0.5 in 0.5 1 - 10

//            Integer1,
//            Decimal15,
//            Integer2,
//            Decimal25,
//            Integer3,
//            Decimal35,
//            Integer4,
//            Decimal45,
//            Integer5,
//            Decimal55,
//            Integer6,
//            Decimal65,
//            Integer7,
//            Decimal75,
//            Integer8,
//            Decimal85,
//            Integer9,
//            Decimal95,

//            // numbres from 10 to 100 jump = 10
//            Integer10,
//            Integer20,
//            Integer30,
//            Integer40,
//            Integer50,
//            Integer60,
//            Integer70,
//            Integer80,
//            Integer90,
//            Integer100,
//        };

//signals
    
    connect(m_widget->gridStyle, SIGNAL(currentIndexChanged(int)), SLOT(setGridStyle(int)));
    connect(m_widget->backgroundColor, SIGNAL(activated(QColor)), SIGNAL(updateGridColor(QColor)));
    connect(m_widget->horizontalLabel, SIGNAL(textChanged(QString)), SIGNAL(setXAxisLabel(QString)));
    connect(m_widget->verticalLabel, SIGNAL(textChanged(QString)), SIGNAL(setYAxisLabel(QString)));
    connect(m_widget->scale, SIGNAL(currentIndexChanged(int)), SLOT(updateScale()));
    connect(m_widget->useSymbols, SIGNAL(toggled(bool)), SLOT(updateScale()));

    connect(m_widget->horizontalMarks, SIGNAL(toggled(bool)), SLOT(updateTicks()));
    connect(m_widget->verticalMarks, SIGNAL(toggled(bool)), SLOT(updateTicks()));
    connect(m_widget->showXAxis, SIGNAL(toggled(bool)), SLOT(updateAxes()));
    connect(m_widget->showYAxis, SIGNAL(toggled(bool)), SLOT(updateAxes()));
    
    //3d
    connect(m_widget->showAxis, SIGNAL(toggled(bool)), SIGNAL(axisIsDrawn(bool)));
    connect(m_widget->showGrid, SIGNAL(toggled(bool)), SIGNAL(gridIsDrawn(bool)));
    connect(m_widget->sceneSize, SIGNAL(valueChanged(int)), SIGNAL(sceneResized(int)));
}

SpaceOptions::~SpaceOptions()
{
    delete m_widget;
}

void SpaceOptions::reset()
{}

void SpaceOptions::setGridStyleIndex(int i)
{
    m_widget->gridStyle->setCurrentIndex(i);
}

void SpaceOptions::setDimension(int d)
{
    m_widget->options->setCurrentIndex(d-2); //-2 porque si dim es 2 se muestra las optiones cero (para el view2d)
}

void SpaceOptions::updateScale()
{
    int i = m_widget->scale->currentIndex();
    QVariantMap vmap = m_widget->scale->itemData(i, Qt::UserRole).toMap();

    emit updateTickScale(vmap["symbol"].toString(),
    vmap["symbolValue"].toReal(),
    vmap["numerator"].toInt(),
    vmap["denominator"].toInt());
    
    // si se eliji un valor diferete de 1 aparece la pcion de usar simbolos o usar los valores
    // es decir si la opcion elegida es 1 (que es el ultimo item del combo) se oculta el check de simbolos
    //caso contrario se muestra
    m_widget->useSymbols->setVisible(i != m_widget->scale->count()-1);
}

void SpaceOptions::setGridStyle(int i)
{
    emit updateGridStyle(i);
    m_widget->backgroundColor->setVisible(i!=0);
}

void SpaceOptions::addTickEntry(QString tick, QString tickScaleSymbol, qreal tickScaleSymbolValue,
        /*bool tickScaleUseSymbols, */int tickScaleNumerator,
        int tickScaleDenominator)
{
    QVariantMap vmap;

    vmap.insert("tick", tick);
    vmap.insert("symbol", tickScaleSymbol);
    vmap.insert("symbolValue", tickScaleSymbolValue);
//    vmap.insert("useSymbols", tickScaleUseSymbols);
    vmap.insert("numerator", tickScaleNumerator);
    vmap.insert("denominator", tickScaleDenominator);

    m_widget->scale->addItem(tick, vmap);
}

void SpaceOptions::updateAxes()
{
    QFlags<Qt::Orientation> o;
    if(m_widget->showXAxis->checkState()==Qt::Checked)
        o|=Qt::Horizontal;
    if(m_widget->showYAxis->checkState()==Qt::Checked)
        o|=Qt::Vertical;
    emit axesShown(o);
}

void SpaceOptions::updateTicks()
{
    QFlags<Qt::Orientation> o;
    if(m_widget->horizontalMarks->checkState()==Qt::Checked)
        o|=Qt::Horizontal;
    if(m_widget->verticalMarks->checkState()==Qt::Checked)
        o|=Qt::Vertical;
    emit ticksShown(o);
}
