/*************************************************************************************
 *  Copyright (C) 2010 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>      *
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


#include "functioneditor.h"

#include <KLocale>

#include "analitza/analyzer.h"
#include "analitza/expression.h"
#include "analitza/variables.h"
#include "analitza/value.h"
#include "analitzagui/expressionedit.h"
#include "analitzagui/algebrahighlighter.h"
#include <KRandom>
#include <math.h>
#include "functionsmodel.h"
#include "functionsview.h"
// #include "keomath/solvers/solver.h"
// #include "keomath/solvers/solverfactory.h"
#include "functionlibraryedit.h"
#include "analitzagui/variablesmodel.h"
#include <analitzaplot/functiongraph.h>
#include <analitzaplot/planecurve.h>

#include "ui_functioneditorwidget.h"

PlotsEditor::PlotsEditor(QWidget * parent)
    : QDockWidget(parent), m_model(0), m_plotsView(0)
{
    Ui::DockWidget dd;
    dd.setupUi(this);
// costri r el selecto cmunto para view 
}

PlotsEditor::~PlotsEditor()
{

}

void PlotsEditor::showList()
{
}

void PlotsEditor::showCreateByName()
{

}

void PlotsEditor::showCreateByExpression()
{

}

void PlotsEditor::showEditor()
{

}




