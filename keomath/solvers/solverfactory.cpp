/*************************************************************************************
 *  Copyright (C) 2007-2009 by Aleix Pol <aleixpol@kde.org>                          *
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


#include "solverfactory.h"

#include "analitza/expressiontype.h"

using Analitza::Expression;
using Analitza::Variables;

namespace Keomath
{

SolverFactory* SolverFactory::m_self=0;

bool SolverFactory::containsFor2D(const SolverFactory::Id& bvars) const
{
    return m_items_2d.contains(bvars.join("|"));
}

bool SolverFactory::containsFor3D(const SolverFactory::Id& bvars) const
{
    return m_items_3d.contains(bvars.join("|"));
}

Solver2D* SolverFactory::item2D(const Id& bvars, const Expression& exp, Variables* v) const
{
    return m_items_2d[bvars.join("|")](exp, v);
}

Solver3D* SolverFactory::item3D(const Id& bvars, const Expression& exp, Variables* v) const
{
    return m_items_3d[bvars.join("|")](exp, v);
}

Analitza::ExpressionType SolverFactory::typeFor2D(const SolverFactory::Id& bvars)
{
    return m_types_2d[bvars.join("|")]();
}

Analitza::ExpressionType SolverFactory::typeFor3D(const SolverFactory::Id& bvars)
{
    return m_types_3d[bvars.join("|")]();
}


bool SolverFactory::registerFunction2D(const Id& bvars, registerFunc_fn_2d f, expectedType_fn ft, dim_fn fd,
                                       const QStringList& examples)
{

    
    m_items_2d[bvars.join("|")]=f;
    m_types_2d[bvars.join("|")]=ft;
    m_dims[bvars.join("|")]=fd;
    m_examples.append(examples);

    return true;
}

bool SolverFactory::registerFunction3D(const Id& bvars, registerFunc_fn_3d f, expectedType_fn ft, dim_fn fd,
                                       const QStringList& examples)
{

    
    m_items_3d[bvars.join("|")]=f;
    m_types_3d[bvars.join("|")]=ft;
    m_dims[bvars.join("|")]=fd;
    m_examples.append(examples);

    return true;
}

QStringList SolverFactory::examples() const
{
    return m_examples;
}

SolverFactory* SolverFactory::self()
{
    if(!m_self)
        m_self=new SolverFactory;
    return m_self;
}

} 
