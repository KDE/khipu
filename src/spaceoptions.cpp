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

#include "ui_spaceoptions.h"

#include <cmath>

SpaceOptions::SpaceOptions(QWidget* parent): QDockWidget(parent)
{
    m_widget = new Ui::SpaceOptionsWidget;
    m_widget->setupUi(this);
    m_widget->moreOptions->setChecked(false);
    
    setObjectName("a33sdasdds");

/// confuro el combo de escale le ponmgo datas y adeamas hago las coneccioneseis con view2d


//RECUERDAAAAAAA RECUERDAAAAAAA RECUERDAAAAAAA RECUERDAAAAAAA RECUERDAAAAAAA
//RECUERDAAAAAAA RECUERDAAAAAAA RECUERDAAAAAAA RECUERDAAAAAAA RECUERDAAAAAAA
//RECUERDAAAAAAA RECUERDAAAAAAA RECUERDAAAAAAA RECUERDAAAAAAA RECUERDAAAAAAA
//
//symbolValue es el valor del simbolo ... no del texto del combobox ...
//
//si el texto del como es pi/2 entonces symval es el valor del simbolo pi ... no es pi/2



//            //pi
//            PiDividedBy6,
//            PiDividedBy5,
//            PiDividedBy4,
//            PiDividedBy3,
//            PiDividedBy2,
//            Pi,
//            PiTimes2,
//            PiTimes3,
//            PiTimes4,
//            PiTimes5,
//            PiTimes6,

    QString sym = QString((QChar(0x03C0)));

    addTickEntry(sym+"/6", sym, M_PI, 1, 6);
    addTickEntry(sym+"/5", sym, M_PI, 1, 5);
    addTickEntry(sym+"/4", sym, M_PI, 1, 4);
    addTickEntry(sym+"/3", sym, M_PI, 1, 3);
    addTickEntry(sym+"/2", sym, M_PI, 1, 2);
    addTickEntry(sym, sym, M_PI, 1, 1);
    addTickEntry("2"+sym, sym, M_PI, 2, 1);
    addTickEntry("3"+sym, sym, M_PI, 3, 1);
    addTickEntry("4"+sym, sym, M_PI, 4, 1);
    addTickEntry("5"+sym, sym, M_PI, 5, 1);
    addTickEntry("6"+sym, sym, M_PI, 6, 1);


//            //e
//            EDividedBy2,
//            E,
//            ETimes2,

    sym = QString("e");

    addTickEntry(sym+"/2", sym, M_E, 1, 2);
    addTickEntry(sym, sym, M_E, 1, 1);
    addTickEntry("2"+sym, sym, M_E, 2, 1);

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
    addTickEntry(sym+"/2", sym, symval, 1, 2);
    addTickEntry(sym, sym, symval, 1, 1);
    addTickEntry("2"+sym, sym, symval, 2, 1);

    sym = QString((QChar(0x221A)))+"3";
    symval = sqrt(3);
    addTickEntry(sym+"/3", sym, symval, 1, 3);
    addTickEntry(sym+"/2", sym, symval, 1, 2);
    addTickEntry(sym, sym, symval, 1, 1);
    addTickEntry("2"+sym, sym, symval, 2, 1);
    addTickEntry("3"+sym, sym, symval, 3, 1);


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
    addTickEntry("2", QString(), 1, 2, 1);
    addTickEntry("3", QString(), 1, 3, 1);
    addTickEntry("4", QString(), 1, 4, 1);
    addTickEntry("5", QString(), 1, 5, 1);
    addTickEntry("6", QString(), 1, 6, 1);
    addTickEntry("7", QString(), 1, 7, 1);
    addTickEntry("8", QString(), 1, 8, 1);
    addTickEntry("9", QString(), 1, 9, 1);

    m_widget->scale->setCurrentIndex(22);


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

}

SpaceOptions::~SpaceOptions()
{
    delete m_widget;
}

void SpaceOptions::reset()
{

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
