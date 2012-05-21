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


#include "../solver.h"
#include "../solverfactory.h"

#include "analitza/value.h"
#include <analitza/vector.h>

namespace Keomath
{

class KEOMATH_EXPORT ImplicitSurface : public Solver3D
{
public:
    explicit ImplicitSurface(const Analitza::Expression &expression, Analitza::Variables *variables);
    ImplicitSurface(const ImplicitSurface &implicitSurface);
    virtual ~ImplicitSurface();

    static QStringList supportedBVars()
    {
        return QStringList() << "u" << "v";
    }
    static Analitza::ExpressionType expectedType()
    {

        return Analitza::ExpressionType(Analitza::ExpressionType::Lambda).addParameter(
                   Analitza::ExpressionType(Analitza::ExpressionType::Value)).addParameter(
                   Analitza::ExpressionType(Analitza::ExpressionType::Value)).addParameter(
                   Analitza::ExpressionType(Analitza::ExpressionType::Vector, Analitza::ExpressionType(
                                                Analitza::ExpressionType::Value), 3));


    }
    static QStringList examples()
    {
        QStringList ret;


        return ret;
    }

    const Analitza::Expression & lambda() const;

    QStringList arguments() const
    {
        return supportedBVars();
    }
    Function::Axe axeType() const
    {
        return Function::Cartesian;
    }
    void solve(const RealInterval::List &spaceBounds);
    Solver * copy()
    {
        return new ImplicitSurface(*this);
    }

    QVector3D evalSurface(qreal u, qreal v);


protected:
    Analitza::Cn* m_u; 
    Analitza::Cn* m_v; 

    Analitza::Expression m_originalCartesianLambda;

};


ImplicitSurface::ImplicitSurface(const Analitza::Expression &expression, Analitza::Variables *variables)
    : Solver3D(expression, variables)
    , m_u(new Analitza::Cn)
    , m_v(new Analitza::Cn)
{
    m_originalCartesianLambda = Analitza::Expression(m_evaluator.expression());

    m_runStack.append(m_u);
    m_runStack.append(m_v);
    m_evaluator.setStack(m_runStack);

    if(m_evaluator.isCorrect())
    {






        m_evaluator.flushErrors();
    }
}

ImplicitSurface::ImplicitSurface(const ImplicitSurface &implicitSurface)
    : Solver3D(implicitSurface)
    , m_u(new Analitza::Cn)
    , m_v(new Analitza::Cn)
{
    m_originalCartesianLambda = Analitza::Expression(m_evaluator.expression());

    m_runStack.append(m_u);
    m_runStack.append(m_v);
    m_evaluator.setStack(m_runStack);

}

ImplicitSurface::~ImplicitSurface()
{
    delete m_u;
    delete m_v;
}


const Analitza::Expression & ImplicitSurface::lambda() const
{
    return m_originalCartesianLambda;
}


void ImplicitSurface::solve(const RealInterval::List &spaceBounds)
{


}

QVector3D ImplicitSurface::evalSurface(qreal u, qreal v)
{
    
    return QVector3D();
}

REGISTER_FUNCTION_3D(ImplicitSurface)


} 

